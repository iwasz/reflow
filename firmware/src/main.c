#include <math.h>
#include "stm32f0xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "vendor_specific_class.h"

SPI_HandleTypeDef hspi1;
USBD_HandleTypeDef usbdDevice;
UART_HandleTypeDef huart1;
Reflow reflow;

void SystemClock_Config (void);
void Error_Handler (void);
static void MX_GPIO_Init (void);
// static void MX_USART1_UART_Init (void);
void reflowClear ();

/*****************************************************************************/

int main (void)
{
        HAL_Init ();

        SystemClock_Config ();

        MX_GPIO_Init ();

        /*---------------------------------------------------------------------------*/

        GPIO_InitTypeDef gpioInitStruct;
        __HAL_RCC_SPI1_CLK_ENABLE ();
        __HAL_RCC_GPIOA_CLK_ENABLE ();

        gpioInitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        gpioInitStruct.Mode = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpioInitStruct.Alternate = GPIO_AF0_SPI1;
        HAL_GPIO_Init (GPIOA, &gpioInitStruct);

        gpioInitStruct.Pin = GPIO_PIN_4;
        gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        gpioInitStruct.Pull = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init (GPIOA, &gpioInitStruct);
        GPIOA->BSRR |= GPIO_PIN_4;

        hspi1.Instance = SPI1;
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
        hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_SOFT;
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial = 7;
        hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

        if (HAL_SPI_Init (&hspi1) != HAL_OK) {
                Error_Handler ();
        }

        __HAL_SPI_ENABLE (&hspi1);
        // 16 it treshold
        CLEAR_BIT (hspi1.Instance->CR2, SPI_RXFIFO_THRESHOLD);

        /*---------------------------------------------------------------------------*/

        //        MX_USART1_UART_Init ();

        // Before USB init
        reflowClear ();
        reflow.actualTemp = 0;

        USBD_Init (&usbdDevice, &usbDescriptorsVirtualTable, 0);
        USBD_RegisterClass (&usbdDevice, &vendorClass);
        USBD_Start (&usbdDevice);

        /*---------------------------------------------------------------------------*/

        uint32_t rawData;

        // Reset once again to have accureate time.
        reflow.startTimeMs = HAL_GetTick ();
        while (1) {
                /*--temperature-reading------------------------------------------------------*/

                GPIOA->BSRR |= GPIO_PIN_4 << 16;
                HAL_Delay (10);
                hspi1.Instance->DR = 0xffff;

                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;

                rawData = (*(uint16_t *)&hspi1.Instance->DR) << 16;

                hspi1.Instance->DR = 0xffff;

                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;

                rawData |= (uint32_t) (*(uint16_t *)&hspi1.Instance->DR);

                reflow.actualTemp = (int16_t) (rawData >> 18);
                reflow.actualTemp /= 4;

                reflow.internalTemp = (int16_t) ((rawData & 0xffff) >> 4);
                reflow.internalTemp /= 16;

                //                printf ("%d, %d, %lx\n", reflow.actualTemp, reflow.internalTemp, reflow.rawData);
                HAL_Delay (10);
                GPIOA->BSRR |= GPIO_PIN_4;
                uint16_t startTemp = reflow.actualTemp;

                /*--State-machine-things-----------------------------------------------------*/
                if (reflow.phase != IDLE) {
                        uint16_t actualTimeS = (HAL_GetTick () - reflow.startTimeMs) / 1000;

                        switch (reflow.phase) {
                        case RAMP1:
                                reflow.setPointTemp = ((reflow.preHeatTemp - startTemp) * actualTimeS / reflow.ramp1S) + startTemp;
                                if (actualTimeS >= reflow.ramp1S) {
                                        reflow.startTimeMs = HAL_GetTick ();
                                        reflow.phase = PREHEAT;
                                }
                                break;

                        case PREHEAT:
                                reflow.setPointTemp = reflow.preHeatTemp;
                                if (actualTimeS >= reflow.preHeatS) {
                                        reflow.phase = RAMP2;
                                        reflow.startTimeMs = HAL_GetTick ();
                                }
                                break;

                        case RAMP2:
                                reflow.setPointTemp = ((reflow.reflowTemp - reflow.preHeatTemp) * actualTimeS / reflow.ramp2S) + reflow.preHeatTemp;
                                if (actualTimeS >= reflow.ramp2S) {
                                        reflow.phase = REFLOW;
                                        reflow.startTimeMs = HAL_GetTick ();
                                }
                                break;

                        case REFLOW:
                                if (actualTimeS >= reflow.reflowS) {
                                        reflow.phase = COOLING;
                                        reflow.startTimeMs = HAL_GetTick ();
                                }
                                break;

                        case COOLING:
                                reflow.setPointTemp = 0;
                                reflow.phase = IDLE; // No fan in the oven, so cooling not implemented.
                                break;

                        case CONSTANT_TEMPERATURE:
                                break;

                        default:
                                reflow.setPointTemp = 0;
                                break;
                        }

                        /*--P-I calculation----------------------------------------------------------*/

                        reflow.error = reflow.setPointTemp - reflow.actualTemp;
                        reflow.integral = reflow.integral + reflow.error * (CYCLE_MS / 1000);
                        reflow.derivative = (reflow.error - reflow.prevError) / (CYCLE_MS / 1000);
                        reflow.dutyCycle = roundf (reflow.kp * reflow.error + reflow.ki * reflow.integral + reflow.kd * reflow.derivative);
                        reflow.prevError = reflow.error;

                        if (reflow.dutyCycle > 100) {
                                reflow.dutyCycle = 100;
                        }

                        if (reflow.dutyCycle < 0) {
                                reflow.dutyCycle = 0;
                        }

                        /*--one-pwm-cycle------------------------------------------------------------*/

                        relay_GPIO_Port->BSRR |= relay_Pin;
                        HAL_Delay (CYCLE_MS * reflow.dutyCycle / 100);

                        relay_GPIO_Port->BSRR |= relay_Pin << 16;
                        HAL_Delay (CYCLE_MS * (100 - reflow.dutyCycle) / 100);
                }
                else { // reflow.running == IDLE
                        // To be sure.
                        relay_GPIO_Port->BSRR |= relay_Pin << 16;
                }
        }
}

/**
 * Czyści wszystko.
 */
void reflowClear ()
{
        reflow.internalTemp = 0;
        reflow.dutyCycle = 0;
        reflow.setPointTemp = 0;
        reflow.kp = 1;
        reflow.ki = 1;
        reflow.kd = 1;
        reflow.integral = 0;
        reflow.ramp1S = 0;
        reflow.preHeatS = 0;
        reflow.ramp2S = 0;
        reflow.reflowS = 0;
        reflow.coolingS = 0;
        reflow.phase = IDLE;
        reflow.prevError = 0;
        reflow.derivative = 0;
        reflow.startTimeMs = HAL_GetTick ();
        reflow.preHeatTemp = 0;
        reflow.reflowTemp = 0;
}

/** System Clock Configuration
*/
void SystemClock_Config (void)
{

        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_PeriphCLKInitTypeDef PeriphClkInit;

        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
        RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK) {
                Error_Handler ();
        }

        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
        if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
                Error_Handler ();
        }

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
        if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInit) != HAL_OK) {
                Error_Handler ();
        }

        HAL_SYSTICK_Config (HAL_RCC_GetHCLKFreq () / 1000);

        HAL_SYSTICK_CLKSourceConfig (SYSTICK_CLKSOURCE_HCLK);

        /* SysTick_IRQn interrupt configuration */
        HAL_NVIC_SetPriority (SysTick_IRQn, 0, 0);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init (void)
{

        GPIO_InitTypeDef GPIO_InitStruct;

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOA_CLK_ENABLE ();
        __HAL_RCC_GPIOB_CLK_ENABLE ();

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin (relay_GPIO_Port, relay_Pin, GPIO_PIN_RESET);

        /*Configure GPIO pin : relay_Pin */
        GPIO_InitStruct.Pin = relay_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init (relay_GPIO_Port, &GPIO_InitStruct);
}

// static void MX_USART1_UART_Init (void)
//{

//        huart1.Instance = USART1;
//        huart1.Init.BaudRate = 9600;
//        huart1.Init.WordLength = UART_WORDLENGTH_8B;
//        huart1.Init.StopBits = UART_STOPBITS_1;
//        huart1.Init.Parity = UART_PARITY_NONE;
//        huart1.Init.Mode = UART_MODE_TX_RX;
//        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//        huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//        huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//        if (HAL_UART_Init (&huart1) != HAL_OK) {
//                Error_Handler ();
//        }
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler (void)
{
        /* USER CODE BEGIN Error_Handler */
        /* User can add his own implementation to report the HAL error return state */
        while (1) {
        }
        /* USER CODE END Error_Handler */
}
