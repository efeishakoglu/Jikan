/**
  ******************************************************************************
  * @file           : hierodule_i2c.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the I2C module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_I2C_H
#define __HIERODULE_I2C_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Hierodule_I2C I2C Module
  * @brief I2C comm management
  * @details @rv_refer_to_usage{I2C_Usage}
  * @{
  */
/** @addtogroup I2C_Public Global
  * @brief @rv_global_private_brief{are not} @rv_corresponds_exc_irqs{header}
  * @details Consists of general I2C comm routines, a I2C wrapper
  * initalizer and typedefs for module routines.\n
  * @rv_inc_main\n
  * @rv_inc_headers{stddef.h and stdlib.h,NULL and malloc/free\, respectively}
  * @{
  */

#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/** @brief I2C wrapper status enumeration.
  * @details Notice that different devices may not follow the same status
  * succession.
  */
typedef enum
{
/** @brief Peripheral is expecting a start condition.
  */
    HIERODULE_I2C_Status_IDLE,

/** @brief Master transmitter, waiting for the start condition to commence.
  */
    HIERODULE_I2C_Status_MTX_SB,
/** @brief Master transmitter, waiting for a slave to acknowledge the address.
  */
    HIERODULE_I2C_Status_MTX_ADR,
/** @brief Master transmitter, transmitting bytes.
  */
    HIERODULE_I2C_Status_MTX,
/** @brief Slave receiver, receiving bytes.
  */
    HIERODULE_I2C_Status_SRX,

/** @brief Master receiver, waiting for the start condition to commence.
  */
    HIERODULE_I2C_Status_MRX_SB,
/** @brief Master receiver, waiting for a slave to acknowledge the address.
  */
    HIERODULE_I2C_Status_MRX_ADR,
/** @brief Master receiver, receiving bytes.
  */
    HIERODULE_I2C_Status_MRX,
/** @brief Slave transmitter, transmitting bytes.
  */
    HIERODULE_I2C_Status_STX

} HIERODULE_I2C_Status;

/** @brief Struct that keeps variables for the data buffers, a pointer to the
  * I2C peripheral, the slave address and pointers to the transmission end
  * callback routines.
  * @details @rv_wrapper_det
  */
typedef struct
{
/** @brief Pointer to the I2C peripheral.
  * @details @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    I2C_TypeDef *_I2C;
/** @brief I2C wrapper status.
  * @details Updated in the background, approach as a read-only field.
  */
    HIERODULE_I2C_Status Status;
/** @brief Target address.
  * @details Updated on a @ref HIERODULE_I2C_MasterTransmit
  * "HIERODULE_I2C_MasterTransmit" or @ref HIERODULE_I2C_MasterReceive
  * "HIERODULE_I2C_MasterReceive" call.
  */
    uint8_t SlaveAddress;

/** @brief I2C clock to system clock ratio.
  * @details @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    uint32_t I2C_Period_Length;

/** @brief The ring buffer where the data received in slave receiver mode
  * is appended.
  * @details Use @ref HIERODULE_I2C_GetNextByte "HIERODULE_I2C_GetNextByte"
  * to unidirectionally parse the buffer.\n
  * @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    uint8_t *SRX_Buffer;
/** @brief Index of next-byte-to-be-received in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    uint32_t SRX_Index;
/** @brief Number of new bytes in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    uint32_t SRX_New;
/** @brief Number of elements in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_I2C_InitWrapper}
  */
    uint16_t SRX_BufferSize;
/** @brief Pointer to callback function to be called on a completed
  * transmission in slave receiver mode.
  */
    void (*SRX_Handler)(void);

/** @brief Buffer that keeps data to be transmitted in master transmitter mode.
  * @details Set via a call to @ref HIERODULE_I2C_MasterTransmit
  * "HIERODULE_I2C_MasterTransmit".
  */
    uint8_t *MTX_Buffer;
/** @brief Used to continue transmission until buffer size is received.
  * @details @rv_common_wrap_field{HIERODULE_I2C_MasterTransmit}
  */
    uint32_t MTX_Counter;
/** @brief Number of bytes in the buffer.
  * @details @rv_common_wrap_field{HIERODULE_I2C_MasterTransmit}
  */
    uint32_t MTX_BufferSize;
/** @brief Pointer to callback function to be called on a completed
  * transmission in master transmitter mode.
  */
    void (*MTX_Handler)(void);

/** @brief Buffer that keeps data to be transmitted in slave transmitter mode.
  * @details This needs to be set manually before the transmission begins.
  */
    uint8_t *STX_Buffer;
/** @brief Used to continue transmission until buffer size is received.
  * @details This needs to be set manually before the transmission begins.
  */
    uint32_t STX_Counter;
/** @brief Pointer to callback function to be called on a completed
  * transmission in slave transmitter mode.
  */
    void (*STX_Handler)(void);

/** @brief The ring buffer where the data received in master receiver mode
  * is appended.
  * @details @rv_common_wrap_field{HIERODULE_I2C_MasterReceive}
  */
    uint8_t *MRX_Buffer;
/** @brief Used to continue transmission until buffer size is received.
  * @details @rv_common_wrap_field{HIERODULE_I2C_MasterReceive}
  */
    uint32_t MRX_Counter;
/** @brief Number of bytes in the buffer.
  * @details @rv_common_wrap_field{HIERODULE_I2C_MasterReceive}
  */
    uint32_t MRX_BufferSize;
/** @brief Pointer to callback function to be called on a completed
  * transmission in master receiver mode.
  */
    void (*MRX_Handler)(void);

} HIERODULE_I2C_Wrapper;

/** @brief Fetches the next byte in the SRX ring buffer.
  * @rv_param_wrapper_ptr{I2C}
  * @return The next byte in the ring buffer, 0 if there's none.
  */
uint8_t HIERODULE_I2C_GetNextByte(HIERODULE_I2C_Wrapper *Wrapper);

/** @rv_init_wrapper_brief_param{I2C,_I2C}
  * @param SRX_BufferSize Length of the SRX ring buffer array.
  * @param SRX_Handler Pointer to the callback routine for slave receiver mode.
  * @param MTX_Handler Pointer to the callback routine for master transmitter mode.
  * @param STX_Handler Pointer to the callback routine for slave transmitter mode.
  * @param MRX_Handler Pointer to the callback routine for master receiver mode.
  * @return @rv_init_wrapper_ret
  */
HIERODULE_I2C_Wrapper **HIERODULE_I2C_InitWrapper(I2C_TypeDef *_I2C, uint16_t SRX_BufferSize, void (*SRX_Handler)(void), void (*MTX_Handler)(void), void (*STX_Handler)(void), void (*MRX_Handler)(void));

/** @brief Frees the memory allocated to an I2C wrapper.
  * @rv_param_wrapper_ptr{I2C}
  * @return None
  */
void HIERODULE_I2C_ReleaseWrapper(HIERODULE_I2C_Wrapper *Wrapper);

/** @brief Puts the peripheral in master transmitter mode and handles the
  * transmission.
  * @rv_param_wrapper_ptr{I2C}
  * @param SlaveAddress Target device address.
  * @param MTX_Buffer Byte array to be transmitted.
  * @param Size Number of bytes to be transmitted.
  * @return None
  */
void HIERODULE_I2C_MasterTransmit(HIERODULE_I2C_Wrapper *Wrapper, uint8_t SlaveAddress, uint8_t *MTX_Buffer, uint32_t Size);

/** @brief Puts the peripheral in master receiver mode and handles the
  * transmission.
  * @rv_param_wrapper_ptr{I2C}
  * @param SlaveAddress Target device address.
  * @param MRX_Buffer Byte array where the received data will be
  * saved.
  * @param Size Number of bytes to be received.
  * @return None
  */
void HIERODULE_I2C_MasterReceive(HIERODULE_I2C_Wrapper *Wrapper, uint8_t SlaveAddress, uint8_t *MRX_Buffer, uint32_t Size);

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_I2C_H */



