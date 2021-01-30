#ifndef ENS_RECORDS_API_H
#define ENS_RECORDS_API_H
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>

// Made up of 1 or more concatenated ltv structures
typedef struct __attribute__((packed))
{
    uint8_t length;    // Size of the ltv_value
    uint8_t type;      // type of the ltv_value
    uint8_t ltv_value; // one of many possivle ltv_values (WENS v0.7)
} ltv_field;

typedef struct __attribute__((packed))
{
    // TODO: 24 bit int? WTF?
    uint32_t sequence_number;   // Start at 0 and incremente with each ENS Record to 0xFFFFFF. Than
                                // start at 0
    uint32_t timestamp;         // Timestamp of epoch Year 2000. Can be set by CTS
    uint16_t length;            // Length of the rest record in bytes (ltv_field length)
    ltv_field ltv_structure[9]; //>= 1 LTV types
} ens_record;

typedef struct __attribute__((packed))
{
    uint8_t seg_flags; // first 2 bits = segmentation, last 6 bits = flags (unused) = 0000
    ens_record record;
} ens_log;

extern ens_record ens_records[10];

extern bool notify_enabled;

bool add_ens_record(ens_record new_entry);

// len is always the amount of records not the length
ens_record* get_all_records(uint8_t* len);
ens_record* get_records_by_timestamps(uint32_t start, uint32_t end);
ens_record* get_records_by_sequences(uint32_t start, uint32_t end);
ens_record* get_first_record();
ens_record* get_last_record();

// define how end is interpreted
/* Delete all records by timestamps.
If start = 0, delete all records with timestamp <= end
If end = 0, delete all records with timestamp >= start
if both are unequal to 0, delete within range start <= timestamp <= end
*/
bool delete_records_by_timestamps(uint32_t start, uint32_t end);
bool delete_records_by_sequences(uint32_t start, uint32_t end);
bool delete_all_records();
bool delete_first_record();
bool delete_last_record();

ens_log get_packed_record(ens_record* record, uint16_t max_mtu);

void generate_test_data(uint32_t timestamp);

#endif