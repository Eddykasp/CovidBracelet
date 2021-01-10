/*
 * Copyright (c) 2020 Max Kasperowski
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

#include "ens_settings.h"
#include "temporary_key_list.h"
#include "wens_definitions.h"
#include "wens_types.h"

// test function not sure how and where to actually trigger this
// should be called from within RACP handlers
// still need to figure out segmentation
// static ssize_t log_notify(struct bt_conn *conn, const struct bt_gatt_attr *attr, u16_t len)
//{
//  return bt_gatt_notify(conn, attr, attr->user_data, len);
//}

// tempkeylist read teststring
// should read N tempkey_timestamp_pairs (N between 1 and 30)
static ssize_t read_tmp_keys(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    void* buf,
    u16_t len,
    u16_t offset)
{
    for (int i = 0; i < temporary_keys_entries; i++)
    {
        printk("time = %x \n", temporary_keys.list[i].time);
        printk("new random value = 0x");
        for (int j = 0; j < TEMPORARY_KEY_LENGTH; j++)
        {
            printk("%x", temporary_keys.list[i].key[j]);
        }
        printk(" \n size = %d \n", sizeof(temporary_key_pair));
    }

    return bt_gatt_attr_read(
        conn,
        attr,
        buf,
        len,
        offset,
        temporary_keys.list,
        sizeof(temporary_key_pair) * temporary_keys_entries);
}

static ssize_t read_ens_settings(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    void* buf,
    u16_t len,
    u16_t offset)
{
    uint8_t setting_bytes[ENS_SETTING_SIZE];

    ens_settings_pack(&current_ens_settings, setting_bytes);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, setting_bytes, ENS_SETTING_SIZE);
}

static ssize_t write_ens_settings(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    const void* buf,
    u16_t len,
    uint16_t offset,
    uint8_t flags)
{
    ens_settings_unpack((const uint8_t*)buf, &current_ens_settings);
    return len;
}

BT_GATT_SERVICE_DEFINE(
    wens_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_WENS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_WENS_LOG,
        BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_NONE,
        NULL,
        NULL,
        NULL),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_WENS_TEMPKEYLIST,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        read_tmp_keys,
        NULL,
        "TESTVAlUE"),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_WENS_SETTINGS,
        BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ,
        BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,
        read_ens_settings,
        write_ens_settings,
        &current_ens_settings));