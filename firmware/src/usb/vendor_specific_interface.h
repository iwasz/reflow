/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com (based on ST).                   *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef __VENDOR_INTERFACE_H
#define __VENDOR_INTERFACE_H

#include "vendor_specific_class.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on CDC_POLLING_INTERVAL */
#define USB_POLLING_INTERVAL 5 /* in ms. The max is 65ms and the min is 1ms */
#define TIM_USB_IRQn TIM3_IRQn

//extern USBD_CDC_ItfTypeDef usbInterfaceFops;

/**
 * @brief Wysyła po USB przez endpoint typu interrupt.
 */
//extern void usbTransmitInterrupt (uint8_t *buf, size_t len);

#endif

#ifdef __cplusplus
}
#endif
