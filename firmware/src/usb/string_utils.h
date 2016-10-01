/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t *uintToStringUtf16 (uint32_t i, uint8_t *b);
//extern void itoa (int n, char s[]);

#ifdef __cplusplus
}
#endif

#endif // STRING_UTILS_H
