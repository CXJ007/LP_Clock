#include "AppMain.h"
#include "AppObj.h"
#include "AppModeCtrl.h"
#include "RX8900.h"
#include "HTU21D.h"
#include "Buzzer.h"

uint8 gApp_Moudle = APP_MODE_STARTUP;

TX_THREAD AppTaskModeCtrlTCB;
TX_THREAD AppTask10msTCB;

static uint8 AppTaskModeCtrlStk[APP_CFG_TASK_MODE_CTRL_STK_SIZE];
static uint8 AppTask10msStk[APP_CFG_TASK_10MS_STK_SIZE];

static void AppTaskModeCtrl(ULONG thread_input);
static void AppTask10ms(ULONG thread_input);

void AppMain(void)
{
    Std_ReturnType RetVal = E_OK;
    HAL_DBGMCU_DisableDBGSleepMode();
    HAL_DBGMCU_EnableDBGSleepMode();
    //	HAL_DBGMCU_EnableDBGStopMode();

    RetVal |= HTU21D_Init();
    RetVal |= RX8900_Init();
    RetVal |= Buzzer_Init();
    RetVal |= App_ObjInit();

    RetVal |= (Std_ReturnType)tx_thread_create(&AppTaskModeCtrlTCB,
                                               "App Task Mode Ctrl",
                                               AppTaskModeCtrl,
                                               0U,
                                               AppTaskModeCtrlStk,
                                               APP_CFG_TASK_MODE_CTRL_STK_SIZE,
                                               APP_CFG_TASK_MODE_CTRL_PRIO,
                                               APP_CFG_TASK_MODE_CTRL_PRIO,
                                               TX_NO_TIME_SLICE,
                                               TX_AUTO_START);

    RetVal |= (Std_ReturnType)tx_thread_create(&AppTask10msTCB,
                                               "App Task 10ms",
                                               AppTask10ms,
                                               0U,
                                               AppTask10msStk,
                                               APP_CFG_TASK_10MS_STK_SIZE,
                                               APP_CFG_TASK_10MS_PRIO,
                                               APP_CFG_TASK_10MS_PRIO,
                                               TX_NO_TIME_SLICE,
                                               TX_AUTO_START);

    gApp_Moudle = APP_MODE_RUN;

    if (E_OK != RetVal)
    {
        Error_Handler();
    }
    else
    {
        /* nothing */
    }
}

uint32 TaskModeCtrlCount = 0;
static void AppTaskModeCtrl(ULONG thread_input)
{
    (void)thread_input;
    Std_ReturnType RetVal;
    uint32 AppCtrlCmd;

    while (1)
    {
        TaskModeCtrlCount++;
        RetVal = (Std_ReturnType)tx_event_flags_get(&gApp_ModeCtrlEventGroup,
                                                    0x00000001U,
                                                    TX_OR_CLEAR,
                                                    (ULONG *)&AppCtrlCmd,
                                                    TX_WAIT_FOREVER);
        if (E_OK == RetVal)
        {
            App_ModeCtrl_MainFunc(AppCtrlCmd);
        }
        else
        {
            /* nothing */
        }
    }
}

uint32 Task10msCount = 0;
static void AppTask10ms(ULONG thread_input)
{
    (void)thread_input;
    Std_ReturnType RetVal;
    uint32 NextTime;
    uint32 DelayTicks;

    RX8900TimeType Time;

    Time.sec = 30;
    Time.min = 59;
    Time.hour = 23;
    Time.week = RX8900_WEEK_WED;
    Time.day = 25;
    Time.mon = 6;
    Time.year = 24;

    RX8900_Set_Time(Time);
    Buzzer_ActiveOnce();
    // RX8900_Set_Alarm(22,23,0x7F);
    tx_time_set(0);
    while (1)
    {
        NextTime = tx_time_get() + APP_TASK_10MS_TICKS;

        Task10msCount++;
        RX8900_MainFunc();
        HTU21D_MainFunc(1000);
        // RX8900_Updata_Time(&gRX8900TimeInfo);

        // HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

        DelayTicks = NextTime - tx_time_get();
        if (APP_TASK_10MS_TICKS <= DelayTicks)
        {
            tx_thread_sleep(DelayTicks);
        }
        else
        {
            /* nothing */
        }
    }
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
    static uint32_t ticks = 0U;
    uint32_t LoopCount;

    if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
    {
        ticks = (uint32_t)tx_time_get();
    }
    else
    {
        for (LoopCount = (SystemCoreClock >> 14U); LoopCount > 0U; LoopCount--)
        {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
        ticks++;
    }

    return ticks;
}
