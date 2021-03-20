#include "record_access_control_point.h"
#include "ens_records_api.h"
#include <string.h>
#include <bluetooth/uuid.h>

// Thread data
#define MY_STACK_SIZE 20000
#define MY_PRIORITY 5
K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
struct k_thread my_thread_data;
k_tid_t my_tid;
uint16_t start_thread_delay = 500;

bool indicate_enabled = false;

// Parse the bytestring to a racp_command structure
racp_command parse_racp_opcodes(const uint8_t* buf, const uint16_t len)
{
    operand_struct operand = {0};
    racp_command command   = {0};
    command.operand        = operand;
    command.opcode         = buf[0];
    command.operator       = buf[1];

    if (len >= 2)
    {
        command.operand.operand_type = buf[2];

        for (int i = 3; i < len; i++)
        {
            *((unsigned char*)command.operand.operand_values + (i - 3)) = buf[i];
        }
    }

    return command;
}

uint32_t getTimeStamp(uint8_t* operand_value)
{
    uint32_t time_stamp = 0;
    time_stamp |= (*(operand_value + 3) << 24);
    time_stamp |= (*(operand_value + 2) << 16);
    time_stamp |= (*(operand_value + 1) << 8);
    time_stamp |= (*operand_value);
    return time_stamp;
}

uint32_t getSequenceNumber(uint8_t* operand_value)
{
    uint32_t sequence_number = 0;
    sequence_number |= (*(operand_value + 2) << 16);
    sequence_number |= (*(operand_value + 1) << 8);
    sequence_number |= (*operand_value);
    return sequence_number;
}

void functionality_not_supported(uint8_t command, uint8_t type)
{
    unsigned char response_if_error[4];
    response_if_error[0] = RACP_RESPONSE_RESPONSE_CODE;
    response_if_error[1] = 0;
    response_if_error[2] = command;
    response_if_error[3] = type;

    send_racp_response(response_if_error, 4);
}

/*
    Handle the delete operator inside the racp request.
*/
void handle_opcode_delete(racp_command command)
{
    uint32_t start        = 0;
    uint32_t end          = 0;
    uint8_t type          = NOCHECK;
    operand_function func = delete_records;

    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        break;

    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        type = command.operand.operand_type;
        break;

    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        type = command.operand.operand_type;
        break;

    case RACP_OPERATOR_WITHIN_RANGE_OF:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));

        type = command.operand.operand_type;
        break;

    case RACP_OPERATOR_FIRST_RECORD:
        func = delete_first_record;
        break;

    case RACP_OPERATOR_LAST_RECORD:
        func = delete_last_record;
        break;

    default:
        functionality_not_supported(command.opcode, RACP_RESPONSE_OPERATOR_NOT_SUPPORTED);
        return;
    }

    my_tid = k_thread_create(
        &my_thread_data,
        my_stack_area,
        K_THREAD_STACK_SIZEOF(my_stack_area),
        func,
        start,
        end,
        type,
        MY_PRIORITY,
        0,
        K_MSEC(start_thread_delay));

    return 0x01;
}

void handle_opcode_abort(racp_command command)
{
    unsigned char* abort = "unknown";
    k_thread_abort(my_tid);
    unsigned char* result = k_thread_state_str(my_tid);
    unsigned char buffer[4];

    buffer[0] = RACP_RESPONSE_RESPONSE_CODE;
    buffer[1] = RACP_RESPONSE_NO_OPERATOR;
    buffer[2] = RACP_OPCODE_ABPORT_OPERATION;
    buffer[3] =
        strcmp(result, abort) == 0 ? RACP_RESPONSE_SUCCESS : RACP_RESPONSE_ABORT_UNSUCCESSFUL;

    send_racp_response(buffer, 4);
}

void handle_opcode_report_records_number(racp_command command)
{
    uint32_t start        = 0;
    uint32_t end          = 0;
    uint8_t type          = NOCHECK;
    operand_function func = count_records;

    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        // Nothing to do since this is the default config
        break;

    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        type = command.operand.operand_type;
        end  = (type == SEQUENCENUMBER) ? getSequenceNumber(command.operand.operand_values)
                                        : getTimeStamp(command.operand.operand_values);

        break;

    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        type  = command.operand.operand_type;
        start = (type == SEQUENCENUMBER) ? getSequenceNumber(command.operand.operand_values)
                                         : getTimeStamp(command.operand.operand_values);
        break;

    case RACP_OPERATOR_WITHIN_RANGE_OF:
        type = command.operand.operand_type;
        if (type == SEQUENCENUMBER)
        {
            start = getSequenceNumber(command.operand.operand_values);
            end   = getSequenceNumber(command.operand.operand_values + 3);
        }
        else
        {
            start = getTimeStamp(command.operand.operand_values);
            end   = getTimeStamp((command.operand.operand_values + 4));
        }
        break;

    case RACP_OPERATOR_FIRST_RECORD:
        // TODO: not Supported
        break;

    case RACP_OPERATOR_LAST_RECORD:
        // TODO: not Supported
        break;

    default:
        functionality_not_supported(command.opcode, RACP_RESPONSE_OPERATOR_NOT_SUPPORTED);
        return;
    }

    printk("start thread\n");
    // The type is a uint8_t and will be cast into a concrete type in the ens_records_api
    my_tid = k_thread_create(
        &my_thread_data,
        my_stack_area,
        K_THREAD_STACK_SIZEOF(my_stack_area),
        func,
        start,
        end,
        type,
        MY_PRIORITY,
        0,
        K_MSEC(start_thread_delay));
}

/*
    Handle the combined report operator inside the racp request.
*/
void handle_opcode_combined_report(racp_command command)
{
    uint32_t start        = 0;
    uint32_t end          = 0;
    uint8_t type          = NOCHECK;
    operand_function func = combined_report;

    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        // Nothing to do since this is the default config
        break;

    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        type = command.operand.operand_type;
        end  = (type == SEQUENCENUMBER) ? getSequenceNumber(command.operand.operand_values)
                                        : getTimeStamp(command.operand.operand_values);

        break;

    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        type  = command.operand.operand_type;
        start = (type == SEQUENCENUMBER) ? getSequenceNumber(command.operand.operand_values)
                                         : getTimeStamp(command.operand.operand_values);
        break;

    case RACP_OPERATOR_WITHIN_RANGE_OF:
        type = command.operand.operand_type;
        if (type == SEQUENCENUMBER)
        {
            start = getSequenceNumber(command.operand.operand_values);
            end   = getSequenceNumber(command.operand.operand_values + 3);
        }
        else
        {
            start = getTimeStamp(command.operand.operand_values);
            end   = getTimeStamp((command.operand.operand_values + 4));
        }
        break;

    case RACP_OPERATOR_FIRST_RECORD:
        func = get_first_record;
        break;

    case RACP_OPERATOR_LAST_RECORD:
        func = get_last_record;
        break;

    default:
        functionality_not_supported(command.opcode, RACP_RESPONSE_OPERATOR_NOT_SUPPORTED);
        return;
    }

    printk("start thread\n");
    // The type is a uint8_t and will be cast into a concrete type in the ens_records_api
    my_tid = k_thread_create(
        &my_thread_data,
        my_stack_area,
        K_THREAD_STACK_SIZEOF(my_stack_area),
        func,
        start,
        end,
        type,
        MY_PRIORITY,
        0,
        K_MSEC(start_thread_delay));
}

// parse command and send delete or get records, use enslog charactacteristic to send
// notifications to client
void execute_racp(racp_command command)
{
    printk("what do i chose? %u", command.opcode);
    switch (command.opcode)
    {
    case RACP_OPCODE_DELETE_STORED_RECORDS:
        handle_opcode_delete(command);
        break;
    case RACP_OPCODE_ABPORT_OPERATION:
        handle_opcode_abort(command);
        break;
    case RACP_OPCODE_REPORT_NUMBER_OF_RECORDS:
        handle_opcode_report_records_number(command);
        break;
    case RACP_OPCODE_COMBINED_REPORT:
        handle_opcode_combined_report(command);
        break;
    default:
        functionality_not_supported(command.opcode, RACP_RESPONSE_OPCODE_NOT_SUPPORTED);
        break;
    }
}
