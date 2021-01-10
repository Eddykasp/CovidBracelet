#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>

// Epoch time
extern uint32_t base_time;
// Will be used as k_uptime_get_32 to remember when the update came while device is running
extern uint32_t last_time_update;
// the current time we calculate from the current time epoch characteristik
extern uint32_t current_time;

// f6002cd8-4fcb-4199-94a9-af733e355582
// Current time epoch
#define BT_UUID_ENS_SETTINGS_CTE                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xf6002cd8, 0x4fcb, 0x4199, 0x94a9, 0xaf733e355582))