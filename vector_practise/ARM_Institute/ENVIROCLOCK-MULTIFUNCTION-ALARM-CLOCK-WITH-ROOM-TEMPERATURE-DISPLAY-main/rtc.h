#include "types.h"


void Init_RTC(void);
void GetRTCTimeInfo(s32 *,s32 *,s32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(s32 *,s32 *,s32 *);
void DisplayRTCDate(u32,u32,u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);

void GetRTCDay(s32 *);
void DisplayRTCDay(u32);
void SetRTCDay(u32);

void SetRTCHour(u32);
void SetRTCMin(u32);
void SetRTCSec(u32);
void SetRTCDate(u32);
void SetRTCMonth(u32);
void SetRTCYear(u32);
void SetRTCDay(u32);

void GetRTCHour(s32 *);
void GetRTCMin(s32 *);
void GetRTCSec(s32 *);
void GetRTCDate(s32 *);
void GetRTCMonth(s32 *);
void GetRTCYear(s32 *);
void GetDay(void);

void edit_rtc_info(void);
void set_alarm_time(s32 *hour,s32 *minutes);
int max_days_in_month(u32 month,u32 year);
void ring_alarm(s32,s32);
