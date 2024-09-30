#ifndef __BH1750FVI_H__
#define __BH1750FVI_H__
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "bh1750fvi_cfg.h"
#include "drv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*******************************Macro Definitions********************************
*******************************************************************************/
#define BH1750FVI_POWER_DOWN_CMD                               (0x00U)
#define BH1750FVI_POWER_ON_CMD                                 (0x01U)
#define BH1750FVI_SOFT_RESET_CMD                               (0x07U)
#define BH1750FVI_CONTIN_1P0_CMD                               (0x10U)
#define BH1750FVI_CONTIN_0P5_CMD                               (0x11U)
#define BH1750FVI_CONTIN_4P0_CMD                               (0x13U)
#define BH1750FVI_ONCE_1P0_CMD                                 (0x20U)
#define BH1750FVI_ONCE_0P5_CMD                                 (0x21U)
#define BH1750FVI_ONCE_4P0_CMD                                 (0x23U)

#define BH1750FVI_SENSOR_DATA_BYTE_LEN                         (2U)

#define BH1750FVI_ACCURACY                                     (1.2F)

#ifdef  BH1750FVI_SENSOR_NAME
#define BH1750FVI_SENSOR_RANGE_MIN                             (0U)
#define BH1750FVI_SENSOR_RANGE_MAX                             (54612U)
#define BH1750FVI_SENSOR_PERIOD_MS_MIN                         (0U)  
#define BH1750FVI_SENSOR_FIFO_MAX                              (5U)  
#endif


#ifdef __cplusplus
}
#endif
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
extern int rt_hw_bh1750fvi_sensor_init(void);

#endif
