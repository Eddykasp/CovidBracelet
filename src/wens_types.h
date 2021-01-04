#include <bluetooth/uuid.h>

#ifndef WENS_TYPES_H
#define WENS_TYPES_H

typedef struct tempkey_timestamp_pair {
  uint32_t timestamp;
  // uint32_t temp_key;
  //TODO: the wens spec demands an integer representing the key
};

#endif