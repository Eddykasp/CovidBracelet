#ifndef TEMP_KEY_API_H
#define TEMP_KEY_API_H
#include "temporary_key_list.h"

// gets recent keys from ENS system
temp_key_list_t* get_temp_key_list();

// replaces ENS keys with new keys
void set_temp_keys(temp_key_list_t* new_keys);

#endif