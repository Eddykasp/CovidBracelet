#include "record_access_control_point.h"
#include "ens_records_api.h"
#include <string.h>
#include <bluetooth/uuid.h>

#define MY_STACK_SIZE 2000
#define MY_PRIORITY 5
// K_THREAD_DEFINE(my_tid, MY_STACK_SIZE, test_transfere, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
struct k_thread my_thread_data;
k_tid_t my_tid;

racp_command parse_racp_opcodes(const uint8_t* buf, const uint16_t len)
{
    operand_struct operand = {0};
    racp_command command   = {0};
    command.operand        = operand;

    command.opcode               = buf[0];
    command.operator             = buf[1];
    command.operand.operand_type = buf[2];

    // TODO: check problem with little/big endian? have to use bitmasks if this doesn't work
    for (int i = 0; i < len - 3; i++)
    {
        *(command.operand.operand_values + i) = buf[i + 3];
    }

    return command;
}

/*
    Handle the delete operator inside the racp request.
*/
RACP_RESPONSE handle_opcode_delete(racp_command command)
{
    uint32_t start        = 0;
    uint32_t end          = 0;
    uint8_t type          = 0;
    operand_function func = delete_all_records;

    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        break;

    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_WITHIN_RANGE_OF:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_FIRST_RECORD:
        func = delete_first_record;
        break;

    case RACP_OPERATOR_LAST_RECORD:
        func = delete_last_record;
        break;

    default:
        return 0x04;
    }

    my_tid = k_thread_create(
        &my_thread_data,
        my_stack_area,
        K_THREAD_STACK_SIZEOF(my_stack_area),
        func,
        &start,
        &end,
        type,
        MY_PRIORITY,
        0,
        K_NO_WAIT);

    return 0x01;
}

RACP_RESPONSE handle_opcode_abort(racp_command command)
{
    RACP_RESPONSE response = 0x01;
    // TODO: Abort function
    return response;
}
RACP_RESPONSE handle_opcode_report_records_number(racp_command command)
{
    RACP_RESPONSE response = 0x01;
    uint32_t start         = 0;
    uint32_t end           = 0;
    return response;
}

/*
    Handle the combined report operator inside the racp request.
*/
RACP_RESPONSE handle_opcode_combined_report(racp_command command)
{
    uint32_t start        = 0;
    uint32_t end          = 0;
    uint8_t type          = 0;
    operand_function func = get_all_records;

    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        // Nothing to do since this is the default config
        break;

    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_WITHIN_RANGE_OF:
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        type = command.operand.operand_type == RACP_OPERAND_TIMESTAMP;
        break;

    case RACP_OPERATOR_FIRST_RECORD:
        // TODO: does not work with current test_
        func = get_first_record;
        break;

    case RACP_OPERATOR_LAST_RECORD:
        func = get_last_record;
        break;

    default:
        return 0x04;
    }

    // The type is a uint8_t and will be cast into a concrete type in the ens_records_api
    my_tid = k_thread_create(
        &my_thread_data,
        my_stack_area,
        K_THREAD_STACK_SIZEOF(my_stack_area),
        func,
        &start,
        &end,
        type,
        MY_PRIORITY,
        0,
        K_NO_WAIT);

    return 0x01;
}

// parse command and send delete or get records, use enslog charactacteristic to send
// notifications to client
RACP_RESPONSE execute_racp(racp_command command)
{
    RACP_RESPONSE response = 0;

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
        response = 0x02;
        break;
    }

    return response;
}
