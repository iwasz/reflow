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
#include "Constants.h"
#include "Exception.h"

/**
 * Internal structures hidden from the user.
 */
struct UsbService::Impl {
        libusb_device_handle *device = nullptr;
        //        static void onControlTransferCompletion (libusb_transfer *xfr);
};

/*--------------------------------------------------------------------------*/

UsbService::UsbService () { impl = new Impl; }

/*--------------------------------------------------------------------------*/

UsbService::~UsbService () { delete impl; }

/*--------------------------------------------------------------------------*/

void UsbService::init ()
{
        int r = libusb_init (NULL);

        if (r < 0) {
                std::cerr << "Error libusb_init!" << std::endl;
                return;
        }

        impl->device = libusb_open_device_with_vid_pid (NULL, VENDOR_ID, PRODUCT_ID);

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

void UsbService::setTempInstant (uint16_t temp)
{
        uint8_t buff[2];
        memcpy (buff, (uint8_t *)&temp, 2);
        sendControlRequest (SET_INSTANT_TEMP_REQUEST, buff, 2, false);
}

/*--------------------------------------------------------------------------*/

void UsbService::setKp (uint16_t ii)
{
        uint8_t buff[2];
        memcpy (buff, (uint8_t *)&ii, 2);
        sendControlRequest (SET_KP_REQUEST, buff, 2, false);
}

/*--------------------------------------------------------------------------*/

void UsbService::setKi (uint16_t ii)
{
        uint8_t buff[2];
        memcpy (buff, (uint8_t *)&ii, 2);
        sendControlRequest (SET_KI_REQUEST, buff, 2, false);
}

/*--------------------------------------------------------------------------*/

void UsbService::setKd (uint16_t ii)
{
        uint8_t buff[2];
        memcpy (buff, (uint8_t *)&ii, 2);
        sendControlRequest (SET_KD_REQUEST, buff, 2, false);
}

/*--------------------------------------------------------------------------*/

uint16_t UsbService::getCurrentTemp () const
{
        uint8_t buff[2];
        sendControlRequest (GET_TEMP_REQUEST, buff, 2, true);
        int16_t temp = 0;
        memcpy ((uint8_t *)&temp, buff, 2);
        return temp;
}

/*--------------------------------------------------------------------------*/

//UsbService::PidData UsbService::getPidData () const
//{
//        uint8_t buff[24];
//        sendControlRequest (GET_PID_DATA_REQUEST, buff, 24, true);
//        UsbService::PidData pd;
//        memcpy ((uint8_t *)&pd, buff, sizeof (pd));
//        return pd;
//}
