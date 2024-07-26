/**
  ******************************************************************************
  * @file           : hierodule_usb.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the USB module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_usb.h>

/** @addtogroup Hierodule_USB USB Module
  * @{
  */

/** @addtogroup USB_Private Static
  * @brief @rv_global_private_brief{are}
  * @details The instance of the wrapper is declared here, an extern declaration
  * exists in the header file.
  * @{
  */

/** @brief Actual declaration for the Wrapper in the source file.
  */
HIERODULE_USB_Wrapper Wrapper;

/** @brief Appends a byte to the RX ring buffer.
  * @param byte Byte to be appended.
  * @return none
  * @details @rv_obvious
  */
void AppendToBuffer(uint8_t byte)
{
    Wrapper.RX_Buffer[Wrapper.RX_Index] = byte;

    Wrapper.RX_Index++;
    Wrapper.RX_Index %= Wrapper.RX_BufferSize;

    if( Wrapper.RX_New < Wrapper.RX_BufferSize )
        Wrapper.RX_New++;
}

/**
  * @}
  */

/** @addtogroup USB_Public Global
  * @{
  */

/** @details @ref HIERODULE_USB_Wrapper::RX_New "RX_New" and @ref
  * HIERODULE_USB_Wrapper::RX_Index "RX_Index" are also updated on call, to
  * revolve the ring buffer, so as to say.
  */
uint8_t HIERODULE_USB_GetNextByte(void)
{
    if(Wrapper.RX_New > 0)
    {
        Wrapper.RX_New--;

        return Wrapper.RX_Buffer
        [
            (Wrapper.RX_Index + Wrapper.RX_BufferSize - (Wrapper.RX_New+1) )
            % Wrapper.RX_BufferSize
        ];
    }
    else
    {
        return 0;
    }
}

/** @details The ring buffer gets a new address allocated to it and is filled with
  * null characters.
  */
void HIERODULE_USB_InitWrapper(uint16_t RX_BufferSize, void (*TC_Handler)(void) )
{
    Wrapper.RX_Index = 0;
    Wrapper.RX_New = 0;

    Wrapper.RX_BufferSize = RX_BufferSize;
    Wrapper.RX_Buffer = (uint8_t*)malloc( (Wrapper.RX_BufferSize) * sizeof(uint8_t));
    for( uint16_t _rxb_init = 0 ; _rxb_init < Wrapper.RX_BufferSize ; _rxb_init++ )
    {
        (Wrapper.RX_Buffer)[_rxb_init] = '\0';
    }

    Wrapper.TC_Handler = TC_Handler;
}

/** @details Basically, the ring buffer address gets freed and nullified.
  */
void HIERODULE_USB_ReleaseWrapper(void)
{
    free(Wrapper.RX_Buffer);
    Wrapper.RX_Buffer = NULL;
}

/** @details CDC_Transmit_FS is really a routine defined outside the module,
  * within usbd_cdc_if.h.
  */
void HIERODULE_USB_TransmitPackage(uint8_t *TX_Buffer, uint32_t Size)
{
    /** \cond */
    #ifdef __USBD_CDC_IF_H__ /** \endcond */
    CDC_Transmit_FS(TX_Buffer, Size);
    /** \cond */
    #endif /** \endcond */
}

/** @details  To be called right before the reception routine's return statement;
  * update CDC_Receive_FS (defined within usbd_cdc_if.c) accordingly.
  */
void HIERODULE_USB_Receive_Callback(uint8_t *Buf, uint32_t *Len)
{
    for(uint32_t _b = 0 ; _b < *Len ; _b++)
    {
        AppendToBuffer( Buf[_b] );
    }

    if(Wrapper.TC_Handler != NULL)
    {
        Wrapper.TC_Handler();
    }
}

/**
  * @}
  */

/**
  * @}
  */



