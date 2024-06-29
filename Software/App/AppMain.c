#include "AppMain.h"
#include "RX8900.h"



TX_THREAD AppTask10msTCB;

static uint32 AppTask10msStk[APP_CFG_TASK_10MS_STK_SIZE / 4];

static void AppTask10ms(ULONG thread_input);



Std_ReturnType AppMain(void)
{
	Std_ReturnType RetVal = E_OK;
	
//	HAL_DBGMCU_EnableDBGSleepMode();
//	HAL_DBGMCU_EnableDBGStopMode();
	
	RetVal |= RX8900_Init();
	
	RetVal |= (Std_ReturnType)tx_thread_create(&AppTask10msTCB,           
																						"App Task 10ms",             
																						AppTask10ms,                 
																						0,                            
																						&AppTask10msStk[0],          
																						APP_CFG_TASK_10MS_STK_SIZE,    
																						APP_CFG_TASK_10MS_PRIO,        
																						APP_CFG_TASK_10MS_PRIO,      
																						TX_NO_TIME_SLICE,              
																						TX_AUTO_START);
	return RetVal;
}

uint8 a, b, c;

static void AppTask10ms(ULONG thread_input)
{
	(void)thread_input;
	Std_ReturnType RetVal;
	RX8900TimeType Time;
	Time.sec = 50;
	Time.min = 21;
	Time.hour = 23;
	Time.week = RX8900_WEEK_WED;
	Time.day = 25;
	Time.mon = 6;
	Time.year = 24;
	
	RX8900_Set_Time(Time);

	RX8900_Set_Alarm(22,23,0x7F);
	RX8900_Get_Alarm(&a,&b,&c);

	while (1)
	{ 
		RX8900_Main_Fun();
		//RX8900_Updata_Time(&Time); 
		tx_thread_sleep(MS_TO_TICKS(10));
		//HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
	}
}


















// HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
// {
//         return HAL_OK;
// }

// uint32_t HAL_GetTick (void)
// {
// 	static uint32_t ticks = 0U;
// 	uint32_t indx;

// 	if (_tx_thread_system_state == TX_INITIALIZE_IS_FINISHED)
// 	{
// 		ticks = (uint32_t)tx_time_get();
// 	}
// 	else
// 	{
// 		for (indx = (SystemCoreClock >> 14U); indx > 0U; indx--)
// 		{
// 						__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
// 						__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
// 		}
// 		ticks++;
// 	}

// 	return ticks;
// }


