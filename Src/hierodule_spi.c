/**
  ******************************************************************************
  * @file           : hierodule_spi.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the SPI module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_spi.h>

/** @addtogroup Hierodule_SPI SPI Module
  * @{
  */

/** @addtogroup SPI_Private Static
  * @brief @rv_global_private_brief{are} @rv_corresponds_exc_irqs{source}
  * @details @rv_static_wrap_det{SPI}\n
  * @rv_irq_defs_eof
  * @{
  */

/** @brief SPI wrapper pointer meant for SPI1.
  */
static HIERODULE_SPI_Wrapper *SPI1_Wrapper;

/** @brief SPI wrapper pointer meant for SPI2.\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  */
/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
static HIERODULE_SPI_Wrapper *SPI2_Wrapper;
/** \cond */
#endif /** \endcond */

/** @brief SPI wrapper pointer meant for SPI3.\n
  * @rv_def_req_device{__STM32F401xC_H}
  */
/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
static HIERODULE_SPI_Wrapper *SPI3_Wrapper;
/** \cond */
#endif /** \endcond */

/** @brief Reads and returns the data register content of the SPI peripheral.
  * @rv_param_wrapper_ptr{SPI}
  * @return uint8_t The byte returned.
  * @details Defined as static to avoid the multiple definition error some
  * compilers throw since a function with the same name also exists within
  * another module; that will be fixed in a future release.
  */
static uint8_t ReceiveData(HIERODULE_SPI_Wrapper *Wrapper)
{
    return (uint8_t)(Wrapper->_SPI->DR);
}


/** @brief Enables the SPI peripheral.
  * @rv_param_wrapper_ptr{SPI}
  * @return None
  * @details @rv_obvious
  */
void Enable(HIERODULE_SPI_Wrapper *Wrapper)
{
    SET_BIT(Wrapper->_SPI->CR1, SPI_CR1_SPE);
}

/** @brief Disables the SPI peripheral.
  * @rv_param_wrapper_ptr{SPI}
  * @return None
  * @details Defined as static to avoid the multiple definition error some
  * compilers throw since a function with the same name also exists within
  * another module; that will be fixed in a future release.
  */
static void Disable(HIERODULE_SPI_Wrapper *Wrapper)
{
    CLEAR_BIT(Wrapper->_SPI->CR1, SPI_CR1_SPE);
}

/**
  * @}
  */

/** @addtogroup SPI_Public Global
  * @{
  */

/** @details @ref HIERODULE_SPI_Wrapper::RX_New "RX_New" and @ref
  * HIERODULE_SPI_Wrapper::RX_Index "RX_Index" are also updated on call, to
  * revolve the ring buffer, so as to say.
  */
uint8_t HIERODULE_SPI_GetNextByte(HIERODULE_SPI_Wrapper *Wrapper)
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
        return 0;
    }
}

/** @details @rv_obvious
  */
void HIERODULE_SPI_TransmitByte(HIERODULE_SPI_Wrapper *Wrapper, uint8_t Byte)
{
    *((volatile uint8_t*) &(Wrapper->_SPI->DR)) = Byte;
}

/** @details The ring buffer gets a new address allocated to it and is filled with
  * null characters.\n
  * @rv_wrapper_future_release{SPI,HIERODULE_SPI_ReleaseWrapper}
  */
HIERODULE_SPI_Wrapper **HIERODULE_SPI_InitWrapper( SPI_TypeDef *_SPI, uint8_t Mode, uint16_t RX_BufferSize, void (*TC_Handler)(void) )
{
    if( _SPI == NULL )
    {
        return NULL;
    }

    HIERODULE_SPI_Wrapper **Wrapper;

    uint32_t SPI_Address = (uint32_t)_SPI;

    switch(SPI_Address)
    {
        case ( (uint32_t)SPI1 ):
            Wrapper = &SPI1_Wrapper;
            break;
        /** \cond */
        #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
        case ( (uint32_t)SPI2 ):
            Wrapper = &SPI2_Wrapper;
            break;
        /** \cond */
        #endif
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)SPI3 ):
            Wrapper = &SPI3_Wrapper;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            return NULL;
            break;
    }

    (*Wrapper) = (HIERODULE_SPI_Wrapper*)malloc(sizeof(HIERODULE_SPI_Wrapper));

    (*Wrapper)->_SPI = _SPI;

    (*Wrapper)->RX_Index = 0;
    (*Wrapper)->RX_New = 0;

    (*Wrapper)->RX_BufferSize = RX_BufferSize;
    (*Wrapper)->RX_Buffer = (uint8_t*)malloc( ((*Wrapper)->RX_BufferSize) * sizeof(uint8_t));
    for( uint16_t _rxb_init = 0 ; _rxb_init < (*Wrapper)->RX_BufferSize ; _rxb_init++ )
    {
        ((*Wrapper)->RX_Buffer)[_rxb_init] = '\0';
    }

    (*Wrapper)->TC_Handler = TC_Handler;

    (*Wrapper)->Mode = Mode;

    if( (*Wrapper)->Mode == 1 )
    {
        SET_BIT((*Wrapper)->_SPI->CR2, SPI_CR2_TXEIE);
    }
    else
    {
        SET_BIT((*Wrapper)->_SPI->CR2, SPI_CR2_RXNEIE);
        Enable(*Wrapper);
        (*Wrapper)->TX_Counter = 0;
        HIERODULE_SPI_TransmitByte(*Wrapper, 0);
    }

    return Wrapper;
}

/** @details Buffer address also gets freed.\n
  * @rv_wrapper_warn_release_det{SPI}
  */
void HIERODULE_SPI_ReleaseWrapper(HIERODULE_SPI_Wrapper *Wrapper)
{
    free(Wrapper->RX_Buffer);
    Wrapper->RX_Buffer = NULL;

    free(Wrapper);
    Wrapper = NULL;
}

/** @details Transmission won't commence if Size is zero.
  */
void HIERODULE_SPI_TransmitPackage(HIERODULE_SPI_Wrapper *Wrapper, uint8_t *TX_Buffer, uint32_t Size)
{
    if(Size == 0)
        return;

    Wrapper->TX_Buffer = TX_Buffer;
    Wrapper->TX_BufferSize = Size;
    Wrapper->TX_Counter = 0;

    Enable(Wrapper);
}

/**
  * @}
  */

/** @addtogroup SPI_Private Static
  * @{
  */
/** @brief The base IRQ body to be used for all SPI IRQs.
  * @rv_param_wrapper_ptr{SPI}
  * @return None
  * @details Transmission completion callback routine gets Called at the end of
  * an entire transmission in master mode. As for slave mode, the routine gets
  * invoked at the end of each byte transmission.
  */
void SPI_IRQ_Handler(HIERODULE_SPI_Wrapper *Wrapper)
{
    if( Wrapper->Mode == 1 )
    {
        if( READ_BIT(Wrapper->_SPI->CR1, SPI_CR1_SPE) == (SPI_CR1_SPE) )
        {
            if( READ_BIT(Wrapper->_SPI->SR, SPI_SR_TXE) == (SPI_SR_TXE) )
            {
                HIERODULE_SPI_TransmitByte(Wrapper, Wrapper->TX_Buffer[Wrapper->TX_Counter++]);
                while( READ_BIT(Wrapper->_SPI->SR, SPI_SR_TXE) != (SPI_SR_TXE) );

                while( READ_BIT(Wrapper->_SPI->SR, SPI_SR_RXNE) != (SPI_SR_RXNE) );
                Wrapper->RX_Buffer[Wrapper->RX_Index++] = ReceiveData(Wrapper);
                Wrapper->RX_Index %= Wrapper->RX_BufferSize;
                if( Wrapper->RX_New < Wrapper->RX_BufferSize )
                    Wrapper->RX_New++;

                if( Wrapper->TX_Counter == Wrapper->TX_BufferSize )
                {
                    while( READ_BIT(Wrapper->_SPI->SR, SPI_SR_BSY) == (SPI_SR_BSY) );
                    Disable(Wrapper);
                    if( Wrapper->TC_Handler != NULL )
                        Wrapper->TC_Handler();
                }
            }
        }
    }
    else if( Wrapper->Mode == 0 )
    {
        if( READ_BIT(Wrapper->_SPI->SR, SPI_SR_RXNE) == (SPI_SR_RXNE) )
        {
            Wrapper->RX_Buffer[Wrapper->RX_Index++] = ReceiveData(Wrapper);
            Wrapper->RX_Index %= Wrapper->RX_BufferSize;
            if( Wrapper->RX_New < Wrapper->RX_BufferSize )
                Wrapper->RX_New++;

            HIERODULE_SPI_TransmitByte(Wrapper, Wrapper->TX_Buffer[Wrapper->TX_Counter++]);

            if( Wrapper->TC_Handler != NULL )
                Wrapper->TC_Handler();
        }
    }
}

/**
  * @}
  */

/** @addtogroup SPI_Public Global
  * @{
  */
            
/** @brief @rv_irq_imp_bri{SPI1}
  * @return None
  * @details @rv_obvious
  */
extern void SPI1_IRQHandler(void)
{
    if( SPI1_Wrapper != NULL )
        SPI_IRQ_Handler(SPI1_Wrapper);
}

/** @brief @rv_irq_imp_bri{SPI2}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
extern void SPI2_IRQHandler(void)
{
    if( SPI2_Wrapper != NULL )
        SPI_IRQ_Handler(SPI2_Wrapper);
}
/** \cond */
#endif /** \endcond */

/** @brief @rv_irq_imp_bri{SPI3}\n
  * @rv_def_req_device{__STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
extern void SPI3_IRQHandler(void)
{
    if( SPI3_Wrapper != NULL )
        SPI_IRQ_Handler(SPI3_Wrapper);
}
/** \cond */
#endif /** \endcond */
/**
  * @}
  */
/**
  * @}
  */



