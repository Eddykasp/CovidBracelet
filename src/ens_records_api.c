#include "ens_records_api.h"
#include "record_access_control_point.h"
#include "wearable_exposure_notification_service.h"
#include <bluetooth/uuid.h>

#define MAX_POSSIBLE_MTU 512
#define MAX_NUM_PACKETS 32
static uint16_t next_record       = 0;
static uint16_t ens_records_count = 0;

ens_record ens_records[10] = {0};
uint32_t next_position     = 0;

bool notify_enabled = false;

static ens_record* get_next_ens_record()
{
    if (next_record < ens_records_count)
        return &ens_records[next_record];
    else
        return NULL;
}

static void ens_record_set_sequence_number(ens_record* record, uint32_t number)
{
    record->sequence_number[0] = (number >> 0) & 0xFF;
    record->sequence_number[1] = (number >> 8) & 0xFF;
    record->sequence_number[2] = (number >> 16) & 0xFF;
}

static uint8_t get_max_packet_size()
{
    return get_max_mtu() - 4; //-3 for the bluetooth header, -1 for the segmentation header
}

uint16_t get_ens_records_count()
{
    return ens_records_count;
}

static uint16_t ens_record_get_total_size(const ens_record* record)
{
    return sizeof(record->sequence_number) + sizeof(record->timestamp) + sizeof(record->length) +
           record->length;
}

static void print_bytes(const void* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printk("%02X", ((const uint8_t*)data)[i]);
    }

    printk("\n");
}

/**
 * Sends one packet. `size` can be at most MAX_MTU - 1.
 *
 * Returns the number of sent bytes or 0 if sending failed.
 */
static size_t send_packet(packet_kind kind, const void* data, size_t size)
{
    uint8_t sending_packet[MAX_POSSIBLE_MTU] = {0};

    switch (kind)
    {
    case PACKAGE_KIND_COMPLETE:
        sending_packet[0] = 0x00;
        break;
    case PACKAGE_KIND_FIRST:
        sending_packet[0] = 0x40;
        break;
    case PACKAGE_KIND_CONTINUATION:
        sending_packet[0] = 0x80;
        break;
    case PACKAGE_KIND_LAST:
        sending_packet[0] = 0xC0;
        break;
    }

    // Packet too large!
    if (size + 1 > get_max_mtu())
    {
        return 0;
    }

    memcpy(&sending_packet[1], data, size);

    print_bytes(sending_packet, size + 1);
    send_notification(sending_packet, size + 1);
    // TODO: FInd out how to get connection intervall and calculate the time to sleep before next
    // send
    // 1 msec is enough right now to not crash the connection
    k_sleep(K_MSEC(1));
    return size;
}

static uint8_t get_packet_size(uint16_t num_bytes_sent, uint16_t num_bytes_total)
{
    uint16_t num_bytes_left = num_bytes_total - num_bytes_sent;

    if (num_bytes_left >= get_max_packet_size())
        return get_max_packet_size();

    return num_bytes_left;
}

static void send_data_buffer(const void* data, uint16_t num_bytes_total)
{
    uint16_t num_bytes_sent  = 0;
    const uint8_t* dataBytes = (const uint8_t*)data;

    if (num_bytes_total <= get_max_packet_size())
    {
        send_packet(PACKAGE_KIND_COMPLETE, data, num_bytes_total);
    }
    else
    {
        num_bytes_sent += send_packet(PACKAGE_KIND_FIRST, dataBytes, get_max_packet_size());

        while (num_bytes_sent + get_packet_size(num_bytes_sent, num_bytes_total) != num_bytes_total)
        {
            num_bytes_sent += send_packet(
                PACKAGE_KIND_CONTINUATION,
                &dataBytes[num_bytes_sent],
                get_max_packet_size());
        }
        uint16_t remaining = get_packet_size(num_bytes_sent, num_bytes_total);

        if (remaining != 0)
        {
            send_packet(PACKAGE_KIND_LAST, &dataBytes[num_bytes_sent], remaining);
        }
    }
}

bool check_filter_condition(uint32_t start, uint32_t end, compare_type type, ens_record* to_check)
{
    if (type == NOCHECK)
    {
        return true;
    }

    bool smaller = false;
    bool greater = false;

    if (type == SEQUENCENUMBER)
    {
        uint32_t ens_sequencenumber = 0;
        ens_sequencenumber          = ens_sequencenumber | (to_check->sequence_number[2] << 16);
        ens_sequencenumber          = ens_sequencenumber | (to_check->sequence_number[1] << 8);
        ens_sequencenumber          = ens_sequencenumber | to_check->sequence_number[0];
        smaller                     = (start == 0) ? true : ens_sequencenumber >= start;
        greater                     = (end == 0) ? true : ens_sequencenumber <= end;
    }
    else
    {
        // If the start = 0, take all records until
        smaller = (start == 0) ? true : to_check->timestamp >= start;
        greater = (end == 0) ? true : to_check->timestamp <= end;
    }

    return smaller && greater;
}

void combined_report(uint32_t start, uint32_t end, compare_type type)
{
    // Get the currently max mtu from the wens
    ens_record* record;

    uint8_t records_as_bytes[MAX_POSSIBLE_MTU];
    uint16_t size_occupied = 0;
    uint16_t size_record   = 0;

    if ((start == 0) && (end == 0))
        type = NOCHECK;

    while (get_next_ens_record() != NULL)
    {
        record = get_next_ens_record();

        // In case this record should not be send, skip it
        if (!check_filter_condition(start, end, type, record))
        {
            next_record++;
            continue;
        }

        size_record = ens_record_get_total_size(record);

        memcpy(&records_as_bytes[size_occupied], record, size_record);

        size_occupied += size_record;
        next_record++;
        // If there is one more record, check if both fit into one packet
        if (next_record < get_ens_records_count())
        {
            ens_record* nextRecord = get_next_ens_record();
            uint16_t size_next     = ens_record_get_total_size(nextRecord);

            if (size_occupied + size_next >= get_max_packet_size())
            {
                // Next packet too big, send only one
                send_data_buffer(&records_as_bytes, size_occupied);
                size_occupied = 0;
            }
        }
        else
        {
            // Last packet in queue, send as usual
            send_data_buffer(&records_as_bytes, size_occupied);
            size_occupied = 0;
        }
    }

    // Check if there is a rest to send
    if (size_occupied != 0)
    {
        send_data_buffer(&records_as_bytes, size_occupied);
    }

    next_record = 0;
}

void count_records(uint32_t start, uint32_t end, compare_type type)
{
    // Get the currently max mtu from the wens
    ens_record* record;
    uint32_t records_counted = 0;

    if ((start == 0) && (end == 0))
        type = NOCHECK;

    while (get_next_ens_record() != NULL)
    {
        record = get_next_ens_record();
        next_record++;
        // In case this record should not be send, skip it
        if (!check_filter_condition(start, end, type, record))
        {
            continue;
        }

        records_counted++;
    }

    // Size of opcode + size of operator + size of uint32_t * 3 at most = 14 bytes
    unsigned char buffer[6] = {};
    buffer[0]               = RACP_RESPONSE_NUMBER_OF_RECORDS;
    buffer[1]               = 0;
    memcpy(buffer + 2, &records_counted, sizeof(uint32_t));

    send_racp_response(buffer, 6);
    next_record = 0;
}

void delete_records(uint32_t start, uint32_t end, compare_type type)
{
    // Get the currently max mtu from the wens
    ens_record* record;

    if ((start == 0) && (end == 0))
        type = NOCHECK;

    while (get_next_ens_record() != NULL)
    {
        record = get_next_ens_record();
        next_record++;

        // In case this record should not be deleted, skip it
        if (!check_filter_condition(start, end, type, record))
        {
            continue;
        }

        // TODO:: Now the ens backend needs to be informed what to delete
    }

    // Size of opcode + size of operator + size of uint32_t * 3 at most = 14 bytes
    unsigned char buffer[3] = {};
    buffer[0]               = RACP_RESPONSE_RESPONSE_CODE;
    buffer[1]               = RACP_OPCODE_DELETE_STORED_RECORDS;
    buffer[2]               = RACP_RESPONSE_SUCCESS;

    send_racp_response(buffer, 3);
    next_record = 0;
}

bool add_ens_record(ens_record new_entry)
{
    ens_records[next_position] = new_entry;
    next_position++;
    return true;
}

void generate_test_ltv_field(ltv_field* field)
{
    for (int i = 0; i < 16; i++)
    {
        memcpy(field->ltv_value + i, &i, sizeof(uint8_t));
    }

    for (int i = 0; i < 4; i++)
    {
        memcpy(field->ltv_value + i + 16, &i, sizeof(uint8_t));
    }
}

void generate_test_data(uint32_t timestamp)
{
    for (int i = 1; i <= 10; i++)
    {
        ltv_field field = {0};
        field.length    = 20;
        field.type      = 0;
        generate_test_ltv_field(&field);

        ens_record record = {0};
        ens_record_set_sequence_number(&record, i);
        record.timestamp        = timestamp + (i * 60);
        record.length           = 22; // Curent size of maximal ltv field (only 1 field)
        record.ltv_structure[0] = field;

        add_ens_record(record);
        ens_records_count = i;
    }
}

void get_first_record()
{
    // TODO:: Add implementation
}

void get_last_record()
{
    // TODO:: Add implementation
}

void delete_first_record()
{
    // TODO:: Add implementation
}

void delete_last_record()
{
    // TODO:: Add implementation
}
