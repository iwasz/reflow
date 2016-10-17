/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com (based on ST).                   *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef __USB_VENDOR_SPECIFIC_H
#define __USB_VENDOR_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "usbd_ioreq.h"
#include "reflow.h"

extern USBD_ClassTypeDef vendorClass;
extern void reflowClear ();

/**
 *
 */
typedef struct _UsbState {

        /*
         * Numer requestu (lista powyżej), dla którego mamy odczytać payload. Biblioteka Stm32Cube
         * obsługuje requesty kontrolne z payloadem w 2 krokach. Najpierw w handlerze USBD_ClassTypeDef::Setup,
         * który u mnie nazywa się usbdVendorSetup jest odbierany numer requestu i tam jest deklarowane ile
         * bajtów payloadu mamy odczytać, a następnie w handlerze USBD_ClassTypeDef::EP0_RxReady zadana
         * liczba bajtów jest odczytywana. Więc pomiędzy tymi dwoma wywołaniami chcę zapamiętać numer
         * requestu.
         */
        uint8_t pendingRequest;
        uint8_t payloadLen;
        uint8_t payload[4];

        /*
         * Główna struktura z KP, KI, KD i tak dalej. Ona jest zdefiniowana w main.c
         */
        Reflow *reflow;
} UsbState;

#endif /* __USB_HID_H */
