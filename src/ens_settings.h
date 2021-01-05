#include <bluetooth/uuid.h>

#ifndef ENS_SETTINGS_H
#define ENS_SETTINGS_H

// Definitions of bluetooth UUIDs of the WENS

// d38a4169-4762-493e-b0fb-6abbdc437e0f 
// ENS Service
#define BT_UUID_ENS_SETTINGS BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xd38a4169, 0x4762, 0x42F0, 0xb0fb, 0x6abbdc437e0f))
// 85118eae-49ef-4255-b253-4967dd7d4f1c
// Data Retention Policy
#define BT_UUID_ENS_SETTINGS_DTP BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x85118eae, 0x49ef, 0x4255, 0xb253, 0x4967dd7d4f1c))
// 1474c96a-38b5-48be-95bc-7f9b78fb4771
// Temporary Key Length
#define BT_UUID_ENS_SETTINGS_TKL BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x1474c96a, 0x38b5, 0x48be, 0x95bc, 0x7f9b78fb4771))




#endif