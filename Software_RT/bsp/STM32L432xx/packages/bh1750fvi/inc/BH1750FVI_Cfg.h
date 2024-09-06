#ifndef __BH1750FVI_CFG_H__
#define __BH1750FVI_CFG_H__
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include <rtthread.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
*******************************Macro Definitions********************************
*******************************************************************************/
#define BH1750FVI_I2C_BUS_NAME                                 "i2c1"

#define BH1750FVI_SENSOR_NAME                                  "bh1750fvi"

#define BH1750FVI_ADDRESS                                      (0x23U)

#define BH1750FVI_USING_DIV_PIN                                (8U)

#define BH1750FVI_THREAD_STACK                                 (512U)
#define BH1750FVI_THREAD_PRIORITY                              (RT_THREAD_PRIORITY_MAX - 1U)
#define BH1750FVI_THREAD_TIMESLICE                             (10U)


#ifdef __cplusplus
}
#endif

#endif
