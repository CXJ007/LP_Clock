#include "AppMain.h"




TX_THREAD AppTaskStartTCB;

static uint32 AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 4];

static void AppTaskStart(ULONG thread_input);



Std_ReturnType AppMain(void)
{
	Std_ReturnType RetVal;
	
	RetVal = (Std_ReturnType)tx_thread_create(&AppTaskStartTCB,           
																						"App Task Start",             
																						AppTaskStart,                 
																						0,                            
																						&AppTaskStartStk[0],          
																						APP_CFG_TASK_START_STK_SIZE,    
																						APP_CFG_TASK_START_PRIO,        
																						APP_CFG_TASK_START_PRIO,      
																						TX_NO_TIME_SLICE,              
																						TX_AUTO_START);

	
	return RetVal;
}

static void AppTaskStart(ULONG thread_input)
{
	(void)thread_input;
	Std_ReturnType RetVal;
	

	while (1)
	{  
		tx_thread_sleep(MS_TO_TICKS(10));
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


