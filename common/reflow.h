/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef REFLOW_H
#define REFLOW_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#define CYCLE_MS 2000

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
#define RESET_REQUEST ((uint8_t)0x0d)
#define STOP_REQUEST ((uint8_t)0x0e)
#define START_REQUEST ((uint8_t)0x0f)
#define SET_PREHEAT_TEMP_REQUEST ((uint8_t)0x10)
#define SET_REFLOW_TEMP_REQUEST ((uint8_t)0x11)

#define USBD_VID 0x20A0
#define USBD_PID 0x41ff

/**
 * Phase of this.
 */
enum { IDLE, RAMP1, PREHEAT, RAMP2, REFLOW, COOLING };

/**
 * The assumption is that the memory representation of this structure is the same on both targets (cortex-m0 and PC).
 */
struct _Reflow {

        float kp;
        float ki;
        float kd;

        float actualTemp;
        float internalTemp;

        float setPointTemp; /// Temperatura do której PI controller będzie dążył.
        float error;        // P
        float prevError;    // Pprev
        float integral;     // I
        float derivative;   // D
        uint32_t startTimeMs;
        int16_t dutyCycle;
        uint16_t preHeatTemp;
        uint16_t reflowTemp;

        uint8_t ramp1S;
        uint8_t preHeatS;
        uint8_t ramp2S;
        uint8_t reflowS;
        uint8_t coolingS;
        uint8_t phase;
};

typedef struct _Reflow Reflow;

extern Reflow reflow;

#endif // REFLOW_H
