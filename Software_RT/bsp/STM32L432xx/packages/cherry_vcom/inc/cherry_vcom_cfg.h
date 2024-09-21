#ifndef __CHERRY_VCOM_CFG_H__
#define __CHERRY_VCOM_CFG_H__
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


#define CHERRY_VCOM_BUSID                          (0U)

#define CHERRY_VCOM_NAME                           "vcom"

#define CHERRY_VCOM_THREAD_STACK                   (256U)
#define CHERRY_VCOM_THREAD_PRIORITY                (RT_THREAD_PRIORITY_MAX / 2U)
#define CHERRY_VCOM_THREAD_TIMESLICE               (10U)

#define CHERRY_VCOM_TX_BUF_MAX                     (512U)
#define CHERRY_VCOM_RX_BUF_MAX                     (256U)

#ifdef __cplusplus
}
#endif


#endif
