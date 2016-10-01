/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com (based on ST).                   *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <stdint.h>
#include <stm32f0xx_hal.h>
#include "vendor_specific_interface.h"
#include <string.h>

/*---------------------------------------------------------------------------*/

/* USB handler declaration */
extern USBD_HandleTypeDef usbdDevice;

/* Private function prototypes -----------------------------------------------*/
static int8_t usbInterfaceInit (void);
static int8_t usbInterfaceDeInit (void);
static int8_t usbInterfaceControl (uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t usbInterfaceReceive (uint8_t *pbuf, uint32_t *Len);
static void usbTimerConfig (void);

USBD_CDC_ItfTypeDef usbInterfaceFops = { usbInterfaceInit, usbInterfaceDeInit, usbInterfaceControl, usbInterfaceReceive };

/**
 * @brief  Initializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t usbInterfaceInit (void)
{
        // ?????????
//        usbTimerConfig ();
// ?????????
//        usbClassSetTxBuffer (&usbdDevice, usbTxBuffer, 0);
//        usbClassSetRxBuffer (&usbdDevice, NULL);
        return (USBD_OK);
}

/**
 * @brief  CDC_Itf_DeInit
 *         DeInitializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t usbInterfaceDeInit (void)
{
        /* DeInitialize the UART peripheral */
        //        if (HAL_UART_DeInit (&UartHandle) != HAL_OK) {
        //                /* Initialization Error */
        //                Error_Handler ();
        //        }
        return (USBD_OK);
}

/**
 * @brief  CDC_Itf_Control
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t usbInterfaceControl (uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
//        switch (cmd) {
//        case CDC_SEND_ENCAPSULATED_COMMAND:
//                /* Add your code here */
//                break;

//        case CDC_GET_ENCAPSULATED_RESPONSE:
//                /* Add your code here */
//                break;

//        case CDC_SET_COMM_FEATURE:
//                /* Add your code here */
//                break;

//        case CDC_GET_COMM_FEATURE:
//                /* Add your code here */
//                break;

//        case CDC_CLEAR_COMM_FEATURE:
//                /* Add your code here */
//                break;

//        case CDC_SET_LINE_CODING:
//                LineCoding.bitrate = (uint32_t) (pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
//                LineCoding.format = pbuf[4];
//                LineCoding.paritytype = pbuf[5];
//                LineCoding.datatype = pbuf[6];

//                /* Set the new configuration */
//                ComPort_Config ();
//                break;

//        case CDC_GET_LINE_CODING:
//                pbuf[0] = (uint8_t) (LineCoding.bitrate);
//                pbuf[1] = (uint8_t) (LineCoding.bitrate >> 8);
//                pbuf[2] = (uint8_t) (LineCoding.bitrate >> 16);
//                pbuf[3] = (uint8_t) (LineCoding.bitrate >> 24);
//                pbuf[4] = LineCoding.format;
//                pbuf[5] = LineCoding.paritytype;
//                pbuf[6] = LineCoding.datatype;
//                break;

//        case CDC_SET_CONTROL_LINE_STATE:
//                /* Add your code here */
//                break;

//        case CDC_SEND_BREAK:
//                /* Add your code here */
//                break;

//        default:
//                break;
//        }

        return (USBD_OK);
}

///**
// * Procedura (no taki callback wołany z procedury ISR) obsługi przerwania zgłaszanego
// * przez timer. To się wykonuje 100 lub 200 razy na sek i wysyła bufory przez USB-CDC
// * i opróżnia je.
// */
//void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
//{
//        uint8_t *pa, *pb;
//        size_t la, lb;

//        __disable_irq (); // Wyłączamy, bo każde inne przerwanie o wyższym prio. może wywołać usbTransmitInterrupt.
//        io_buffer_retrieve (usbTxBuffer, &pa, &la, &pb, &lb, GPA_MAX_PACKET_SIZE - 1);
//        __enable_irq ();

//        if (pa) {
//                usbClassSetTxBuffer (&usbdDevice, pa, la);

//                if (usbClassTransmitPacket (&usbdDevice) == USBD_OK) {
//                        io_buffer_declare_read (usbTxBuffer, la);
//                }
//        }

//        if (pb) {
//                usbClassSetTxBuffer (&usbdDevice, pb, lb);

//                if (usbClassTransmitPacket (&usbdDevice) == USBD_OK) {
//                        io_buffer_declare_read (usbTxBuffer, lb);
//                }
//        }

//#if 0
////        __disable_irq ();
//        uint16_t offsetInCopy = offsetIn;
////        __enable_irq ();

//        uint16_t len;
//        if (offsetOut < offsetInCopy) {
//                len = offsetInCopy - offsetOut;

//                /*
//                 * Ustawia wewnętrzny bufor :
//                 *   USBD_Device->pClassData->TxBuffer = UserTxBuffer + UserTxBufPtrOut
//                 *
//                 * i jego długość :
//                 *   USBD_Device->pClassData->TxLength = buffsize
//                 */
//                usbClassSetTxBuffer (&usbdDevice, usbTxBuffer + offsetOut, len);

//                // Wysyła z wewnętrznego bufora USBD_Device->pClassData->TxBuffer paczkę danych o długości USBD_Device->pClassData->TxLength
//                if (usbClassTransmitPacket (&usbdDevice) == USBD_OK) {
//                        offsetOut = (offsetOut + len) % USB_TX_DATA_SIZE;
//                }
//        }
//        else if (offsetOut > offsetInCopy) {
//                len = USB_TX_DATA_SIZE - offsetOut;
//                usbClassSetTxBuffer (&usbdDevice, usbTxBuffer + offsetOut, len);

//                if (usbClassTransmitPacket (&usbdDevice) == USBD_OK) {
//                        offsetOut = 0;
//                }

//                len = offsetInCopy - offsetOut;
//                usbClassSetTxBuffer (&usbdDevice, usbTxBuffer, len);

//                if (usbClassTransmitPacket (&usbdDevice) == USBD_OK) {
//                        offsetOut = (offsetOut + len) % USB_TX_DATA_SIZE;
//                }
//        }
//#endif
//}

/*****************************************************************************/

//void usbTransmitInterrupt (uint8_t *buf, size_t len)
//{
//        HAL_NVIC_DisableIRQ (TIM_USB_IRQn);
//        io_buffer_store (usbTxBuffer, buf, len);
//        HAL_NVIC_EnableIRQ (TIM_USB_IRQn);
//}

/**
 * @brief  CDC_Itf_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be transmitted
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t usbInterfaceReceive (uint8_t *Buf, uint32_t *Len)
{
        //        HAL_UART_Transmit_DMA (&UartHandle, Buf, *Len);
        return (USBD_OK);
}

/**
 * @brief  TIM_Config: Configure TIM_CDC timer
 * @param  None.
 * @retval None
 */
//static void usbTimerConfig (void)
//{
//        /* Set TIM_CDC instance */
//        usbTimHandle.Instance = TIM_USB;

//        /* Initialize TIM3 peripheral as follows:
//         + Period = (CDC_POLLING_INTERVAL * 10000) - 1
//         + Prescaler = ((APB1 frequency / 1000000) - 1)
//         + ClockDivision = 0
//         + Counter direction = Up  */
//        usbTimHandle.Init.Period = (USB_POLLING_INTERVAL * 1000) - 1;
//        usbTimHandle.Init.Prescaler = 84 - 1;
//        usbTimHandle.Init.ClockDivision = 0;
//        usbTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

//        if (HAL_TIM_Base_Init (&usbTimHandle) != HAL_OK) {
//                /* Initialization Error */
//                Error_Handler ();
//        }

//        /* TIM_CDC Peripheral clock enable */
//        TIM_USB_CLK_ENABLE ();

//        /*##-2- Configure the NVIC for TIM_CDC ########################################*/
//        /* Set the TIM_CDC priority */
//        HAL_NVIC_SetPriority (TIM_USB_IRQn, 6, 1);

//        /* Enable the TIM_CDC global Interrupt */
//        HAL_NVIC_EnableIRQ (TIM_USB_IRQn);

//        if (HAL_TIM_Base_Start_IT (&usbTimHandle) != HAL_OK) {
//                Error_Handler ();
//        }
//}
