#include <stdint.h>
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include "string_utils.h"

#define USBD_VID 0x20A0
#define USBD_PID 0x41ff
#define USBD_LANGID_STRING 0x409
#define USBD_MANUFACTURER_STRING "iwasz"
#define USBD_PRODUCT_FS_STRING "reflowctl"
#define USBD_CONFIGURATION_FS_STRING "reflow"
#define USBD_INTERFACE_FS_STRING "reflow"
#define USB_SIZ_STRING_SERIAL 0x1a

uint8_t *USBD_VCP_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_VCP_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length);

#define DEVICE_ID1 (0x1FFFF7AC)

/* Private variables ---------------------------------------------------------*/
USBD_DescriptorsTypeDef usbDescriptorsVirtualTable = {
        USBD_VCP_DeviceDescriptor,    USBD_VCP_LangIDStrDescriptor, USBD_VCP_ManufacturerStrDescriptor, USBD_VCP_ProductStrDescriptor,
        USBD_VCP_SerialStrDescriptor, USBD_VCP_ConfigStrDescriptor, USBD_VCP_InterfaceStrDescriptor,
};

/* USB Standard Device Descriptor */
uint8_t USBD_DeviceDesc[USB_LEN_DEV_DESC] = {
        USB_LEN_DEV_DESC,          /* bLength (1B) Rozmiar deskryptora, ktory jest stały i wynosi 0x12. */
        USB_DESC_TYPE_DEVICE,      /* bDescriptorType (1B) Typ deskryptora (stała, ktora, patrz tabelka wyżej, wynosi 0x01). */
        0x00,                      /* bcdUSB (2B) USB specification release number (BCD). Dla USB 1.1 : 0x0110, */
        0x02,                      /* bcdUSB (2B). Drugi bajt. */
        0x00,                      /* bDeviceClass (1B) Kod klasy. Ale to pole nie jest obowiązkowe, bo klasę urządzenia można */
                                   /* zdefiniować w ''interface descriptor'' (tak się zazwyczaj robi). W takim przypadku */
                                   /* ustawiamy tu 0x00 gdy urządzenie NIE posiada ''association descriptor'', a 0xef gdy posiada. */
        0x00,                      /* bDeviceSubClass (1B) Podklasa. Gdy bDeviceClass jest 0x00, to bDeviceSubClass */
                                   /* MUSI być 0x00. W innym przypadku, trzeba sprawdzić w tabelkach. */
        0x00,                      /* bDeviceProtocol (1B) Kolejna specyficzna wartość ustalana dla klas przez USB-IF. */
        USB_MAX_EP0_SIZE,          /* bMaxPacketSize0 (1B) Max rozmiar pakietu dla EP0. W USB 2.0 : 8 dla LS, */
                                   /* 8, 16, 32 lub 64 dla FS, oraz 64 dla HS. */
        LOBYTE (USBD_VID),         /* idVendor (2B) */
        HIBYTE (USBD_VID),         /* idVendor (2B) */
        LOBYTE (USBD_PID),         /* idProduct (2B) */
        HIBYTE (USBD_PID),         /* idProduct (2B) */
        0x00,                      /* bcdDevice (2B) Numer wersji urządzenia. Host może wybrać odpowiedni driver na */
        0x02,                      /* podstawie tego pola. */
        USBD_IDX_MFC_STR,          /* iManufacturer (1B) Index of manufacturer string descriptor, albo 0 jeśli nie ma. */
        USBD_IDX_PRODUCT_STR,      /* iProduct (1B) Index of product string descriptor, or 0 if not present. */
        USBD_IDX_SERIAL_STR,       /* iSerialNumber (1B) Index of serial number string descriptor, or 0. */
        USBD_MAX_NUM_CONFIGURATION /* bNumConfigurations (1B) Liczba konfiguracji obsługiwanych przez urządzenie w aktualnej */
};                                 /* USB_DeviceDescriptor */

uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] = {
        USB_LEN_LANGID_STR_DESC, USB_DESC_TYPE_STRING, LOBYTE (USBD_LANGID_STRING), HIBYTE (USBD_LANGID_STRING),
};

uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] = {
        USB_SIZ_STRING_SERIAL, USB_DESC_TYPE_STRING,
};

uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ];

/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        *length = sizeof (USBD_DeviceDesc);
        return (uint8_t *)USBD_DeviceDesc;
}

/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        *length = sizeof (USBD_LangIDDesc);
        return (uint8_t *)USBD_LangIDDesc;
}

/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        USBD_GetString ((uint8_t *)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
        return USBD_StrDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        USBD_GetString ((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
        return USBD_StrDesc;
}

/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
#if 0
uint8_t *USBD_VCP_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        *length = USB_SIZ_STRING_SERIAL;

        /* Update the serial number string descriptor with the data from the unique ID*/
        Get_SerialNum ();

        return (uint8_t *)USBD_StringSerial;
}
#else
uint8_t *USBD_VCP_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        *length = USB_SIZ_STRING_SERIAL;

        /* Update the serial number string descriptor with the data from the unique ID*/
        uint32_t deviceserial = *(uint32_t *)DEVICE_ID1;
        uintToStringUtf16 (deviceserial, USBD_StringSerial + 2);

        return (uint8_t *)USBD_StringSerial;
}
#endif

/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        USBD_GetString ((uint8_t *)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
        return USBD_StrDesc;
}

/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_VCP_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t *length)
{
        USBD_GetString ((uint8_t *)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
        return USBD_StrDesc;
}


