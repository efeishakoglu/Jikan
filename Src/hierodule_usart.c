/**
  ******************************************************************************
  * @file           : hierodule_usart.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the USART module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_usart.h>

/** @addtogroup Hierodule_Usart USART Module
  * @{
  */

/** @addtogroup USART_Private Static
  * @brief @rv_global_private_brief{are} @rv_corresponds_exc_irqs{source}
  * @details @rv_static_wrap_det{USART}\n
  * @rv_irq_defs_eof
  * @{
  */

/** @brief USART wrapper pointer meant for USART1.
  */
static HIERODULE_USART_Wrapper *USART1_Wrapper = NULL;

/** @brief USART wrapper pointer meant for USART2.\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  */
/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
static HIERODULE_USART_Wrapper *USART2_Wrapper = NULL;
/** \cond */
#endif /** \endcond */

/** @brief USART wrapper pointer meant for USART3.\n
  * @rv_def_req_device{__STM32F103xB_H}
  */
/** \cond */
#ifdef __STM32F103xB_H /** \endcond */
static HIERODULE_USART_Wrapper *USART3_Wrapper = NULL;
/** \cond */
#endif /** \endcond */

/** @brief USART wrapper pointer meant for USART6.\n
  * @rv_def_req_device{__STM32F401xC_H}
  */
/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
static HIERODULE_USART_Wrapper *USART6_Wrapper = NULL;
/** \cond */
#endif /** \endcond */

/** @brief Reads and returns a single byte received by the USART peripheral.
  * @rv_param_wrapper_ptr{USART}
  * @return Byte read from RDR.
  * @details @rv_static_for_irq
  */
uint8_t ReceiveByte(HIERODULE_USART_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    return (uint8_t)(READ_BIT(Wrapper->USART->RDR, USART_RDR_RDR));
    /** \cond */
    #else /** \endcond */
    return (uint8_t)(READ_BIT(Wrapper->USART->DR, USART_DR_DR));
    /** \cond */
    #endif /** \endcond */
}

/**
  * @}
  */

/** @addtogroup USART_Public Global
  * @{
  */

/** @details The ring buffer gets a new address allocated to it.\n
  * The ring buffer is filled with null characters.\n
  * Disables the RE bit of the control register, as well as the RXNE flag in
  * the status register.\n
  * @rv_wrapper_future_release{USART,HIERODULE_USART_ReleaseWrapper}
  */
HIERODULE_USART_Wrapper **HIERODULE_USART_InitWrapper
(
    USART_TypeDef *USART,
    uint16_t RX_BufferSize,
    void (*RX_Handler)(uint8_t)
)
{
    if( USART == NULL )
    {
        return NULL;
    }

    HIERODULE_USART_Wrapper **Wrapper;

    uint32_t USART_Address = (uint32_t)USART;

    switch(USART_Address)
    {
        case ( (uint32_t)USART1 ):
            Wrapper = &USART1_Wrapper;
            break;
        /** \cond */
        #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
        case ( (uint32_t)USART2 ):
            Wrapper = &USART2_Wrapper;
            break;
        /** \cond */
        #endif
        #ifdef __STM32F103xB_H /** \endcond */
        case ( (uint32_t)USART3 ):
            Wrapper = &USART3_Wrapper;
            break;
        /** \cond */
        #endif
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)USART6 ):
            Wrapper = &USART6_Wrapper;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            return NULL;
            break;
    }

    (*Wrapper) = (HIERODULE_USART_Wrapper*)malloc(sizeof(HIERODULE_USART_Wrapper));

    (*Wrapper)->USART = USART;

    (*Wrapper)->RX_Index = 0;
    (*Wrapper)->RX_New = 0;

    (*Wrapper)->RX_BufferSize = RX_BufferSize;
    (*Wrapper)->RX_Buffer = (uint8_t*)malloc( ((*Wrapper)->RX_BufferSize) * sizeof(uint8_t));
    for( uint16_t _rxb_init = 0 ; _rxb_init < (*Wrapper)->RX_BufferSize ; _rxb_init++ )
    {
        ((*Wrapper)->RX_Buffer)[_rxb_init] = '\0';
    }
    (*Wrapper)->RX_Handler = RX_Handler;

    CLEAR_BIT((*Wrapper)->USART->CR1, USART_CR1_RE);

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    CLEAR_BIT((*Wrapper)->USART->ISR, USART_ISR_RXNE);
    /** \cond */
    #else /** \endcond */
    CLEAR_BIT((*Wrapper)->USART->SR, USART_SR_RXNE);
    /** \cond */
    #endif /** \endcond */

    return Wrapper;
}

/** @details Ring buffer address is also freed.\n
  * @rv_wrapper_warn_release_det{USART}
  */
void HIERODULE_USART_ReleaseWrapper(HIERODULE_USART_Wrapper *Wrapper)
{

    CLEAR_BIT(Wrapper->USART->CR1, USART_CR1_RE);

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    CLEAR_BIT(Wrapper->USART->ISR, USART_ISR_RXNE);
    /** \cond */
    #else /** \endcond */
    CLEAR_BIT(Wrapper->USART->SR, USART_SR_RXNE);
    /** \cond */
    #endif /** \endcond */

    free(Wrapper->RX_Buffer);
    Wrapper->RX_Buffer = NULL;

    free(Wrapper);
    Wrapper = NULL;
}
 
/** @details @rv_obvious
  */
void HIERODULE_USART_Enable_IT_RXNE(HIERODULE_USART_Wrapper *Wrapper)
{
    SET_BIT(Wrapper->USART->CR1, USART_CR1_RXNEIE);
    SET_BIT(Wrapper->USART->CR1, USART_CR1_RE);
}

/** @details @rv_obvious
  */
void HIERODULE_USART_Disable_IT_RXNE(HIERODULE_USART_Wrapper *Wrapper)
{
    CLEAR_BIT(Wrapper->USART->CR1, USART_CR1_RXNEIE);
    CLEAR_BIT(Wrapper->USART->CR1, USART_CR1_RE);
}

/** @details @rv_bit_is_set_det{USART_ISR_RXNE/USART_SR_RXNE}
  */
uint32_t HIERODULE_USART_IsActiveFlag_RXNE(HIERODULE_USART_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    return (READ_BIT(Wrapper->USART->ISR, USART_ISR_RXNE) == (USART_ISR_RXNE));
    /** \cond */
    #else /** \endcond */
    return (READ_BIT(Wrapper->USART->SR, USART_SR_RXNE) == (USART_SR_RXNE));
    /** \cond */
    #endif /** \endcond */
}

/** @details @rv_bit_is_set_det{USART_ISR_TXE/USART_SR_TXE}
  */
uint32_t HIERODULE_USART_IsActiveFlag_TXE(HIERODULE_USART_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    return (READ_BIT(Wrapper->USART->ISR, USART_ISR_TXE) == (USART_ISR_TXE));
    /** \cond */
    #else /** \endcond */
    return (READ_BIT(Wrapper->USART->SR, USART_SR_TXE) == (USART_SR_TXE));
    /** \cond */
    #endif /** \endcond */
}

/** @details @ref HIERODULE_USART_Wrapper::RX_New "RX_New" and @ref
  * HIERODULE_USART_Wrapper::RX_Index "RX_Index" are also updated on call, to
  * revolve the ring buffer, so as to say.
  */
uint8_t HIERODULE_USART_GetNextByte(HIERODULE_USART_Wrapper *Wrapper)
{
    if(Wrapper->RX_New > 0)
    {
        Wrapper->RX_New--;

        return Wrapper->RX_Buffer
        [
            (Wrapper->RX_Index + Wrapper->RX_BufferSize - (Wrapper->RX_New+1) )
            % Wrapper->RX_BufferSize
        ];
    }
    else
    {
        return '\0';
    }
}

/** @details Will block until both RDR and TDR are empty, which means it's safe
  * to write to the transmit data register.\n
  * @rv_bit_assumption_usart{TE}
  */
void HIERODULE_USART_TransmitByte(HIERODULE_USART_Wrapper *Wrapper, uint8_t Byte)
{
    while
    (
        ( HIERODULE_USART_IsActiveFlag_RXNE(Wrapper) )
        ||
        ( !(HIERODULE_USART_IsActiveFlag_TXE(Wrapper)) )
    );

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    Wrapper->USART->TDR = Byte;
    /** \cond */
    #else /** \endcond */
    Wrapper->USART->DR = Byte;
    /** \cond */
    #endif /** \endcond */
}

/** @details Basically calls @ref HIERODULE_USART_TransmitByte
  * "HIERODULE_USART_TransmitByte" for each byte in the string
  * until a null character shows up.\n
  * @rv_bit_assumption_usart{TE}
  */
void HIERODULE_USART_TransmitString(HIERODULE_USART_Wrapper *Wrapper, char *String)
{
    volatile uint32_t _ctr = 0;
    while( 1 )
    {
        if( String[_ctr] == '\0' )
        {
            break;
        }
        else
        {
            HIERODULE_USART_TransmitByte( Wrapper, String[_ctr] );
            _ctr++;
        }
    }
}

/**
  * @}
  */

/** @addtogroup USART_Private Static
  * @{
  */

/** @brief The base IRQ body to be used for all USART IRQs.
  * @rv_param_wrapper_ptr{USART}
  * @return None
  * @details Byte received in the RDR is not handled if the ISR has not been
  * assigned.\n
  * If the wrapper is indeed assigned, the received byte is loaded at the next
  * index of the ring buffer and the ISR @ref HIERODULE_USART_Wrapper::RX_Handler
  * "HIERODULE_USART_Wrapper::RX_Handler" is called if it's not NULL.
  */
void USART_IRQHandler(HIERODULE_USART_Wrapper *Wrapper)
{
    if( HIERODULE_USART_IsActiveFlag_RXNE(Wrapper) )
    {
        if(Wrapper != NULL)
        {
            Wrapper->RX_Buffer[Wrapper->RX_Index] =
            ReceiveByte(Wrapper);

            Wrapper->RX_Index++;
            Wrapper->RX_Index %= Wrapper->RX_BufferSize;

            if( Wrapper->RX_New < Wrapper->RX_BufferSize )
                Wrapper->RX_New++;

            if(Wrapper->RX_Handler != NULL)
            {
                Wrapper->RX_Handler
                (
                    HIERODULE_USART_GetNextByte(Wrapper)
                );
            }
        }
    }
}
/**
  * @}
  */

/** @addtogroup USART_Public Global
  * @{
  */

/** @brief @rv_irq_imp_bri{USART1}
  * @return None
  * @details @rv_obvious
  */
extern void USART1_IRQHandler(void)
{
    USART_IRQHandler(USART1_Wrapper);
}

/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
/** @brief @rv_irq_imp_bri{USART2}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
extern void USART2_IRQHandler(void)
{
    USART_IRQHandler(USART2_Wrapper);
}
/** \cond */
#endif

#ifdef __STM32F103xB_H /** \endcond */
/** @brief @rv_irq_imp_bri{USART3}\n
  * @rv_def_req_device{__STM32F103xB_H}
  * @return None
  * @details @rv_obvious
  */
extern void USART3_IRQHandler(void)
{
    USART_IRQHandler(USART3_Wrapper);
}
/** \cond */
#endif

#ifdef __STM32F401xC_H /** \endcond */
/** @brief @rv_irq_imp_bri{USART6}\n
  * @rv_def_req_device{__STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
extern void USART6_IRQHandler(void)
{
    USART_IRQHandler(USART6_Wrapper);
}
/** \cond */
#endif /** \endcond */

/**
  * @}
  */
/**
  * @}
  */


