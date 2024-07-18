/**
  ******************************************************************************
  * @file           : hierodule_i2c.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the I2C module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_i2c.h>

/** @addtogroup Hierodule_I2C I2C Module
  * @{
  */

/** @addtogroup I2C_Private Static
  * @brief @rv_global_private_brief{are} @rv_corresponds_exc_irqs{source}
  * @details @rv_static_wrap_det{I2C}\n
  * @rv_irq_defs_eof
  * @{
  */

/** @brief I2C wrapper pointer meant for I2C1.
  */
static HIERODULE_I2C_Wrapper *I2C1_Wrapper;

/** @brief I2C wrapper pointer meant for I2C2.\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  */
/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
static HIERODULE_I2C_Wrapper *I2C2_Wrapper;

/** \cond */
#endif /** \endcond */

/** @brief I2C wrapper pointer meant for I2C3.\n
  * @rv_def_req_device{__STM32F401xC_H}
  */
/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
static HIERODULE_I2C_Wrapper *I2C3_Wrapper;
/** \cond */
#endif /** \endcond */

/** @brief Blocks for given number of I2C clock periods.
  * @rv_param_wrapper_ptr{I2C}
  * @param NumberOfPeriods Number of I2C clock periods.
  * @return None
  * @details The period length is calculated beforehand with a @ref
  * HIERODULE_I2C_InitWrapper "HIERODULE_I2C_InitWrapper" call.
  */
void Idle(HIERODULE_I2C_Wrapper *Wrapper, uint32_t NumberOfPeriods)
{
    for(uint32_t i = 0 ; i < Wrapper->I2C_Period_Length*NumberOfPeriods ; i++)
    {
        __NOP();
    }
}

/** @brief Enables clock stretching for the I2C peripheral of a wrapper.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void EnableClockStretching(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    CLEAR_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE);
    while( 1 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE) == (I2C_CR1_PE)) );
    /** \cond */
    #endif /** \endcond */

    CLEAR_BIT(Wrapper->_I2C->CR1, I2C_CR1_NOSTRETCH);

    while( 0 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_NOSTRETCH) != (I2C_CR1_NOSTRETCH)) );

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE);
    while( 0 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE) == (I2C_CR1_PE)) );
    /** \cond */
    #endif /** \endcond */
}

/** @brief Disables clock stretching for the I2C peripheral of a wrapper.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void DisableClockStretching(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    CLEAR_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE);
    while( 1 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE) == (I2C_CR1_PE)) );
    /** \cond */
    #endif /** \endcond */

    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_NOSTRETCH);

    while( 1 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_NOSTRETCH) != (I2C_CR1_NOSTRETCH)) );

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE);
    while( 0 == (READ_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE) == (I2C_CR1_PE)) );
    /** \cond */
    #endif /** \endcond */
}

/** @brief Read and returns the data register content of the I2C peripheral.
  * @rv_param_wrapper_ptr{I2C}
  * @return uint8_t The byte returned.
  * @details Defined as static to avoid the multiple definition error some
  * compilers throw since a function with the same name also exists within
  * another module; that will be fixed in a future release.
  */
static uint8_t ReceiveData(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    return (uint8_t)(READ_BIT(Wrapper->_I2C->RXDR, I2C_RXDR_RXDATA));
    /** \cond */
    #else /** \endcond */
    return (uint8_t)(READ_BIT(Wrapper->_I2C->DR, I2C_DR_DR));
    /** \cond */
    #endif /** \endcond */
}

/** @brief Writes a byte into the data register of the I2C peripheral.
  * @rv_param_wrapper_ptr{I2C}
  * @param Byte Byte to be written into the data register.
  * @return None
  * @details @rv_obvious
  */
void TransmitData(HIERODULE_I2C_Wrapper *Wrapper, uint8_t Byte)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    WRITE_REG(Wrapper->_I2C->TXDR, Byte);
    /** \cond */
    #else /** \endcond */
    MODIFY_REG(Wrapper->_I2C->DR, I2C_DR_DR, Byte);
    /** \cond */
    #endif /** \endcond */
}

/** @brief Configure the I2C peripheral to "ACK" the next byte received.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void ACK_Next(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR1, I2C_CR1_ACK, I2C_CR1_ACK);
    /** \cond */
    #else /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_NACK, 0);
    /** \cond */
    #endif /** \endcond */
}

/** @brief Configure the I2C peripheral to "NACK" the next byte received.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void NACK_Next(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR1, I2C_CR1_ACK, 0);
    /** \cond */
    #else /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_NACK, I2C_CR2_NACK);
    /** \cond */
    #endif /** \endcond */
}

/** @brief Makes the I2C peripheral generate a start condition.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void Start(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_START);
    /** \cond */
    #else /** \endcond */
    SET_BIT(Wrapper->_I2C->CR2, I2C_CR2_START);
    /** \cond */
    #endif /** \endcond */
}

/** @brief Makes the I2C peripheral generate a stop condition.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details @rv_obvious
  */
void Stop(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_STOP);
    /** \cond */
    #else /** \endcond */
    SET_BIT(Wrapper->_I2C->CR2, I2C_CR2_STOP);
    /** \cond */
    #endif /** \endcond */
}

/** @brief Handles incoming data for an I2C peripheral in master receiver mode.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details The received byte is appended to the MRX buffer of the wrapper.\n
  * The data is "ACKed" or "NACKed", depending on whether it's the last
  * byte the master expects to receive. The wrapper is returned to the idle
  * state in the latter case and depending on the device, clock stretching
  * is enabled back again.
  */
void ReceiveByteAsMaster(HIERODULE_I2C_Wrapper *Wrapper)
{
    (Wrapper->MRX_Buffer)[Wrapper->MRX_Counter] =
    ReceiveData(Wrapper);

    Wrapper->MRX_Counter++;
    if(Wrapper->MRX_Counter == Wrapper->MRX_BufferSize)
    {
        Wrapper->Status = HIERODULE_I2C_Status_IDLE;
        Wrapper->SlaveAddress = 0;

        Stop(Wrapper);

        /** \cond */
        #ifndef __STM32F030x6_H /** \endcond */
        EnableClockStretching(Wrapper);
        /** \cond */
        #endif /** \endcond */

        ACK_Next(Wrapper);

        if( Wrapper->MRX_Handler != NULL)
            Wrapper->MRX_Handler();
    }
    else if(Wrapper->MRX_Counter == Wrapper->MRX_BufferSize-1)
    {
        NACK_Next(Wrapper);
        __NOP();
    }
}

/** @brief Handles incoming data for an I2C peripheral in slave receiver mode.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details The routine is stateless, assuming the wrapper status handling is
  * performed accordingly.
  */
void ReceiveByteAsSlave(HIERODULE_I2C_Wrapper *Wrapper)
{
    Wrapper->SRX_Buffer[Wrapper->SRX_Index] =
    ReceiveData(Wrapper);

    Wrapper->SRX_Index++;
    Wrapper->SRX_Index %= Wrapper->SRX_BufferSize;

    if( Wrapper->SRX_New < Wrapper->SRX_BufferSize )
        Wrapper->SRX_New++;
}

/** @brief Transmits the next data in the buffer for an I2C peripheral in
  * master transmitter mode.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details The MTX buffer should be set before initializing the transmission.
  */
void TransmitByteAsMaster(HIERODULE_I2C_Wrapper *Wrapper)
{
    TransmitData(Wrapper, (Wrapper->MTX_Buffer)[Wrapper->MTX_Counter]);
    Wrapper->MTX_Counter++;
    if( Wrapper->MTX_Counter == Wrapper->MTX_BufferSize )
    {
        /** \cond */
        #ifndef __STM32F030x6_H /** \endcond */
        while(READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_TXE) != (I2C_SR1_TXE))
        /** \cond */
        #else /** \endcond */
        while(READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_TC) != (I2C_ISR_TC));
        /** \cond */
        #endif /** \endcond */
        Wrapper->Status = HIERODULE_I2C_Status_IDLE;
        Wrapper->SlaveAddress = 0;
        Stop(Wrapper);
        /** \cond */
        #ifdef __STM32F030x6_H /** \endcond */
        while( 0 == READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_STOPF) );
        /** \cond */
        #endif /** \endcond */
        EnableClockStretching(Wrapper);
        if( Wrapper->MTX_Handler != NULL)
            Wrapper->MTX_Handler();
    }
}

/** @brief Transmits the next data in the buffer for an I2C peripheral in
  * slave transmitter mode.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details The STX buffer should be set before initializing the transmission.
  */
void TransmitByteAsSlave(HIERODULE_I2C_Wrapper *Wrapper)
{
    if( Wrapper->STX_Buffer != NULL )
        TransmitData(Wrapper, (Wrapper->STX_Buffer)[(Wrapper->STX_Counter)++]);
    else
        TransmitData(Wrapper, 0);

    Idle(Wrapper, 1);
}

/** @brief Invokes the on-SRX-finished routine if assigned and sets the I2C
  * wrapper status back to idle.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details Only to be used when exiting slave receiver mode.
  */
void ReturnToIdleState(HIERODULE_I2C_Wrapper *Wrapper)
{
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_I2C->ICR, I2C_ICR_STOPCF);
    /** \cond */
    #else /** \endcond */
    SET_BIT(Wrapper->_I2C->CR1, I2C_CR1_PE);
    /** \cond */
    #endif /** \endcond */
    if( Wrapper->SRX_Handler != NULL)
        Wrapper->SRX_Handler();
    Wrapper->Status = HIERODULE_I2C_Status_IDLE;
}

/** @brief Receives the NACK response, invokes the on-SRX-finished routine and
  * sets the wrapper status back to idle.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details Only to be used when exiting slave transmitter mode.
  */
void Receive_NACK(HIERODULE_I2C_Wrapper *Wrapper)
{
    if(Wrapper->Status == HIERODULE_I2C_Status_STX )
    {
        /** \cond */
        #ifdef __STM32F030x6_H /** \endcond */
        SET_BIT(Wrapper->_I2C->ICR, I2C_ICR_NACKCF);
        while( 1 == READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_NACKF) );
        /** \cond */
        #else /** \endcond */
        CLEAR_BIT(Wrapper->_I2C->SR1, I2C_SR1_AF);
        /** \cond */
        #endif /** \endcond */

        /** \cond */
        #ifdef __STM32F030x6_H /** \endcond */
        TransmitByteAsSlave(Wrapper);
        while( 0 == READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_STOPF) );
        SET_BIT(Wrapper->_I2C->ICR, I2C_ICR_STOPCF);
        while( 1 == READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_STOPF) );
        /** \cond */
        #endif /** \endcond */

        if( Wrapper->STX_Handler != NULL)
            Wrapper->STX_Handler();
        Wrapper->Status = HIERODULE_I2C_Status_IDLE;

    }
}

/**
  * @}
  */

/** @addtogroup I2C_Public Global
  * @{
  */

/** @details @ref HIERODULE_I2C_Wrapper::SRX_New "SRX_New" and @ref
  * HIERODULE_I2C_Wrapper::SRX_Index "SRX_Index" are also updated on call, to
  * revolve the ring buffer, so as to say.
  */
uint8_t HIERODULE_I2C_GetNextByte(HIERODULE_I2C_Wrapper *Wrapper)
{
    if(Wrapper->SRX_New > 0)
    {
        Wrapper->SRX_New--;

        return Wrapper->SRX_Buffer
        [
            (Wrapper->SRX_Index + Wrapper->SRX_BufferSize - (Wrapper->SRX_New+1) )
            % Wrapper->SRX_BufferSize
        ];
    }
    else
    {
        return 0;
    }
}

/** @details The buffers get a new addresses allocated to them.\n
  * The SRX ring buffer is filled with null characters.\n
  * Also configures the peripheral's control register and calculates the
  * I2C clock period.\n
  * @rv_wrapper_future_release{I2C,HIERODULE_I2C_ReleaseWrapper}
  */
HIERODULE_I2C_Wrapper **HIERODULE_I2C_InitWrapper(I2C_TypeDef *_I2C, uint16_t SRX_BufferSize, void (*SRX_Handler)(void), void (*MTX_Handler)(void), void (*STX_Handler)(void), void (*MRX_Handler)(void))
{
    if( _I2C == NULL )
    {
        return NULL;
    }

    HIERODULE_I2C_Wrapper **Wrapper;

    uint32_t I2C_Address = (uint32_t)_I2C;

    switch(I2C_Address)
    {
        case ( (uint32_t)I2C1 ):
            Wrapper = &I2C1_Wrapper;
            break;
        /** \cond */
        #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
        case ( (uint32_t)I2C2 ):
            Wrapper = &I2C2_Wrapper;
            break;
        /** \cond */
        #endif
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)I2C3 ):
            Wrapper = &I2C3_Wrapper;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            return NULL;
            break;
    }

    (*Wrapper) = (HIERODULE_I2C_Wrapper*)malloc(sizeof(HIERODULE_I2C_Wrapper));

    (*Wrapper)->_I2C = _I2C;

    (*Wrapper)->SRX_Index = 0;
    (*Wrapper)->SRX_New = 0;

    (*Wrapper)->SRX_BufferSize = SRX_BufferSize;
    (*Wrapper)->SRX_Buffer = (uint8_t*)malloc( ((*Wrapper)->SRX_BufferSize) * sizeof(uint8_t));
    for( uint16_t _rxb_init = 0 ; _rxb_init < (*Wrapper)->SRX_BufferSize ; _rxb_init++ )
    {
        ((*Wrapper)->SRX_Buffer)[_rxb_init] = '\0';
    }

    (*Wrapper)->SRX_Handler = SRX_Handler;
    (*Wrapper)->MTX_Handler = MTX_Handler;
    (*Wrapper)->STX_Handler = STX_Handler;
    (*Wrapper)->MRX_Handler = MRX_Handler;

    (*Wrapper)->STX_Buffer = NULL;
    (*Wrapper)->STX_Counter = 0;
    
    (*Wrapper)->Status = HIERODULE_I2C_Status_IDLE;
    (*Wrapper)->SlaveAddress = 0;

    
    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    SET_BIT((*Wrapper)->_I2C->CR1, I2C_CR1_ADDRIE);
    SET_BIT((*Wrapper)->_I2C->CR1, I2C_CR1_NACKIE);
    SET_BIT((*Wrapper)->_I2C->CR1, I2C_CR1_STOPIE);
    CLEAR_BIT((*Wrapper)->_I2C->CR2, I2C_CR2_AUTOEND);
    SET_BIT((*Wrapper)->_I2C->CR1, I2C_CR1_TXIE);
    SET_BIT((*Wrapper)->_I2C->CR1, I2C_CR1_RXIE);
    /** \cond */
    #else /** \endcond */
    SET_BIT((*Wrapper)->_I2C->CR2, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
    SET_BIT((*Wrapper)->_I2C->CR2, I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN);
    /** \cond */
    #endif /** \endcond */

    EnableClockStretching(*Wrapper);
    ACK_Next(*Wrapper);

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */

    uint32_t bus_clock;
    if( ((uint32_t)READ_BIT(RCC->CFGR3, RCC_CFGR3_I2C1SW) ) == RCC_CFGR3_I2C1SW_SYSCLK)
        bus_clock = SystemCoreClock;
    else
        bus_clock = HSI_VALUE;

    double core_clock_rate = ((double)SystemCoreClock)/(bus_clock);

    (*Wrapper)->I2C_Period_Length = core_clock_rate *
    ((((uint32_t)(READ_BIT((*Wrapper)->_I2C->TIMINGR, I2C_TIMINGR_PRESC) >> I2C_TIMINGR_PRESC_Pos))+1) *
    (
    ((uint32_t)(READ_BIT((*Wrapper)->_I2C->TIMINGR, I2C_TIMINGR_SCLL) >> I2C_TIMINGR_SCLL_Pos))+1
    +((uint32_t)(READ_BIT((*Wrapper)->_I2C->TIMINGR, I2C_TIMINGR_SCLH) >> I2C_TIMINGR_SCLH_Pos))+1
    +((uint32_t)(READ_BIT((*Wrapper)->_I2C->TIMINGR, I2C_TIMINGR_SDADEL) >> I2C_TIMINGR_SDADEL_Pos))
    +((uint32_t)(READ_BIT((*Wrapper)->_I2C->TIMINGR, I2C_TIMINGR_SCLDEL) >> I2C_TIMINGR_SCLDEL_Pos))+1
    )
    );

    /** \cond */
    #else /** \endcond */
    (*Wrapper)->I2C_Period_Length = SystemCoreClock /
    (( ((uint32_t)(READ_BIT((*Wrapper)->_I2C->CCR, I2C_CCR_FS))) == 0)
    ? 100000 : 400000);
    /** \cond */
    #endif /** \endcond */

    return Wrapper;
}

/** @details Buffer addresses are also freed.\n
  * @rv_wrapper_warn_release_det{I2C}
  */
void HIERODULE_I2C_ReleaseWrapper(HIERODULE_I2C_Wrapper *Wrapper)
{
    free(Wrapper->SRX_Buffer);
    Wrapper->SRX_Buffer = NULL;

    free(Wrapper->MTX_Buffer);
    Wrapper->MTX_Buffer = NULL;

    free(Wrapper->MRX_Buffer);
    Wrapper->MRX_Buffer = NULL;

    free(Wrapper->STX_Buffer);
    Wrapper->STX_Buffer = NULL;

    free(Wrapper);
    Wrapper = NULL;
}

/** @details Transmission won't commence if Size is zero.
  */
void HIERODULE_I2C_MasterTransmit(HIERODULE_I2C_Wrapper *Wrapper, uint8_t SlaveAddress, uint8_t *MTX_Buffer, uint32_t Size)
{
    if(Size == 0)
        return;

    DisableClockStretching(Wrapper);

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_SADD, SlaveAddress<<1);
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_NBYTES, Size << I2C_CR2_NBYTES_Pos);
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_RD_WRN, 0);
    Wrapper->Status = HIERODULE_I2C_Status_MTX;
    /** \cond */
    #else /** \endcond */
    Wrapper->SlaveAddress = SlaveAddress<<1;
    Wrapper->Status = HIERODULE_I2C_Status_MTX_SB;
    /** \cond */
    #endif /** \endcond */

    Wrapper->MTX_Buffer = MTX_Buffer;
    Wrapper->MTX_BufferSize = Size;
    Wrapper->MTX_Counter = 0;

    Start(Wrapper);
}

/** @details Transmission won't commence if Size is zero.
  */
void HIERODULE_I2C_MasterReceive(HIERODULE_I2C_Wrapper *Wrapper, uint8_t SlaveAddress, uint8_t *MRX_Buffer, uint32_t Size)
{
    if(Size == 0)
        return;

    DisableClockStretching(Wrapper);

    /** \cond */
    #ifdef __STM32F030x6_H /** \endcond */
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_SADD, (SlaveAddress<<1)+1);
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_NBYTES, Size << I2C_CR2_NBYTES_Pos);
    MODIFY_REG(Wrapper->_I2C->CR2, I2C_CR2_RD_WRN, I2C_CR2_RD_WRN);
    Wrapper->Status = HIERODULE_I2C_Status_MRX;
    /** \cond */
    #else /** \endcond */
    Wrapper->SlaveAddress = (SlaveAddress<<1)+1;
    Wrapper->Status = HIERODULE_I2C_Status_MRX_SB;
    /** \cond */
    #endif /** \endcond */

    Wrapper->MRX_Buffer = MRX_Buffer;
    Wrapper->MRX_BufferSize = Size;
    Wrapper->MRX_Counter = 0;

    if(Wrapper->MRX_BufferSize == 1)
    {
        NACK_Next(Wrapper);
        __NOP();
    }

    Start(Wrapper);
}
/**
  * @}
  */

/** @addtogroup I2C_Private Static
  * @{
  */

/** @brief The base IRQ body to be used for all I2C IRQs.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  * @details Transmission control is performed by handling the interrupt flags
  * in accordance to the wrapper's status.
  */
void I2C_IRQ_Handler(HIERODULE_I2C_Wrapper *Wrapper)
{

    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    uint8_t _ADDR_Active = (READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_ADDR) == (I2C_SR1_ADDR));
    /** \cond */
    #else /** \endcond */
    uint8_t _ADDR_Active = (READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_ADDR) == (I2C_ISR_ADDR));
    /** \cond */
    #endif

    #ifdef __STM32F030x6_H /** \endcond */
    if ( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_NACKF) == (I2C_ISR_NACKF) )
    {
        Receive_NACK(Wrapper);
    }
    /** \cond */
    #else /** \endcond */
    if ( READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_AF) == (I2C_SR1_AF) )
    {
        Receive_NACK(Wrapper);
    }
    /** \cond */
    #endif

    #ifdef __STM32F030x6_H /** \endcond */
    else if( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_RXNE) == (I2C_ISR_RXNE) )
    {
        if( Wrapper->Status == HIERODULE_I2C_Status_MRX )
            ReceiveByteAsMaster(Wrapper);
        else
        {
            if( Wrapper->Status == HIERODULE_I2C_Status_SRX )
                ReceiveByteAsSlave(Wrapper);
            else
                ReceiveData(Wrapper);
        }
    }
    /** \cond */
    #else /** \endcond */
    else if( READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_RXNE) == (I2C_SR1_RXNE) )
    {
        if( Wrapper->Status == HIERODULE_I2C_Status_MRX )
            ReceiveByteAsMaster(Wrapper);
        else
        {
            if( Wrapper->Status == HIERODULE_I2C_Status_SRX )
                ReceiveByteAsSlave(Wrapper);
            else
                ReceiveData(Wrapper);
        }
    }
    /** \cond */
    #endif

    #ifndef __STM32F030x6_H /** \endcond */
    else if ( READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_SB) == (I2C_SR1_SB) )
    {
        if( Wrapper->Status == HIERODULE_I2C_Status_MTX_SB )
            Wrapper->Status = HIERODULE_I2C_Status_MTX_ADR;
        else if( Wrapper->Status == HIERODULE_I2C_Status_MRX_SB )
            Wrapper->Status = HIERODULE_I2C_Status_MRX_ADR;
        TransmitData(Wrapper, Wrapper->SlaveAddress);
    }
    /** \cond */
    #endif /** \endcond */
    else if ( _ADDR_Active )
    {
        if(Wrapper->Status == HIERODULE_I2C_Status_IDLE)
        {
            /** \cond */
            #ifndef __STM32F030x6_H /** \endcond */
            uint32_t _t_dir = (uint32_t)(READ_BIT(Wrapper->_I2C->SR2, I2C_SR2_TRA));
            /** \cond */
            #else /** \endcond */
            uint32_t _t_dir = (uint32_t)(READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_DIR));
            /** \cond */
            #endif /** \endcond */
            if( 0 == _t_dir )
                Wrapper->Status = HIERODULE_I2C_Status_SRX;
            else
                Wrapper->Status = HIERODULE_I2C_Status_STX;
        }
        /** \cond */
        #ifndef __STM32F030x6_H /** \endcond */
        else if(Wrapper->Status == HIERODULE_I2C_Status_MTX_ADR)
        {
            Wrapper->Status = HIERODULE_I2C_Status_MTX;
        }
        else if(Wrapper->Status == HIERODULE_I2C_Status_MRX_ADR)
        {
            Wrapper->Status = HIERODULE_I2C_Status_MRX;
        }
        READ_REG(Wrapper->_I2C->SR1);
        READ_REG(Wrapper->_I2C->SR2);
        /** \cond */
        #else /** \endcond */
        SET_BIT(Wrapper->_I2C->ICR, I2C_ICR_ADDRCF);
        /** \cond */
        #endif /** \endcond */
    }
    /** \cond */
    #ifndef __STM32F030x6_H /** \endcond */
    else if ( (READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_TXE) == (I2C_SR1_TXE)) )
    {
        if(Wrapper->Status == HIERODULE_I2C_Status_MTX )
        {
            TransmitByteAsMaster(Wrapper);
        }
        else if(Wrapper->Status == HIERODULE_I2C_Status_STX )
        {
            TransmitByteAsSlave(Wrapper);
            while(READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_TXE) != (I2C_SR1_TXE))
            {
                if ( READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_AF) == (I2C_SR1_AF) )
                {
                    Receive_NACK(Wrapper);
                    break;
                }
            }
        }
    }
    /** \cond */
    #else /** \endcond */
    else if ( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_TXIS) == (I2C_ISR_TXIS) )
    {
        if(Wrapper->Status == HIERODULE_I2C_Status_MTX )
        {
            TransmitByteAsMaster(Wrapper);
        }
        else if(Wrapper->Status == HIERODULE_I2C_Status_STX )
        {
            TransmitByteAsSlave(Wrapper);
            while( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_TXIS) != (I2C_ISR_TXIS) )
            {
                if ( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_NACKF) == (I2C_ISR_NACKF) )
                {
                    Receive_NACK(Wrapper);
                    break;
                }
            }
        }
    }
    /** \cond */
    #endif

    #ifdef __STM32F030x6_H /** \endcond */
    else if ( READ_BIT(Wrapper->_I2C->ISR, I2C_ISR_STOPF) == (I2C_ISR_STOPF) )
    {
        ReturnToIdleState(Wrapper);
    }
    /** \cond */
    #else /** \endcond */
    else if ( READ_BIT(Wrapper->_I2C->SR1, I2C_SR1_STOPF) == (I2C_SR1_STOPF) )
    {
        ReturnToIdleState(Wrapper);
    }
    /** \cond */
    #endif /** \endcond */
}

/**
  * @}
  */

/** @addtogroup I2C_Public Global
  * @{
  */
            
/** @brief @rv_irq_imp_bri{I2C1}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
/** \cond */
#if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
extern void I2C1_EV_IRQHandler(void)
{
    if( I2C1_Wrapper != NULL )
        I2C_IRQ_Handler(I2C1_Wrapper);
}

/** @brief @rv_irq_imp_bri{I2C2}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
extern void I2C2_EV_IRQHandler(void)
{
    if( I2C2_Wrapper != NULL )
        I2C_IRQ_Handler(I2C2_Wrapper);
}

/** @brief @rv_irq_imp_bri{I2C1}\n
  * @rv_def_req_device{__STM32F030x6_H}
  * @return None
  * @details @rv_obvious
  */
/** \cond */
#elif defined __STM32F030x6_H /** \endcond */
extern void I2C1_IRQHandler(void)
{
    if( I2C1_Wrapper != NULL )
        I2C_IRQ_Handler(I2C1_Wrapper);
}
/** \cond */
#endif /** \endcond */

/** @brief @rv_irq_imp_bri{I2C3}\n
  * @rv_def_req_device{__STM32F401xC_H}
  * @return None
  * @details @rv_obvious
  */
/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
extern void I2C3_EV_IRQHandler(void)
{
    if( I2C3_Wrapper != NULL )
        I2C_IRQ_Handler(I2C3_Wrapper);
}
/** \cond */
#endif /** \endcond */
/**
  * @}
  */
/**
  * @}
  */



