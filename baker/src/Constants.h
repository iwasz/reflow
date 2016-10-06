/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const unsigned int VENDOR_ID = 0x20a0;
const unsigned int PRODUCT_ID = 0x41ff;

const size_t OUTPUT_DATA_SIZE = 2;

const uint8_t GET_TEMP_REQUEST = 0x01;
const uint8_t SET_INSTANT_TEMP_REQUEST = 0x02;
const uint8_t SET_KP_REQUEST = 0x03;
const uint8_t SET_KI_REQUEST = 0x04;
const uint8_t SET_KD_REQUEST = 0x05;
const uint8_t SET_RAMP1_S_REQUEST = 0x06;
const uint8_t SET_PREHEAT_S_REQUEST = 0x07;
const uint8_t SET_RAMP2_S_REQUEST = 0x08;
const uint8_t SET_REFLOW_S_REQUEST = 0x09;
const uint8_t SET_COOLING_S_REQUEST = 0x0a;

#endif /* CONSTANTS_H_ */
