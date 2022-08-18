/*!
 * \file      timer.h
 *
 * \brief     Timer objects and scheduling management implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \addtogroup LORA
 *
 * \{
 *
 * \defgroup  LORA_TIMER
 *
 * \{
 *
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>     
#include "utilities.h"

/* Exported types ------------------------------------------------------------*/

/*!
 * \brief Timer object description
 */
typedef struct TimerEvent_s
{
    uint32_t Timestamp;         //! Expiring timer value in ticks from TimerContext
    uint32_t ReloadValue;       //! Reload Value when Timer is restarted
    bool IsRunning;             //! Is the timer currently running
    void ( *Callback )( void ); //! Timer IRQ callback function
    struct TimerEvent_s *Next;  //! Pointer to the next Timer object.
} TimerEvent_t;


#ifndef TimerTime_t
typedef uint64_t TimerTime_t;
#endif

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/*!
 * \brief Number of seconds elapsed between Unix and GPS epoch
 */
#define UNIX_GPS_EPOCH_OFFSET                       315964800

/*!
 * \brief Structure holding the system time in seconds and miliseconds.
 */
typedef struct TimerSysTime_s
{
    uint32_t Seconds;
    int16_t SubSeconds;
}TimerSysTime_t;

/*!
 * Adds 2 TimerSysTime_t values
 *
 * \param a Value
 * \param b Value to added
 *
 * \retval result Addition result (TimerSysTime_t value)
 */
static inline TimerSysTime_t TimerAddSysTime( TimerSysTime_t a, TimerSysTime_t b )
{
    TimerSysTime_t c = { 0 };

    c.Seconds = a.Seconds + b.Seconds;
    c.SubSeconds = a.SubSeconds + b.SubSeconds;
    if( c.SubSeconds >= 1000 )
    {
        c.Seconds++;
        c.SubSeconds -= 1000;
    }
    return c;
}

/*!
 * Subtracts 2 TimerSysTime_t values
 *
 * \param a Value
 * \param b Value to be subtracted
 *
 * \retval result Subtraction result (TimerSysTime_t value)
 */
static inline TimerSysTime_t TimerSubSysTime( TimerSysTime_t a, TimerSysTime_t b )
{
    TimerSysTime_t c = { 0 };

    c.Seconds = a.Seconds - b.Seconds;
    c.SubSeconds = a.SubSeconds - b.SubSeconds;
    if( c.SubSeconds < 0 )
    {
        c.Seconds--;
        c.SubSeconds += 1000;
    }
    return c;
}

/*!
 * \brief Sets the system time with the number of sconds elapsed since epoch
 *
 * \param [IN] sysTime Structure provideing the number of seconds and 
 *                     subseconds elapsed since epoch
  */
void TimerSetSysTime( TimerSysTime_t sysTime );

/*!
 * \brief Gets the current system number of sconds elapsed since epoch
 *
 * \retval sysTime Structure provideing the number of seconds and 
 *                 subseconds elapsed since epoch
  */
TimerSysTime_t TimerGetSysTime( void );

/*!
 * \brief Initializes the timer object
 *
 * \remark TimerSetValue function must be called before starting the timer.
 *         this function initializes timestamp and reload value at 0.
 *
 * \param [IN] obj          Structure containing the timer object parameters
 * \param [IN] callback     Function callback called at the end of the timeout
 */
void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) );

/*!
 * \brief Timer IRQ event handler
 *
 * \note Head Timer Object is automaitcally removed from the List
 *
 * \note e.g. it is snot needded to stop it
 */
void TimerIrqHandler( void );

/*!
 * \brief Starts and adds the timer object to the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStart( TimerEvent_t *obj );

/*!
 * \brief Stops and removes the timer object from the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStop( TimerEvent_t *obj );

/*!
 * \brief Resets the timer object
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerReset( TimerEvent_t *obj );

/*!
 * \brief Set timer new timeout value
 *
 * \param [IN] obj   Structure containing the timer object parameters
 * \param [IN] value New timer timeout value
 */
//定时器获取距离最近一次闹钟唤醒之后的时间戳差值
void TimerSetValue( TimerEvent_t *obj, uint32_t value );


/*!
 * \brief Read the current time
 *
 * \retval returns current time in ms
 */
////定时器从内部RTC获取当前时间戳，实际是系统启动运行的时间。
TimerTime_t TimerGetCurrentTime( void );

/*!
 * \brief Return the Time elapsed since a fix moment in Time
 *
 * \param [IN] savedTime    fix moment in Time
 * \retval time             returns elapsed time in ms
 */
////定时器获取距离savedTime的时间戳差值
TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime );

/*!
 * \brief Computes the temperature compensation for a period of time on a
 *        specific temperature.
 *
 * \param [IN] period Time period to compensate
 * \param [IN] temperature Current temperature
 *
 * \retval Compensated time period
 */
TimerTime_t TimerTempCompensation( TimerTime_t period, float temperature );

/*!
 * \brief Manages the entry into ARM cortex deep-sleep mode
 */
void TimerLowPowerHandler( void );

#ifdef __cplusplus
}
#endif

/*! \} defgroup LORA_TIMER */
/*! \} addtogroup LORA */

#endif /* __TIMER_H__ */

/*定时器链表的使用方法:
1.新建一个TimerEvent_t类型的全局结构体变量，例如，TimerEvent_t Led1Timer;
2.调用TimerInit()函数对结构体变量进行初始化，并且注册定时器超时回调函数，例如：TimerInit( &Led1Timer, OnLed1TimerEvent );
3.调用TimerSetValue()函数设置定时器超时时间，例如：TimerSetValue( &Led1Timer, 50 );
4.调用TimerStart()函数启动定时器，例如：TimerStart( &Led1Timer );
5.编写定时器超时回调函数OnLed1TimerEvent(),在超时回调函数中，关闭定时器TimerStop( &Led1Timer );
https://blog.csdn.net/u010561799/article/details/78345497/
RTC定时器的用法主要分为三步：
1. 初始化，注册回调函数
	void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) )//设置回调函数
2. 设置定时时间
	void TimerSetValue( TimerEvent_t *obj, uint32_t value )
3. 开启定时时间
	void TimerStart( TimerEvent_t *obj )*/