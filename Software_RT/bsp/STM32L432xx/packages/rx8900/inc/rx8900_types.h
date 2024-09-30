#ifndef __RX8900_types_H__
#define __RX8900_types_H__
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
/* Temperature compensation selection */
#define RX8900_CSEL_0X5S    (0x0U)
#define RX8900_CSEL_2S      (0x1U)
#define RX8900_CSEL_10S     (0x2U)
#define RX8900_CSEL_30S     (0x3U)
/* Timer source clock selection */
#define RX8900_TSEL_4096HZ  (0x0U)
#define RX8900_TSEL_64HZ    (0x1U)
#define RX8900_TSEL_SEC     (0x2U)
#define RX8900_TSEL_MIN     (0x3U)
/* Output Frequency selection */
#define RX8900_FSEL_32768HZ (0x0U)
#define RX8900_FSEL_1024HZ  (0x1U)
#define RX8900_FSEL_1HZ     (0x2U)
/* Update Interrupt Select */
#define RX8900_USEL_SEC     (0x0U)
#define RX8900_USEL_MIN     (0x1U)
/* Week Alarm/Day Alarm */
#define RX8900_WADA_WEEK    (0x0U)
#define RX8900_WADA_DAY     (0x1U)
/*******************************************************************************
*******************************Enum Definitions*********************************
*******************************************************************************/
typedef enum
{
    RX8900_SEC_REG = 0x0U,
    RX8900_MIN_REG,
    RX8900_HOUR_REG,
    RX8900_WEEK_REG,
    RX8900_DAY_REG,
    RX8900_MONTH_REG,
    RX8900_YEAR_REG,
    RX8900_RAM_REG,
    RX8900_MIN_ALARM_REG,
    RX8900_HOUR_ALARM_REG,
    RX8900_WEEKDAY_ALARM_REG,
    RX8900_TIMER_COUNTER0_REG,
    RX8900_TIMER_COUNTER1_REG,
    RX8900_EXTENSION_REG,
    RX8900_FLAG_REG,
    RX8900_CONTROL_REG,
    RX8900_SEC_MIRROR_REG,
    RX8900_MIN_MIRROR_REG,
    RX8900_HOUR_MIRROR_REG,
    RX8900_WEEK_MIRROR_REG,
    RX8900_DAY_MIRROR_REG,
    RX8900_MONTH_MIRROR_REG,
    RX8900_YEAR_MIRROR_REG,
    RX8900_TEMP_REG,
    RX8900_BACKUP_FUNCTION_REG,
    RX8900_NOT_USE0_REG,
    RX8900_NOT_USE1_REG,
    RX8900_TIMER_COUNTER0_MIRROR_REG,
    RX8900_TIMER_COUNTER1_MIRROR_REG,
    RX8900_EXTENSION_MIRROR_REG,
    RX8900_FLAG_MIRROR_REG,
    RX8900_CONTROL_MIRROR_REG,
    RX8900_MAX_REG
} rx8900_register_t;
/*******************************************************************************
*******************************Union Definitions********************************
*******************************************************************************/
typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t SECBCD : 7;
        rt_uint8_t RW0_0  : 1;
    } B;
} rx8900_sec_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t MINBCD : 7;
        rt_uint8_t RW0_0  : 1;
    } B;
} rx8900_min_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t HOURBCD : 6;
        rt_uint8_t RW0_0   : 2;
    } B;
} rx8900_hour_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t SUN   : 1;
        rt_uint8_t MON   : 1;
        rt_uint8_t TUE   : 1;
        rt_uint8_t WED   : 1;
        rt_uint8_t THU   : 1;
        rt_uint8_t FRI   : 1;
        rt_uint8_t SAT   : 1;
        rt_uint8_t RW0_0 : 1;
    } B;
} rx8900_week_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t DAYBCD : 6;
        rt_uint8_t RW0_0  : 2;
    } B;
} rx8900_day_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t MONTHBCD : 5;
        rt_uint8_t RW0_0    : 3;
    } B;
} rx8900_month_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t YEARBCD : 8;
    } B;
} rx8900_year_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t RAM_0 : 1;
        rt_uint8_t RAM_1 : 1;
        rt_uint8_t RAM_2 : 1;
        rt_uint8_t RAM_3 : 1;
        rt_uint8_t RAM_4 : 1;
        rt_uint8_t RAM_5 : 1;
        rt_uint8_t RAM_6 : 1;
        rt_uint8_t RAM_7 : 1;
    } B;
} rx8900_ram_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t ALARMBCD : 7;
        rt_uint8_t AE       : 1;
    } B;
} rx8900_min_alarm_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t ALARMBCD : 6;
        rt_uint8_t RAM      : 1;
        rt_uint8_t AE       : 1;
    } B;
} rx8900_hour_alarm_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t ALARMBCD : 7;
        rt_uint8_t AE       : 1;
    } WEEKB;
    struct
    {
        rt_uint8_t ALARMBCD : 6;
        rt_uint8_t RAM_0    : 1;
        rt_uint8_t AE       : 1;
    } DAYB;
} rx8900_week_day_alarm_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t TIMELOW : 8;
    } B;
} rx8900_time_count0_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t TIMEHIGH : 4;
        rt_uint8_t RAM_0    : 1;
        rt_uint8_t RAM_1    : 1;
        rt_uint8_t RAM_2    : 1;
        rt_uint8_t RAM_3    : 1;
    } B;
} rx8900_time_count1_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t TSEL : 2;
        rt_uint8_t FSEL : 2;
        rt_uint8_t TE   : 1;
        rt_uint8_t USEL : 1;
        rt_uint8_t WADA : 1;
        rt_uint8_t TEST : 1;
    } B;
} rx8900_ext_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t VDET  : 1;
        rt_uint8_t VLF   : 1;
        rt_uint8_t RW0_0 : 1;
        rt_uint8_t AF    : 1;
        rt_uint8_t TF    : 1;
        rt_uint8_t UF    : 1;
        rt_uint8_t RW0_1 : 2;
    } B;
} rx8900_flag_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t RESET : 1;
        rt_uint8_t RW0_0 : 2;
        rt_uint8_t AIE   : 1;
        rt_uint8_t TIE   : 1;
        rt_uint8_t UIE   : 1;
        rt_uint8_t CSEL  : 2;
    } B;
} rx8900_control_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    {
        rt_uint8_t TEMP : 8;
    } B;
} rx8900_temp_reg_t;

typedef union
{
    rt_uint8_t U;
    struct
    { 
        rt_uint8_t BKSMP0  : 1;
        rt_uint8_t BKSMP1  : 1;
        rt_uint8_t SWOFF   : 1;
        rt_uint8_t VDETOFF : 1;
        rt_uint8_t RW0_0   : 4;
    } B;
} rx8900_backup_reg_t;

typedef struct
{
    rt_uint8_t sec;
    rt_uint8_t min;
    rt_uint8_t hour;
    rt_uint8_t week;
    rt_uint8_t day;
    rt_uint8_t mon;
    rt_uint8_t year;
} rx8900_time_t;
/*******************************************************************************
*******************************Struct Definitions*******************************
*******************************************************************************/
typedef struct
{
    rx8900_control_reg_t     ControlRegCfg;
    rx8900_ext_reg_t         ExtRegCfg;
    rx8900_time_count0_reg_t TimeCount0RegCfg;
    rx8900_time_count1_reg_t TimeCount1RegCfg;
} rx8900_Cfg_t;



#ifdef __cplusplus
}
#endif
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/


#endif
