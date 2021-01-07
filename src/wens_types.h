#include <bluetooth/uuid.h>

#ifndef WENS_TYPES_H
#define WENS_TYPES_H

typedef struct {
  uint32_t timestamp;
  // uint32_t temp_key;
  //TODO: the wens spec demands an integer representing the key
} tempkey_timestamp_pair;

typedef struct {
  uint8_t data_retention_policy;    // 0-14 time in days until data is purged
  uint8_t temporary_key_length;     // 0-16 octests integer
  uint16_t maximum_key_duration;    // 0-1440(24h) minutes 
  uint8_t ens_advertisement_length; // 0-29 number of octests in ens advertisement field of the ens advertisement record
  uint8_t maximum_advertisement_duration;     // 1-255 minutes
  uint8_t scan_on_time;             // 1-255 (multiple of 50 ms)  (suggestes 0x06)
  uint16_t scan_off_time;           // 0x0001 - 0xFFFF duration between scanns for ens advertisement (suggested 0x03C = 1 minute)
  uint16_t minimum_advertisement_interval;    // advertising interval when the server broadcasts ENS packets (suggestest 0x0140)
  uint16_t maximum_advertisement_interval;    // must be => minimum (suggestes 0x01B0)
  uint8_t self_pause_resume;        // 0x00 = Disabled; 0x01 = Enabled
} ens_settings;

#endif