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

#define GET_TEMP_REQUEST ((uint8_t)0x01)
#define SET_INSTANT_TEMP_REQUEST ((uint8_t)0x02)
#define SET_KP_REQUEST ((uint8_t)0x03)
#define SET_KI_REQUEST ((uint8_t)0x04)
#define SET_KD_REQUEST ((uint8_t)0x05)
#define SET_RAMP1_S_REQUEST ((uint8_t)0x06)
#define SET_PREHEAT_S_REQUEST ((uint8_t)0x07)
#define SET_RAMP2_S_REQUEST ((uint8_t)0x08)
#define SET_REFLOW_S_REQUEST ((uint8_t)0x09)
#define SET_COOLING_S_REQUEST ((uint8_t)0x0a)
#define GET_INTERNAL_TEMP_REQUEST ((uint8_t)0x0b)
#define GET_RAW_DATA_REQUEST ((uint8_t)0x0c)

extern USBD_ClassTypeDef vendorClass;

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
        uint8_t payload[2];

        /*
         * Główna struktura z KP, KI, KD i tak dalej. Ona jest zdefiniowana w main.c
         */
        Reflow *reflow;
} UsbState;

#endif /* __USB_HID_H */
