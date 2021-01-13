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

// parse command and send delete or get records, use enslog charactacteristic to send
// notifications to client
RACP_RESPONSE execute_racp(racp_command command)
{
    RACP_RESPONSE response = 0;
    uint32_t start         = 0;
    uint32_t end           = 0;
    bool done              = false;
    switch (command.opcode)
    {
    case RACP_OPCODE_DELETE_STORED_RECORDS:
        switch (command.operator)
        {
        case RACP_OPERATOR_ALL_RECORDS:
            delete_all_records();
            break;
        case RACP_OPERATOR_LESS_OR_EQUAL_TO:
            // TODO: Maybe use unpack?
            memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
            if (command.operand.operand_type == RACP_OPERAND_SEQUENCE_NUMBER)
            {
                delete_records_by_sequences(0, end);
            }
            else
            {
                delete_records_by_timestamps(0, end);
            }
            break;
        case RACP_OPERATOR_GREATER_OR_EQUAL_TO:
            // TODO: Maybe use unpack?
            memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
            if (command.operand.operand_type == RACP_OPERAND_SEQUENCE_NUMBER)
            {
                delete_records_by_sequences(start, 0);
            }
            else
            {
                delete_records_by_timestamps(start, 0);
            }
            break;
        case RACP_OPERATOR_WITHIN_RANGE_OF:
            // TODO: Maybe use unpack?
            memcpy(&start, command.operand.operand_values, sizeof(uint32_t));
            memcpy(&end, (command.operand.operand_values + 4), sizeof(uint32_t));
            if (command.operand.operand_type == RACP_OPERAND_SEQUENCE_NUMBER)
            {
                // done = delete_records_by_sequences(start, end);
            }
            else
            {
                // done = delete_records_by_timestamps(start, end);
            }
            break;
        case RACP_OPERATOR_FIRST_RECORD:
            delete_all_records();
            break;
        case RACP_OPERATOR_LAST_RECORD:
            delete_all_records();
            break;
        default:
            break;
        }
        break;
    case RACP_OPCODE_ABPORT_OPERATION:
        /* code */
        break;
    case RACP_OPCODE_REPORT_NUMBER_OF_RECORDS:
        /* code */
        break;
    case RACP_OPCODE_COMBINED_REPORT:
        /* code */
        break;
    default:
        break;
    }

    return response;
}
