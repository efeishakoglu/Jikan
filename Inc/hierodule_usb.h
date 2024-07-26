/**
  ******************************************************************************
  * @file           : hierodule_usb.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the USB module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_USB_H
#define __HIERODULE_USB_H

#ifdef __cplusplus
extern "C"
{
#endif
/** @addtogroup Hierodule_USB USB Module
  * @brief USB comm management
  * @details @rv_refer_to_usage{USB_Usage}
  * @{
  */

/** @addtogroup USB_Public Global
  * @brief @rv_global_private_brief{are not}
  * @details Consists of general USB comm routines, a USB wrapper
  * initalizer a typedef for the wrapper itself, and an extern declaration for
  * the wrapper in the source file.\n
  * @rv_inc_main\n
  * An include directive is performed to usbd_cdc_if.h for CDC_Transmit_FS.
  * @rv_inc_headers{stddef.h and stdlib.h,NULL and malloc/free\, respectively}
  * @{
  */

#include <main.h>
#include <stddef.h>
#include <stdlib.h>

/** \cond */
#if __has_include("usbd_cdc_if.h") /** \endcond */
#include <usbd_cdc_if.h>
/** \cond */
#endif /** \endcond */

/** @brief Struct that keeps variables for the ring buffer and a pointer to the
  * transmission end callback routine.
  * @details Alias typedefs for custom pointers are not defined, since they're
  * not used excessively in the module. 
  */
typedef struct
{
/** @brief The ring buffer where the data received is appended.
  * @details Use @ref HIERODULE_USB_GetNextByte "HIERODULE_USB_GetNextByte"
  * to unidirectionally parse the buffer.\n
  * @rv_common_wrap_field{HIERODULE_USB_InitWrapper}
  */
    uint8_t *RX_Buffer;
/** @brief Index of next-byte-to-be-received in the ring buffer.
  * @details @rv_common_wrap_field{HIERODULE_USB_InitWrapper}
  */
    uint32_t RX_Index;
/** @brief Number of new bytes in the ring buffer.
  * @details @rv_common_wrap_field{HIERODULE_USB_InitWrapper}
  */
    uint32_t RX_New;
/** @brief Number of elements in the ring buffer.
  * @details @rv_common_wrap_field{HIERODULE_USB_InitWrapper}
  */
    uint16_t RX_BufferSize;

/** @brief Pointer to the callback function to be called on a completed
  * transmission.
  */
    void (*TC_Handler)(void);

} HIERODULE_USB_Wrapper;

/** @brief Extern declaration for the wrapper instance in the source file.
  * @details The instance of the wrapper is declared in the source file, the
  * extern declaration here makes it available outside the scope of the
  * compilation unit.\n
  * Unlike other modules, pointers to wrappers are not deemed necessary in this
  * case, since there's only one instance of the peripheral.
  */
extern HIERODULE_USB_Wrapper Wrapper;

/** @brief Fetches the next byte in the RX ring buffer.
  * @return The next byte in the ring buffer, 0 if there's none.
  */
uint8_t HIERODULE_USB_GetNextByte(void);

/** @brief Initializes the wrapper for the USB peripheral.
  * @param RX_BufferSize Ring buffer length.
  * @param TC_Handler Pointer to the callback function to be called on a completed
  * transmission.
  * @return None
  */
void HIERODULE_USB_InitWrapper(uint16_t RX_BufferSize, void (*TC_Handler)(void) );

/** @brief De-initializes the wrapper for the USB peripheral.
  * @return None
  */
void HIERODULE_USB_ReleaseWrapper(void);

/** @brief Sets up the data to be transmitted.
  * @param TX_Buffer Byte array to be transmitted.
  * @param Size Number of bytes to be transmitted.
  * @return None
  */
void HIERODULE_USB_TransmitPackage(uint8_t *TX_Buffer, uint32_t Size);

/** @brief Callback function that updates the ring buffer and invokes
  * @ref HIERODULE_USB_Wrapper::TC_Handler "TC_Handler" if it's not NULL.
  * @param Buf Address of the ring buffer.
  * @param Len Number of bytes received.
  * @return None
  */
void HIERODULE_USB_Receive_Callback(uint8_t *Buf, uint32_t *Len);
/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_USB_H */



