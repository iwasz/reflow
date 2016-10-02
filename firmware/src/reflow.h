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

        int16_t instantTemp;
        int16_t kp;
        int16_t ki;
        int16_t kd;

        uint8_t ramp1S;
        uint8_t preHeatS;
        uint8_t ramp2S;
        uint8_t reflowS;
        uint8_t coolingS;

};

typedef _Reflow Reflow;

#endif // REFLOW_H
