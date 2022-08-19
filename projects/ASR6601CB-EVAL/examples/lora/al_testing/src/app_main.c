#include "my_config1.h"
/*************************************************************
                 global variable
**************************************************************/
rtc_calendar_t time = {2022, 5, 8, 12, 10, 40, 0, 0};
float temperature = 0.0; //温度
float humidity = 0.0;    //湿度
u32 time_stamp;          //时间戳
char sendBuf[100];   //发送缓存

typedef enum
{
   SYSID_IDLE=0,
   SYSID_GET,
   SYSID_SEND,
   SYSID_REC,
}sys_id_t;

/*************************************************************
                 //获取发送数据
**************************************************************/
void get_sys_data(void)
{
   uint16_t gateway_freqcy=4700;
   uint16_t gateway_addr=1200;
   uint16_t node_freqcy=4700;
   uint16_t node_addr=2200;
   uint8_t signal=99;
   uint8_t charge=100;
   temperature=32.25;
   humidity=50.32;
   rtc_check_syn();
   rtc_get_calendar(&time);
  time_stamp = unixtime();
  printf("time_stamp =%d\r\n", time_stamp);
  //获取温湿度
   SHT20_get_temp_humi(&temperature,&humidity);

  sprintf(sendBuf,"G_freqcy:%d,G_addr:%d,N_freqcy:%d,N_addr:%d,temper:%.2f,humi%.2f,signal:%d,charge:%d,time_stamp:%d",gateway_freqcy,gateway_addr,node_freqcy,node_addr,temperature,humidity,signal,charge,time_stamp);
  printf("sendBuf:%s\r\n",sendBuf);
} 

/*************************************************************
                 //向网关发送数据
**************************************************************/
u8 send_data_to_gateway()
{
  static u8 sta = 1;
  Radio.Send((uint8_t *)sendBuf, strlen(sendBuf) + 1);
 
  if (sta)
    return 1;
  else
    return 0;
}

u8 rec_gateway_to_node()
{
  return 1;
}


void app_main()
{
    u8 sta = SYSID_SEND;  
  while (1)
  {
//    switch (sta)
//    {
//    case /* constant-expression */:
//      /* code */
//      break;
//    
//    default:
//      break;
//    }
    // 1.准备数据

    // 2将数据发送给指定网关
    sta = send_data_to_geteway();
    if (sta)
    {
      printf("send_data_ok!\r\n");
    }
    else
      printf("send_data_fail !!!\r\n");
    // 3同步网关设置
    sta = Sync_data();
    if (sta)
    {
      printf("sync_data_ok!\r\n");
    }
    else
      printf("sync_data_fail !!!\r\n");
    // 4.休眠

    rtc_check_syn();
    rtc_get_calendar(&time);
    printf("%d/%d/%d,%d:%d:%d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);


  }
}