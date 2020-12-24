#include <stdint.h>
#include <zephyr.h>

typedef struct {
  uint16_t year;// 1582-9999
  uint8_t month;// 1-12
  uint8_t day;// 1-31
  uint8_t hours;// 0-23
  uint8_t minutes;// 0-59
  uint8_t seconds;// 0-59
  uint8_t day_of_week;// 1-7(monday-Sunday)  
  uint8_t fractions256;// 0-255 (number of 1/256 fractions of a second)
  uint8_t adjust_reason;// 0 = manual update, 1 = external reference time update, 2 = change timezone, 3 = change of DST
} date_time;

date_time get_date_time();
date_time update_time();
void set_date_time(date_time time);