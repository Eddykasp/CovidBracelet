#include "temporary_key_list.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>

temporary_key_pair test = {
    .time = 0,
    .key = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x00000000, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893)),
};

// TODO:: implement random uuid. libuuid not available?
void get_random_uuid(struct bt_uuid_128* uuid)
{
    test.key =
        BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355F7, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893));
}

// TODO:: implement list of time/key pairs and use them instead of current time
void get_temporary_advertisement_data(struct bt_uuid_128* uuid, uint32_t* time)
{
    get_random_uuid(uuid);
    *time = k_uptime_get_32();
}