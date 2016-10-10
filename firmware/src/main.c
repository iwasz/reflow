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
static void MX_USART1_UART_Init (void);

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

        gpioInitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
        gpioInitStruct.Mode = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpioInitStruct.Alternate = GPIO_AF0_SPI1;
        HAL_GPIO_Init (GPIOA, &gpioInitStruct);

        hspi1.Instance = SPI1;
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
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

        USBD_Init (&usbdDevice, &usbDescriptorsVirtualTable, 0);
        USBD_RegisterClass (&usbdDevice, &vendorClass);
        USBD_Start (&usbdDevice);

        reflow.actualTemp = 0;
        reflow.internalTemp = 0;

        HAL_Delay (500);

        while (1) {
#if 0
                relay_GPIO_Port->BSRR |= relay_Pin;
                //                HAL_Delay (500);

                for (int i = 0; i < 1000000; ++i)
                        ;

                relay_GPIO_Port->BSRR |= relay_Pin << 16;

                for (int i = 0; i < 1000000; ++i)
                        ;
                //                HAL_Delay (500);
#endif

#if 1

                hspi1.Instance->DR = 0xffff;

                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;

                reflow.rawData = (*(volatile uint16_t *)&hspi1.Instance->DR) << 16;

                hspi1.Instance->DR = 0xffff;

                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;

                reflow.rawData |= (*(volatile uint16_t *)&hspi1.Instance->DR);

                reflow.actualTemp = (int16_t) (reflow.rawData >> 18);
                reflow.actualTemp /= 4;

                reflow.internalTemp = (int16_t) ((reflow.rawData & 0xffff) >> 4);
                reflow.internalTemp /= 16;

                HAL_Delay (1000);
#endif
        }
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

static void MX_USART1_UART_Init (void)
{

        huart1.Instance = USART1;
        huart1.Init.BaudRate = 9600;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init (&huart1) != HAL_OK) {
                Error_Handler ();
        }
}

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

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed (uint8_t *file, uint32_t line)
{
        /* USER CODE BEGIN 6 */
        /* User can add his own implementation to report the file name and line number,
          ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
