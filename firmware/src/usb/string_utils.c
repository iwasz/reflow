/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "string_utils.h"
#include <string.h>

uint8_t *uintToStringUtf16 (uint32_t i, uint8_t *b)
{
        char const digit[] = "0123456789";
        uint8_t *p = b;
        uint32_t shifter = i;

        do { // Move to where representation ends
                p += 2;
                shifter = shifter / 10;
        } while (shifter);

        do { // Move back, inserting digits as u go
                *--p = '\0';
                *--p = digit[i % 10];
                i = i / 10;
        } while (i);

        return b;
}

#if 0
/* reverse:  reverse string s in place */
static void reverse (char s[])
{
        int i, j;
        char c;

        for (i = 0, j = strlen (s) - 1; i < j; i++, j--) {
                c = s[i];
                s[i] = s[j];
                s[j] = c;
        }
}

/* itoa:  convert n to characters in s */
void itoa (int n, char s[])
{
        int i, sign;

        if ((sign = n) < 0) /* record sign */
                n = -n;     /* make n positive */
        i = 0;
        do {                           /* generate digits in reverse order */
                s[i++] = n % 10 + '0'; /* get next digit */
        } while ((n /= 10) > 0);       /* delete it */
        if (sign < 0) s[i++] = '-';
        s[i] = '\0';
        reverse (s);
}
#endif
