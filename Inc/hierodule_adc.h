/**
  ******************************************************************************
  * @file           : hierodule_adc.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the ADC module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_ADC_H
#define __HIERODULE_ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Hierodule_ADC ADC Module
  * @brief ADC data & interrupt management
  * @details @rv_refer_to_usage{ADC_Usage}
  * @{
  */
/** @addtogroup ADC_Public Global
  * @brief @rv_global_private_brief{are not} @rv_corresponds_exc_irqs{header}
  * @details Consists of basic ADC peripheral handler routines and an ADC
  * Wrapper struct, as well as an initializer for it.\n
  * @rv_inc_main\n
  * @rv_inc_headers{stdlib.h,malloc}
  * @{
  */

/** @brief Precompiler constant to toggle ADC smoothing filter.
  * @details When commented out, filter relevant variables and functions
  * won't be compiled and received data won't be filtered.
  */
#define HIERODULE_ADC_SMOOTHENING_FILTER

#include <main.h>
#include <stdlib.h>

/** @brief @rv_wrapper_brief{ADC data, ADC, EOC}
  * @details @rv_wrapper_det
  */
typedef struct
{
/** @brief Pointer to the ADC peripheral.
  * @details @rv_common_wrap_field{HIERODULE_ADC_InitWrapper}
  */
    ADC_TypeDef *_ADC;
/** @brief Variable to assign the received ADC data.
  * @details @rv_common_wrap_field{HIERODULE_ADC_InitWrapper}
  */
    uint16_t Data;
    /** \cond */
    #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
/** @brief Filter weight for the smoothening filter.
  * @details @rv_def_req{HIERODULE_ADC_SMOOTHENING_FILTER}
  */
    double FilterWeight;
    /** \cond */
    #endif /** \endcond */
/** @brief Pointer to the ISR for EOC.
  * @details @rv_wrapper_isr_det{uint16_t}
  */
    void (*Data_Handler)(uint16_t);
} HIERODULE_ADC_Wrapper;

/** @rv_init_wrapper_brief_param{ADC,_ADC}
  * @param ISR Pointer to the ISR for EOC.
  * @return @rv_init_wrapper_ret
  */
HIERODULE_ADC_Wrapper **HIERODULE_ADC_InitWrapper(ADC_TypeDef *_ADC,
    void (*ISR)(uint16_t));

/** @brief Frees the memory allocated to an ADC wrapper.
  * @rv_param_wrapper_ptr{ADC}
  * @return None
  */
void HIERODULE_ADC_ReleaseWrapper(HIERODULE_ADC_Wrapper *Wrapper);

/** @brief Updates relevant control register bits to start ADC action.
  * @rv_param_wrapper_ptr{ADC}
  * @return None
  */
void HIERODULE_ADC_Enable(HIERODULE_ADC_Wrapper *Wrapper);

/** @brief Updates relevant control register bits to stop ADC action.
  * @rv_param_wrapper_ptr{ADC}
  * @return None
  */
void HIERODULE_ADC_Disable(HIERODULE_ADC_Wrapper *Wrapper);

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_ADC_H */
