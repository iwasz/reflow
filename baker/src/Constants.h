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
const uint8_t SET_TEMP_REQUEST = 0x02;
const uint8_t SET_P_REQUEST = 0x03;
const uint8_t SET_I_REQUEST = 0x04;
const uint8_t SET_D_REQUEST = 0x05;
const uint8_t GET_PID_DATA_REQUEST = 0x06;

#endif /* CONSTANTS_H_ */
