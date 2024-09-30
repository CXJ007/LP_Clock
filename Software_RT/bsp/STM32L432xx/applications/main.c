#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED0_PIN GET_PIN(A, 3)

#define RX8900_EXTERN_CTRL_RTC_GET_TEMP  (RT_DEVICE_CTRL_BASE(RTC) + 0x0AU) 

void user_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    float temp;
    rt_device_t dev = rt_device_find("rtc");
    rt_device_control(dev, RX8900_EXTERN_CTRL_RTC_GET_TEMP, &temp);
    rt_kprintf("rx8900 temperature: [%d.%dC] \n", (int)temp, (int)(temp * 10) % 10);
}

void alarm_sample(void)
{  
    rt_device_t dev = rt_device_find("rtc");
    struct rt_alarm_setup setup;
    struct rt_alarm * alarm = RT_NULL;
    struct rt_alarm * alarm1 = RT_NULL;
    static time_t now;
    struct tm p_tm;

    if (alarm != RT_NULL)
        return;

    /* 获取当前时间戳，并把下一秒时间设置为闹钟时间 */
    now = time(NULL) + 1;
    gmtime_r(&now,&p_tm);

    setup.flag = RT_ALARM_DAILY;            
    setup.wktime.tm_year = 2024;
    setup.wktime.tm_mon = 9;
    setup.wktime.tm_mday = 27;
    setup.wktime.tm_wday = 5;
    setup.wktime.tm_hour = 20;
    setup.wktime.tm_min = 10;
    setup.wktime.tm_sec = 0;   

    alarm = rt_alarm_create(user_alarm_callback, &setup);    
    if(RT_NULL != alarm)
    {
        rt_alarm_start(alarm);
    }


    setup.flag = RT_ALARM_DAILY;            
    setup.wktime.tm_year = 2024;
    setup.wktime.tm_mon = 9;
    setup.wktime.tm_mday = 27;
    setup.wktime.tm_wday = 5;
    setup.wktime.tm_hour = 20;
    setup.wktime.tm_min = 11;
    setup.wktime.tm_sec = 0;   

    alarm1 = rt_alarm_create(user_alarm_callback, &setup); 
    if(RT_NULL != alarm1)
    {
        rt_alarm_start(alarm1);
    }
}



#define SYS_ASSERT(EX)       \
    do {                     \
        if (RT_EOK != (EX))  \
        {                    \
            Error_Handler(); \
        }                    \
        else                 \
        {                    \
            /* nothing */    \
        }                    \
    } while (0)


#define SYS_200MS_THREAD_STACK     (1024U)
#define SYS_200MS_THREAD_PRIORITY  (5U)
#define SYS_200MS_THREAD_TIMESLICE (5U)
#define SYS_200MS_THREAD_SLEEP     (200U)
static struct rt_thread sys_200ms_thread;


rt_align(RT_ALIGN_SIZE) static rt_uint8_t sys_200ms_thread_stack[SYS_200MS_THREAD_STACK];

static void sys_200ms_thread_entry(void *parameter);
int         main(void)
{
    rt_err_t ret = RT_EOK;

#ifdef RT_USING_CONSOLE
    /* Set the console output device */
    rt_console_set_device("vcom");
#endif
#ifdef RT_USING_FINSH
    /* Set the shell output device */
    finsh_set_device("vcom");
#endif

    ret |= rt_thread_init(&sys_200ms_thread,
                          "200ms",
                          sys_200ms_thread_entry,
                          RT_NULL,
                          &sys_200ms_thread_stack[0U],
                          SYS_200MS_THREAD_STACK,
                          SYS_200MS_THREAD_PRIORITY,
                          SYS_200MS_THREAD_TIMESLICE);
    ret |= rt_thread_startup(&sys_200ms_thread);
    SYS_ASSERT(ret);

    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

rt_device_t           li_dev;
struct rt_sensor_data data;
static void           sys_200ms_thread_entry(void *parameter)
{
    rt_err_t ret;

    rt_tick_t sys_200ms_inc_tick  = rt_tick_from_millisecond(SYS_200MS_THREAD_SLEEP);
    rt_tick_t sys_200ms_next_tick = rt_tick_get() + sys_200ms_inc_tick;

    li_dev = rt_device_find("li_bh1750fvi");
    rt_device_open(li_dev, RT_DEVICE_FLAG_FIFO_RX);
    rt_device_control(li_dev, RT_SENSOR_CTRL_SET_POWER, (void *)RT_SENSOR_POWER_LOW);
alarm_sample();
    while (1)
    {
        rt_device_read(li_dev, 0, &data, 1);
        //rt_kprintf("%d\n", data.data.light);
        ret = rt_thread_delay_until(&sys_200ms_next_tick, sys_200ms_inc_tick);
        SYS_ASSERT(ret);
    }
}


void rt_hw_board_init(void)
{
#ifdef BSP_SCB_ENABLE_I_CACHE
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef BSP_SCB_ENABLE_D_CACHE
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();

    /* System clock initialization */
    SystemClock_Config();

#if defined(RT_USING_HEAP)
    /* Heap initialization */
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

#ifdef RT_USING_PIN
    rt_hw_pin_init();
#endif

#ifdef RT_USING_UART
    extern int rt_hw_usart_init(void);
    rt_hw_usart_init();
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE) && defined(RT_USING_UART)
    /* Set the shell console output device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_NANO)
    extern void rt_hw_console_init(void);
    rt_hw_console_init();
#endif

#ifdef RT_USING_COMPONENTS_INIT
    extern int cherry_usb_vcom_ringbuffer_create(void);
    cherry_usb_vcom_ringbuffer_create();
    /* Board underlying hardware initialization */
    rt_components_board_init();
#endif
}

