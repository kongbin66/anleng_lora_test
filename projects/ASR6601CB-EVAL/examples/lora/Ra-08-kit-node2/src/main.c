
#include "my_config1.h"
 rtc_calendar_t Time = { 2019, 6, 12, 31, 23, 59, 55, 0 };
 float temp1=0;//温度
float humi1=0;//湿度
u8 buf[10];

 #define TEST_DATA_SIZE1 4096
 uint8_t test_data[TEST_DATA_SIZE1];
 uint32_t test_addr = 0x0800D000;
 
 
 #define RF_FREQUENCY 470000000 // Hz
 static RadioEvents_t RadioEvents;
 #define TX_OUTPUT_POWER 14
#define LORA_BANDWIDTH 0		// 带宽[0: 125 kHz,
								//  1: 250 kHz,
								//  2: 500 kHz,
								//  3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1		// [1: 4/5,
								//  2: 4/6,
								//  3: 4/7,
								//  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8	// Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0	// Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 1800
int mencpy23(void);

 void init()
{
   my_i2c_init();//i2c init
   rtc_timestamp(&Time);//calendar init+set
}



void data_format()
{
   //AA BBBB CC DDDD EEEE FFFF GGGG BB
   u16 i;
   buf[0]=0xaa;
   i=temp1*100;
   buf[1]=i<<24;
   buf[2]=i<<16;
   buf[3]=i<<8;
   buf[4]=i;
   i=humi1*100;
   buf[5]=i<<24;
   buf[6]=i<<16;
   buf[7]=i<<8;
   buf[8]=i;
   buf[9]=0xbb;
   for(int i=0;i<10;i++)
   {
            printf("buf[%d]=%d\r\n",i,buf[i]);
   }
}











int main()
{
   uart_log_init();
   printf("Ra-08-kit test ok!!!\r\n");
   init();
   while(1)
   {
      //  SHT20_get_temp_humi(&temp1,&humi1);
      //  RTC_get_time();
      //  data_format();//报文格式化
      mencpy23();
      RTC_get_time();
      delay_ms(1000);
   }
}













void OnTxDone(void)
{

}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{

}

void OnTxTimeout(void)
{

}

void OnRxTimeout(void)
{

}

void OnRxError(void)
{

}


void radio_set_par()
{
  // 1.初始化LORA
RadioEvents.TxDone = OnTxDone;   
RadioEvents.RxDone = OnRxDone;   
RadioEvents.TxTimeout = OnTxTimeout;   
RadioEvents.RxTimeout = OnRxTimeout;   
RadioEvents.RxError = OnRxError;
Radio.Init(&RadioEvents);
//2.设置无线通道
Radio.SetChannel(RF_FREQUENCY);
Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,  
                  LORA_SPREADING_FACTOR, LORA_CODINGRATE,   
                  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,  
                  true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,                      
                  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,                      
                  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,                      
                  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
//4.设定无线接收超时
Radio.Rx(RX_TIMEOUT_VALUE);
}


























int mencpy23(void)
{
    for (int i = 0; i < TEST_DATA_SIZE1; i++) {
        test_data[i] = i & 0xFF;
    }

    flash_erase_page(test_addr);

    flash_program_bytes(test_addr, test_data, TEST_DATA_SIZE1);

    for (int i = 0; i < TEST_DATA_SIZE1; i++) {
      printf("test_data[i]=%d\r\n",test_data[i]);
        if (*(uint8_t*)(test_addr + i) != (i & 0xFF)) {
            // error
            while (1)
                ;
        }
    }

    /* Infinite loop */
    while (1) { }
}


















// int main()
// {
// 	float temp1=0;
//    rcc_enable_oscillator(RCC_OSC_XO32K, true);//使能外部32.768晶振
//    rtc_cyc();
//    uart_log_init();
// 	 my_i2c_init();
//    board_lora_init();
// 	u32 j=0;
//    while(1){
// 		    temp1=read_sht20_temp(0xe3);
//         printf("valuee3=%.2f\r\n",temp1/10);
//         temp1=read_sht20_temp(0xe5);
//         printf("valuee5=%.2f\r\n",temp1/10);
//          delay_ms(100);
//          //发送数据到网关
//          send_data_to_gateway();
//          //接收和同步网关数据
//          recing_syn_data();
// 		   printf("wakeup:  j=%d\r\n",j++);
//          printf("sleep\r\n");
//          delay_ms(10);
// 		   rtc_check_syn();
//          pwr_deepsleep_wfi(PWR_LP_MODE_STOP3);//阻塞型函数

//    }

// }

#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;
    printf("assert_in\r\n");

    while (1) { }
}
#endif

// void board_lora_init(void)
// {
//    rcc_enable_oscillator(RCC_OSC_XO32K, true);//使能外部32.768晶振
//    rcc_enable_peripheral_clk(RCC_PERIPHERAL_SAC, true);
//    rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);
//    delay_ms(100);   
//    pwr_xo32k_lpm_cmd(true);
//    RadioEvents.TxDone = OnTxDone;    
//    RadioEvents.RxDone = OnRxDone;    
//    RadioEvents.TxTimeout = OnTxTimeout;    
//    RadioEvents.RxTimeout = OnRxTimeout;    
//    RadioEvents.RxError = OnRxError;
//    LORA Radio.Init(&RadioEvents);
// }
//发送数据到网关
void send_data_to_gateway(void)
{


}
//接收网关同步数据
void recing_syn_data(void)
{

}

// void board_init()
// {
//   //外设开关
//     rcc_enable_oscillator(RCC_OSC_XO32K, true);//使能外部32.768晶振

//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOC, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOD, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_SAC, true);
//     rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);
//   //
//     delay_ms(100);
//     pwr_xo32k_lpm_cmd(true);
    
//     uart_log_init();
// 	  RtcInit();

// 	//按键中断初始化
// 	  gpio_set_iomux(GPIOA, GPIO_PIN_2,0);
// 	//rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
//     gpio_init(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT_PULL_DOWN);
//     gpio_config_interrupt(GPIOA, GPIO_PIN_2, GPIO_INTR_RISING_EDGE);

//     /* NVIC config */
//     NVIC_EnableIRQ(GPIO_IRQn);
//     NVIC_SetPriority(GPIO_IRQn, 2);

//     //初始化GPIO
// 	  gpio_set_iomux(GPIOA, GPIO_PIN_4,0);
// 	  gpio_set_iomux(GPIOA, GPIO_PIN_5,0);
// 	  gpio_set_iomux(GPIOA, GPIO_PIN_7,0);
//     gpio_init(GPIOA, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP_LOW);
// 	  gpio_init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP_LOW);
// 	  gpio_init(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP_LOW);
// }

// int main(void)
// {
//     // Target board initialization
//     board_init();

//     //Ra08KitLoraTestStart();
// }