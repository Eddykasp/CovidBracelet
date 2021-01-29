#include "record_access_control_point.h"
#include "ens_records_api.h"
#include <string.h>
#include <bluetooth/uuid.h>

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

// TODO:: Refactor more if this at least works
RACP_RESPONSE handle_opcode_delete(racp_command command)
{
    RACP_RESPONSE response = 0x01;
    uint32_t start         = 0;
    uint32_t end           = 0;
    bool operation_applied = false;
    operand_function func  = &delete_records_by_sequences;
    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        response = delete_all_records();
        break;
    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        // TODO: Maybe use unpack?
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &delete_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        // TODO: Maybe use unpack?
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &delete_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_WITHIN_RANGE_OF:
        // TODO: Maybe use unpack?
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &delete_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_FIRST_RECORD:
        operation_applied = delete_first_record();
        break;
    case RACP_OPERATOR_LAST_RECORD:
        operation_applied = delete_last_record();
        break;
    default:
        response = 0x04;
        break;
    }
    return response;
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
RACP_RESPONSE handle_opcode_combined_report(racp_command command)
{
    RACP_RESPONSE response = 0x01;
    uint32_t start         = 0;
    uint32_t end           = 0;
    bool operation_applied = false;
    uint8_t length         = 0;
    operand_function func  = &get_records_by_sequences;
    struct ens_records* data;
    switch (command.operator)
    {
    case RACP_OPERATOR_ALL_RECORDS:
        data = get_all_records(&length);
        break;
    case RACP_OPERATOR_LESS_OR_EQUAL_TO:
        // TODO: Maybe use unpack?
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &get_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
        // TODO: Maybe use unpack?
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &get_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_WITHIN_RANGE_OF:
        // TODO: Maybe use unpack?
        memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
        memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
        if (command.operand.operand_type == RACP_OPERAND_TIMESTAMP)
        {
            func = &get_records_by_timestamps;
        }
        operation_applied = func(start, end);
        break;
    case RACP_OPERATOR_FIRST_RECORD:
        operation_applied = get_first_record();
        break;
    case RACP_OPERATOR_LAST_RECORD:
        operation_applied = get_last_record();
        break;
    default:
        response = 0x04;
        break;
    }

    send_notification(data, length);

    return response;
}

// parse command and send delete or get records, use enslog charactacteristic to send
// notifications to client
RACP_RESPONSE execute_racp(racp_command command)
{
    RACP_RESPONSE response = 0;
    uint8_t length         = 0;

    struct ens_records* data;
    data = get_all_records(&length);
    send_notification(data, length);
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
