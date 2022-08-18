#include "my_config1.h"

void rtc_cyc()
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
    // enable the clk
    rcc_enable_oscillator(RCC_OSC_XO32K, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
    //set calendar
 

   
    /* NVIC config */
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);

    rtc_config_cyc_max(327680);
    rtc_config_cyc_wakeup(ENABLE);
    rtc_cyc_cmd(true);
    rtc_config_interrupt(RTC_CYC_IT, ENABLE);   
}

//  void rtc_IRQHandler(void)
//  {
//      uint8_t intr_stat;
//      intr_stat = rtc_get_status(RTC_CYC_SR);
//      if (intr_stat == true) {
//          rtc_config_interrupt(RTC_CYC_IT, DISABLE);
//          rtc_set_status(RTC_CYC_SR, false);
//          rtc_config_interrupt(RTC_CYC_IT, ENABLE);
//          rtc_check_syn();
//      }
//  }




 void rtc_timestamp(rtc_calendar_t time)
{
   
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);

    // enable the clk
    rcc_enable_oscillator(RCC_OSC_XO32K, true);

    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    rtc_set_calendar(&time);
    rtc_calendar_cmd(true);

     rtc_get_calendar(&time);
    printf("%d:%d:%d\r\n",time.hour,time.minute,time.second);

}


//获取系统时间
void RTC_get_time()
{
   rtc_get_calendar(&Time);
   printf("%d/%d/%d %d:%d:%d\r\n",time.year,time.month,time.day,time.hour,time.minute,time.second);
}
