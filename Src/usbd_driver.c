#include "usbd_driver.h"
#include "usb_standards.h"
#include "string.h"

static void initialize_gpio_pins()
{
	// Enables the clock for GPIOB.
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);

	// Sets alternate function 12 for: PB14 (-), and PB15 (+).
	MODIFY_REG(GPIOB->AFR[1],
		GPIO_AFRH_AFSEL14 | GPIO_AFRH_AFSEL15,
		_VAL2FLD(GPIO_AFRH_AFSEL14, 0xC) | _VAL2FLD(GPIO_AFRH_AFSEL15, 0xC)
	);

	// Configures USB pins (in GPIOB) to work in alternate function mode.
	MODIFY_REG(GPIOB->MODER,
		GPIO_MODER_MODER14 | GPIO_MODER_MODER15,
		_VAL2FLD(GPIO_MODER_MODER14, 2) | _VAL2FLD(GPIO_MODER_MODER15, 2)
	);
}

static void initialize_core()
{
	// Enables the clock for USB core.
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);

	// Configures the USB core to run in device mode, and to use the embedded full-speed PHY.
	MODIFY_REG(USB_OTG_HS->GUSBCFG,
		USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | USB_OTG_GUSBCFG_TRDT,
		USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | _VAL2FLD(USB_OTG_GUSBCFG_TRDT, 0x09)
	);

	// Configures the device to run in full speed mode.
	MODIFY_REG(USB_OTG_HS_DEVICE->DCFG,
		USB_OTG_DCFG_DSPD,
		_VAL2FLD(USB_OTG_DCFG_DSPD, 0x03)
	);

	// Enables VBUS sensing device.
	SET_BIT(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBUSBSEN);

	// Unmasks the main USB core interrupts.
	SET_BIT(USB_OTG_HS->GINTMSK,
		USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
		USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
		USB_OTG_GINTSTS_OEPINT | USB_OTG_GINTMSK_RXFLVLM
	);

	// Clears all pending core interrupts.
	WRITE_REG(USB_OTG_HS->GINTSTS, 0xFFFFFFFF);

	// Unmasks USB global interrupt.
	SET_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_GINT);

	// Unmasks transfer completed interrupts for all endpoints.
	SET_BIT(USB_OTG_HS_DEVICE->DOEPMSK, USB_OTG_DOEPMSK_XFRCM);
	SET_BIT(USB_OTG_HS_DEVICE->DIEPMSK, USB_OTG_DIEPMSK_XFRCM);
}

static void set_device_address(uint8_t address)
{
    MODIFY_REG(
		USB_OTG_HS_DEVICE->DCFG,
		USB_OTG_DCFG_DAD,
		_VAL2FLD(USB_OTG_DCFG_DAD, address)
	);
}

/** \brief Connects the USB device to the bus.
 */
static void connect()
{
	// Powers the transceivers on.
    SET_BIT(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_PWRDWN);

	// Connects the device to the bus.
    CLEAR_BIT(USB_OTG_HS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);
}

/** \brief Disconnects the USB device from the bus.
 */
static void disconnect()
{
	// Disconnects the device from the bus.
	SET_BIT(USB_OTG_HS_DEVICE->DCTL, USB_OTG_DCTL_SDIS);

	// Powers the transceivers off.
	CLEAR_BIT(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_PWRDWN);
}

/** \brief Pops data from the RxFIFO and stores it in the buffer.
 * \param buffer Pointer to the buffer, in which the popped data will be stored.
 * \param size Count of bytes to be popped from the dedicated RxFIFO memory.
 */
static void read_packet(void *buffer, uint16_t size)
{
	// Note: There is only one RxFIFO.
	uint32_t *fifo = FIFO(0);

	for (; size >= 4; size -=4, buffer += 4)
	{
		// Pops one 32-bit word of data (until there is less than one word remaining).
		uint32_t data = *fifo;
		// Stores the data in the buffer.
		*((uint32_t*)buffer) = data;
	}

	if (size > 0)
	{
		// Pops the last remaining bytes (which are less than one word).
		uint32_t data = *fifo;

		for(; size > 0; size--, buffer++, data >>= 8)
		{
			// Stores the data in the buffer with the correct alignment.
			*((uint8_t*)buffer) = 0xFF & data;
		}
	}
}


