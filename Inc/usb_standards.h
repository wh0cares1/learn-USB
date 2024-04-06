#ifndef USB_STANDARDS_H_
#define USB_STANDARDS_H_

typedef enum UsbEndpointType
{
	USB_ENDPOINT_TYPE_CONTROL,
	USB_ENDPOINT_TYPE_ISOCHRONOUS,
	USB_ENDPOINT_TYPE_BULK,
	USB_ENDPOINT_TYPE_INTERRUPT
} UsbEndpointType;

typedef struct
{
	void (*on_usb_reset_received)();
	void (*on_setup_data_received)(uint8_t endpoint_number, uint16_t bcnt);
	void (*on_out_data_received)(uint8_t endpoint_number, uint16_t bcnt);
	void (*on_in_transfer_completed)(uint8_t endpoint_number);
	void (*on_out_transfer_completed)(uint8_t endpoint_number);
	void (*on_usb_polled)();
} UsbEvents;

typedef enum
{
	USB_DEVICE_STATE_DEFAULT,
	USB_DEVICE_STATE_ADDRESSED,
	USB_DEVICE_STATE_CONFIGURED,
	USB_DEVICE_STATE_SUSPENDED
} UsbDeviceState;

typedef enum
{
	USB_CONTROL_STAGE_SETUP, // Can also be called USB_CONTROL_STAGE_IDLE
	USB_CONTROL_STAGE_DATA_OUT,
	USB_CONTROL_STAGE_DATA_IN,
	USB_CONTROL_STAGE_DATA_IN_IDLE,
	USB_CONTROL_STAGE_DATA_IN_ZERO,
	USB_CONTROL_STAGE_STATUS_OUT,
	USB_CONTROL_STAGE_STATUS_IN
} UsbControlTransferStage;

/** \brief USB control request. */
typedef struct
{
	uint8_t bmRequestType; // The transfer direction, the type of request, and the recipient
	uint8_t bRequest; // Request identity
	uint16_t wValue; // Parameter passed to the device
	uint16_t wIndex; // Parameter passed to the device
	uint16_t wLength; // The count of bytes that will be transmitted in the data data stage
} UsbRequest;

/** \defgroup UsbDeviceBitMappedRequestTypeFields
 *@{*/
#define USB_BM_REQUEST_TYPE_DIRECTION_MASK (1 << 7)
#define USB_BM_REQUEST_TYPE_DIRECTION_TODEVICE (0 << 7)
#define USB_BM_REQUEST_TYPE_DIRECTION_TOHOST (1 << 7)

#define USB_BM_REQUEST_TYPE_TYPE_MASK (3 << 5)
#define USB_BM_REQUEST_TYPE_TYPE_STANDARD (0 << 5)
#define USB_BM_REQUEST_TYPE_TYPE_CLASS (1 << 5)
#define USB_BM_REQUEST_TYPE_TYPE_VENDOR (2 << 5)

#define USB_BM_REQUEST_TYPE_RECIPIENT_MASK (3 << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_DEVICE (0 << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_INTERFACE (1 << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_ENDPOINT (2 << 0)
#define USB_BM_REQUEST_TYPE_RECIPIENT_OTHER (3 << 0)
/**@}*/

/**\name USB standard requests
 * @{ */
#define USB_STANDARD_GET_STATUS 0x00 /**<\brief Returns the status if the specified recipient.*/
#define USB_STANDARD_CLEAR_FEATURE 0x01 /**<\brief Clears or disables a specific feature.*/
#define USB_STANDARD_SET_FEATURE 0x03 /**<\brief Sets or enables a specific feature.*/
#define USB_STANDARD_SET_ADDRESS 0x05 /**<\brief Sets the device address for all future device communications.*/
#define USB_STANDARD_GET_DESCRIPTOR 0x06 /**<\brief Returns the specified descriptor if the descriptor exists.*/
#define USB_STANDARD_SET_DESCRIPTOR 0x07 /**<\brief Updates existing descriptors or new descriptors may be added.*/
#define USB_STANDARD_GET_CONFIG 0x08 /**<\brief Returns the current device configuration value.*/
#define USB_STANDARD_SET_CONFIG 0x09 /**<\brief Sets the device configuration.*/
#define USB_STANDARD_GET_INTERFACE 0x0A /**<\brief Returns the selected alternate setting for the specified interface.*/
#define USB_STANDARD_SET_INTERFACE 0x0B /**<\brief Allows the host to select an alternate setting for the specified interface.*/
#define USB_STANDARD_SYNCH_FRAME 0x0C /**<\brief Sets and then reports an endpoint's synchronization frame.*/
/** @} */


