#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "boards.h"
#include "app_error.h"
#include <stdint.h>
#include <stdbool.h>

#define COMPARE_COUNTERTIME  (3UL)  /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */

#ifdef BSP_LED_0
    #define TICK_EVENT_OUTPUT       BSP_LED_0   /**< Pin number for indicating tick event. */
#endif
#ifndef TICK_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif
#ifdef BSP_LED_1
    #define COMPARE_EVENT_OUTPUT    BSP_LED_1   /**< Pin number for indicating compare event. */
#endif
#ifndef COMPARE_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif

bool flag_64Hz = false;

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */

/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
//    static uint32_t cnt = 0;
    
    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    {
//        cnt++;
//        if(0 == (++cnt % 64) )
//            nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
    }
    else if (int_type == NRF_DRV_RTC_INT_TICK)
    {
        flag_64Hz = true;
        
//        cnt++;
//        if(0 == (++cnt % 32) )
//            nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
    }
}

/** @brief Function configuring gpio for pin toggling.
 */
static void leds_config(void)
{
    bsp_board_init(BSP_INIT_LEDS);
}

/** @brief Function starting the internal LFCLK XTAL oscillator.
 */
static void lfclk_config(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

/** @brief Function initialization and configuration of RTC driver instance.
 */
static void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = RTC_FREQ_TO_PRESCALER(64);
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,true);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    err_code = nrf_drv_rtc_cc_set(&rtc,0,COMPARE_COUNTERTIME * 8,true);
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}

void rtc_init(void)
{
    leds_config();
    lfclk_config();
    rtc_config();
}

///**
// * @brief Function for application main entry.
// */
//int main(void)
//{
//    leds_config();

//    lfclk_config();

//    rtc_config();

//    while (true)
//    {
//        __SEV();
//        __WFE();
//        __WFE();
//    }
//}


///**  @} */
