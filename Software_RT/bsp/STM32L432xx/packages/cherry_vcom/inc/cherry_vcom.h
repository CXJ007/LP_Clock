#ifndef __CHERRY_VCOM_H__
#define __CHERRY_VCOM_H__
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "cherry_vcom_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
*******************************Macro Definitions********************************
*******************************************************************************/
#define CHERRY_VCOM_TX_EVENT                            (0x1U)
#define CHERRY_VCOM_RX_EVENT                            (0x2U)
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
extern int cherry_usb_vcom_init(void);
extern int cherry_usb_vcom_ringbuffer_create(void);

#ifdef __cplusplus
}
#endif


#endif
