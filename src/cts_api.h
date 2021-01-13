#ifndef CTS_API_H
#define CTS_API_H
#include <bluetooth/uuid.h>

// Return the last epoch time when the client sent a time update
uint32_t get_last_client_update();

// return the current epoch time;
uint32_t get_current_time();

#endif