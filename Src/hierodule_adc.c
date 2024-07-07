/**
  ******************************************************************************
  * @file           : hierodule_adc.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the ADC module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_adc.h>

/** @addtogroup Hierodule_ADC ADC Module
  * @{
  */

/** @addtogroup ADC_Private Static
  * @brief @rv_global_private_brief{are} @rv_corresponds_exc_irqs{source}
  * @details @rv_static_wrap_det{ADC}\n
  * @rv_irq_defs_eof
  * @{
  */

/** @brief ADC wrapper pointer meant for ADC1.
  */
static HIERODULE_ADC_Wrapper *ADC1_Wrapper = NULL;

/** @brief ADC wrapper pointer meant for ADC2.\n
  * @rv_def_req_device{__STM32F103xB_H}
  */
/** \cond */
#ifdef __STM32F103xB_H /** \endcond */
static HIERODULE_ADC_Wrapper *ADC2_Wrapper = NULL;
/** \cond */
#endif /** \endcond */

/** @brief Applies a cumulative-smoothening filter before updating the wrapper
  * data field.
  * @rv_param_wrapper_ptr{ADC}
  * @return None
  * @details @rv_static_for_irq\n
  * @details @rv_def_req{HIERODULE_ADC_SMOOTHENING_FILTER}
  */
/** \cond */
#ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
void Smoothen(HIERODULE_ADC_Wrapper *Wrapper)
{
    Wrapper->Data = (Wrapper->Data*Wrapper->FilterWeight) +
        (Wrapper->_ADC->DR*(1.0-Wrapper->FilterWeight));
}
/** \cond */
#endif /** \endcond */

/**
  * @}
  */

/** @addtogroup ADC_Public Global
  * @{
  */

/** @details Smoothening filter weight is initially set to 0.5, which may be
  * changed during the wrapper's lifetime.\n
  * @rv_wrapper_future_release{ADC,HIERODULE_ADC_ReleaseWrapper}
  */
HIERODULE_ADC_Wrapper **HIERODULE_ADC_InitWrapper(ADC_TypeDef *_ADC,
    void (*ISR)(uint16_t))
{
    HIERODULE_ADC_Wrapper **Wrapper;
    
    uint32_t ADC_Address = (uint32_t)_ADC;
    switch(ADC_Address)
    {
        case ( (uint32_t)ADC1 ):
            Wrapper = &ADC1_Wrapper;
            break;
        /** \cond */
        #ifdef __STM32F103xB_H /** \endcond */
        case ( (uint32_t)ADC2 ):
            Wrapper = &ADC2_Wrapper;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            return NULL;
            break;
    }

    (*Wrapper) = (HIERODULE_ADC_Wrapper*)malloc(sizeof(HIERODULE_ADC_Wrapper));
    
    (*Wrapper)->_ADC = _ADC;
    
    (*Wrapper)->Data = 0;
    /** \cond */
    #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
    (*Wrapper)->FilterWeight = 0.5;
    /** \cond */
    #endif /** \endcond */

    (*Wrapper)->Data_Handler = ISR;

    return Wrapper;
}

/** @details @rv_wrapper_warn_release_det{ADC}
  */
void HIERODULE_ADC_ReleaseWrapper(HIERODULE_ADC_Wrapper *Wrapper)
{
    HIERODULE_ADC_Disable(Wrapper);

    free(Wrapper);
    Wrapper = NULL;
}

/** @details @rv_obvious
  */
void HIERODULE_ADC_Enable(HIERODULE_ADC_Wrapper *Wrapper)
{
    /** \cond */
    #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
    SET_BIT(Wrapper->_ADC->CR1, ADC_CR1_EOCIE);
    CLEAR_BIT(Wrapper->_ADC->SR, ADC_SR_EOC);
    SET_BIT(Wrapper->_ADC->CR2, ADC_CR2_ADON);

    /** \cond */
    #ifdef __STM32F103xB_H /** \endcond */
    SET_BIT(Wrapper->_ADC->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
    /** \cond */
    #elif defined __STM32F401xC_H /** \endcond */
    SET_BIT(Wrapper->_ADC->CR2, ADC_CR2_SWSTART);
    /** \cond */
    #endif

    #elif defined __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_ADC->IER, ADC_IER_EOCIE);
    CLEAR_BIT(Wrapper->_ADC->ISR, ADC_ISR_EOC);
    SET_BIT(Wrapper->_ADC->CR, ADC_CR_ADEN);
    SET_BIT(Wrapper->_ADC->CR, ADC_CR_ADSTART);
    /** \cond */
    #endif /** \endcond */
}

/** @details @rv_obvious
  */
void HIERODULE_ADC_Disable(HIERODULE_ADC_Wrapper *Wrapper)
{
    /** \cond */
    #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
    CLEAR_BIT(Wrapper->_ADC->CR1, ADC_CR1_EOCIE);
    CLEAR_BIT(Wrapper->_ADC->CR2, ADC_CR2_ADON);

    /** \cond */
    #ifdef __STM32F103xB_H /** \endcond */
    CLEAR_BIT(Wrapper->_ADC->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
    /** \cond */
    #elif defined __STM32F401xC_H /** \endcond */
    CLEAR_BIT(Wrapper->_ADC->CR2, ADC_CR2_SWSTART);
    /** \cond */
    #endif

    #elif defined __STM32F030x6_H /** \endcond */
    SET_BIT(Wrapper->_ADC->CR, ADC_CR_ADDIS);
    SET_BIT(Wrapper->_ADC->CR, ADC_CR_ADSTP);
    /** \cond */
    #endif /** \endcond */
}

/** @brief @rv_irq_imp_bri{ADC1_2}\n
  * @rv_def_req_device{__STM32F103xB_H}
  * @return None
  * @details @rv_adc_irq_handler_det
  */
/** \cond */
#ifdef __STM32F103xB_H /** \endcond */
extern void ADC1_2_IRQHandler(void)
{
    if(ADC1_Wrapper != NULL)
    {
        if (READ_BIT(ADC1_Wrapper->_ADC->CR1, ADC_CR1_EOCIE) == (ADC_CR1_EOCIE))
        {
            if (READ_BIT(ADC1_Wrapper->_ADC->SR, ADC_SR_EOC) == (ADC_SR_EOC))
            {
                /** \cond */
                #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
                Smoothen(ADC1_Wrapper);
                /** \cond */
                #else /** \endcond */
                ADC1_Wrapper->Data = ADC1_Wrapper->_ADC->DR;
                /** \cond */
                #endif /** \endcond */
                
                if(ADC1_Wrapper->Data_Handler != NULL)
                {
                    ADC1_Wrapper->Data_Handler(ADC1_Wrapper->Data);
                }
            }
        }
    }
    
    if(ADC2_Wrapper != NULL)
    {
        if (READ_BIT(ADC2_Wrapper->_ADC->CR1, ADC_CR1_EOCIE) == (ADC_CR1_EOCIE))
        {
            if (READ_BIT(ADC2_Wrapper->_ADC->SR, ADC_SR_EOC) == (ADC_SR_EOC))
            {
                /** \cond */
                #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
                Smoothen(ADC2_Wrapper);
                /** \cond */
                #else /** \endcond */
                ADC2_Wrapper->Data = ADC2_Wrapper->_ADC->DR;
                /** \cond */
                #endif /** \endcond */
                
                if(ADC2_Wrapper->Data_Handler != NULL)
                {
                    ADC2_Wrapper->Data_Handler(ADC2_Wrapper->Data);
                }
            }
        }
    }
}
/** \cond */
#endif

#ifdef __STM32F401xC_H /** \endcond */
/** @brief @rv_irq_imp_bri{ADC}\n
  * @rv_def_req_device{__STM32F401xC_H}
  * @return None
  * @details @rv_adc_irq_handler_det
  */
extern void ADC_IRQHandler(void)
{
    if(ADC1_Wrapper != NULL)
    {
        if (READ_BIT(ADC1_Wrapper->_ADC->CR1, ADC_CR1_EOCIE) == (ADC_CR1_EOCIE))
        {
            if (READ_BIT(ADC1_Wrapper->_ADC->SR, ADC_SR_EOC) == (ADC_SR_EOC))
            {
                /** \cond */
                #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
                Smoothen(ADC1_Wrapper);
                /** \cond */
                #else /** \endcond */
                ADC1_Wrapper->Data = ADC1_Wrapper->_ADC->DR;
                /** \cond */
                #endif /** \endcond */
                
                if(ADC1_Wrapper->Data_Handler != NULL)
                {
                    ADC1_Wrapper->Data_Handler(ADC1_Wrapper->Data);
                }
            }
        }
    }
}
/** \cond */
#endif

#ifdef __STM32F030x6_H /** \endcond */
/** @brief @rv_irq_imp_bri{ADC1}\n
  * @rv_def_req_device{__STM32F030x6_H}
  * @return None
  * @details @rv_adc_irq_handler_det
  */
extern void ADC1_IRQHandler(void)
{
    if(ADC1_Wrapper != NULL)
    {
        if (READ_BIT(ADC1_Wrapper->_ADC->IER, ADC_IER_EOCIE) == (ADC_IER_EOCIE))
        {
            if (READ_BIT(ADC1_Wrapper->_ADC->ISR, ADC_ISR_EOC) == (ADC_ISR_EOC))
            {
                /** \cond */
                #ifdef HIERODULE_ADC_SMOOTHENING_FILTER /** \endcond */
                Smoothen(ADC1_Wrapper);
                /** \cond */
                #else /** \endcond */
                ADC1_Wrapper->Data = ADC1_Wrapper->_ADC->DR;
                /** \cond */
                #endif /** \endcond */
                
                if(ADC1_Wrapper->Data_Handler != NULL)
                {
                    ADC1_Wrapper->Data_Handler(ADC1_Wrapper->Data);
                }
            }
        }
    }
}
/** \cond */
#endif /** \endcond */

/**
  * @}
  */
/**
  * @}
  */



