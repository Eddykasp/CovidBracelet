#include "record_access_control_point.h"
#include <bluetooth/uuid.h>

racp_command parse_racp_opcodes(const void* buf, const uint16_t len)
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
RACP_RESPONSE execute_racp(racp_command* command)
{
    return 0;
}
