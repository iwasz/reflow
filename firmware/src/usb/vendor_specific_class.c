#include <stdio.h>
#include "vendor_specific_class.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

/*****************************************************************************/

static uint8_t usbdVencorInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t usbdVendorDeinit (USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t usbdVendorSetup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t EP0_RxReady (struct _USBD_HandleTypeDef *pdev);
static uint8_t *usbdVendorGetCfgDesc (uint16_t *length);
static uint8_t *usbdVendorGetDeviceQualifierDesc (uint16_t *length);

USBD_ClassTypeDef vendorClass = {
        usbdVencorInit,   // Init
        usbdVendorDeinit, // DeInit

        // Control Endpoints
        usbdVendorSetup, // Setup
        NULL,            // EP0_TxSent
        EP0_RxReady,     // EP0_RxReady

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
static uint8_t usbdVencorInit (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
        static UsbState usbState;
        usbState.pendingRequest = 0;
        usbState.reflow = &reflow;
        pdev->pUserData = &usbState;
        return USBD_OK;
}

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
        UsbState *usbState = (UsbState *)pdev->pUserData;

        switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_VENDOR:
#if 0
                printf ("USB_REQ_TYPE_VENDOR [%d, %d, %d]\n", req->bRequest, req->wValue, req->wIndex);
#endif
                switch (req->bRequest) {
                case GET_TEMP_REQUEST:
                        USBD_CtlSendData (pdev, (uint8_t *)&usbState->reflow->actualTemp, 4);
                        break;

                case GET_INTERNAL_TEMP_REQUEST:
                        USBD_CtlSendData (pdev, (uint8_t *)&usbState->reflow->internalTemp, 4);
                        break;

                case GET_RAW_DATA_REQUEST:
                        USBD_CtlSendData (pdev, (uint8_t *)usbState->reflow, sizeof (Reflow));
                        break;

                case RESET_REQUEST:
                        reflowClear ();
                        break;

                case STOP_REQUEST:
                        reflow.phase = IDLE;
                        break;

                case START_REQUEST:
                        reflow.phase = RAMP1;
//                        reflowClear ();
                        break;

                case SET_INSTANT_TEMP_REQUEST:
                case SET_KP_REQUEST:
                case SET_KI_REQUEST:
                case SET_KD_REQUEST:
                case SET_RAMP1_S_REQUEST:
                case SET_PREHEAT_S_REQUEST:
                case SET_RAMP2_S_REQUEST:
                case SET_REFLOW_S_REQUEST:
                case SET_COOLING_S_REQUEST:
                case SET_PREHEAT_TEMP_REQUEST:
                case SET_REFLOW_TEMP_REQUEST:
                        usbState->pendingRequest = req->bRequest;
                        usbState->payloadLen = req->wLength;
#if 0
                        printf ("SET_REQ rq=%d, len=%d, wLen=%d\n", usbState->pendingRequest, usbState->payloadLen, req->wLength);
#endif
                        USBD_CtlPrepareRx (pdev, usbState->payload, usbState->payloadLen);
                        break;

                default:
                        usbState->pendingRequest = 0;
                        USBD_CtlError (pdev, req);
                        return USBD_FAIL;
                }

                break;

        case USB_REQ_TYPE_STANDARD:
                switch (req->bRequest) {
                case USB_REQ_GET_INTERFACE: {
                        static uint8_t ifalt = 0;
                        USBD_CtlSendData (pdev, &ifalt, 1);
                        break;
                }
                }

        case USB_REQ_TYPE_CLASS:
        default:
                break;
        }

        return USBD_OK;
}

/*****************************************************************************/

static uint8_t EP0_RxReady (struct _USBD_HandleTypeDef *pdev)
{
        UsbState *usbState = (UsbState *)pdev->pUserData;
        uint8_t *p = usbState->payload;

        switch (usbState->pendingRequest) {
        case SET_INSTANT_TEMP_REQUEST:
                memcpy (&usbState->reflow->setPointTemp, p, usbState->payloadLen);
                break;

        case SET_KP_REQUEST:
                memcpy (&usbState->reflow->kp, p, usbState->payloadLen);
#if 0
                printf ("KP set to [%d], %d, %d\n", usbState->reflow->kp, p[0], p[1]);
#endif
                break;

        case SET_KI_REQUEST:
                memcpy (&usbState->reflow->ki, p, usbState->payloadLen);
                break;

        case SET_KD_REQUEST:
                memcpy (&usbState->reflow->kd, p, usbState->payloadLen);
                break;

        case SET_RAMP1_S_REQUEST:
                usbState->reflow->ramp1S = p[0];
                break;

        case SET_PREHEAT_S_REQUEST:
                usbState->reflow->preHeatS = p[0];
                break;

        case SET_RAMP2_S_REQUEST:
                usbState->reflow->ramp2S = p[0];
                break;

        case SET_REFLOW_S_REQUEST:
                usbState->reflow->reflowS = p[0];
                break;

        case SET_COOLING_S_REQUEST:
                usbState->reflow->coolingS = p[0];
                break;

        case SET_PREHEAT_TEMP_REQUEST:
                memcpy (&usbState->reflow->preHeatTemp, p, usbState->payloadLen);
                break;

        case SET_REFLOW_TEMP_REQUEST:
                memcpy (&usbState->reflow->reflowTemp, p, usbState->payloadLen);
                break;

        default:
                usbState->pendingRequest = 0;
                return USBD_FAIL;
        }

        usbState->pendingRequest = 0;
        usbState->payloadLen = 0;
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
