#ifndef CURRENT_TIME_SERVICE_H
#define CURRENT_TIME_SERVICE_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
typedef struct
{
    uint16_t year;         // 1582-9999
    uint8_t month;         // 1-12
    uint8_t day;           // 1-31
    uint8_t hours;         // 0-23
    uint8_t minutes;       // 0-59
    uint8_t seconds;       // 0-59
    uint8_t day_of_week;   // 1-7(monday-Sunday)
    uint8_t fractions256;  // 0-255 (number of 1/256 fractions of a second)
    uint8_t adjust_reason; // 0 = manual update, 1 = external reference time update, 2 = change
                           // timezone, 3 = change of DST
} date_time;

// Epoch time
extern uint32_t base_time;
// Will be used as k_uptime_get_32 to remember when the update came while device is running
extern uint32_t last_time_update;
// the current time we calculate from the current time epoch characteristik
extern uint32_t current_time;

// Current time epoch
#define BT_UUID_ENS_SETTINGS_CTE                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xf6002cd8, 0x4fcb, 0x4199, 0x94a9, 0xaf733e355582))

#endif