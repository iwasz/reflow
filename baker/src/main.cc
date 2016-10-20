/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <unistd.h>
#include <iostream>
#include <csignal>
#include "UsbService.h"

void sigHandler (int signo);
bool running = true;

/*--------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
        if (signal (SIGINT, sigHandler) == SIG_ERR) {
                std::cerr << "Can't connect signals." << std::endl;
        }

        try {
                UsbService service;
                service.init ();
                service.reset ();

//                service.setTempInstant (150);
                service.setKp (4);
                service.setKi (0.004);
                service.setKd (40);

                service.setPreheatTemp (150);
                service.setReflowTemp (220);
                service.setRamp1S (120);
                service.setPreheatS (120);
                service.setRamp2S (70);
                service.setReflowS (10);
                service.setCoolingS (100);

                service.start ();

                Reflow pd = service.getPidData ();
                std::cout << 0 << " " << pd.actualTemp << std::endl;
                std::cout << (int)pd.ramp1S << " " << pd.preHeatTemp << std::endl;
                std::cout << (int)pd.ramp1S + pd.preHeatS << " " << pd.preHeatTemp << std::endl;
                std::cout << (int)pd.ramp1S + pd.preHeatS + pd.ramp2S << " " << pd.reflowTemp << std::endl;
                std::cout << (int)pd.ramp1S + pd.preHeatS + pd.ramp2S + pd.reflowS << " " << pd.reflowTemp << std::endl;
                std::cout << (int)pd.ramp1S + pd.preHeatS + pd.ramp2S + pd.reflowS + pd.coolingS << " " << pd.actualTemp << std::endl;
                std::cout << std::endl;

                int seconds = 0;
                while (running) {
                        pd = service.getPidData ();
                        std::cerr << seconds << ", aT = " << pd.actualTemp << ", sT = " << pd.setPointTemp << ", Kp = " << pd.kp << ", Ki = " << pd.ki << ", Kd = " << pd.kd
                                  << ", P = " << pd.kp << ", I = " << pd.ki << ", D = " << pd.kd << ", error (P) = " << pd.error
                                  << ", integral = " << pd.integral << ", derivative = " << pd.derivative << ", duty = " << (int)pd.dutyCycle << std::endl;

                        std::cout << ++seconds << " " << pd.actualTemp << std::endl;
                        sleep (1);
                }
        }
        catch (std::exception const &e) {
                std::cerr << "An exception has occured in ~UsbService Message : " << e.what () << std::endl;
        }
        catch (...) {
                std::cerr << "Unknown exception has occured in ~UsbService" << std::endl;
        }

        return 0;
}
/*--------------------------------------------------------------------------*/

void sigHandler (int signo)
{
        if (signo == SIGINT) {
                running = false;
        }
}
