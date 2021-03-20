#ifndef ENS_RECORDS_API_H
#define ENS_RECORDS_API_H
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>

// Made up of 1 or more concatenated ltv structures
typedef struct __attribute__((packed))
{
    uint8_t length;        // Size of the ltv_value
    uint8_t type;          // type of the ltv_value
    uint8_t ltv_value[20]; // one of many possivle ltv_values (WENS v0.7)
} ltv_field;

typedef struct __attribute__((packed))
{
    uint8_t sequence_number[3]; // Start at 0 and incremente with each ENS Record to 0xFFFFFF. Than
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

// Used for ENS Logs to figure out what kind of log will be sent
typedef enum
{
    PACKAGE_KIND_COMPLETE,
    PACKAGE_KIND_FIRST,
    PACKAGE_KIND_CONTINUATION,
    PACKAGE_KIND_LAST,
} packet_kind;

typedef enum
{
    NOCHECK,
    SEQUENCENUMBER,
    TIMESTAMP,
} compare_type;

extern ens_record ens_records[10];

extern bool notify_enabled;

// TODO:: Only tests right now. This will later be removed and the flash memory will be used to
// acces records, add or remove records
bool add_ens_record(ens_record new_entry);
static ens_record* get_next_ens_record();
uint16_t get_ens_records_count();
void generate_test_data(uint32_t timestamp);
void generate_test_ltv_field(ltv_field* field);

// TODO::Unimplemented right now since they will depend on the implementation of accessing records
// from flash memory
void delete_first_record();
void delete_last_record();
void get_first_record();
void get_last_record();

// The methods to delete, count and send reports (ens logs)
// start and end are the values to filter the ens recods by
// type will be timestamp or sequence number as the value to be used in filtering the records
void delete_records(uint32_t start, uint32_t end, compare_type type);
void count_records(uint32_t start, uint32_t end, compare_type type);
void combined_report(uint32_t start, uint32_t end, compare_type type);

// Funktion to check if a record holds the filter condition and should be processed
bool check_filter_condition(uint32_t start, uint32_t end, compare_type type, ens_record* to_check);

#endif