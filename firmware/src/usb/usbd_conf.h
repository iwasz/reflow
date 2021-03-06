#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define USBD_MAX_NUM_INTERFACES 1
#define USBD_MAX_NUM_CONFIGURATION 1
#define USBD_MAX_STR_DESC_SIZ 0x100
#define USBD_SUPPORT_USER_STRING 0
#define USBD_SELF_POWERED 0
#define USBD_DEBUG_LEVEL 0
#define MAX_STATIC_ALLOC_SIZE 4 /*HID Class Driver Structure size*/
#define USB_DEVICE_REVISION 0x01
#define USB_DEVICE_VERSION 0x07

#define USBD_malloc malloc
#define USBD_free free
#define USBD_memset memset
#define USBD_memcpy memcpy

#define USBD_UsrLog(...)
#define USBD_ErrLog(...)
#define USBD_DbgLog(...)

#endif /* __USBD_CONF_H */
