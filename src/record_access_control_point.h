#ifndef RECORD_ACCESS_CONTROL_POINT_H
#define RECORD_ACCESS_CONTROL_POINT_H
#include "wearable_exposure_notification_service.h"
#include <bluetooth/uuid.h>

// Opcode
#define RACP_OPCODE_DELETE_STORED_RECORDS 0x02
#define RACP_OPCODE_ABPORT_OPERATION 0x03
#define RACP_OPCODE_REPORT_NUMBER_OF_RECORDS 0x04
#define RACP_OPCODE_COMBINED_REPORT 0x07

// Response Opcodes
#define RACP_RESPONSE_NO_OPERATOR 0x00
#define RACP_RESPONSE_NUMBER_OF_RECORDS 0x05
#define RACP_RESPONSE_RESPONSE_CODE 0x06
#define RACP_RESPONSE_COMBINED_REPORT_RESPONSE 0x08

// Operator
#define RACP_OPERATOR_ALL_RECORDS 0x01
#define RACP_OPERATOR_LESS_OR_EQUAL_TO 0x02
#define RACP_OPERATOR_GREATER_OR_EQUAL_TO 0x03
#define RACP_OPERATOR_WITHIN_RANGE_OF 0x04
#define RACP_OPERATOR_FIRST_RECORD 0x05
#define RACP_OPERATOR_LAST_RECORD 0x06

// Operand
// First part of the message is always the type to compare to
// Either the Sequence Number or the Timestamp
// Type will be derived at runtime and compared to 1/2 max/min Values
#define RACP_OPERAND_SEQUENCE_NUMBER 0x01
#define RACP_OPERAND_TIMESTAMP 0x01

// Response
#define RACP_RESPONSE_SUCCESS 0x01
#define RACP_RESPONSE_OPCODE_NOT_SUPPORTED 0x02
#define RACP_RESPONSE_INVALID_OPERATOR 0x03
#define RACP_RESPONSE_OPERATOR_NOT_SUPPORTED 0x04
#define RACP_RESPONSE_INVALID_OPERAND 0x05
#define RACP_RESPONSE_NO_RECORDS_FOUND 0x06
#define RACP_RESPONSE_ABORT_UNSUCCESSFUL 0x07
#define RACP_RESPONSE_PROCEDURE_NOT_COMPLETE 0x08
#define RACP_RESPONSE_OPERAND_NOT_SUPPORTED 0x09

#define RACP_RESPONSE uint16_t

extern bool indicate_enabled;

typedef struct __attribute__((packed))
{
    uint8_t operand_type; // timestamp or sequence number
    uint8_t operand_values[8];
} operand_struct;

typedef struct __attribute__((packed))
{
    uint8_t opcode;
    uint8_t operator;
    operand_struct operand;
} racp_command;

typedef bool (*operand_function)(uint32_t, uint32_t);

racp_command parse_racp_opcodes(const uint8_t* buf, const uint16_t len);

void handle_opcode_delete(racp_command);
void handle_opcode_abort(racp_command);
void handle_opcode_report_records_number(racp_command);
void handle_opcode_combined_report(racp_command);

// delete records or send records via enslog characteristic
void execute_racp(racp_command command);
#endif