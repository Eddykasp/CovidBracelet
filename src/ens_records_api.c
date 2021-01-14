#include "ens_records_api.h"
#include <bluetooth/uuid.h>

ens_record* ens_records = {0};
uint32_t next_position  = 0;

bool add_ens_record(ens_record new_entry)
{
    ens_records[next_position] = new_entry;
    next_position++;
    return true;
}

ens_record* get_all_records()
{
    return ens_records;
}

ens_record* get_first_record()
{
    return ens_records;
}

ens_record* get_last_record()
{
    return ens_records;
}

ens_record* get_records_by_sequences(uint32_t start, uint32_t end)
{
    return ens_records;
}

ens_record* get_records_by_timestamps(uint32_t start, uint32_t end)
{
    return ens_records;
}

bool delete_records_by_timestamps(uint32_t start, uint32_t end)
{
    return true;
}
bool delete_records_by_sequences(uint32_t start, uint32_t end)
{
    return true;
}
bool delete_all_records()
{
    return true;
}

bool delete_first_record()
{
    return true;
}
bool delete_last_record()
{
    return true;
}