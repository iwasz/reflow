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

/**
 * USB stuff for anyKey.
 */
class UsbService {
public:

        typedef std::vector <uint8_t> Buffer;

//        struct __attribute__ ((packed)) PidData {
//                int16_t kp;
//                int16_t ki;
//                int16_t kd;
//                int32_t p;
//                int32_t i;
//                int32_t d;
//                int16_t fac;
//        };

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

        void setTempInstant (uint16_t temp);
        void setKp (uint16_t p);
        void setKi (uint16_t i);
        void setKd (uint16_t d);
        uint16_t getCurrentTemp () const;
//        PidData getPidData () const;

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

        ~UsbServiceGuard ()
        {
                try {
                        service.destroy ();
                }
                catch (std::exception const &e) {
                        std::cerr << "An exception has occured in ~UsbServiceGuard. Message : " << e.what () << std::endl;
                }
                catch (...) {
                        std::cerr << "Unknown exception has occured in ~UsbServiceGuard." << std::endl;
                }
        }

        Service service;
};

#endif /* USBSERVICE_H_ */
