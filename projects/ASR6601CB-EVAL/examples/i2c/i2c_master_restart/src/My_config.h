#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include <stdio.h>
#include "tremo_uart.h"
#include "tremo_rcc.h"
#include "tremo_gpio.h"
#include "tremo_i2c.h"
#include "tremo_delay.h"
#include "rtc-board.h"
#include"lora_config.h"
#include <string.h>
#include "delay.h"
#include "timer.h"
#include "radio.h"
#include <stdlib.h>
#include "tremo_system.h"


//rtc
void rtc_cyc(void);
void rtc_IRQHandler(void);

//UART
void uart_log_init(void);

//i2c
#define SHT20_Measurement_Temp 0xe3
#define SHT20_Measurement_Humi 0xe5

#define slave_addr  0x40
i2c_config_t config;

void my_i2c_init(void);
float read_sht20_temp(u8 Cmd);


#endif