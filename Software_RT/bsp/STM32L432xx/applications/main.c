#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define LED0_PIN GET_PIN(A, 3)

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

extern void cdc_acm_init(uint8_t busid, uint32_t reg_base);
    
int main(void)
{
    rt_err_t ret = RT_EOK;
    
    cdc_acm_init(0,USB_BASE);

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
        extern void cdc_acm_data_send_with_dtr_test(uint8_t busid);
        cdc_acm_data_send_with_dtr_test(0);
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

rt_device_t li_dev;
struct rt_sensor_data data;
static void sys_200ms_thread_entry(void *parameter)
{
    rt_err_t  ret;
    
    rt_tick_t sys_200ms_inc_tick  = rt_tick_from_millisecond(SYS_200MS_THREAD_SLEEP);
    rt_tick_t sys_200ms_next_tick = rt_tick_get() + sys_200ms_inc_tick;
    
    li_dev = rt_device_find("li_bh1750fvi");
    rt_device_open(li_dev, RT_DEVICE_FLAG_FIFO_RX);
    rt_device_control(li_dev, RT_SENSOR_CTRL_SET_POWER, (void *)RT_SENSOR_POWER_LOW);

    while (1)
    {
        rt_device_read(li_dev, 0, &data, 1);
        ret = rt_thread_delay_until(&sys_200ms_next_tick, sys_200ms_inc_tick);
        SYS_ASSERT(ret);
    }
}
