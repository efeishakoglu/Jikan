/**
  ******************************************************************************
  * @file           : hierodule_tim.c
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Source file for the timer module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#include <hierodule_tim.h>

/** @addtogroup Hierodule_Tim Timer Module
  * @{
  */

/** @addtogroup TIM_Private Static
  * @brief Elements of the module that are confined to the scope of the
  * compilation unit. Perfectly corresponds to the module's source file, except
  * the IRQ handlers.
  * @details Consists of definitions for routines declared in the header
  * file, as well as typedefs, variables and routines that are used to
  * implement those.\n
  * IRQ definitions with custom ISR implementations are also included at the
  * end of the file.
  * @{
  */

/** \cond */
#ifdef HIERODULE_TIM_HANDLE_IRQ
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
/** @brief Used to reference a clear-flag routine for @ref Check_IT "Check_IT".
  */
        typedef void (*T2V_Function_Pointer)(TIM_TypeDef*);
    /** \cond */
    #endif
#endif /** \endcond */

/** @brief @rv_apb_check 1.
  */
static const uint32_t APB1_DIV1 = (RCC_CFGR_PPRE1_DIV1>>RCC_CFGR_PPRE1_Pos);

/** @brief @rv_apb_check 2.
  */
static const uint32_t APB2_DIV1 = (RCC_CFGR_PPRE2_DIV1>>RCC_CFGR_PPRE2_Pos);

/** \cond */
#ifdef HIERODULE_TIM_HANDLE_IRQ
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
/** @brief @rv_fp_conv_isr{update,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 3,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC3_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 4,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC4_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{break,1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER BRK_Handler_TIM1 = NULL;

/** @brief @rv_fp_conv_isr{update,2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM2 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM2 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM2 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 3,2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC3_Handler_TIM2 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 4,2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC4_Handler_TIM2 = NULL;

/** @brief @rv_fp_conv_isr{update,3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM3 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM3 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM3 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 3,3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC3_Handler_TIM3 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 4,3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC4_Handler_TIM3 = NULL;

/** @brief @rv_fp_conv_isr{update,4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM4 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM4 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM4 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 3,4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC3_Handler_TIM4 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 4,4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC4_Handler_TIM4 = NULL;

        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
/** @brief @rv_fp_conv_isr{update,5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM5 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM5 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM5 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 3,5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC3_Handler_TIM5 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 4,5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC4_Handler_TIM5 = NULL;

/** @brief @rv_fp_conv_isr{update,9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM9 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM9 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 2,9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC2_Handler_TIM9 = NULL;

/** @brief @rv_fp_conv_isr{update,10}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM10 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,10}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM10 = NULL;

/** @brief @rv_fp_conv_isr{update,11}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER UPD_Handler_TIM11 = NULL;

/** @brief @rv_fp_conv_isr{capture compare channel 1,11}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER CC1_Handler_TIM11 = NULL;
        /** \cond */
        #endif
    #else /** \endcond */

/** @brief @rv_fp_plain_isr{Timer 1 Capture Compare}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_CC_ISR            = NULL;

/** @brief @rv_fp_plain_isr{Timer 2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM2_ISR               = NULL;

/** @brief @rv_fp_plain_isr{Timer 3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM3_ISR               = NULL;

/** @brief @rv_fp_plain_isr{Timer 4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM4_ISR               = NULL;
        /** \cond */
        #ifdef __STM32F103xB_H /** \endcond */
/** @brief @rv_fp_plain_isr{Timer 1 update}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_UP_ISR            = NULL;

/** @brief @rv_fp_plain_isr{Timer 1 break}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_BRK_ISR           = NULL;
        /** \cond */
        #elif defined __STM32F401xC_H /** \endcond */

/** @brief @rv_fp_plain_isr{Timer 1 update - timer 10}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_UP_TIM10_ISR      = NULL;

/** @brief @rv_fp_plain_isr{Timer 1 break - timer 9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_BRK_TIM9_ISR      = NULL;

/** @brief @rv_fp_plain_isr{Timer 5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM5_ISR               = NULL;

/** @brief @rv_fp_plain_isr{Timer 1 trigger comm - timer 11}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  */
        static FUNC_POINTER TIM1_TRG_COM_TIM11_ISR = NULL;
        /** \cond */
        #endif
    #endif //HIERODULE_TIM_CONVENIENT_IRQ
#endif //HIERODULE_TIM_HANDLE_IRQ /** \endcond */

/** @brief The array the timer PWM output channel enable bitmasks are kept.
  * @details @rv_single_func_convenience
  */
static const uint32_t TimerChannel_EN[4] =
{
    TIM_CCER_CC1E,
    TIM_CCER_CC2E,
    TIM_CCER_CC3E,
    TIM_CCER_CC4E
};

/** @brief Keeps the offsets to capture compare registers within a TIM_Typedef
  * for all four channels.
  * @details @rv_single_func_convenience
  */
static const uint32_t TimerChannel_CCR[4] =
{
    offsetof(TIM_TypeDef, CCR1),
    offsetof(TIM_TypeDef, CCR2),
    offsetof(TIM_TypeDef, CCR3),
    offsetof(TIM_TypeDef, CCR4)
};

/** @brief Returns the base frequency of a timer.
  * @rv_param_timer
  * @return Frequency in Hertz.
  * @details Base frequency is basically the peripheral bus clock prescaled.\n
  * Timers are driven by the different advanced peripheral bus clocks,
  * depending on the device and type of the timer, that's configured here
  * via device specific macro constant checks.\n
  * Secondly, the bus clock is by default doubled if the peripheral bus
  * divider is greater than unity, which is also managed in the function
  * with constants @ref APB1_DIV1 "APB1_DIV1" and @ref APB2_DIV1 "APB2_DIV1".\n
  * Finally, the peripheral clock is divided by the prescaler value to get
  * the clock frequency to be scaled by ARR, in turn, to set frequency and
  * period.\n\n
  * \f$Base Frequency = System Clock / (APB Prescaler * (PSC+1)*(ARR+1)) \f$\n
  */
static uint32_t GetBaseFreq(TIM_TypeDef *Timer)
{
    volatile uint32_t BaseFreq;
    /** \cond */
    #ifdef __STM32F103xB_H /** \endcond */
    if(Timer != TIM1)
    {
        BaseFreq = (SystemCoreClock >>
            APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);

        if( (READ_REG(RCC->CFGR) & RCC_CFGR_PPRE1) != APB1_DIV1 )
        {
            BaseFreq *= 2;
        }
    }
    else
    {
        BaseFreq = (SystemCoreClock >>
            APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);

        if( (READ_REG(RCC->CFGR) & RCC_CFGR_PPRE2) != APB2_DIV1 )
        {
            BaseFreq *= 2;
        }
    }
    /** \cond */
    #elif defined __STM32F401xC_H /** \endcond */
    if(Timer != TIM1 && Timer != TIM9 && Timer != TIM10 && Timer != TIM11)
    {
        BaseFreq = (SystemCoreClock >>
            APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);

        if( (READ_REG(RCC->CFGR) & RCC_CFGR_PPRE1) != APB1_DIV1 )
        {
            BaseFreq *= 2;
        }
    }
    else
    {
        BaseFreq = (SystemCoreClock >> 
            APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);

        if( (READ_REG(RCC->CFGR) & RCC_CFGR_PPRE2) != APB2_DIV1 )
        {
            BaseFreq *= 2;
        }
    }
    /** \cond */
    #endif /** \endcond */
    BaseFreq /= (Timer->PSC+1);
    return BaseFreq;
}

/** @brief Returns the pointer to the target channel's capture compare register.
  * @rv_param_timer
  * @param ChannelOffset: Offset of the targeted register within the struct.
  * @return Pointer to the targeted CCR
  * @details This function is meant to be used with the array @ref TimerChannel_CCR
  * "TimerChannel_CCR".\n
  */
static inline uint32_t *ChannelSelector(TIM_TypeDef *Timer, uint32_t ChannelOffset)
{
    return (uint32_t*)((char*)Timer + (size_t)ChannelOffset);
}

/** \cond */
#ifdef HIERODULE_TIM_HANDLE_IRQ
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
/** @brief Performs a routine if it's not NULL, and clears the flag of a timer
  * as specified with the clear-flag routine.\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @rv_param_timer
  * @param Handler: Pointer to the assigned void function
  * @param ClearFlag: Pointer to the clear-flag routine
  * @return None
  * @details @rv_obvious
  */
        static void Check_IT
        (
            TIM_TypeDef *Timer,
            FUNC_POINTER Handler,
            T2V_Function_Pointer ClearFlag
        )
        {
            if(Handler != NULL)
            {
                 Handler();
            }
            ClearFlag(Timer);
        }

/** @brief @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @return None
  * @details @rv_obvious
  */
        static void InfiniteLoopOfError(void)
        {
            while(1);
        }
    /** \cond */
    #endif
#endif /** \endcond */

/**
  * @}
  */

/** @addtogroup TIM_Public Global
  * @{
  */

/** @details @rv_upd_via_psc_arr\n\n
  * @rv_frm_period\n
  * @rv_frm_arr
  */
void HIERODULE_TIM_SetPeriod(TIM_TypeDef *Timer, double DurationSec)
{
    WRITE_REG(Timer->ARR, GetBaseFreq(Timer)*DurationSec-1.0);
}

/** @details @rv_otf_arr_base_calc{The period}\n\n
  * @rv_frm_period\n
  */
double HIERODULE_TIM_GetPeriod(TIM_TypeDef *Timer)
{
    return ((double)READ_REG(Timer->ARR)+1.0)/((double)GetBaseFreq(Timer));
}

/** @details @rv_upd_via_psc_arr\n\n
  * @rv_frm_freq\n
  * @rv_frm_arr
  */
void HIERODULE_TIM_SetFrequency(TIM_TypeDef *Timer, double Frequency_Hz)
{
    WRITE_REG(Timer->ARR, GetBaseFreq(Timer)/Frequency_Hz-1.0);
}

/** @details @rv_otf_arr_base_calc{The frequency}\n\n
  * @rv_frm_freq
  */
double HIERODULE_TIM_GetFrequency(TIM_TypeDef *Timer)
{
    return ((double)GetBaseFreq(Timer))/((double)(READ_REG(Timer->ARR)+1.0));
}

/** @details @rv_obvious
  */
void HIERODULE_TIM_ClearCounter(TIM_TypeDef *Timer)
{
    Timer->CNT = 0U;
}

/** @details Timer's RCR value is set to the input parameter. The UPD interrupt
  * of the timer will get triggered after 0+Reps periods.\n
  * Keep in mind repetition value the hardware supports may not be as high as
  * 65535, even though it's a 32 bit register. See the device manual to verify
  * the maximum amount of repetitions allowed.\n
  * @rv_req_xreg{repetition counter}
  */
void HIERODULE_TIM_SetRepetition(TIM_TypeDef *Timer, uint32_t Reps)
{
    WRITE_REG(Timer->RCR, Reps);
}

/** @details Basically reads from the repetition counter register and returns
  * the value.\n
  * @rv_req_xreg{repetition counter}
  */
uint32_t HIERODULE_TIM_GetRepetition(TIM_TypeDef *Timer)
{
    return READ_REG(Timer->RCR);
}

/** @details @rv_ccr_bit_action{set}\n
  * @rv_ch_14_avoid_fail
  */
void HIERODULE_TIM_EnableChannel(TIM_TypeDef *Timer, uint8_t Channel)
{
    if( (Channel > 0) && (Channel < 5) )
    {
        SET_BIT(Timer->CCER, TimerChannel_EN[Channel-1]);
    }
}

/** @details @rv_ccr_bit_action{clear}\n
  * @rv_ch_14_avoid_fail
  */
void HIERODULE_TIM_DisableChannel(TIM_TypeDef *Timer, uint8_t Channel)
{
    if( (Channel > 0) && (Channel < 5) )
    {
        CLEAR_BIT(Timer->CCER, TimerChannel_EN[Channel-1]);
    }
}

/** @details @rv_ccr_bit_action{check}
  * Returns 1 if so and 0 otherwise.\n
  * @rv_ch_14_avoid_fail
  */
uint32_t HIERODULE_TIM_IsEnabledChannel(TIM_TypeDef *Timer, uint8_t Channel)
{
    if( (Channel < 1) && (Channel > 4) )
    {
        return 0UL;
    }
    else if( (READ_BIT(Timer->CCER, TimerChannel_EN[Channel-1]))
        == (TimerChannel_EN[Channel-1]))
    {
        return 1UL;
    }
    else
    {
        return 0UL;
    }
}

/** @details @rv_req_xreg{main output enable bit in a break and dead time}
  */
void HIERODULE_TIM_EnableMainOutput(TIM_TypeDef *Timer)
{
    SET_BIT(Timer->BDTR, TIM_BDTR_MOE);
}

/** @details @rv_req_xreg{main output enable bit in a break and dead time}
  */
void HIERODULE_TIM_DisableMainOutput(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->BDTR, TIM_BDTR_MOE);
}

/** @details Scaling ARR with the normalized duty cycle yields the value
  * meant to be loaded to the designated CCR, @rv_tim_ch_acq\n
  * @rv_ch_14_avoid_fail
  */
void HIERODULE_TIM_SetDutyCycle
(
    TIM_TypeDef *Timer,
    uint8_t Channel,
    double NormalizedDutyCycle
)
{
    if( (Channel > 0) && (Channel < 5) )
    {
        (*(ChannelSelector(Timer, TimerChannel_CCR[Channel-1]))) =
            ((uint32_t)(READ_REG(Timer->ARR)))*NormalizedDutyCycle;
    }
}

/** @details The duty cycle is simply calculated dividing ARR by CCR,
  * @rv_tim_ch_acq\n
  * @rv_ch_14_avoid_fail
  * @rv_ch_ret_min_1_on_fail
  */
double HIERODULE_TIM_GetDutyCycle(TIM_TypeDef *Timer, uint8_t Channel)
{
    if( (Channel < 1) && (Channel > 4) )
    {
        return -1.0;
    }
    else
    {
        return
            ((double)(*(ChannelSelector(Timer, TimerChannel_CCR[Channel-1]))))
            / ((double)(READ_REG(Timer->ARR)));
    }
}

/** @details @rv_clear_tim_it_flag_det{Update}
  */
void HIERODULE_TIM_ClearFlag_UPD(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_UIF));
}

/** @details @rv_clear_tim_it_flag_det{Capture compare channel 1}
  */
void HIERODULE_TIM_ClearFlag_CC1(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_CC1IF));
}

/** @details @rv_clear_tim_it_flag_det{Capture compare channel 2}
  */
void HIERODULE_TIM_ClearFlag_CC2(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_CC2IF));
}

/** @details @rv_clear_tim_it_flag_det{Capture compare channel 3}
  */
void HIERODULE_TIM_ClearFlag_CC3(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_CC3IF));
}

/** @details @rv_clear_tim_it_flag_det{Capture compare channel 4}
  */
void HIERODULE_TIM_ClearFlag_CC4(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_CC4IF));
}

/** @details @rv_clear_tim_it_flag_det{Break}
  */
void HIERODULE_TIM_ClearFlag_BRK(TIM_TypeDef *Timer)
{
    WRITE_REG(Timer->SR, ~(TIM_SR_BIF));
}

/** @details @rv_bit_is_set_det{UIF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_UPD(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_UIF) == (TIM_SR_UIF)) ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{CC1IF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC1(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_CC1IF) == (TIM_SR_CC1IF)) ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{CC2IF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC2(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_CC2IF) == (TIM_SR_CC2IF)) ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{CC3IF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC3(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_CC3IF) == (TIM_SR_CC3IF)) ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{CC4IF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC4(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_CC4IF) == (TIM_SR_CC4IF)) ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{BIF}
  */
uint32_t HIERODULE_TIM_IsSetFlag_BRK(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->SR, TIM_SR_BIF) == (TIM_SR_BIF)) ? 1UL : 0UL);
}

/** @details @rv_tim_it_enable_det{update}
  */
void HIERODULE_TIM_Enable_IT_UPD(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_UPD(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_UIE);
}

/** @details @rv_tim_it_enable_det{capture compare channel 1}
  */
void HIERODULE_TIM_Enable_IT_CC1(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_CC1(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_CC1IE);
}

/** @details @rv_tim_it_enable_det{capture compare channel 2}
  */
void HIERODULE_TIM_Enable_IT_CC2(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_CC2(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_CC2IE);
}

/** @details @rv_tim_it_enable_det{capture compare channel 3}
  */
void HIERODULE_TIM_Enable_IT_CC3(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_CC3(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_CC3IE);
}

/** @details @rv_tim_it_enable_det{capture compare channel 4}
  */
void HIERODULE_TIM_Enable_IT_CC4(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_CC4(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_CC4IE);
}

/** @details @rv_tim_it_enable_det{break}
  */
void HIERODULE_TIM_Enable_IT_BRK(TIM_TypeDef *Timer)
{
    HIERODULE_TIM_ClearFlag_BRK(Timer);
    SET_BIT(Timer->DIER, TIM_DIER_BIE);
}

/** @details @rv_tim_it_disable_det{update}
  */
void HIERODULE_TIM_Disable_IT_UPD(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_UIE);
}

/** @details @rv_tim_it_disable_det{capture compare channel 1}
  */
void HIERODULE_TIM_Disable_IT_CC1(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_CC1IE);
}

/** @details @rv_tim_it_disable_det{capture compare channel 2}
  */
void HIERODULE_TIM_Disable_IT_CC2(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_CC2IE);
}

/** @details @rv_tim_it_disable_det{capture compare channel 3}
  */
void HIERODULE_TIM_Disable_IT_CC3(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_CC3IE);
}

/** @details @rv_tim_it_disable_det{capture compare channel 4}
  */
void HIERODULE_TIM_Disable_IT_CC4(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_CC4IE);
}

/** @details @rv_tim_it_disable_det{break}
  */
void HIERODULE_TIM_Disable_IT_BRK(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->DIER, TIM_DIER_BIE);
}

/** @details @rv_bit_is_set_det{UIE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_UPD(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_UIE) == (TIM_DIER_UIE))
        ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{TIM_DIER_CC1IE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC1(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_CC1IE) == (TIM_DIER_CC1IE))
        ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{TIM_DIER_CC2IE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC2(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_CC2IE) == (TIM_DIER_CC2IE))
        ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{TIM_DIER_CC3IE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC3(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_CC3IE) == (TIM_DIER_CC3IE))
        ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{TIM_DIER_CC4IE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC4(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_CC4IE) == (TIM_DIER_CC4IE))
        ? 1UL : 0UL);
}

/** @details @rv_bit_is_set_det{TIM_DIER_BIE}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_BRK(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->DIER, TIM_DIER_BIE) == (TIM_DIER_BIE))
        ? 1UL : 0UL);
}

/** @details @rv_req_xreg{automatic output enable bit in a break and dead time}
  */
void HIERODULE_TIM_EnableAutomaticOutput(TIM_TypeDef *Timer)
{
    SET_BIT(Timer->BDTR, TIM_BDTR_AOE);
}

/** @details @rv_req_xreg{automatic output enable bit in a break and dead time}
  */
void HIERODULE_TIM_DisableAutomaticOutput(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->BDTR, TIM_BDTR_AOE);
}

/** @details @rv_obvious
  */
void HIERODULE_TIM_EnableCounter(TIM_TypeDef *Timer)
{
    SET_BIT(Timer->CR1, TIM_CR1_CEN);
}

/** @details @rv_obvious
  */
void HIERODULE_TIM_DisableCounter(TIM_TypeDef *Timer)
{
    CLEAR_BIT(Timer->CR1, TIM_CR1_CEN);
}

/** @details @rv_bit_is_set_det{CEN}
  */
uint32_t HIERODULE_TIM_IsEnabledCounter(TIM_TypeDef *Timer)
{
    return ((READ_BIT(Timer->CR1, TIM_CR1_CEN) == (TIM_CR1_CEN)) ? 1UL : 0UL);
}

/** \cond */
#ifdef HIERODULE_TIM_HANDLE_IRQ

#ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
/** @details @rv_conv_isr_assign_det{update}
  */
void HIERODULE_TIM_Assign_ISR_UPD(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            UPD_Handler_TIM1 = ISR;
            break;
        case ( (uint32_t)TIM2 ):
            UPD_Handler_TIM2 = ISR;
            break;
        case ( (uint32_t)TIM3 ):
            UPD_Handler_TIM3 = ISR;
            break;
        case ( (uint32_t)TIM4 ):
            UPD_Handler_TIM4 = ISR;
            break;
        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)TIM5 ):
            UPD_Handler_TIM5 = ISR;
            break;
        case ( (uint32_t)TIM9 ):
            UPD_Handler_TIM9 = ISR;
            break;
        case ( (uint32_t)TIM10 ):
            UPD_Handler_TIM10 = ISR;
            break;
        case ( (uint32_t)TIM11 ):
            UPD_Handler_TIM11 = ISR;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            InfiniteLoopOfError();
    }
}

/** @details @rv_conv_isr_assign_det{capture compare channel 1}
  */
void HIERODULE_TIM_Assign_ISR_CC1(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            CC1_Handler_TIM1 = ISR;
            break;
        case ( (uint32_t)TIM2 ):
            CC1_Handler_TIM2 = ISR;
            break;
        case ( (uint32_t)TIM3 ):
            CC1_Handler_TIM3 = ISR;
            break;
        case ( (uint32_t)TIM4 ):
            CC1_Handler_TIM4 = ISR;
            break;
        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)TIM5 ):
            CC1_Handler_TIM5 = ISR;
            break;
        case ( (uint32_t)TIM9 ):
            CC1_Handler_TIM9 = ISR;
            break;
        case ( (uint32_t)TIM10 ):
            CC1_Handler_TIM10 = ISR;
            break;
        case ( (uint32_t)TIM11 ):
            CC1_Handler_TIM11 = ISR;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            InfiniteLoopOfError();
    }
}

/** @details @rv_conv_isr_assign_det{capture compare channel 2}
  */
void HIERODULE_TIM_Assign_ISR_CC2(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            CC2_Handler_TIM1 = ISR;
            break;
        case ( (uint32_t)TIM2 ):
            CC2_Handler_TIM2 = ISR;
            break;
        case ( (uint32_t)TIM3 ):
            CC2_Handler_TIM3 = ISR;
            break;
        case ( (uint32_t)TIM4 ):
            CC2_Handler_TIM4 = ISR;
            break;
        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)TIM5 ):
            CC2_Handler_TIM5 = ISR;
            break;
        case ( (uint32_t)TIM9 ):
            CC2_Handler_TIM9 = ISR;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            InfiniteLoopOfError();
    }
}

/** @details @rv_conv_isr_assign_det{capture compare channel 3}
  */
void HIERODULE_TIM_Assign_ISR_CC3(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            CC3_Handler_TIM1 = ISR;
            break;
        case ( (uint32_t)TIM2 ):
            CC3_Handler_TIM2 = ISR;
            break;
        case ( (uint32_t)TIM3 ):
            CC3_Handler_TIM3 = ISR;
            break;
        case ( (uint32_t)TIM4 ):
            CC3_Handler_TIM4 = ISR;
            break;
        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)TIM5 ):
            CC3_Handler_TIM5 = ISR;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            InfiniteLoopOfError();
    }
}

/** @details @rv_conv_isr_assign_det{capture compare channel 4}
  */
void HIERODULE_TIM_Assign_ISR_CC4(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            CC4_Handler_TIM1 = ISR;
            break;
        case ( (uint32_t)TIM2 ):
            CC4_Handler_TIM2 = ISR;
            break;
        case ( (uint32_t)TIM3 ):
            CC4_Handler_TIM3 = ISR;
            break;
        case ( (uint32_t)TIM4 ):
            CC4_Handler_TIM4 = ISR;
            break;
        /** \cond */
        #ifdef __STM32F401xC_H /** \endcond */
        case ( (uint32_t)TIM5 ):
            CC4_Handler_TIM5 = ISR;
            break;
        /** \cond */
        #endif /** \endcond */
        default:
            InfiniteLoopOfError();
    }
}

/** @details @rv_conv_isr_assign_det{break}
  */
void HIERODULE_TIM_Assign_ISR_BRK(TIM_TypeDef *Timer, FUNC_POINTER ISR)
{
    uint32_t TimerAddress = (uint32_t)Timer;

    switch(TimerAddress)
    {
        case ( (uint32_t)TIM1 ):
            BRK_Handler_TIM1 = ISR;
            break;
        default:
            InfiniteLoopOfError();
    }
}
/** \cond */
#else /** \endcond */
/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_CC_ISR(FUNC_POINTER ISR)
    {
        TIM1_CC_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM2_ISR(FUNC_POINTER ISR)
    {
        TIM2_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM3_ISR(FUNC_POINTER ISR)
    {
        TIM3_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM4_ISR(FUNC_POINTER ISR)
    {
        TIM4_ISR = ISR;
    }
    /** \cond */
    #ifdef __STM32F103xB_H /** \endcond */

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_UP_ISR(FUNC_POINTER ISR)
    {
        TIM1_UP_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_BRK_ISR(FUNC_POINTER ISR)
    {
        TIM1_BRK_ISR = ISR;
    }
    /** \cond */
    #elif defined __STM32F401xC_H /** \endcond */

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_UP_TIM10_ISR(FUNC_POINTER ISR)
    {
        TIM1_UP_TIM10_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_BRK_TIM9_ISR(FUNC_POINTER ISR)
    {
        TIM1_BRK_TIM9_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM5_ISR(FUNC_POINTER ISR)
    {
        TIM5_ISR = ISR;
    }

/** @details @rv_obvious
  */
    void HIERODULE_TIM_Assign_TIM1_TRG_COM_TIM11_ISR(FUNC_POINTER ISR)
    {
        TIM1_TRG_COM_TIM11_ISR = ISR;
    }
    /** \cond */
    #endif
#endif //HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */

/** \cond */
#ifdef __STM32F103xB_H /** \endcond */
/** @brief @rv_irq_imp_bri{Timer 1 update interrupt}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_solo_det{update}
  */
extern void TIM1_UP_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
    Check_IT(TIM1, UPD_Handler_TIM1, &HIERODULE_TIM_ClearFlag_UPD);
    /** \cond */
    #else /** \endcond */
    TIM1_UP_ISR();
    /** \cond */
    #endif /** \endcond */
}
/** \cond */
#elif defined __STM32F401xC_H /** \endcond */
/** @brief @rv_irq_imp_bri{Timer 1 update interrupt - timer 10}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM1_UP_TIM10_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
    if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM1) && HIERODULE_TIM_IsSetFlag_UPD(TIM1))
    {
        Check_IT(TIM1, UPD_Handler_TIM1, &HIERODULE_TIM_ClearFlag_UPD);
    }
    else if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM10) && HIERODULE_TIM_IsSetFlag_UPD(TIM10))
    {
        Check_IT(TIM10, UPD_Handler_TIM10, &HIERODULE_TIM_ClearFlag_UPD);
    }
    else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM10) && HIERODULE_TIM_IsSetFlag_CC1(TIM10))
    {
        Check_IT(TIM10, CC1_Handler_TIM10, &HIERODULE_TIM_ClearFlag_CC1);
    }
    /** \cond */
    #else /** \endcond */
    TIM1_UP_TIM10_ISR();
    /** \cond */
    #endif /** \endcond */
}
    /** \cond */
#endif  /** \endcond */

/** @brief @rv_irq_imp_bri{Timer 1 capture compare interrupt}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM1_CC_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM1) && HIERODULE_TIM_IsSetFlag_CC1(TIM1))
        {
            Check_IT(TIM1, CC1_Handler_TIM1, &HIERODULE_TIM_ClearFlag_CC1);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM1) && HIERODULE_TIM_IsSetFlag_CC2(TIM1))
        {
            Check_IT(TIM1, CC2_Handler_TIM1, &HIERODULE_TIM_ClearFlag_CC2);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC3(TIM1) && HIERODULE_TIM_IsSetFlag_CC3(TIM1))
        {
            Check_IT(TIM1, CC3_Handler_TIM1, &HIERODULE_TIM_ClearFlag_CC3);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC4(TIM1) && HIERODULE_TIM_IsSetFlag_CC4(TIM1))
        {
            Check_IT(TIM1, CC4_Handler_TIM1, &HIERODULE_TIM_ClearFlag_CC4);
        }
    /** \cond */
    #else /** \endcond */
        TIM1_CC_ISR();
    /** \cond */
    #endif /** \endcond */
}

/** \cond */
#ifdef __STM32F103xB_H /** \endcond */
/** @brief @rv_irq_imp_bri{Timer 1 break interrupt}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_solo_det{break}
  */
extern void TIM1_BRK_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
    Check_IT(TIM1, BRK_Handler_TIM1, &HIERODULE_TIM_ClearFlag_BRK);
    /** \cond */
    #else /** \endcond */
    TIM1_BRK_ISR();
    /** \cond */
    #endif /** \endcond */
}
/** \cond */
#elif defined __STM32F401xC_H /** \endcond */
/** @brief @rv_irq_imp_bri{Timer 1 break interrupt - timer 9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM1_BRK_TIM9_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
    if(HIERODULE_TIM_IsEnabled_IT_BRK(TIM1) && HIERODULE_TIM_IsSetFlag_BRK(TIM1))
    {
        Check_IT(TIM1, BRK_Handler_TIM1, &HIERODULE_TIM_ClearFlag_BRK);
    }
    else if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM9) && HIERODULE_TIM_IsSetFlag_UPD(TIM9))
    {
        Check_IT(TIM9, UPD_Handler_TIM9, &HIERODULE_TIM_ClearFlag_UPD);
    }
    else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM9) && HIERODULE_TIM_IsSetFlag_CC1(TIM9))
    {
        Check_IT(TIM9, CC1_Handler_TIM9, &HIERODULE_TIM_ClearFlag_CC1);
    }
    else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM9) && HIERODULE_TIM_IsSetFlag_CC2(TIM9))
    {
        Check_IT(TIM9, CC2_Handler_TIM9, &HIERODULE_TIM_ClearFlag_CC2);
    }
    /** \cond */
    #else /** \endcond */
    TIM1_BRK_TIM9_ISR();
    /** \cond */
    #endif /** \endcond */
}
/** \cond */
#endif /** \endcond */

/** @brief @rv_irq_imp_bri{Timer 2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM2_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM2) && HIERODULE_TIM_IsSetFlag_UPD(TIM2))
        {
            Check_IT(TIM2, UPD_Handler_TIM2, &HIERODULE_TIM_ClearFlag_UPD);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM2) && HIERODULE_TIM_IsSetFlag_CC1(TIM2))
        {
            Check_IT(TIM2, CC1_Handler_TIM2, &HIERODULE_TIM_ClearFlag_CC1);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM2) && HIERODULE_TIM_IsSetFlag_CC2(TIM2))
        {
            Check_IT(TIM2, CC2_Handler_TIM2, &HIERODULE_TIM_ClearFlag_CC2);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC3(TIM2) && HIERODULE_TIM_IsSetFlag_CC3(TIM2))
        {
            Check_IT(TIM2, CC3_Handler_TIM2, &HIERODULE_TIM_ClearFlag_CC3);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC4(TIM2) && HIERODULE_TIM_IsSetFlag_CC4(TIM2))
        {
            Check_IT(TIM2, CC4_Handler_TIM2, &HIERODULE_TIM_ClearFlag_CC4);
        }
    /** \cond */
    #else /** \endcond */
        TIM2_ISR();
    /** \cond */
    #endif /** \endcond */
}

/** @brief @rv_irq_imp_bri{Timer 3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM3_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM3) && HIERODULE_TIM_IsSetFlag_UPD(TIM3))
        {
            Check_IT(TIM3, UPD_Handler_TIM3, &HIERODULE_TIM_ClearFlag_UPD);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM3) && HIERODULE_TIM_IsSetFlag_CC1(TIM3))
        {
            Check_IT(TIM3, CC1_Handler_TIM3, &HIERODULE_TIM_ClearFlag_CC1);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM3) && HIERODULE_TIM_IsSetFlag_CC2(TIM3))
        {
            Check_IT(TIM3, CC2_Handler_TIM3, &HIERODULE_TIM_ClearFlag_CC2);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC3(TIM3) && HIERODULE_TIM_IsSetFlag_CC3(TIM3))
        {
            Check_IT(TIM3, CC3_Handler_TIM3, &HIERODULE_TIM_ClearFlag_CC3);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC4(TIM3) && HIERODULE_TIM_IsSetFlag_CC4(TIM3))
        {
            Check_IT(TIM3, CC4_Handler_TIM3, &HIERODULE_TIM_ClearFlag_CC4);
        }
    /** \cond */
    #else /** \endcond */
        TIM3_ISR();
    /** \cond */
    #endif /** \endcond */
}

/** @brief @rv_irq_imp_bri{Timer 4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM4_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM4) && HIERODULE_TIM_IsSetFlag_UPD(TIM4))
        {
            Check_IT(TIM4, UPD_Handler_TIM4, &HIERODULE_TIM_ClearFlag_UPD);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM4) && HIERODULE_TIM_IsSetFlag_CC1(TIM4))
        {
            Check_IT(TIM4, CC1_Handler_TIM4, &HIERODULE_TIM_ClearFlag_CC1);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM4) && HIERODULE_TIM_IsSetFlag_CC2(TIM4))
        {
            Check_IT(TIM4, CC2_Handler_TIM4, &HIERODULE_TIM_ClearFlag_CC2);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC3(TIM4) && HIERODULE_TIM_IsSetFlag_CC3(TIM4))
        {
            Check_IT(TIM4, CC3_Handler_TIM4, &HIERODULE_TIM_ClearFlag_CC3);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC4(TIM4) && HIERODULE_TIM_IsSetFlag_CC4(TIM4))
        {
            Check_IT(TIM4, CC4_Handler_TIM4, &HIERODULE_TIM_ClearFlag_CC4);
        }
    /** \cond */
    #else /** \endcond */
        TIM4_ISR();
    /** \cond */
    #endif /** \endcond */
}

/** \cond */
#ifdef __STM32F401xC_H /** \endcond */
/** @brief @rv_irq_imp_bri{Timer 5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM5_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM5) && HIERODULE_TIM_IsSetFlag_UPD(TIM5))
        {
            Check_IT(TIM5, UPD_Handler_TIM5, &HIERODULE_TIM_ClearFlag_UPD);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM5) && HIERODULE_TIM_IsSetFlag_CC1(TIM5))
        {
            Check_IT(TIM5, CC1_Handler_TIM5, &HIERODULE_TIM_ClearFlag_CC1);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC2(TIM5) && HIERODULE_TIM_IsSetFlag_CC2(TIM5))
        {
            Check_IT(TIM5, CC2_Handler_TIM5, &HIERODULE_TIM_ClearFlag_CC2);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC3(TIM5) && HIERODULE_TIM_IsSetFlag_CC3(TIM5))
        {
            Check_IT(TIM5, CC3_Handler_TIM5, &HIERODULE_TIM_ClearFlag_CC3);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC4(TIM5) && HIERODULE_TIM_IsSetFlag_CC4(TIM5))
        {
            Check_IT(TIM5, CC4_Handler_TIM5, &HIERODULE_TIM_ClearFlag_CC4);
        }
    /** \cond */
    #else /** \endcond */
        TIM5_ISR();
    /** \cond */
    #endif /** \endcond */
}

/** @brief @rv_irq_imp_bri{Timer 1 trigger comm interrupt - timer 11}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  * @return None
  * @details @rv_irq_imp_det
  */
extern void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    /** \cond */
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
        if(HIERODULE_TIM_IsEnabled_IT_UPD(TIM11) && HIERODULE_TIM_IsSetFlag_UPD(TIM11))
        {
            Check_IT(TIM11, UPD_Handler_TIM11, &HIERODULE_TIM_ClearFlag_UPD);
        }
        else if(HIERODULE_TIM_IsEnabled_IT_CC1(TIM11) && HIERODULE_TIM_IsSetFlag_CC1(TIM11))
        {
            Check_IT(TIM11, CC1_Handler_TIM11, &HIERODULE_TIM_ClearFlag_CC1);
        }
    /** \cond */
    #else /** \endcond */
        TIM1_TRG_COM_TIM11_ISR();
    /** \cond */
    #endif /** \endcond */
}
/** \cond */
#endif //__STM32F401xC_H /** \endcond */
/**
  * @}
  */
/**
  * @}
  */

#endif // HIERODULE_TIM_HANDLE_IRQ

