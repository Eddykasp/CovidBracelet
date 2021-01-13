#include "cts_api.h"
#include "current_time_service.h"
#include <bluetooth/uuid.h>

uint32_t get_last_client_update()
{
    return last_time_update;
}

uint32_t get_current_time()
{
    return current_time + k_uptime_delta_32(&last_time_update);
}
