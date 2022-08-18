#ifndef __MY_CONFIG_H__
#define __MY_CONFIG_H__

#include "lora_config.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "timer.h"
#include "radio.h"
#include "tremo_uart.h"
#include "tremo_gpio.h"

#include "tremo_rcc.h"
#include "tremo_pwr.h"
#include "tremo_delay.h"
#include "rtc-board.h"


#include "tremo_regs.h"
#include "tremo_rtc.h"

#include "tremo_it.h"

#include "tremo_i2c.h"


#include <stdlib.h>

#include "tremo_system.h"

#include "tremo_flash.h"

#include "radio.h"
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
//RTC获取日历
void RTC_get_time();

//获取温湿度
void SHT20_get_temp_humi(float*temp,float*humi);



#endif


