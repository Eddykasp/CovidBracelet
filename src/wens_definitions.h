/* 
 * Copyright (c) 2020 Max Kasperowski
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/uuid.h>

#ifndef WENS_DEFINITIONS_H
#define WENS_DEFINITIONS_H

// Definitions of bluetooth UUIDs of the WENS

// 4BA23969-090E-42F0-8A89-EAD187DE91D4
#define BT_UUID_WENS BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x4BA23969, 0x090E, 0x42F0, 0x8A89, 0xEAD187DE91D4))
// 9B3C4906-A2CA-4AC6-AF42-B39246005055
#define BT_UUID_WENS_LOG BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x9B3C4906, 0xA2CA, 0x4AC6, 0xAF42, 0xB39246005055))
// 671F1775-DC33-40E4-86B0-B8D458C1C786
#define BT_UUID_WENS_FEATURES BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x671F1775, 0xDC33, 0x40E4, 0x86B0, 0xB8D458C1C786))
// A16B5A93-1C15-4F4A-81B8-3AE4BF5EF7CA
#define BT_UUID_WENS_IDENTIFIER BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA16B5A93, 0x1C15, 0x4F4A, 0x81B8, 0x3AE4BF5EF7CA))
// 3D62B0B1-27D4-41D7-99EF-865576A7F5C0
#define BT_UUID_WENS_SETTINGS BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x3D62B0B1, 0x27D4, 0x41D7, 0x99EF, 0x865576A7F5C0))
// F02355CB-C8F9-4557-96CB-69B0CC0BBA43
#define BT_UUID_WENS_TEMPKEYLIST BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xF02355CB, 0xC8F9, 0x4557, 0x96CB, 0x69B0CC0BBA43))
// 2A52
#define BT_UUID_WENS_RACP BT_UUID_DECLARE_16(0x2A52)
// A2E355F6-504F-4CF9-A110-9E373AE8A893
#define BT_UUID_WENS_STATUS BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355F6, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893))

//TEST
#define BT_UUID_TEST_ID_1 BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355F7, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893))
#define BT_UUID_TEST_ID_1 BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355F8, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893))
#define BT_UUID_TEST_ID_1 BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355F9, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893))
#define BT_UUID_TEST_ID_1 BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xA2E355FA, 0x504F, 0x4CF9, 0xA110, 0x9E373AE8A893))

#endif