#ifndef USBD_DESCRIPTORS_H_
#define USBD_DESCRIPTORS_H_

#include "usb_standards.h"
#include "Hid/usb_hid_standards.h"

const UsbDeviceDescriptor device_descriptor = {
    .bLength            = sizeof(UsbDeviceDescriptor),
    .bDescriptorType    = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB             = 0x0200, // 0xJJMN
    .bDeviceClass       = USB_CLASS_PER_INTERFACE,
    .bDeviceSubClass    = USB_SUBCLASS_NONE,
    .bDeviceProtocol    = USB_PROTOCOL_NONE,
    .bMaxPacketSize0    = 8,
    .idVendor           = 0x6666,
    .idProduct          = 0x13AA,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0,
    .iProduct           = 0,
    .iSerialNumber      = 0,
    .bNumConfigurations = 1,
};

const uint8_t hid_report_descriptor[] = {
	HID_USAGE_PAGE(HID_PAGE_DESKTOP),
	HID_USAGE(HID_DESKTOP_MOUSE),
	HID_COLLECTION(HID_APPLICATION_COLLECTION),
		HID_USAGE(HID_DESKTOP_POINTER),
		HID_COLLECTION(HID_PHYSICAL_COLLECTION),
			HID_USAGE(HID_DESKTOP_X),
			HID_USAGE(HID_DESKTOP_Y),
			HID_LOGICAL_MINIMUM(-127),
			HID_LOGICAL_MAXIMUM(127),
			HID_REPORT_SIZE(8),
			HID_REPORT_COUNT(2),
			HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),

			HID_USAGE_PAGE(HID_PAGE_BUTTON),
			HID_USAGE_MINIMUM(1),
			HID_USAGE_MAXIMUM(3),
			HID_LOGICAL_MINIMUM(0),
			HID_LOGICAL_MAXIMUM(1),
			HID_REPORT_SIZE(1),
			HID_REPORT_COUNT(3),
			HID_INPUT(HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
			HID_REPORT_SIZE(1),
			HID_REPORT_COUNT(5),
			HID_INPUT(HID_IOF_CONSTANT),
		HID_END_COLLECTION,
	HID_END_COLLECTION
};

typedef struct {
	UsbConfigurationDescriptor usb_configuration_descriptor;
	UsbInterfaceDescriptor usb_interface_descriptor;
	UsbHidDescriptor usb_mouse_hid_descriptor;
	UsbEndpointDescriptor usb_mouse_endpoint_descriptor;
} UsbConfigurationDescriptorCombination;

typedef struct {
	int8_t      x;
	int8_t      y;
	uint8_t     buttons;
} __attribute__((__packed__)) HidReport;

#endif /* USBD_DESCRIPTORS_H_ */
