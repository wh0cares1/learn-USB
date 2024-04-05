#include "stddef.h"
#include "usbd_framework.h"
#include "usbd_driver.h"
#include "usb_device.h"
#include "usbd_descriptors.h"
#include "usb_standards.h"
#include "Helpers/logger.h"
#include "Helpers/math.h"

static UsbDevice *usbd_handle;

void usbd_initialize(UsbDevice *usb_device)
{
	usbd_handle = usb_device;
	usb_driver.initialize_gpio_pins();
	usb_driver.initialize_core();
	usb_driver.connect();
}

void usbd_poll()
{
	usb_driver.poll();
}

static void usb_reset_received_handler()
{
	usbd_handle->in_data_size = 0;
	usbd_handle->out_data_size = 0;
	usbd_handle->configuration_value = 0;
	usbd_handle->device_state = USB_DEVICE_STATE_DEFAULT;
	usbd_handle->control_transfer_stage = USB_CONTROL_STAGE_SETUP;
	usb_driver.set_device_address(0);
}

void usbd_configure()
{
	usb_driver.configure_in_endpoint(
		(configuration_descriptor_combination.usb_mouse_endpoint_descriptor.bEndpointAddress & 0x0F),
		(configuration_descriptor_combination.usb_mouse_endpoint_descriptor.bmAttributes & 0x03),
		configuration_descriptor_combination.usb_mouse_endpoint_descriptor.wMaxPacketSize
	);

	usb_driver.write_packet(
		(configuration_descriptor_combination.usb_mouse_endpoint_descriptor.bEndpointAddress & 0x0F),
		NULL,
		0
	);
}


