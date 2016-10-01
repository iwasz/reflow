#include "stm32f0xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "vendor_specific_class.h"
#include "vendor_specific_interface.h"

SPI_HandleTypeDef hspi1;
USBD_HandleTypeDef usbdDevice;
UART_HandleTypeDef huart1;

void SystemClock_Config (void);
void Error_Handler (void);
static void MX_GPIO_Init (void);
static void MX_SPI1_Init (void);
static void MX_USB_PCD_Init (void);
static void MX_USART1_UART_Init (void);

/*---------------------------------------------------------------------------*/

int main (void)
{
        HAL_Init ();

        SystemClock_Config ();

        MX_GPIO_Init ();
        MX_SPI1_Init ();
        MX_USART1_UART_Init ();

        for (int i = 0; i < 1000000; ++i)
                ;

//        USBD_Init (&usbdDevice, &HID_Desc, 0);
        USBD_Init (&usbdDevice, &usbDescriptorsVirtualTable, 0);
        USBD_RegisterClass (&usbdDevice, &USBD_HID);
//        usbClassRegisterInterface (&usbdDevice, &usbInterfaceFops);
        USBD_Start (&usbdDevice);

#define BUFFERSIZE 4
        uint8_t aRxBuffer[BUFFERSIZE];
        uint8_t aTxBuffer[BUFFERSIZE];

        while (1) {
                relay_GPIO_Port->BSRR |= relay_Pin;
                //                HAL_Delay (500);

                for (int i = 0; i < 1000000; ++i)
                        ;

                relay_GPIO_Port->BSRR |= relay_Pin << 16;

                for (int i = 0; i < 1000000; ++i)
                        ;
                //                HAL_Delay (500);

//                HAL_UART_Transmit (&aRxBufferhuart1, "test\n", 5, 5000);

#if 0
                GPIOA->BSRR |= GPIO_PIN_4 << 16;
                HAL_Delay (1);
                //                HAL_StatusTypeDef status = HAL_SPI_TransmitReceive (&hspi1, (uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, 4, 5000);

                hspi1.Instance->DR = (uint8_t)0xff;

                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;

                aRxBuffer[0] = hspi1.Instance->DR;
                hspi1.Instance->DR = (uint8_t)0xff;
                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;
                aRxBuffer[1] = hspi1.Instance->DR;
                hspi1.Instance->DR = (uint8_t)0xff;
                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;
                aRxBuffer[2] = hspi1.Instance->DR;
                hspi1.Instance->DR = (uint8_t)0xff;
                while (!(hspi1.Instance->SR & SPI_FLAG_RXNE))
                        ;
                aRxBuffer[3] = hspi1.Instance->DR;

                HAL_Delay (1);
                GPIOA->BSRR |= GPIO_PIN_4;

                //                if (status == HAL_OK) {
                int16_t temp = (((uint16_t)aRxBuffer[0] << 8) | (uint16_t)aRxBuffer[1]) >> 2;
                temp /= 4;
                printf ("%d\n", temp);
                relay_GPIO_Port->BSRR |= relay_Pin;
                //                }
                //                else {
                //                        Error_Handler ();
                //                }

                HAL_Delay (500);
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

/* SPI1 init function */
static void MX_SPI1_Init (void)
{

        hspi1.Instance = SPI1;
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
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
}

/* USB init function */
// static void MX_USB_PCD_Init (void)
//{

//        hpcd_USB_FS.Instance = USB;
//        hpcd_USB_FS.Init.dev_endpoints = 8;
//        hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
//        hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_64;
//        hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
//        hpcd_USB_FS.Init.low_power_enable = DISABLE;
//        hpcd_USB_FS.Init.lpm_enable = DISABLE;
//        hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
//        if (HAL_PCD_Init (&hpcd_USB_FS) != HAL_OK) {
//                Error_Handler ();
//        }
//}

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