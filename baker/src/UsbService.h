/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef USBSERVICE_H_
#define USBSERVICE_H_

#include <vector>
#include <cstdint>
#include <libusb.h>
#include <iostream>
#include "reflow.h"

/**
 * USB stuff for anyKey.
 */
class UsbService {
public:

        typedef std::vector <uint8_t> Buffer;

        UsbService ();
        ~UsbService ();

        UsbService (UsbService const &) = delete;
        UsbService &operator= (UsbService const &) = delete;

        /**
         * Connects to the device, and claims its interface.
         */
        void init ();

        /**
         * Releases the interface.
         */
        void destroy ();

        /**
         * Prints all the available USB devices connected to the host we are running on.
         */
        void printDevs ();

        void setTempInstant (float temp);
        void setKp (float p);
        void setKi (float i);
        void setKd (float d);
        float getCurrentTemp () const;
        float getInternalTemp () const;
        Reflow getPidData () const;
        void reset ();
        void stop ();

private:

        void sendControlRequest (uint8_t request, uint8_t *buf, uint8_t len, bool in) const;

private:

        // PIMPL idiom prevents exposing usb headers to user (I am the only user, but who cares).
        struct Impl;
        Impl *impl;
};

/**
 * RAII capabilities for UsbService.
 */
template <typename Service>
struct UsbServiceGuard {
        UsbServiceGuard ()
        {
                service.init ();
        }

        Service service;
};

#endif /* USBSERVICE_H_ */
