#include "temporary_key_list.h"
#include "current_time_service.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>

// TODO:: Once the advertisement of temporary keys is finished, add functions
// to create new keys once the old key expired

temp_key_list_t temporary_keys = {.list = {0}};
uint8_t temporary_keys_entries = 0;

temporary_key_pair generate_time_key_pair()
{
    temporary_key_pair new_pair = {
        .time = 0,
        .key  = {0},
    };

    // time since last CTS update
    get_temporary_advertisement_data(new_pair.key, &new_pair.time);
    return new_pair;
}

void get_random_uuid(uint8_t* key)
{
    sys_rand_get(key, TEMPORARY_KEY_LENGTH);
}

void get_temporary_advertisement_data(uint8_t* key, uint32_t* time)
{
    get_random_uuid(key);
    *time = current_time + k_uptime_delta_32(&last_time_update);
}