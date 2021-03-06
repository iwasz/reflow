CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (CMAKE_VERBOSE_MAKEFILE OFF)
include (stm32.cmake)

PROJECT (reflow)

# +-----------------+
# | My code         |
# +-----------------+
INCLUDE_DIRECTORIES("src/")
INCLUDE_DIRECTORIES("../common/")
LIST (APPEND APP_SOURCES "src/main.c")
LIST (APPEND APP_SOURCES "src/config.h")
LIST (APPEND APP_SOURCES "../common/reflow.h")
LIST (APPEND APP_SOURCES "src/mxconstants.h")
LIST (APPEND APP_SOURCES "src/stm32f0xx_hal_conf.h")
LIST (APPEND APP_SOURCES "src/stm32f0xx_hal_msp.c")
LIST (APPEND APP_SOURCES "src/syscalls.c")
LIST (APPEND APP_SOURCES "src/stm32f0xx_it.c")
LIST (APPEND APP_SOURCES "src/stm32f0xx_it.h")
LIST (APPEND APP_SOURCES "src/system_stm32f0xx.c")

INCLUDE_DIRECTORIES("src/usb")
LIST (APPEND APP_SOURCES "src/usb/usbd_conf.c")
LIST (APPEND APP_SOURCES "src/usb/usbd_conf.h")
LIST (APPEND APP_SOURCES "src/usb/usbd_desc.c")
LIST (APPEND APP_SOURCES "src/usb/usbd_desc.h")
LIST (APPEND APP_SOURCES "src/usb/usbd_it.c")
LIST (APPEND APP_SOURCES "src/usb/vendor_specific_class.c")
LIST (APPEND APP_SOURCES "src/usb/vendor_specific_class.h")
LIST (APPEND APP_SOURCES "src/usb/string_utils.c")
LIST (APPEND APP_SOURCES "src/usb/string_utils.h")

# +--------------+
# | STM Cube     |
# +--------------+
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_can.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cec.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_comp.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2s.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_irda.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_iwdg.c")
#LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_msp_template.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smbus.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tsc.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_usart.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_wwdg.c")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_iwdg.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rcc.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_def.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pwr_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_smartcard.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pcd.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_wwdg.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2s.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tsc.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_usart.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rtc.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_adc_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_crc.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pwr.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dac_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_irda_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_irda.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_smbus.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_adc.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_spi_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_can.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_cec.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rcc_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_pcd_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_comp.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dac.h")
#LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_conf_template.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_usart_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_spi.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_smartcard_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_crc_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_rtc_ex.h")
LIST (APPEND APP_SOURCES "${CUBE_ROOT}/Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_cortex.h")

LIST (APPEND APP_SOURCES "${STARTUP_CODE}")
ADD_DEFINITIONS ("-DHSE_VALUE=${CRYSTAL_HZ}")

ADD_DEFINITIONS (-DUSE_USB_FS)
INCLUDE_DIRECTORIES("${USB_LIB}/Core/Inc")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Src/usbd_core.c")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Src/usbd_ctlreq.c")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Src/usbd_ioreq.c")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Inc/usbd_core.h")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Inc/usbd_ctlreq.h")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Inc/usbd_def.h")
LIST (APPEND APP_SOURCES "${USB_LIB}/Core/Inc/usbd_ioreq.h")

#INCLUDE_DIRECTORIES("${USB_LIB}/Class/HID/Inc")
#LIST (APPEND APP_SOURCES "${USB_LIB}/Class/HID/Inc/usbd_hid.h")
#LIST (APPEND APP_SOURCES "${USB_LIB}/Class/HID/Src/usbd_hid.c")

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${APP_SOURCES})
SET (CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy)
SET (CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)

FIND_PROGRAM (ST_FLASH st-flash)
ADD_CUSTOM_TARGET("upload" DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${ST_FLASH} --reset write ${CMAKE_PROJECT_NAME}.bin 0x8000000)
