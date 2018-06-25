/*
 * @file clock.h
 *
 * Created on: 11 juil. 2012
 * @author tombelle
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#define MS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MS_PER_MINUTE (60*MS_PER_SECOND)
#define MINUTES_PER_HOUR 60
#define MS_PER_HOUR (MINUTES_PER_HOUR*MS_PER_MINUTE)
#define HOURS_PER_DAY 24
#define MS_PER_DAY (HOURS_PER_DAY*MS_PER_HOUR)
#define DAYS_PER_WEEK 7
#define MS_PER_WEEK (DAYS_PER_WEEK*MS_PER_DAY)

/*
 * ***************************************
 * NE PAS MODIFIER LES FICHIERS D'EN-TETE
 * ***************************************
 */

typedef struct Clock *IClock;

long long clkGetTime(IClock clk);
void clkSetTime(IClock clk, long long lNow);
void clkSetDay(IClock clk, int iDay);
int clkGetDay(IClock clk);
void clkSetHour(IClock clk, int iHour);
int clkGetHour(IClock clk);
void clkSetMinute(IClock clk, int iMinute);
int clkGetMinute(IClock clk);
int clkGetSecond(IClock clk);

IClock createClock(void);

#endif /* CLOCK_H_ */
