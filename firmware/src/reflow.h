/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef REFLOW_H
#define REFLOW_H

#include <stdint.h>

struct _Reflow {

        uint32_t rawData;

        int16_t actualTemp;
        int16_t internalTemp;
        int16_t kp;
        int16_t ki;
        int16_t kd;

        uint8_t ramp1S;
        uint8_t preHeatS;
        uint8_t ramp2S;
        uint8_t reflowS;
        uint8_t coolingS;
};

typedef struct _Reflow Reflow;

extern Reflow reflow;

#endif // REFLOW_H
