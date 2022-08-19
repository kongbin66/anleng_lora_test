#include "my_config1.h"

// void rtc_cyc()
// {
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
//     // enable the clk
//     rcc_enable_oscillator(RCC_OSC_XO32K, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
//     /* NVIC config */
//     NVIC_EnableIRQ(RTC_IRQn);
//     NVIC_SetPriority(RTC_IRQn, 2);

//     rtc_config_cyc_max(327680);
//     rtc_config_cyc_wakeup(ENABLE);
//     rtc_cyc_cmd(true);
//     rtc_config_interrupt(RTC_CYC_IT, ENABLE);   
// }

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




//  void rtc_timestamp(rtc_calendar_t* time)
// {
   
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);

//     // enable the clk
//     rcc_enable_oscillator(RCC_OSC_XO32K, true);

//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
//     rtc_set_calendar(time);
//     rtc_calendar_cmd(true);

//     rtc_get_calendar(time);

// }


// //获取系统时间
// void RTC_get_time()
// {
//    rtc_get_calendar(&time);
//    printf("%d/%d/%d %d:%d:%d\r\n",time.year,time.month,time.day,time.hour,time.minute,time.second);
// }




//rtc中断函数
void rtc_IRQHandler(void)
{
         uint8_t intr_stat;
     intr_stat = rtc_get_status(RTC_CYC_SR);
     if (intr_stat == true) {
         rtc_config_interrupt(RTC_CYC_IT, DISABLE);
         rtc_set_status(RTC_CYC_SR, false);
         rtc_config_interrupt(RTC_CYC_IT, ENABLE);
         rtc_check_syn();
     }
}

//RTC中断参数设定
void rtc_alarm_wakeup()
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    
   
    rtc_set_calendar(&time);
    rtc_calendar_cmd(true);
    rtc_check_syn();
    rtc_get_calendar(&time);
    printf("Set_time=%d/%d/%d,%d:%d:%d\r\n",time.year,time.month,time.day,time.hour,time.minute,time.second);

        /* NVIC config */
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);
   
    rtc_config_cyc_max(327680);//9830400
    rtc_config_cyc_wakeup(ENABLE);
    rtc_cyc_cmd(true);
    rtc_config_interrupt(RTC_CYC_IT, ENABLE);   
 
}


//时间戳转换

#define SECONDS_FROM_1970_TO_2000 \
  946656000
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
const u8 daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
u16 date2days(u16 y, u8 m, u8 d)
{
  if (y >= 2000U)
    y -= 2000U;
  u16 days = d;
  for (u8 i = 1; i < m; ++i)
    days += pgm_read_byte(daysInMonth + i - 1);
  if (m > 2 && y % 4 == 0)
    ++days;
  return days + 365 * y + (y + 3) / 4 - 1;
}

u32 time2ulong(u16 days, u8 h, u8 m, u8 s)
{
  return ((days * 24UL + h) * 60 + m) * 60 + s;
}

u32 unixtime() //转换时间戳 只有ds1302操作时使用
{
  time_t t;
  u16 days = date2days(time.year, time.month, time.day);
  t = time2ulong(days, time.hour, time.minute, time.second);
  t += SECONDS_FROM_1970_TO_2000; // seconds from 1970 to 2000
  return t;
}