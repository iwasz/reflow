#include <stdio.h>
#include "vendor_specific_class.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

/*****************************************************************************/

static uint8_t usbdVencorInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t usbdVendorDeinit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t usbdVendorSetup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t *usbdVendorGetCfgDesc (uint16_t *length);
static uint8_t *usbdVendorGetDeviceQualifierDesc (uint16_t *length);

USBD_ClassTypeDef vendorClass = {
        usbdVencorInit,   // Init
        usbdVendorDeinit, // DeInit

        // Control Endpoints
        usbdVendorSetup, // Setup
        NULL,            // EP0_TxSent
        NULL,            // EP0_RxReady

        // Class Specific Endpoints
        NULL, // DataIn
        NULL, // DataOut
        NULL, // SOF
        NULL, // IsoINIncomplete
        NULL, // IsoOUTIncomplete

        // Deskryptory.
        usbdVendorGetCfgDesc,             // GetHSConfigDescriptor : zwraca ten główny cfg-descriptor.
        usbdVendorGetCfgDesc,             // GetFSConfigDescriptor : zwraca ten główny cfg-descriptor.
        usbdVendorGetCfgDesc,             // GetOtherSpeedConfigDescriptor : zwraca ten główny cfg-descriptor.
        usbdVendorGetDeviceQualifierDesc, // GetDeviceQualifierDescriptor (?)
};

/**
 * A high-speed capable device that has different device information for full-speed and high-speed must
 * have a Device Qualifier Descriptor (USB_DEVICE_QUALIFIER_DESCRIPTOR). For example, if the device is
 * currently operating at full-speed, the Device Qualifier returns information about how it would operate
 * at high-speed and vice-versa.
 */
static uint8_t usbdVendorDeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] = {
        USB_LEN_DEV_QUALIFIER_DESC,     // bLength : Size of this descriptor in bytes.
        USB_DESC_TYPE_DEVICE_QUALIFIER, // bDescriptorType : Device Qualifier Descriptor Type = 6.
        0x00,                           // bcdUSB : At least V2.00 is required to use this descriptor.
        0x02,                           //
        0xff,                           // bDeviceClass : If this field is set to FFh, the device class is vendor specific.
        0xff,                           // bDeviceSubClass
        0xff,                           // bDeviceProtocol
        0x40,                           // bMaxPacketSize0
        0x01,                           // bNumConfigurations
        0x00,                           // bReserved
};

//#define USB_CONFIG_DESC_SIZ 27
#define USB_CONFIG_DESC_SIZ 18

/**
 * Configuration descriptor.
 */
uint8_t usbdVendorCfgFsDesc[USB_CONFIG_DESC_SIZ] = {
        0x09,                        /* bLength (1) Configuration Descriptor size in bytes, always 0x09. */
        USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType (1B) Stała 0x02 (patrz tabelka typów deskryptorów */
        USB_CONFIG_DESC_SIZ,         /* wTotalLength (2B) Rozmiar w bajtach całego deskryptora wraz z jego "doklejonymi" */
        0x00,                        /* deskryptorami interfejsów i endpointów. To jest drugi bajt. */
        0x01,                        /* bNumInterfaces (1B) Liczba interfejsów. */
        0x01,                        /* bConfigurationValue (1B) numer porządkowy konfiguracji. Musi być większy równy 1. Rozumiem, że musi być */
                                     /* unikalny dla każdej konfiguracji. */
        0x00,                        /* iConfiguration (1B) Index of string descriptor describing the configuration, albo 0, jeśli nie ma. */
        0xc0,                        /* bmAttributes (1B) Atrybuty w postaci maski bitowej. Bit 6==1 -> self powered. Bit 6==0 -> bus powered */
                                     /* Bit 5==1 -> urządzenie obsługuje "remote wakeup feature". Bit5==0 -> nie obsługuje. Bit 4..0 muszą być 0 */
                                     /* Bit7 musi być równy 1. */
        0xfa,                        /* bMaxPower (1B) Max prąd w jednostkach 2mA dla USB 2.0 i w jednostkach 8mA dla USB 3.0. W tym przypadku */
                                     /* 0xfa = 250, czyli 500mA. */

        /*
         * Interface descriptor.
         */
        0x09,                    /* bLength : Interface Descriptor size == 0x09. */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType : Interface descriptor type (constant 0x04). */
        0x00,                    /* bInterfaceNumber: Unikalny numer. Urządzenia typu composite będą miały następne interfejsy, a każdy */
                                 /* będzie miał kolejne numery. Domyślny interfejs ma numer 0. */
        0x00,                    /* bAlternateSetting: Unikalny numer "alternate settings". Każdy interfejs może mieć odmiany, które */
                                 /* właśnie nazywamy "alternate setting". Każde takie ustawienie musi mieć unikalny numer w tym polu. */
                                 /* Domyślny numer to 0 i kazdy interfejs musi mieć takie "alternatywne ustawienie" o nr. 0. */
        0x00,                    /* bNumEndpoints : Liczba endpointów prócz EP0. */
        0xff,                    /* bInterfaceClass : Klasa. Listę można znaleźć na wikipedii. 0xff to vendor specific. Klasę urządzenia */
                                 /* można także podać w deskryptorze urządzenia, ale najczęściej się podaj tu. 0x03 to HID. */
        0xff,                    /* bInterfaceSubClass : 1=BOOT, 0=no boot. Ustanowione przez USB-IF dla klas. 0xff == vendor specific. */
        0xff,                    /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse. Ustanowione przez USB-IF dla klas. 0xff == vendor */
                                 /* specific.*/
        0,                       /* iInterface: Index of string descriptor, or 0 if there is no string. */

//        /*
//         * Alternative intrerface descriptor?
//         */
//        0x09,                    /* bLength : Interface Descriptor size == 0x09. */
//        USB_DESC_TYPE_INTERFACE, /* bDescriptorType : Interface descriptor type (constant 0x04). */
//        0x00,                    /* bInterfaceNumber: Unikalny numer. Urządzenia typu composite będą miały następne interfejsy, a każdy */
//                                 /* będzie miał kolejne numery. Domyślny interfejs ma numer 0. */
//        0x01,                    /* bAlternateSetting: Unikalny numer "alternate settings". Każdy interfejs może mieć odmiany, które */
//                                 /* właśnie nazywamy "alternate setting". Każde takie ustawienie musi mieć unikalny numer w tym polu. */
//                                 /* Domyślny numer to 0 i kazdy interfejs musi mieć takie "alternatywne ustawienie" o nr. 0. */
//        0x00,                    /* bNumEndpoints : Liczba endpointów prócz EP0. */
//        0xff,                    /* bInterfaceClass : Klasa. Listę można znaleźć na wikipedii. 0xff to vendor specific. Klasę urządzenia */
//                                 /* można także podać w deskryptorze urządzenia, ale najczęściej się podaj tu. 0x03 to HID. */
//        0xff,                    /* bInterfaceSubClass : 1=BOOT, 0=no boot. Ustanowione przez USB-IF dla klas. 0xff == vendor specific. */
//        0xff,                    /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse. Ustanowione przez USB-IF dla klas. 0xff == vendor */
//                                 /* specific.*/
//        0,                       /* iInterface: Index of string descriptor, or 0 if there is no string. */
};

/** @defgroup USBD_HID_Private_Functions
  * @{
  */

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t usbdVencorInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx) { return USBD_OK; }

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t usbdVendorDeinit (USBD_HandleTypeDef *pdev, uint8_t cfgidx) { return USBD_OK; }

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t usbdVendorSetup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
        //        uint16_t len = 0;
        //        uint8_t *pbuf = NULL;
        //        USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassData;

        switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_CLASS:
                printf ("USB_REQ_TYPE_CLASS [%d, %d]\n", req->bRequest, req->wValue);

                //                switch (req->bRequest) {

                //                case HID_REQ_SET_PROTOCOL:
                //                        hhid->Protocol = (uint8_t) (req->wValue);
                //                        break;

                //                case HID_REQ_GET_PROTOCOL:
                //                        USBD_CtlSendData (pdev, (uint8_t *)&hhid->Protocol, 1);
                //                        break;

                //                case HID_REQ_SET_IDLE:
                //                        hhid->IdleState = (uint8_t) (req->wValue >> 8);
                //                        break;

                //                case HID_REQ_GET_IDLE:
                //                        USBD_CtlSendData (pdev, (uint8_t *)&hhid->IdleState, 1);
                //                        break;

                //                default:
                //                        USBD_CtlError (pdev, req);
                //                        return USBD_FAIL;
                //                }
                break;

        case USB_REQ_TYPE_VENDOR:
                printf ("USB_REQ_TYPE_VENDOR [%d, %d, %d]\n", req->bRequest, req->wValue, req->wIndex);
                break;

        case USB_REQ_TYPE_STANDARD:
                switch (req->bRequest) {
                //                case USB_REQ_GET_DESCRIPTOR:
                //                        if (req->wValue >> 8 == HID_REPORT_DESC) {
                //                                len = MIN (HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
                //                                pbuf = HID_MOUSE_ReportDesc;
                //                        }
                //                        else if (req->wValue >> 8 == HID_DESCRIPTOR_TYPE) {
                //                                pbuf = USBD_HID_Desc;
                //                                len = MIN (USB_HID_DESC_SIZ, req->wLength);
                //                        }

                //                        USBD_CtlSendData (pdev, pbuf, len);
                //                        break;

                case USB_REQ_GET_INTERFACE: {
                        static uint8_t ifalt = 0;
                        USBD_CtlSendData (pdev, &ifalt, 1);
                        break;
                }
                }
        }

        return USBD_OK;
}

/**
  * @brief  USBD_HID_GetCfgDesc
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *usbdVendorGetCfgDesc (uint16_t *length)
{
        *length = sizeof (usbdVendorCfgFsDesc);
        return usbdVendorCfgFsDesc;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t *usbdVendorGetDeviceQualifierDesc (uint16_t *length)
{
        *length = sizeof (usbdVendorDeviceQualifierDesc);
        return usbdVendorDeviceQualifierDesc;
}
