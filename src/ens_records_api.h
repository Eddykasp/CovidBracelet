#ifndef ENS_RECORDS_API_H
#define ENS_RECORDS_API_H
#include <bluetooth/uuid.h>

typedef struct __attribute__((packed))
{
    uint8_t length;
    uint8_t type;
    uint8_t* ltv_structure;
} ltv_field;

typedef struct __attribute__((packed))
{
    // TODO: 24 bit int? WTF?
    uint32_t sequence_number;
    uint32_t timestamp;
    uint16_t length;
    ltv_field* ltv_structure;
} ens_record;

extern ens_record* ens_records;

void add_ens_record(const ens_record* new_entry);
ens_record* get_all_records();

ens_record* get_recods_by_timestamps(uint8_t length, uint32_t* timestamps);
ens_record* get_recods_by_sequences(uint8_t length, uint32_t* sequences);

#endif