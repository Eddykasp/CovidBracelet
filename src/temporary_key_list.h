#ifndef TEMPORARY_KEY_LIST_H
#define TEMPORARY_KEY_LIST_H
#include <bluetooth/uuid.h>

#define TEMPORARY_KEY_LENGTH 16
#define MAX_LIST_LENGTH 30

void get_random_uuid(uint8_t* key);
void get_temporary_advertisement_data(uint8_t* key, uint32_t* time);

void setup_temporary_advertisement_data();

typedef struct __attribute__((packed))
{
    uint32_t time;
    uint8_t key[TEMPORARY_KEY_LENGTH];
} temporary_key_pair;

typedef struct temp_key_list
{
    temporary_key_pair list[MAX_LIST_LENGTH];
} __packed temp_key_list_t;

extern temporary_key_pair test;

#endif