/**
  ******************************************************************************
  * @file           : hierodule_spi.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the SPI module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_SPI_H
#define __HIERODULE_SPI_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Hierodule_SPI SPI Module
  * @brief SPI comm management
  * @details @rv_refer_to_usage{SPI_Usage}
  * @{
  */
/** @addtogroup SPI_Public Global
  * @brief @rv_global_private_brief{are not} @rv_corresponds_exc_irqs{header}
  * @details Consists of general SPI comm routines, a SPI wrapper
  * initalizer and typedefs for module routines.\n
  * @rv_inc_main\n
  * @rv_inc_headers{stddef.h and stdlib.h,NULL and malloc/free\, respectively}
  * @{
  */

#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/** @brief Struct that keeps variables for the data buffers, a pointer to the
  * SPI peripheral, the and a pointer to the transmission end callback routine.
  * @details @rv_wrapper_det
  */
typedef struct
{
/** @brief Pointer to the SPI peripheral.
  * @details @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    SPI_TypeDef *_SPI;
/** @brief Master/Slave mode. 0 if Slave, 1 if Master.
  * @details @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    uint8_t Mode;
/** @brief The ring buffer where the data received is appended.
  * @details Use @ref HIERODULE_SPI_GetNextByte "HIERODULE_SPI_GetNextByte"
  * to unidirectionally parse the buffer.\n
  * @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    uint8_t *RX_Buffer;
/** @brief Index of next-byte-to-be-received in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    uint32_t RX_Index;
/** @brief Number of new bytes in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    uint32_t RX_New;
/** @brief Number of elements in the ring buffer.\n
  * @details @rv_common_wrap_field{HIERODULE_SPI_InitWrapper}
  */
    uint16_t RX_BufferSize;

/** @brief Buffer that keeps data to be transmitted.
  * @details Set via a call to @ref HIERODULE_SPI_TransmitPackage
  * "HIERODULE_SPI_TransmitPackage".
  */
    uint8_t *TX_Buffer;
/** @brief Used to continue transmission until buffer size is received.
  * @details @rv_common_wrap_field{HIERODULE_SPI_TransmitPackage}
  */
    uint32_t TX_Counter;
/** @brief Number of bytes to be transmitted in the buffer.
  * @details @rv_common_wrap_field{HIERODULE_SPI_TransmitPackage}
  */
    uint32_t TX_BufferSize;

/** @brief Pointer to callback function to be called on a completed
  * transmission.
  * @details Called at the end of an entire transmission in master mode. As for
  * slave mode, the routine gets invoked at the end of each byte transmission.\n
  * @rv_common_wrap_field{HIERODULE_SPI_TransmitPackage}
  */
    void (*TC_Handler)(void);

} HIERODULE_SPI_Wrapper;

/** @brief Fetches the next byte in the RX ring buffer.
  * @rv_param_wrapper_ptr{SPI}
  * @return The next byte in the ring buffer, 0 if there's none.
  */
uint8_t HIERODULE_SPI_GetNextByte(HIERODULE_SPI_Wrapper *Wrapper);

/** @brief Writes a byte into the data register of the SPI peripheral.
  * @rv_param_wrapper_ptr{SPI}
  * @param Byte Byte to be written into the data register.
  * @return None
  */
void HIERODULE_SPI_TransmitByte(HIERODULE_SPI_Wrapper *Wrapper, uint8_t Byte);

/** @rv_init_wrapper_brief_param{SPI,_SPI}
  * @param Mode 1 For master, 0 for slave.
  * @param RX_BufferSize Ring buffer length.
  * @param TC_Handler Pointer to callback function to be called on a completed
  * transmission.
  * @return @rv_init_wrapper_ret
  */
HIERODULE_SPI_Wrapper **HIERODULE_SPI_InitWrapper( SPI_TypeDef *_SPI, uint8_t Mode, uint16_t RX_BufferSize, void (*TC_Handler)(void) );

/** @brief Frees the memory allocated to an SPI wrapper.
  * @rv_param_wrapper_ptr{SPI}
  * @return None
  */
void HIERODULE_SPI_ReleaseWrapper(HIERODULE_SPI_Wrapper *Wrapper);

/** @brief Starts a transmission as the master.
  * @rv_param_wrapper_ptr{SPI}
  * @param TX_Buffer Byte array to be transmitted.
  * @param Size Number of bytes to be transmitted.
  * @return None
  */
void HIERODULE_SPI_TransmitPackage(HIERODULE_SPI_Wrapper *Wrapper, uint8_t *TX_Buffer, uint32_t Size);

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_SPI_H */



