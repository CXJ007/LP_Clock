#ifndef APPMAIN_H
#define APPMAIN_H

#include "tx_api.h"
#include "tx_initialize.h"
#include "tx_thread.h"
#include "main.h"
#include "Std_Types.h"
                

#define  APP_CFG_TASK_10MS_PRIO                          (2U)

#define  APP_CFG_TASK_10MS_STK_SIZE                      (1024U)

Std_ReturnType AppMain(void);

#endif
