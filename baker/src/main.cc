/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <unistd.h>
#include <iostream>
#include "UsbService.h"
#include "Constants.h"

/*--------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
        try {
                UsbServiceGuard<UsbService> guard;

                //                guard.service.setTempInstant (50);
                guard.service.setKp (-666);
                //                guard.service.setI (1);
                //                guard.service.setD (1);

                while (true) {
                        std::cerr << "Thermocouple : " << guard.service.getCurrentTemp () << ", internal : " << guard.service.getInternalTemp ()
                                  << ", raw data : " << std::hex << guard.service.getRawData () << std::endl;

                        //                        UsbService::PidData pd = guard.service.getPidData ();

                        //                        std::cerr << "T = " << guard.service.getCurrentTemp () << ", Kp = " << pd.kp << ", Ki = " << pd.ki << ", Kd =
                        //                        " <<
                        //                        pd.kd
                        //                                  << ", P = " << pd.p << ", I = " << pd.i << ", D = " << pd.d << ", fac = " << pd.fac << ", U = " <<
                        //                                  pd.p +
                        //                                  pd.i - pd.d
                        //                                  << std::endl;
                        sleep (1);
                }
        }
        catch (std::exception const &e) {
                std::cerr << "An exception has occured in ~UsbServiceGuard. Message : " << e.what () << std::endl;
        }
        catch (...) {
                std::cerr << "Unknown exception has occured in ~UsbServiceGuard." << std::endl;
        }

        return 0;
}
