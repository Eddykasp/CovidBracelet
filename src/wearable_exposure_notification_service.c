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
#include "record_access_control_point.h"
#include "temporary_key_list.h"
#include "wearable_exposure_notification_service.h"
#include "wens_definitions.h"
#include "wens_types.h"

// test function not sure how and where to actually trigger this
// should be called from within RACP handlers
// still need to figure out segmentation
// static ssize_t log_notify(struct bt_conn *conn, const struct bt_gatt_attr *attr, u16_t len)
//{
//  return bt_gatt_notify(conn, attr, attr->user_data, len);
//}
uint16_t max_mtu = 0;
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
    uint8_t test[3] = {1, 2, 3};
    uint8_t gg[3];
    ens_settings_unpack((const uint8_t*)buf, &current_ens_settings);
    // printk("offset %d", offset);
    // printk("len %d", len);
    // memcpy(buf, test, sizeof(uint8_t) * 3);
    uint8_t length = bt_gatt_attr_read(conn, attr, buf, len, offset, test, 3);
    memcpy(gg, buf, 3);
    for (int i = 0; i < 3; i++)
    {
        printk("byte %x ", gg[i]);
    }
    return len;
}

RACP_RESPONSE response = 0x0001;
struct bt_gatt_indicate_params racp_indication_params;

static ssize_t apply_racp_command(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    const void* buf,
    u16_t len,
    uint16_t offset,
    uint8_t flags)
{
    // Check for MTU every racp request in case MTU changed
    max_mtu = bt_gatt_get_mtu(conn);

    execute_racp(parse_racp_opcodes(buf, len));

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
    BT_GATT_CCC(notify_enabled_ens_records, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
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
        &current_ens_settings),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_WENS_RACP,
        BT_GATT_CHRC_WRITE | BT_GATT_CHRC_INDICATE,
        BT_GATT_PERM_WRITE,
        NULL,
        apply_racp_command,
        &response),
    BT_GATT_CCC(racp_response_indicate, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));

static void notify_enabled_ens_records(const struct bt_gatt_attr* attr, uint16_t value)
{
    notify_enabled = value == BT_GATT_CCC_NOTIFY;
    printk("notify enabled is now %s", notify_enabled ? "true" : "false");
}

void send_notification(uint8_t* ens_log, uint8_t len)
{
    printk("send notify with len %i \n", len);
    bt_gatt_notify(NULL, &wens_svc.attrs[1], ens_log, len);
}

static void racp_response_indicate(const struct bt_gatt_attr* attr, u16_t value)
{
    indicate_enabled = value == BT_GATT_CCC_INDICATE;
    printk("indicate enabled is now %s\n", indicate_enabled ? "true" : "false");
}

static void racp_indication_cb(struct bt_conn* conn, const struct bt_gatt_attr* attr, u8_t err)
{
    printk("Indication %s\n", err != 0 ? "fail" : "success");
}

void send_racp_response(void* response, uint16_t len)
{
    printk("uudi %s", wens_svc.attrs[5].uuid);
    racp_indication_params.attr = &wens_svc.attrs[5];
    racp_indication_params.func = racp_indication_cb;
    racp_indication_params.data = response;
    racp_indication_params.len  = len;

    for (size_t i = 0; i < len; i++)
    {
        printk("%02X", ((const uint8_t*)response)[i]);
    }

    bt_gatt_indicate(NULL, &racp_indication_params);
}

uint16_t get_max_mtu()
{
    return max_mtu;
}