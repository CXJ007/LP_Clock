/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "BH1750FVI.h"
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
static struct rt_i2c_bus_device *ibh1750fvi_pi2c_bus = RT_NULL;
static rt_thread_t               ibh1750fvi_thread   = RT_NULL;
static rt_uint16_t              *ibh1750fvi_pbuffer  = RT_NULL;
static rt_uint8_t                u8front;
static rt_uint8_t                u8rear;
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
static rt_err_t   bh1750fvi_write_cmd(const rt_uint8_t u8cmd);
static rt_err_t   bh1750fvi_read_data(const rt_uint8_t *pdata, const rt_uint8_t u8len);
static rt_err_t   bh1750fvi_set_power_mode(struct rt_sensor_device *psensor, const rt_uint8_t u8cmd);
static rt_err_t   bh1750fvi_hw_init(void);
static rt_err_t   bh1750fvi_read_data_poll(struct rt_sensor_device *psensor, rt_uint16_t *pdata);
static rt_err_t   bh1750fvi_write_fifo(rt_uint16_t u16data);
static rt_err_t   bh1750fvi_read_fifo(rt_uint16_t *pdata);
static void       bh1750fvi_thread(void *parameter);
static rt_ssize_t bh1750fvi_fetch_data(struct rt_sensor_device *psensor, void *pbuf, rt_size_t nlen);
static rt_err_t   bh1750fvi_control(struct rt_sensor_device *psensor, int s32cmd, void *pargs);
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
int bh1750fvi_sensor_init(void)
{
    rt_err_t                 nret          = RT_EOK;
    struct rt_sensor_module *pmodule       = RT_NULL;
    rt_sensor_t              psensor_light = RT_NULL;
    struct rt_sensor_ops    *psensor_ops   = RT_NULL;

    pmodule = rt_calloc(0x1U, sizeof(struct rt_sensor_module));
    if (RT_NULL == pmodule)
    {
        nret = -RT_ENOMEM;
    }
    else
    {
        psensor_light = rt_calloc(1U, sizeof(struct rt_sensor_device));
        if (RT_NULL == pmodule)
        {
            rt_free(pmodule);
            pmodule = RT_NULL;
            nret    = -RT_ENOMEM;
        }
        else
        {
            psensor_ops = rt_calloc(1U, sizeof(struct rt_sensor_ops));
            if (RT_NULL == psensor_ops)
            {
                rt_free(pmodule);
                rt_free(psensor_light);
                pmodule       = RT_NULL;
                psensor_light = RT_NULL;
                nret          = -RT_ENOMEM;
            }
            else
            {
                psensor_ops->fetch_data = bh1750fvi_fetch_data;
                psensor_ops->control    = bh1750fvi_control;

                psensor_light->info.type       = RT_SENSOR_CLASS_LIGHT;
                psensor_light->info.vendor     = RT_SENSOR_VENDOR_UNKNOWN;
                psensor_light->info.model      = "BH1750FVI";
                psensor_light->info.unit       = RT_SENSOR_UNIT_LUX;
                psensor_light->info.intf_type  = RT_SENSOR_INTF_I2C;
                psensor_light->info.range_max  = BH1750FVI_SENSOR_RANGE_MAX;
                psensor_light->info.range_min  = BH1750FVI_SENSOR_RANGE_MIN;
                psensor_light->info.period_min = BH1750FVI_SENSOR_PERIOD_MS_MIN;
                psensor_light->info.fifo_max   = 0U;
                psensor_light->data_len        = 0U;

                psensor_light->config.intf.dev_name = BH1750FVI_SENSOR_NAME;
                psensor_light->config.intf.type     = RT_SENSOR_INTF_I2C;

                psensor_light->ops    = psensor_ops;
                psensor_light->module = pmodule;

                nret = rt_hw_sensor_register(psensor_light, BH1750FVI_SENSOR_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_FIFO_RX, RT_NULL);
                if (RT_EOK == nret)
                {
                    pmodule->sen[0]  = psensor_light;
                    pmodule->sen_num = 1U;
                }
                else
                {
                    rt_free(pmodule);
                    rt_free(psensor_light);
                    rt_free(psensor_ops);
                    pmodule       = RT_NULL;
                    psensor_light = RT_NULL;
                    psensor_ops   = RT_NULL;
                }
            }
        }
    }

    return nret;
}
INIT_DEVICE_EXPORT(bh1750fvi_sensor_init);

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
static rt_err_t bh1750fvi_write_cmd(rt_uint8_t u8cmd)
{
    rt_err_t          nret;
    struct rt_i2c_msg smsgs;

    smsgs.addr  = BH1750FVI_ADDRESS;
    smsgs.flags = RT_I2C_WR;
    smsgs.buf   = (rt_uint8_t *)&u8cmd;
    smsgs.len   = 0x1U;

    if (rt_i2c_transfer(ibh1750fvi_pi2c_bus, &smsgs, 1) == 1)
    {
        nret = RT_EOK;
    }
    else
    {
        nret = -RT_ERROR;
    }

    return nret;
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
static rt_err_t bh1750fvi_read_data(const rt_uint8_t *pdata, const rt_uint8_t u8len)
{
    rt_err_t          nret;
    struct rt_i2c_msg smsgs;

    smsgs.addr  = BH1750FVI_ADDRESS;
    smsgs.flags = RT_I2C_RD;
    smsgs.buf   = (rt_uint8_t *)pdata;
    smsgs.len   = u8len;

    if (rt_i2c_transfer(ibh1750fvi_pi2c_bus, &smsgs, 1) == 1)
    {
        nret = RT_EOK;
    }
    else
    {
        nret = -RT_ERROR;
    }

    return nret;
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
static rt_err_t bh1750fvi_hw_init(void)
{
    rt_err_t nret = RT_EOK;

    ibh1750fvi_pi2c_bus = (struct rt_i2c_bus_device *)rt_device_find(BH1750FVI_I2C_BUS_NAME);

    if (RT_NULL == ibh1750fvi_pi2c_bus)
    {
        nret = RT_ERROR;
    }
    else
    {
#ifdef BH1750FVI_USING_DIV_PIN
        rt_pin_mode(BH1750FVI_USING_DIV_PIN, PIN_MODE_OUTPUT);
        /* Hard reset */
        rt_pin_write(BH1750FVI_USING_DIV_PIN, PIN_LOW);
        rt_hw_us_delay(10U);
        rt_pin_write(BH1750FVI_USING_DIV_PIN, PIN_HIGH);
#endif
        /* Soft reset */
        nret |= (-bh1750fvi_write_cmd(BH1750FVI_POWER_ON_CMD));
        /* Clear data register value */
        nret |= (-bh1750fvi_write_cmd(BH1750FVI_SOFT_RESET_CMD));
    }

    return -nret;
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
static rt_err_t bh1750fvi_read_data_poll(struct rt_sensor_device *psensor, rt_uint16_t *pdata)
{
    rt_uint8_t  u8cmd;
    rt_uint16_t RxBuffer;
    rt_err_t    nret = RT_EOK;

    if (RT_SENSOR_POWER_NORMAL == psensor->config.power)
    {
        u8cmd = BH1750FVI_CONTIN_0P5_CMD;
    }
    else
    {
        u8cmd = BH1750FVI_ONCE_0P5_CMD;
    }

    nret |= (-bh1750fvi_write_cmd(u8cmd));
    rt_thread_mdelay(150);
    nret |= (-bh1750fvi_read_data((rt_uint8_t *)&RxBuffer, BH1750FVI_SENSOR_DATA_BYTE_LEN));

    *pdata = RxBuffer / BH1750FVI_ACCURACY;

    return nret;
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
static void bh1750fvi_thread(void *parameter)
{
    rt_err_t                        nret;
    rt_uint16_t                     u16buf;
    rt_uint8_t                      u8cmd;
    struct rt_sensor_device * const psensor = (struct rt_sensor_device *)parameter;

    while (1)
    {
        if (((u8rear + 1) % BH1750FVI_SENSOR_FIFO_MAX) != u8front)
        {
            if (RT_SENSOR_POWER_NORMAL == psensor->config.power)
            {
                u8cmd = BH1750FVI_CONTIN_0P5_CMD;
            }
            else
            {
                u8cmd = BH1750FVI_ONCE_0P5_CMD;
            }
            nret = bh1750fvi_write_cmd(u8cmd);
            rt_thread_mdelay(150);
            nret = bh1750fvi_read_data((rt_uint8_t *)&u16buf, BH1750FVI_SENSOR_DATA_BYTE_LEN);
            if (RT_EOK == nret)
            {
                nret = bh1750fvi_write_fifo(u16buf / BH1750FVI_ACCURACY);
            }
            else
            {
                Error_Handler();
            }
        }
        else
        {
            if(RT_EOK == rt_thread_suspend(ibh1750fvi_thread))
            {
                rt_schedule();
            }
            else
            {
                Error_Handler();
            }
        }
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
static rt_err_t bh1750fvi_write_fifo(rt_uint16_t u16data)
{
    rt_err_t nret = RT_EOK;

    if (((u8rear + 1) % BH1750FVI_SENSOR_FIFO_MAX) != u8front)
    {
        ibh1750fvi_pbuffer[u8rear] = u16data;
        u8rear                     = (u8rear + 1) % BH1750FVI_SENSOR_FIFO_MAX;
    }
    else
    {
        nret = -RT_EFULL;
    }

    return nret;
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
static rt_err_t bh1750fvi_read_fifo(rt_uint16_t *pdata)
{
    rt_err_t nret = RT_EOK;

    if (u8front != u8rear)
    {
        *pdata  = ibh1750fvi_pbuffer[u8front];
        u8front = (u8front + 1) % BH1750FVI_SENSOR_FIFO_MAX;
    }
    else
    {
        nret = -RT_EEMPTY;
    }

    return nret;
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
static rt_err_t bh1750fvi_set_power_mode(struct rt_sensor_device *psensor, const rt_uint8_t u8cmd)
{
    rt_err_t nret;

    switch (u8cmd)
    {
    case RT_SENSOR_POWER_DOWN:
        nret = bh1750fvi_write_cmd(BH1750FVI_POWER_DOWN_CMD);
        if (RT_SENSOR_MODE_FIFO == psensor->config.mode)
        {
            nret = rt_thread_delete(ibh1750fvi_thread);
            rt_free(ibh1750fvi_pbuffer);
            ibh1750fvi_pbuffer = RT_NULL;
        }
        else
        {
            /* nothing */
        }
        break;
    case RT_SENSOR_POWER_NORMAL:
        nret = bh1750fvi_hw_init();
        break;
    case RT_SENSOR_POWER_LOW:
        nret = RT_EOK;
        break;
    default:
        nret = -RT_EINVAL;
        break;
    }

    return nret;
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
static rt_err_t bh1750fvi_set_work_mode(struct rt_sensor_device * const psensor, const rt_uint8_t u8cmd)
{
    rt_err_t nret;

    switch (u8cmd)
    {
    case RT_SENSOR_MODE_POLLING: {
        nret = RT_EOK;
        break;
    }
    case RT_SENSOR_MODE_FIFO: {
        ibh1750fvi_pbuffer = rt_calloc(BH1750FVI_SENSOR_FIFO_MAX, sizeof(rt_uint16_t));
        if (RT_NULL != ibh1750fvi_pbuffer)
        {
            u8front = 0x0U;
            u8rear  = 0x0U;

            ibh1750fvi_thread = rt_thread_create("bh1750fvi_thread",
                                                 bh1750fvi_thread,
                                                 psensor,
                                                 BH1750FVI_THREAD_STACK,
                                                 BH1750FVI_THREAD_PRIORITY,
                                                 BH1750FVI_THREAD_TIMESLICE);
            if (RT_NULL != ibh1750fvi_thread)
            {
                nret = rt_thread_startup(ibh1750fvi_thread);
            }
            else
            {
                rt_free(ibh1750fvi_pbuffer);
                ibh1750fvi_pbuffer = RT_NULL;
                nret               = -RT_ENOMEM;
            }
        }
        else
        {
            /* nothing */
        }


        break;
    }
    break;
    default: {
        nret = -RT_EINVAL;
        break;
    }
    }

    return nret;
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
static rt_ssize_t bh1750fvi_fetch_data(struct rt_sensor_device *psensor, void *pbuf, rt_size_t nlen)
{
    rt_ssize_t             nidx;
    rt_uint16_t            u16data;
    rt_err_t               nret;
    struct rt_sensor_data *psensor_data = pbuf;

    for (nidx = 0x0U; nidx < nlen; nidx++)
    {
        if (RT_SENSOR_MODE_POLLING == psensor->config.mode)
        {
            nret = bh1750fvi_read_data_poll(psensor, &u16data);
        }
        else if (RT_SENSOR_MODE_FIFO == psensor->config.mode)
        {
            nret = bh1750fvi_read_fifo(&u16data);
            if(RT_THREAD_SUSPEND_UNINTERRUPTIBLE == ibh1750fvi_thread->stat)
            {
                if(RT_EOK == rt_thread_resume (ibh1750fvi_thread))
                {
                    /* nothing */
                }
                else
                {
                    Error_Handler();
                }
            }
            else
            {
                /* nothing */
            }
        }
        else
        {
            nret = -RT_EINVAL;
        }
        if (RT_EOK == nret)
        {
            psensor_data[nidx].data.light = u16data;
        }
        else
        {
            break;
        }
    }


    return nidx;
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
static rt_err_t bh1750fvi_control(struct rt_sensor_device *psensor, int s32cmd, void *pargs)
{
    rt_err_t nret;

    switch (s32cmd)
    {
    case RT_SENSOR_CTRL_SET_MODE: {
        nret = bh1750fvi_set_work_mode(psensor, (const rt_uint8_t)pargs);
        break;
    }
    case RT_SENSOR_CTRL_SET_POWER: {
        nret = bh1750fvi_set_power_mode(psensor, (const rt_uint8_t)pargs);
        break;
    }

    default:
        nret = -RT_EINVAL;
        break;
    }

    return -nret;
}
