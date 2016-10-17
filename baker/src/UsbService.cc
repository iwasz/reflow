/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <cstring>
#include <unistd.h>
#include "UsbService.h"
#include "reflow.h"
#include "Exception.h"

/**
 * Internal structures hidden from the user.
 */
struct UsbService::Impl {
        libusb_device_handle *device = nullptr;
        //        static void onControlTransferCompletion (libusb_transfer *xfr);
        bool deviceConnected = false;
};

/*--------------------------------------------------------------------------*/

UsbService::UsbService () { impl = new Impl; }

/*--------------------------------------------------------------------------*/

UsbService::~UsbService ()
{
        if (impl->deviceConnected) {
                try {
                        reset ();
                        stop ();
                        destroy ();
                        std::cerr << "Stopped." << std::endl;
                }
                catch (std::exception const &e) {
                        std::cerr << "An exception has occured in ~UsbService. Message : " << e.what () << std::endl;
                }
                catch (...) {
                        std::cerr << "Unknown exception has occured in ~UsbService." << std::endl;
                }
        }

        delete impl;
}

/*--------------------------------------------------------------------------*/

void UsbService::init ()
{
        int r = libusb_init (NULL);

        if (r < 0) {
                std::cerr << "Error libusb_init!" << std::endl;
                return;
        }

        impl->device = libusb_open_device_with_vid_pid (NULL, USBD_VID, USBD_PID);

        if (!impl->device) {
                throw Exception ("Error finding USB device");
        }
        else {
                std::cerr << "USB device has been found." << std::endl;
        }

        int rc = libusb_set_auto_detach_kernel_driver (impl->device, true);

        if (rc == LIBUSB_SUCCESS) {
                std::cerr << "libusb_set_auto_detach_kernel_driver OK" << std::endl;
        }
        else {
                throw Exception ("libusb_set_auto_detach_kernel_driver Failed!");
        }

        rc = libusb_claim_interface (impl->device, 0);

        if (rc < 0) {
                throw Exception ("Error claiming interface : " + std::string (libusb_error_name (rc)));
        }
        else {
                std::cerr << "Interface claimed OK" << std::endl;
        }

        //        if ((rc = libusb_set_interface_alt_setting (impl->device, 0, 1)) != 0) {
        //                throw Exception ("Error libusb_set_interface_alt_setting : " + std::string (libusb_error_name (rc)));
        //        }
        //        else {
        //                std::cerr << "libusb_set_interface_alt_setting OK" << std::endl;
        //        }

        impl->deviceConnected = true;
}

/*--------------------------------------------------------------------------*/

void UsbService::destroy ()
{
        if (!libusb_release_interface (impl->device, 0)) {
                std::cerr << "Interface released OK" << std::endl;
        }
        else {
                throw Exception ("Error releasing interface.");
        }

        if (impl->device) {
                libusb_close (impl->device);
                std::cerr << "Device closed OK" << std::endl;
        }

        libusb_exit (NULL);
}

/*--------------------------------------------------------------------------*/

void UsbService::sendControlRequest (uint8_t request, uint8_t *buff, uint8_t len, bool in) const
{
        uint8_t direction = (in) ? (LIBUSB_ENDPOINT_IN) : (LIBUSB_ENDPOINT_OUT);

        // Ustaw temperaturę:
        int ret = libusb_control_transfer (impl->device,
                                           LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE | direction, // LIBUSB_RECIPIENT_DEVICE
                                           request, 0x0000, 0x0000, buff, len, 500);

        if (ret < 0) {
                throw Exception ("UsbService::transmitConfiguration : " + std::string (libusb_error_name (ret)) + ".");
        }
}

/*--------------------------------------------------------------------------*/

void UsbService::setTempInstant (float temp) { sendControlRequest (SET_INSTANT_TEMP_REQUEST, (uint8_t *)&temp, sizeof (temp), false); }

/*--------------------------------------------------------------------------*/

void UsbService::setKp (float ii) { sendControlRequest (SET_KP_REQUEST, (uint8_t *)&ii, sizeof (ii), false); }

/*--------------------------------------------------------------------------*/

void UsbService::setKi (float ii) { sendControlRequest (SET_KI_REQUEST, (uint8_t *)&ii, sizeof (ii), false); }

/*--------------------------------------------------------------------------*/

void UsbService::setKd (float ii) { sendControlRequest (SET_KD_REQUEST, (uint8_t *)&ii, sizeof (ii), false); }

/*--------------------------------------------------------------------------*/

float UsbService::getCurrentTemp () const
{
        float buff;
        sendControlRequest (GET_TEMP_REQUEST, (uint8_t *)&buff, sizeof (buff), true);
        return buff;
}

/*--------------------------------------------------------------------------*/

float UsbService::getInternalTemp () const
{
        float buff;
        sendControlRequest (GET_INTERNAL_TEMP_REQUEST, (uint8_t *)&buff, sizeof (buff), true);
        return buff;
}

/*--------------------------------------------------------------------------*/

Reflow UsbService::getPidData () const
{
        Reflow buff;
        sendControlRequest (GET_RAW_DATA_REQUEST, (uint8_t *)&buff, sizeof (Reflow), true);
        return buff;
}

/*--------------------------------------------------------------------------*/

void UsbService::reset ()
{
        uint8_t dummy;
        sendControlRequest (RESET_REQUEST, &dummy, 0, true);
}

/*--------------------------------------------------------------------------*/

void UsbService::stop ()
{
        uint8_t dummy;
        sendControlRequest (STOP_REQUEST, &dummy, 0, true);
}
