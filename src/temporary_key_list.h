#ifndef TEMPORARY_KEY_LIST_H
#define TEMPORARY_KEY_LIST_H
#include <bluetooth/uuid.h>

void get_random_uuid(struct bt_uuid_128* uuid);
void get_temporary_advertisement_data(struct bt_uuid_128* uuid, uint32_t* time);

void setup_temporary_advertisement_data();

typedef struct __attribute__((packed))
{
    uint32_t time;
    struct bt_uuid_128 key;
} temporary_key_pair;

extern temporary_key_pair test;

#endif