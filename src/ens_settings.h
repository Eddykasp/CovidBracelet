#include <bluetooth/uuid.h>

#ifndef ENS_SETTINGS_H
#define ENS_SETTINGS_H
#define ENS_SETTING_SIZE 14

//#pragma pack(push, 1)
// 0 = not supported
typedef struct __attribute__((packed))
{
    uint8_t data_retention_policy;    // 0-14 time in days until data is purged
    uint8_t temporary_key_length;     // 0-16 octests integer
    uint16_t maximum_key_duration;    // 0-1440(24h) minutes
    uint8_t ens_advertisement_length; // 0-29 number of octests in ens advertisement field of the
                                      // ens advertisement record
    uint8_t maximum_advertisement_duration; // 1-255 minutes
    uint8_t scan_on_time;                   // 1-255 (multiple of 50 ms)  (suggestes 0x06)
    uint16_t scan_off_time; // 0x0001 - 0xFFFF duration between scanns for ens advertisement
                            // (suggested 0x03C = 1 minute)
    uint16_t minimum_advertisement_interval; // advertising interval when the server broadcasts ENS
                                             // packets (suggestest 0x0140)
    uint16_t maximum_advertisement_interval; // must be => minimum (suggestes 0x01B0)
    uint8_t self_pause_resume;               // 0x00 = Disabled; 0x01 = Enabled
} ens_settings;

//#pragma pack(pop)

// Definitions of bluetooth UUIDs of the WENS
extern ens_settings current_ens_settings;
// d38a4169-4762-493e-b0fb-6abbdc437e0f
// ENS Service
#define BT_UUID_ENS_SETTINGS                                                                       \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xd38a4169, 0x4762, 0x42F0, 0xb0fb, 0x6abbdc437e0f))
// 85118eae-49ef-4255-b253-4967dd7d4f1c
// Data Retention Policy
#define BT_UUID_ENS_SETTINGS_DTP                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x85118eae, 0x49ef, 0x4255, 0xb253, 0x4967dd7d4f1c))
// 1474c96a-38b5-48be-95bc-7f9b78fb4771
// Temporary Key Length
#define BT_UUID_ENS_SETTINGS_TKL                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x1474c96a, 0x38b5, 0x48be, 0x95bc, 0x7f9b78fb4771))
// c783b906-edee-4d31-8803-22d162c526ec
// Maximum Key Duration
#define BT_UUID_ENS_SETTINGS_MKD                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xc783b906, 0xedee, 0x4d31, 0x8803, 0x22d162c526ec))
// ce97ee98-b1ff-4335-a8c4-721d81793730
// ENS Advertisement Length
#define BT_UUID_ENS_SETTINGS_EAL                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xce97ee98, 0xb1ff, 0x4335, 0xa8c4, 0x721d81793730))
// 3c8c47f5-bd0c-4965-9c9c-c4a77dc7b83f
// Maximum Advertisement Duration
#define BT_UUID_ENS_SETTINGS_MDA                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x3c8c47f5, 0xbd0c, 0x4965, 0x9c9c, 0xc4a77dc7b83f))
// f0c3a656-51bb-453b-9a0a-12cfe2b8e2bb
// Scan On Time
#define BT_UUID_ENS_SETTINGS_SOnTime                                                               \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xf0c3a656, 0x51bb, 0x453b, 0x9a0a, 0x12cfe2b8e2bb))
// a861f64a-7132-45f2-926a-e759a3a3596a
// Scan Off Time
#define BT_UUID_ENS_SETTINGS_SOffTime                                                              \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xa861f64a, 0x7132, 0x45f2, 0x926a, 0xe759a3a3596a))
// 93fcff79-cc5f-470a-98fe-3ddcb46f44ab
// Minimum Advertising Interval
#define BT_UUID_ENS_SETTINGS_MinAI                                                                 \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x93fcff79, 0xcc5f, 0x470a, 0x98fe, 0x3ddcb46f44ab))
// 2d39f769-45ba-4a8e-a233-bac709e7fef0
// Maximum Advertising Interval
#define BT_UUID_ENS_SETTINGS_MaxAI                                                                 \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x2d39f769, 0x45ba, 0x4a8e, 0xa233, 0xbac709e7fef0))
// dd6e490f-5d2a-4688-a1f9-73e2043c188f
// Self-Pause and Self-Resume
#define BT_UUID_ENS_SETTINGS_SPR                                                                   \
    BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xdd6e490f, 0x5d2a, 0x4688, 0xa1f9, 0x73e2043c188f))

void ens_settings_pack(const ens_settings* setting_to_unpack, uint8_t* setting_bytes);
void ens_settings_unpack(const uint8_t* setting_bytes, ens_settings* settings);

#endif