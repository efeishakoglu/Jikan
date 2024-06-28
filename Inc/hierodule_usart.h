/**
  ******************************************************************************
  * @file           : hierodule_usart.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the USART module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_USART_H
#define __HIERODULE_USART_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Hierodule_Usart USART Module
  * @brief USART comm management
  * @details @rv_refer_to_usage{USART_Usage}
  * @{
  */
/** @addtogroup USART_Public Global
  * @brief @rv_global_private_brief{are not} @rv_corresponds_exc_irqs{header}
  * @details Consists of general USART comm routines, a USART wrapper
  * initalizer, a function to get the next byte in the ring buffer and a typedef
  * to be used for the wrapper routines.\n
  * @rv_inc_main\n
  * @rv_inc_headers{stddef.h and stdlib.h,NULL and malloc/free\, respectively}
  * @{
  */

#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/** @brief Struct that keeps variables for the ring buffer, a pointer to the
  * USART peripheral and an pointer to the ISR for RXNE.
  * @details Used by all routines in the module.\n
  * Alias typedefs for custom pointers are not defined, since they're not used
  * excessively in the module.
  */
typedef struct 
{
/** @brief Pointer to the USART peripheral.
  * @details @rv_usart_wrap_field
  */
    USART_TypeDef *USART;

/** @brief Index of next-byte-to-be-received in the ring buffer.
  * @details @rv_usart_wrap_field
  */
    uint32_t RX_Index;

/** @brief Number of new bytes in the ring buffer.
  * @details @rv_usart_wrap_field
  */
    uint32_t RX_New;

/** @brief The ring buffer.
  * @details @rv_usart_wrap_field
  */
    uint8_t *RX_Buffer;

/** @brief Number of elements in the ring buffer.
  * @details @rv_usart_wrap_field
  */
    uint16_t RX_BufferSize;

/** @brief Pointer to the ISR for RXNE.
  * @details Notice this points to a function with a uint8_t parameter, ergo the
  * ISR assigned must be declared accordingly, even if it won't use the received
  * byte in its definition.\n
  * This is the only field of the struct, manual changes on which are not
  * discouraged.
  */
    void (*RX_Handler)(uint8_t);
} HIERODULE_USART_Wrapper;

/** @brief Initializes a wrapper for the specified USART peripheral.
  * @param USART USART peripheral of the wrapper.
  * @param RX_BufferSize Length of the ring buffer array.
  * @param RX_Handler Pointer to the ISR for RXNE.
  * @return Double pointer to the initialized wrapper.
  */
HIERODULE_USART_Wrapper **HIERODULE_USART_InitWrapper
(
    USART_TypeDef *USART,
    uint16_t RX_BufferSize,
    void (*RX_Handler)(uint8_t)
);

/** @brief Frees the memory allocated to a USART wrapper and clears USART
  * status flags and control bits.
  * @rv_usart_wrapper_ptr
  * @return None
  */
void HIERODULE_USART_ReleaseWrapper(HIERODULE_USART_Wrapper *Wrapper);

/** @brief @rv_toggle_periph_it{Enables,RX not empty,the USART peripheral
  * \, also enables the RE bit of the control register.}
  * @rv_usart_wrapper_ptr
  * @return None
  */
void HIERODULE_USART_Enable_IT_RXNE(HIERODULE_USART_Wrapper *Wrapper);

/** @brief @rv_toggle_periph_it{Disables,RX not empty,the USART peripheral
  * \, also disables the RE bit of the control register.}
  * @rv_usart_wrapper_ptr
  * @return None
  */
void HIERODULE_USART_Disable_IT_RXNE(HIERODULE_USART_Wrapper *Wrapper);

/** @brief @rv_action_periph_it_flag{Checks, RX not empty, USART peripheral}
  * @rv_usart_wrapper_ptr
  * @return @rv_periph_it_ret
  */
uint32_t HIERODULE_USART_IsActiveFlag_RXNE(HIERODULE_USART_Wrapper *Wrapper);

/** @brief @rv_action_periph_it_flag{Checks, TX is empty, USART peripheral}
  * @rv_usart_wrapper_ptr
  * @return @rv_periph_it_ret
  */
uint32_t HIERODULE_USART_IsActiveFlag_TXE(HIERODULE_USART_Wrapper *Wrapper);

/** @brief Fetches the next byte in the ring buffer.
  * @rv_usart_wrapper_ptr
  * @return The next byte in the ring buffer, null character if there's none.
  */
uint8_t HIERODULE_USART_GetNextByte(HIERODULE_USART_Wrapper *Wrapper);

/** @brief Transmits a single byte.
  * @rv_usart_wrapper_ptr
  * @param Byte to be transmitted.
  * @return None
  */
void HIERODULE_USART_TransmitByte(HIERODULE_USART_Wrapper *Wrapper, uint8_t Byte);

/** @brief Transmits a string.
  * @rv_usart_wrapper_ptr
  * @param String to be transmitted.
  * @return None
  */
void HIERODULE_USART_TransmitString(HIERODULE_USART_Wrapper *Wrapper, char *String);

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_USART_H */


