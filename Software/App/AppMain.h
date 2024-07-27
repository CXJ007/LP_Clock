#ifndef APPMAIN_H
#define APPMAIN_H

#include "tx_api.h"
#include "tx_initialize.h"
#include "tx_thread.h"
#include "main.h"
#include "Std_Types.h"
                
#define APP_MODE_STARTUP                                 (0x00U)
#define APP_MODE_RUN                                     (0x01U)
#define APP_MODE_LP                                      (0x02U)


#define  APP_CFG_TASK_MODE_CTRL_PRIO                     (1U)
#define  APP_CFG_TASK_10MS_PRIO                          (2U)
#define  APP_TASK_10MS_TICKS                             MS_TO_TICKS(10U)

#define  APP_CFG_TASK_MODE_CTRL_STK_SIZE                 (1024U)
#define  APP_CFG_TASK_10MS_STK_SIZE                      (1024U)

extern uint8 gApp_Moudle;

void AppMain(void);

#endif
