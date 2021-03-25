#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

#include "current_time_service.h"
#include "ens_records_api.h"
#include "temporary_key_list.h"

date_time time = {2021, 1, 1, 12, 25, 25, 1, 1, 0};

// Epoch time 01.01.2000 00:00:00 H
uint32_t base_time        = 946684800;
uint32_t last_time_update = 0;
uint32_t current_time     = 0;

static ssize_t read_time(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    void* buf,
    u16_t len,
    u16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &time, sizeof(time));
}

static ssize_t write_time(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    const void* buf,
    uint16_t len,
    uint16_t offset,
    uint8_t flags)
{
    time = *(date_time*)buf;
    return len;
}

static ssize_t write_time_epoch(
    struct bt_conn* conn,
    const struct bt_gatt_attr* attr,
    const void* buf,
    uint16_t len,
    uint16_t offset,
    uint8_t flags)
{
    memcpy(&last_time_update, buf, sizeof(uint32_t));
    current_time     = last_time_update - base_time;
    last_time_update = k_uptime_get_32();

    if (temporary_keys_entries == 0)
    {
        temporary_keys.list[0] = generate_time_key_pair();
        temporary_keys_entries++;
    }

    // TODO: Remove this test function if not neccesary anymore
    generate_test_data(current_time);
    return len;
}

BT_GATT_SERVICE_DEFINE(
    cts,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_CTS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_CTS_CURRENT_TIME,
        BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ,
        BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,
        read_time,
        write_time,
        NULL),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_ENS_SETTINGS_CTE,
        BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_WRITE,
        NULL,
        write_time_epoch,
        NULL));