/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "rx8900.h"
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
static struct rt_device irx8900_rtc_dev;
#ifdef RT_USING_DEVICE_OPS
static struct rt_device_ops irx8900_rtc_ops;
#endif
#ifdef RX8900_INT_PIN
static time_t   irx8900_timestamp;
struct rt_timer irx8900_timer;
#endif
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
static rt_err_t rx8900_write_reg(rt_device_t dev, rt_uint8_t reg, rt_uint8_t *pdata, rt_uint8_t len);
static rt_err_t rx8900_read_reg(rt_device_t dev, rt_uint8_t reg, rt_uint8_t *pdata, rt_uint8_t len);
static rt_err_t rx8900_get_time(rt_device_t dev, time_t * const ptime);
static rt_err_t rx8900_set_time(rt_device_t dev, time_t * const ptime);
static rt_err_t rx8900_rtc_init(struct rt_device *dev);
static rt_err_t rx8900_rtc_control(rt_device_t dev, int cmd, void *pargs);
#ifdef RX8900_INT_PIN
static void rx8900_rtc_event_process_timer(void *pargs);
static void rx8900_rtc_interrupt_Callout(void *pargs);
#endif
#ifdef RT_USING_ALARM
static rt_err_t rx8900_rtc_set_alarm(struct rt_device *dev, struct rt_rtc_wkalarm *palarm);
static rt_err_t rx8900_rtc_get_alarm(struct rt_device *dev, struct rt_rtc_wkalarm *palarm);
#endif
static rt_err_t rx8900_rtc_get_temp(struct rt_device *dev, float *ptemp);
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
int rt_hw_rx8900_rtc_init(void)
{
    rt_err_t                  ret      = RT_EOK;
    struct rt_i2c_bus_device *pi2c_bus = RT_NULL;

    /* make sure only one 'rtc' device */
    if (RT_NULL != rt_device_find("rtc"))
    {
        ret = -RT_EFULL;
    }
    else
    {
        pi2c_bus = (struct rt_i2c_bus_device *)rt_device_find(RX8900_I2C_BUS_NAME);
        if (RT_NULL == pi2c_bus)
        {
            ret = -RT_EEMPTY;
        }
        else
        {
            /* register rtc device */
            irx8900_rtc_dev.type      = RT_Device_Class_RTC;
            irx8900_rtc_dev.user_data = (void *)pi2c_bus; /* save i2cbus */
#ifdef RT_USING_DEVICE_OPS

            irx8900_rtc_ops.init    = rx8900_rtc_init;
            irx8900_rtc_ops.open    = RT_NULL;
            irx8900_rtc_ops.close   = RT_NULL;
            irx8900_rtc_ops.read    = RT_NULL;
            irx8900_rtc_ops.write   = RT_NULL;
            irx8900_rtc_ops.control = rx8900_rtc_control;
            irx8900_rtc_dev.ops     = &irx8900_rtc_ops;
#else

            irx8900_rtc_dev.init    = rx8900_rtc_init;
            irx8900_rtc_dev.open    = RT_NULL;
            irx8900_rtc_dev.close   = RT_NULL;
            irx8900_rtc_dev.read    = RT_NULL;
            irx8900_rtc_dev.write   = RT_NULL;
            irx8900_rtc_dev.control = rx8900_rtc_control;
#endif
            ret = rt_device_register(&irx8900_rtc_dev, RX8900_DEVICE_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
        }
    }


    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_rx8900_rtc_init);

/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_read_reg(rt_device_t dev, rt_uint8_t reg, rt_uint8_t *pdata, rt_uint8_t len)
{
    struct rt_i2c_msg         msg[2];
    struct rt_i2c_bus_device *pi2c_bus = RT_NULL;

    pi2c_bus     = (struct rt_i2c_bus_device *)dev->user_data;
    msg[0].addr  = RX8900_ARRD;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = &reg;
    msg[1].addr  = RX8900_ARRD;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = len;
    msg[1].buf   = pdata;

    if (rt_i2c_transfer(pi2c_bus, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_write_reg(rt_device_t dev, rt_uint8_t reg, rt_uint8_t *pdata, rt_uint8_t len)
{
    struct rt_i2c_msg         msg[2];
    struct rt_i2c_bus_device *pi2c_bus = RT_NULL;

    pi2c_bus     = (struct rt_i2c_bus_device *)dev->user_data;
    msg[0].addr  = RX8900_ARRD;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = &reg;
    msg[1].addr  = RX8900_ARRD;
    msg[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msg[1].len   = len;
    msg[1].buf   = pdata;
    if (rt_i2c_transfer(pi2c_bus, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_get_time(rt_device_t dev, time_t * const ptime)
{
    rt_err_t   ret = RT_EOK;
    struct tm  time_temp;
    rt_uint8_t buff[7];

    ret = rx8900_read_reg(dev, RX8900_SEC_REG, buff, 7);
    if (ret == RT_EOK)
    {
        time_temp.tm_year = RX8900_BCD_TO_DEC(buff[6]) + 2000U - 1900U;
        time_temp.tm_mon  = RX8900_BCD_TO_DEC(buff[5]) - 1U;
        time_temp.tm_mday = RX8900_BCD_TO_DEC(buff[4]);
        time_temp.tm_hour = RX8900_BCD_TO_DEC(buff[2]);
        time_temp.tm_min  = RX8900_BCD_TO_DEC(buff[1]);
        time_temp.tm_sec  = RX8900_BCD_TO_DEC(buff[0]);
        *ptime            = mktime(&time_temp);
    }
    else
    {
        /* nothing */
    }

    return ret;
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_set_time(rt_device_t dev, time_t * const ptime)
{
    rt_err_t   ret = RT_EOK;
    struct tm *ptime_temp;
    rt_uint8_t buff[7];

    ptime_temp = localtime(ptime);
    if (RT_NULL != ptime_temp)
    {
        buff[6] = RX8900_DEC_TO_BCD(ptime_temp->tm_year + 1900 - 2000);
        buff[5] = RX8900_DEC_TO_BCD(ptime_temp->tm_mon + 1);
        buff[4] = RX8900_DEC_TO_BCD(ptime_temp->tm_mday);
        buff[3] = 0x1 << ptime_temp->tm_wday;
        buff[2] = RX8900_DEC_TO_BCD(ptime_temp->tm_hour);
        buff[1] = RX8900_DEC_TO_BCD(ptime_temp->tm_min);
        buff[0] = RX8900_DEC_TO_BCD(ptime_temp->tm_sec);
        ret     = rx8900_write_reg(dev, RX8900_SEC_REG, buff, 7);
    }
    else
    {
        ret = -RT_ERROR;
    }

    return ret;
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_rtc_init(struct rt_device *dev)
{
    rt_err_t             ret;
    rx8900_control_reg_t control_reg;
    rx8900_ext_reg_t     ext_reg;

    control_reg.U = 0U;
    ext_reg.U     = 0U;

    if (RT_EOK == rx8900_write_reg(dev, RX8900_EXTENSION_REG, &ext_reg.U, 1))
    {
#ifdef RX8900_INT_PIN
        control_reg.B.UIE = 1U;

        if ((RT_EOK == rx8900_write_reg(&irx8900_rtc_dev, RX8900_CONTROL_REG, &control_reg.U, 1))
            && (RT_EOK == rx8900_get_time(dev, &irx8900_timestamp)))
        {
            rt_pin_mode(RX8900_INT_PIN, PIN_MODE_INPUT_PULLUP);
            ret = rt_pin_attach_irq(RX8900_INT_PIN, PIN_IRQ_MODE_FALLING, rx8900_rtc_interrupt_Callout, dev);
            if (RT_EOK == ret)
            {
                ret = rt_pin_irq_enable(RX8900_INT_PIN, PIN_IRQ_ENABLE);

                rt_timer_init(&irx8900_timer,
                              "rx8900_timer",
                              rx8900_rtc_event_process_timer,
                              dev,
                              1,
                              RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
            }
            else
            {
                /* nothing */
            }
        }
        else
        {
            ret = -RT_ERROR;
        }
#else
        ret = rx8900_write_reg(&irx8900_rtc_dev, RX8900_CONTROL_REG, &control_reg.U, 1);
#endif
    }
    else
    {
        ret = -RT_ERROR;
    }

    return ret;
}

#ifdef RT_USING_ALARM
/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_rtc_set_alarm(struct rt_device *dev, struct rt_rtc_wkalarm *palarm)
{
    rt_err_t                    ret;
    rx8900_control_reg_t        control_reg;
    rx8900_min_alarm_reg_t      min_alarm_reg;
    rx8900_hour_alarm_reg_t     hour_alarm_reg;
    rx8900_week_day_alarm_reg_t week_day_alarm_reg;

    rt_uint8_t buff[3];


    if (RT_EOK == rx8900_read_reg(dev, RX8900_CONTROL_REG, &control_reg.U, 1))
    {
        control_reg.B.AIE = 0U;
        if (RT_EOK == rx8900_write_reg(dev, RX8900_CONTROL_REG, &control_reg.U, 1))
        {
            min_alarm_reg.U             = 0U;
            min_alarm_reg.B.AE          = 0U;
            min_alarm_reg.B.ALARMBCD    = RX8900_DEC_TO_BCD(palarm->tm_min);
            hour_alarm_reg.U            = 0U;
            hour_alarm_reg.B.AE         = 0U;
            hour_alarm_reg.B.ALARMBCD   = RX8900_DEC_TO_BCD(palarm->tm_hour);
            week_day_alarm_reg.U        = 0U;
            week_day_alarm_reg.WEEKB.AE = 1U;

            buff[0] = min_alarm_reg.U;
            buff[1] = hour_alarm_reg.U;
            buff[2] = week_day_alarm_reg.U;

            if (RT_EOK == rx8900_write_reg(dev, RX8900_MIN_ALARM_REG, buff, 3))
            {
                if (RT_TRUE == palarm->enable)
                {
                    control_reg.B.AIE = 1U;
                }
                else
                {
                    control_reg.B.AIE = 0U;
                }
                ret = rx8900_write_reg(dev, RX8900_CONTROL_REG, &control_reg.U, 1);
            }
            else
            {
                ret = -RT_ERROR;
            }
        }
        else
        {
            ret = -RT_ERROR;
        }
    }
    else
    {
        ret = -RT_ERROR;
    }

    return ret;
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_rtc_get_alarm(struct rt_device *dev, struct rt_rtc_wkalarm *palarm)
{
    rt_err_t                ret;
    rx8900_control_reg_t    control_reg;
    rx8900_min_alarm_reg_t  min_alarm_reg;
    rx8900_hour_alarm_reg_t hour_alarm_reg;
    rt_uint8_t              buff[2] = {0x0U};

    if (RT_EOK == rx8900_read_reg(dev, RX8900_CONTROL_REG, &control_reg.U, 1))
    {
        palarm->enable = control_reg.B.AIE;
        if (RT_EOK == rx8900_read_reg(dev, RX8900_MIN_ALARM_REG, buff, 2))
        {
            min_alarm_reg.U  = buff[0];
            hour_alarm_reg.U = buff[1];

            palarm->tm_min  = RX8900_BCD_TO_DEC(min_alarm_reg.B.ALARMBCD);
            palarm->tm_hour = RX8900_BCD_TO_DEC(hour_alarm_reg.B.ALARMBCD);
        }
        else
        {
            ret = -RT_ERROR;
        }
    }
    else
    {
        ret = -RT_ERROR;
    }

    return ret;
}
#endif

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_rtc_get_temp(struct rt_device *dev, float *ptemp)
{
    rt_err_t          ret;
    rx8900_temp_reg_t temp_reg;

    if (RT_EOK == rx8900_read_reg(dev, RX8900_TEMP_REG, &temp_reg.U, 1))
    {
        *ptemp = (temp_reg.U * 2 - 187.19) / 3.218;
    }
    else
    {
        ret = -RT_ERROR;
    }


    return ret;
}


#ifdef RX8900_INT_PIN
/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static void rx8900_rtc_event_process_timer(void *pargs)
{
    rt_err_t          ret;
    rx8900_flag_reg_t flag_reg;
    struct rt_device *dev = (struct rt_device *)pargs;

    ret = rx8900_read_reg(dev, RX8900_FLAG_REG, &flag_reg.U, 1);
    if ((RT_EOK == ret) && (flag_reg.U != 0U))
    {
        /* Voltage Detection Flag */
        /* Not an interrupt source */
        if (flag_reg.B.VDET == 0x1U)
        {
            // gRX8900RuntimeNotify |= RX8900_VDET_ERR;
        }
        else
        {
            /* nothing */
        }
        /* Voltage Low Flag */
        /* Not an interrupt source */
        if (flag_reg.B.VLF == 0x1U)
        {
            //gRX8900RuntimeNotify |= RX8900_VLF_ERR;
        }
        else
        {
            /* nothing */
        }
        /* Alarm Flag */
        if (flag_reg.B.AF == 0x1U)
        {
            rt_alarm_update(dev, 1);
        }
        else
        {
            /* nothing */
        }
        /* Wakeup Timer Flag */
        if (flag_reg.B.TF == 0x1U)
        {
            //gRX8900RuntimeNotify |= RX8900_WAKEUP_EVENT;
        }
        else
        {
            /* nothing */
        }
        /* Timer Updata Flag */
        if (flag_reg.B.UF == 0x1U)
        {
            irx8900_timestamp++;
        }
        else
        {
            /* nothing */
        }
        /* clear interrupt flag */
        (void)rx8900_write_reg(dev, RX8900_FLAG_REG, 0, 1);
    }
    else
    {
        /* nothing */
    }
}

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static void rx8900_rtc_interrupt_Callout(void *pargs)
{
    (void)rt_timer_start(&irx8900_timer);
}
#endif

/**
 * @brief   None
 *
 * @note    None
 *
 * @param   None
 *
 * @return  None
 *
 */
static rt_err_t rx8900_rtc_control(rt_device_t dev, int cmd, void *pargs)
{
    time_t          t;
    struct timeval *ptv = RT_NULL;
    rt_err_t        ret = RT_EOK;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_RTC_GET_TIME:
#ifdef RX8900_INT_PIN
        *((time_t *)pargs) = irx8900_timestamp;
#else
        ret = rx8900_get_time(dev, (time_t *)pargs);
#endif
        break;
    case RT_DEVICE_CTRL_RTC_SET_TIME:
#ifdef RX8900_INT_PIN
        irx8900_timestamp = *((time_t *)pargs);
#endif
        ret = rx8900_set_time(dev, (time_t *)pargs);
        break;
    case RT_DEVICE_CTRL_RTC_GET_TIMEVAL:
#ifdef RX8900_INT_PIN
        ptv          = (struct timeval *)pargs;
        ptv->tv_sec  = irx8900_timestamp;
        ptv->tv_usec = 0U;
#else
        ret          = rx8900_get_time(dev, &t);
        ptv          = (struct timeval *)pargs;
        ptv->tv_sec  = t;
        ptv->tv_usec = 0U;
#endif
        break;
    case RT_DEVICE_CTRL_RTC_SET_TIMEVAL:
        ptv = (struct timeval *)pargs;
        t   = ptv->tv_sec;
        ret = rx8900_set_time(dev, &t);
        break;
#ifdef RT_USING_ALARM
    case RT_DEVICE_CTRL_RTC_GET_ALARM:
        ret = rx8900_rtc_get_alarm(dev, (struct rt_rtc_wkalarm *)pargs);
        break;
    case RT_DEVICE_CTRL_RTC_SET_ALARM:
        ret = rx8900_rtc_set_alarm(dev, (struct rt_rtc_wkalarm *)pargs);
        break;
#endif
    case RX8900_EXTERN_CTRL_RTC_GET_TEMP:
        ret = rx8900_rtc_get_temp(dev, (float *)pargs);
        break;
    default:
        return -RT_EINVAL;
    }

    return ret;
}

