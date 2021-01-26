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

void generate_test_data(uint32_t timestamp)
{
    printk("starting creation of test data");
    for (int i = 0; i <= 10; i++)
    {
        ltv_field field         = {0};
        field.length            = 1;
        field.type              = 5;
        field.ltv_value         = 1 + i;
        ens_record record       = {0};
        record.sequence_number  = i;
        record.timestamp        = timestamp + (i * 60);
        record.length           = sizeof(ltv_field);
        record.ltv_structure[0] = field;
        printk("add element %i", i);
        add_ens_record(record);
    }
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
