#include "temporary_key_list.h"
#include "current_time_service.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>

temporary_key_pair test = {
    .time = 0,
    .key  = {0},
};

// TODO:: implement random uuid. libuuid not available?
void get_random_uuid(uint8_t* key)
{
    sys_rand_get(key, TEMPORARY_KEY_LENGTH);
}

// TODO:: implement list of time/key pairs and use them instead of current time
void get_temporary_advertisement_data(uint8_t* key, uint32_t* time)
{
    get_random_uuid(key);
    *time = current_time + k_uptime_get_32();
}