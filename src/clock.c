#include "clock.h"
#include <string.h>

#define TIME_SIZE 6
#define ALARM_SIZE 4

#define START_VALUE 0

#define SECONDS_UNITS 5
#define SECONDS_TENS 4
#define MINUTES_UNITS 3
#define MINUTES_TENS 2
#define HOURS_UNITS 1
#define HOURS_TENS 0


struct clock_s{
    bool valid;
    bool enabled;
    uint16_t ticks_per_second;
    uint16_t ticks_count;
    uint8_t time[TIME_SIZE];
    uint8_t alarm[ALARM_SIZE];
    
};
static struct clock_s instances;

clock_t ClockCreate(uint16_t ticks_per_second){
    instances.valid = false;
    instances.enabled = false;
    instances.ticks_per_second = ticks_per_second;
    instances.ticks_count = START_VALUE;
    memset(instances.time, START_VALUE, TIME_SIZE);
    return &instances;
}

bool ClockGetTime(clock_t clock, uint8_t * time, uint8_t size){
    memcpy(time, clock->time, size);
    return clock->valid;
}

void ClockSetupTime(clock_t clock, uint8_t const * const time, uint8_t size){
    memcpy(clock->time, time, size);
    clock->valid = true;
}

void ClockNewTick(clock_t clock){
    clock->ticks_count++;
    if (clock->ticks_count == clock->ticks_per_second){
        clock->ticks_count = START_VALUE;
        clock->time[SECONDS_UNITS]++;
    }
    if (clock->time[SECONDS_UNITS] == 10){
        clock->time[SECONDS_UNITS] = START_VALUE;
        clock->time[SECONDS_TENS]++;
    }
    if (clock->time[SECONDS_TENS] == 6){
        clock->time[SECONDS_TENS] = START_VALUE;
        clock->time[MINUTES_UNITS]++;
    }
    if (clock->time[MINUTES_UNITS] == 10){
        clock->time[MINUTES_UNITS] = START_VALUE;
        clock->time[MINUTES_TENS]++;
    }
    if (clock->time[MINUTES_TENS] == 6){
        clock->time[MINUTES_TENS] = START_VALUE;
        clock->time[HOURS_UNITS]++;
    }
    if (clock->time[HOURS_UNITS] == 10){
        clock->time[HOURS_UNITS] = START_VALUE;
        clock->time[HOURS_TENS]++;
    }
    if (clock->time[HOURS_TENS] == 2 & clock->time[HOURS_UNITS] == 4 ){
        clock->time[HOURS_UNITS] = START_VALUE;
        clock->time[HOURS_TENS]= START_VALUE;
    }
    
}

void ClockSetupAlarm(clock_t clock, uint8_t const * const alarm, uint8_t size){
    memcpy(clock->alarm, alarm, size);
    clock->enabled = true;
}

bool ClockGetAlarm(clock_t clock, uint8_t * alarm, uint8_t size){
    memcpy(alarm, clock->alarm, size);

    return clock->enabled;
}

bool ClockToggleAlarm(clock_t clock){
    clock->enabled = !clock->enabled;
    return clock->enabled;
}