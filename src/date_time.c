#include "date_time.h"

date_time time = {
  2021,1,1,12,25,25,1,1,0
};

s64_t date_time_refreshed = 0;

date_time update_time()
{
    //TODO: Calculate time update since last check
    s64_t tmp_time_difference = k_uptime_delta(&date_time_refreshed) / 1000;
    time.seconds = (time.seconds + tmp_time_difference);
    tmp_time_difference = time.seconds / 60;    //minutes
    time.seconds %= 60;
    //Minutes
    printk("time seconds passed %" PRIu64 "\n", tmp_time_difference);
    time.minutes = (time.minutes + tmp_time_difference);
    tmp_time_difference = time.minutes / 60;    //hours
    time.minutes %= 60;
    //Hours
    printk("time minutes passed %" PRIu64 "\n", tmp_time_difference);
    time.hours = (time.hours + tmp_time_difference);
    tmp_time_difference = (time.hours / 24);    //days
    time.hours %= 24;
    //days //TODO: how do find out how days in the month?
    return time;
}

date_time get_date_time()
{
    update_time();
    date_time_refreshed = k_uptime_get();
    return time;
}

void set_date_time(date_time new_time)
{
    time = new_time;
    date_time_refreshed = k_uptime_get();
}