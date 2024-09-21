/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "cherry_vcom.h"


/*!< endpoint address */
#define CDC_IN_EP  0x81
#define CDC_OUT_EP 0x02
#define CDC_INT_EP 0x83

#define USBD_VID           0xFFFF
#define USBD_PID           0xFFFF
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

/*!< config descriptor size */
#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN)

#ifdef CONFIG_USB_HS
#define CDC_MAX_MPS 512
#else
#define CDC_MAX_MPS 64
#endif
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
/* descriptor */
static const rt_uint8_t icherry_vcom_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x02, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, CDC_MAX_MPS, 0x02),
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'C', 0x00,                  /* wcChar10 */
    'D', 0x00,                  /* wcChar11 */
    'C', 0x00,                  /* wcChar12 */
    ' ', 0x00,                  /* wcChar13 */
    'D', 0x00,                  /* wcChar14 */
    'E', 0x00,                  /* wcChar15 */
    'M', 0x00,                  /* wcChar16 */
    'O', 0x00,                  /* wcChar17 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x02,
    0x02,
    0x01,
    0x40,
    0x01,
    0x00,
#endif
    0x00};

static struct usbd_interface icherry_vcom_intf0;
static struct usbd_interface icherry_vcom_intf1;

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX rt_uint8_t icherry_vcom_rxbuf[CDC_MAX_MPS];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX rt_uint8_t icherry_vcom_txbuf[CDC_MAX_MPS];

volatile rt_uint8_t ep_tx_busy_flag = RT_FALSE;

static rt_thread_t           icherry_vcom_thread        = RT_NULL;
static struct rt_ringbuffer *icherry_vcom_ptxringbuffer = RT_NULL;
static struct rt_ringbuffer *icherry_vcom_prxringbuffer = RT_NULL;
static rt_event_t            icherry_vcom_event         = RT_NULL;
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/
static void     cherry_usb_vcom_bulk_out(rt_uint8_t busid, rt_uint8_t ep, rt_uint32_t nbytes);
static void     cherry_usb_vcom_bulk_in(rt_uint8_t busid, rt_uint8_t ep, rt_uint32_t nbytes);
static void     cherry_usb_vcom_handler(uint8_t busid, uint8_t event);
static rt_err_t cherry_usb_vcom_configure(struct rt_serial_device *serial, struct serial_configure *cfg);
static rt_err_t cherry_usb_vcom_control(struct rt_serial_device *serial, int cmd, void *arg);
static int      cherry_usb_vcom_putc(struct rt_serial_device *serial, char c);
static int      cherry_usb_vcom_getc(struct rt_serial_device *serial);
static void     cherry_vcom_thread_entry(void *parameter);
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
int cherry_usb_vcom_init(void)
{
    rt_err_t                 ret;
    struct serial_configure  config  = RT_SERIAL_CONFIG_DEFAULT;
    struct rt_serial_device *pserial = RT_NULL;
    struct rt_uart_ops      *pops    = RT_NULL;
    /*!< endpoint call back */
    struct usbd_endpoint cdc_out_ep = {
        .ep_addr = CDC_OUT_EP,
        .ep_cb   = cherry_usb_vcom_bulk_out};

    struct usbd_endpoint cdc_in_ep = {
        .ep_addr = CDC_IN_EP,
        .ep_cb   = cherry_usb_vcom_bulk_in};

    usbd_desc_register(CHERRY_VCOM_BUSID, icherry_vcom_descriptor);
    usbd_add_interface(CHERRY_VCOM_BUSID, usbd_cdc_acm_init_intf(CHERRY_VCOM_BUSID, &icherry_vcom_intf0));
    usbd_add_interface(CHERRY_VCOM_BUSID, usbd_cdc_acm_init_intf(CHERRY_VCOM_BUSID, &icherry_vcom_intf1));
    usbd_add_endpoint(CHERRY_VCOM_BUSID, &cdc_out_ep);
    usbd_add_endpoint(CHERRY_VCOM_BUSID, &cdc_in_ep);
    ret = usbd_initialize(CHERRY_VCOM_BUSID, USB_BASE, cherry_usb_vcom_handler);

    if (RT_EOK != ret)
    {
        /* nothing */
    }
    else
    {
        pserial = rt_calloc(1U, sizeof(struct rt_serial_device));
        if (RT_NULL == pserial)
        {
            ret = -RT_ENOMEM;
        }
        else
        {
            pops = rt_calloc(1U, sizeof(struct rt_uart_ops));
            if (RT_NULL == pops)
            {
                ret = -RT_ENOMEM;
                goto exit1;
            }
            else
            {
                pops->configure    = cherry_usb_vcom_configure;
                pops->control      = cherry_usb_vcom_control;
                pops->putc         = cherry_usb_vcom_putc;
                pops->getc         = cherry_usb_vcom_getc;
                pops->dma_transmit = RT_NULL;

                pserial->ops    = pops;
                pserial->config = config;

                ret = rt_hw_serial_register(pserial,
                                            CHERRY_VCOM_NAME,
                                            RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX,
                                            RT_NULL);
                if (RT_EOK != ret)
                {
                    goto exit2;
                }
                else
                {
                    if ((RT_NULL == icherry_vcom_ptxringbuffer) || (RT_NULL == icherry_vcom_prxringbuffer))
                    {
                        ret = -RT_ENOMEM;
                        goto exit2;
                    }
                    else
                    {
                        icherry_vcom_event = rt_event_create("vcom_event", RT_IPC_FLAG_PRIO);
                        if (RT_NULL == icherry_vcom_event)
                        {
                            ret = -RT_ENOMEM;
                            goto exit3;
                        }
                        else
                        {
                            icherry_vcom_thread = rt_thread_create(CHERRY_VCOM_NAME,
                                                                   cherry_vcom_thread_entry,
                                                                   pserial,
                                                                   CHERRY_VCOM_THREAD_STACK,
                                                                   CHERRY_VCOM_THREAD_PRIORITY,
                                                                   CHERRY_VCOM_THREAD_TIMESLICE);
                            if (RT_NULL != icherry_vcom_thread)
                            {
                                ret = rt_thread_startup(icherry_vcom_thread);
                            }
                            else
                            {
                                ret = -RT_ENOMEM;
                                goto exit4;
                            }
                        }
                    }
                }
            }
        }
        if (RT_EOK == ret)
        {
        }
        else
        {
            /* nothing */
        }
    }

    return ret;

exit4:
    rt_event_delete(icherry_vcom_event);
exit3:
    rt_ringbuffer_destroy(icherry_vcom_ptxringbuffer);
    rt_ringbuffer_destroy(icherry_vcom_prxringbuffer);
exit2:
    rt_free(pops);
exit1:
    rt_free(pserial);
    usbd_deinitialize(CHERRY_VCOM_BUSID);

    return ret;
}
INIT_PREV_EXPORT(cherry_usb_vcom_init);

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
int cherry_usb_vcom_ringbuffer_create(void)
{
    icherry_vcom_ptxringbuffer = rt_ringbuffer_create(CHERRY_VCOM_TX_BUF_MAX);
    icherry_vcom_prxringbuffer = rt_ringbuffer_create(CHERRY_VCOM_RX_BUF_MAX);

    return 0U;
}
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
static rt_err_t cherry_usb_vcom_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    return RT_EOK;
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
static rt_err_t cherry_usb_vcom_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    return RT_EOK;
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

volatile uint8_t dtr_enable = 0;

void usbd_cdc_acm_set_dtr(uint8_t busid, uint8_t intf, bool dtr)
{
    if (dtr)
    {
        dtr_enable = 1;
    }
    else
    {
        dtr_enable = 0;
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
static int cherry_usb_vcom_putc(struct rt_serial_device *serial, char c)
{
    if (c == '\n' && (serial->parent.open_flag & RT_DEVICE_FLAG_STREAM))
    {
        rt_ringbuffer_putchar_force(icherry_vcom_ptxringbuffer, '\r');
    }
    else
    {
        /* nothing */
    }
    rt_ringbuffer_putchar_force(icherry_vcom_ptxringbuffer, c);
    rt_event_send(icherry_vcom_event, CHERRY_VCOM_TX_EVENT);

    return 1;
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
static int cherry_usb_vcom_getc(struct rt_serial_device *serial)
{
    int ch;

    if (rt_ringbuffer_getchar(icherry_vcom_prxringbuffer, (rt_uint8_t *)&ch) != 0)
    {
        /* nothing */
    }
    else
    {
        ch = -1;
    }

    return ch;
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
static void cherry_usb_vcom_handler(uint8_t busid, uint8_t event)
{
    switch (event)
    {
    case USBD_EVENT_RESET:
        break;
    case USBD_EVENT_CONNECTED:
        break;
    case USBD_EVENT_DISCONNECTED:
        break;
    case USBD_EVENT_RESUME:
        break;
    case USBD_EVENT_SUSPEND:
        break;
    case USBD_EVENT_CONFIGURED:
        ep_tx_busy_flag = RT_FALSE;
        /* setup first out ep read transfer */
        usbd_ep_start_read(busid, CDC_OUT_EP, icherry_vcom_rxbuf, CDC_MAX_MPS);
        break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
        break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
        break;

    default:
        break;
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
static void cherry_usb_vcom_bulk_out(rt_uint8_t busid, rt_uint8_t ep, rt_uint32_t nbytes)
{
    usbd_ep_start_read(busid, CDC_OUT_EP, icherry_vcom_rxbuf, CDC_MAX_MPS);
    rt_ringbuffer_put_force(icherry_vcom_prxringbuffer, icherry_vcom_rxbuf, nbytes);
    rt_event_send(icherry_vcom_event, CHERRY_VCOM_RX_EVENT);
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
static void cherry_usb_vcom_bulk_in(rt_uint8_t busid, rt_uint8_t ep, rt_uint32_t nbytes)
{
    if ((nbytes % usbd_get_ep_mps(busid, ep)) == 0 && nbytes)
    {
        usbd_ep_start_write(busid, CDC_IN_EP, NULL, 0);
    }
    else
    {
        ep_tx_busy_flag = RT_FALSE;
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
static void cherry_vcom_thread_entry(void *parameter)
{
    rt_uint32_t              tx_data_len;
    rt_uint32_t              event;
    struct rt_serial_device *pserial = (struct rt_serial_device *)parameter;

    while (1)
    {
        rt_event_recv(icherry_vcom_event,
                      CHERRY_VCOM_TX_EVENT | CHERRY_VCOM_RX_EVENT,
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER,
                      &event);
        if ((event & CHERRY_VCOM_TX_EVENT) == CHERRY_VCOM_TX_EVENT)
        {
            while ((rt_ringbuffer_data_len(icherry_vcom_ptxringbuffer) > 0U) && (dtr_enable))
            {
                ep_tx_busy_flag = true;
                tx_data_len     = rt_ringbuffer_get(icherry_vcom_ptxringbuffer, icherry_vcom_txbuf, CDC_MAX_MPS);
                usbd_ep_start_write(CHERRY_VCOM_BUSID, CDC_IN_EP, icherry_vcom_txbuf, tx_data_len);
                while (ep_tx_busy_flag)
                {
                }
            }
        }
        else
        {
            /* nothing */
        }
        if ((event & CHERRY_VCOM_RX_EVENT) == CHERRY_VCOM_RX_EVENT)
        {
            rt_hw_serial_isr(pserial, RT_SERIAL_EVENT_RX_IND);
        }
        else
        {
            /* nothing */
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
void rt_hw_console_output(const char *pstr)
{
    rt_uint32_t strlen;
    rt_uint32_t idx;

    if ((RT_NULL != pstr) && (RT_NULL != icherry_vcom_ptxringbuffer))
    {
        strlen = rt_strlen(pstr);
        for (idx = 0; idx < strlen; idx++)
        {
            if (*(pstr + idx) == '\n')
            {
                rt_ringbuffer_putchar_force(icherry_vcom_ptxringbuffer, '\r');
            }
            else
            {
                /* nothing */
            }
            rt_ringbuffer_putchar_force(icherry_vcom_ptxringbuffer, (rt_uint8_t) * (pstr + idx));
        }
    }
    else
    {
        /* nothing */
    }
}
