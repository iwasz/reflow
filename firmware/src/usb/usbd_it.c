/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <stdint.h>
#include <stm32f0xx_hal.h>

extern PCD_HandleTypeDef hpcd;
// extern TIM_HandleTypeDef usbTimHandle;

/**
 * @brief  This function handles USB-On-The-Go FS global interrupt request.
 * @param  None
 * @retval None
 */
void USB_IRQHandler (void) { HAL_PCD_IRQHandler (&hpcd); }

/**
 * @brief  This function handles TIM interrupt request.
 * @param  None
 * @retval None
 */
// void TIM3_IRQHandler (void) { HAL_TIM_IRQHandler (&usbTimHandle); }
