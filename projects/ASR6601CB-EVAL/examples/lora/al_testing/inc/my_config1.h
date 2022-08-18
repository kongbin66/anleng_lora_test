#ifndef __MY_CONFIG_H__
#define __MY_CONFIG_H__

#include "lora_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#include "timer.h"
#include "radio.h"

#include "tremo_system.h"
#include "tremo_gpio.h"

#include "tremo_uart.h"

#include "tremo_rcc.h"
#include "tremo_pwr.h"
#include "tremo_delay.h"
#include "rtc-board.h"


#include "tremo_regs.h"
#include "tremo_rtc.h"

#include "tremo_it.h"

#include "tremo_i2c.h"
#include "tremo_flash.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "tremo_gpio.h"

#define CONFIG_LORA_RFSW_CTRL_GPIOX GPIOD
#define CONFIG_LORA_RFSW_CTRL_PIN   GPIO_PIN_11

#define CONFIG_LORA_RFSW_VDD_GPIOX GPIOA
#define CONFIG_LORA_RFSW_VDD_PIN   GPIO_PIN_10

#ifdef __cplusplus
}
#endif

#define u8 unsigned char
#define u16 unsigned short






//内存




//串口
void uart_log_init(void);

// int Ra08KitLoraTestStart(void);


void rtc_cyc(void);


//i2c
#define SHT20_Measurement_Temp 0xe3
#define SHT20_Measurement_Humi 0xe5
#define slave_addr  0x40
i2c_config_t config;
void my_i2c_init(void);
float read_sht20_temp(u8 Cmd);//

rtc_calendar_t time;
void rtc_timestamp(rtc_calendar_t* time);//calendar init+set
void rtc_IRQHandler(void);
void rtc_alarm_wakeup();
//RTC获取日历
void RTC_get_time();
u32 unixtime(); //转换时间戳 只有ds1302操作时使用

//获取温湿度
void SHT20_get_temp_humi(float*temp,float*humi);







// //RF发送参数
//  typedef struct 
// {
//    uint32_t freqcy;
//    uint16_t addr;
// }RFPAR;

// RFPAR gateway = {47000,0};
// RFPAR node={4700,100};

char sendBuf[100];   //发送缓存

//设置LORA
void Ra08KitLoraTestStart(void);


u8 fun2();

#endif


