#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

#include "current_time_service.h"

typedef struct
{
    uint16_t year;         // 1582-9999
    uint8_t month;         // 1-12
    uint8_t day;           // 1-31
    uint8_t hours;         // 0-23
    uint8_t minutes;       // 0-59
    uint8_t seconds;       // 0-59
    uint8_t day_of_week;   // 1-7(monday-Sunday)
    uint8_t fractions256;  // 0-255 (number of 1/256 fractions of a second)
    uint8_t adjust_reason; // 0 = manual update, 1 = external reference time update, 2 = change
                           // timezone, 3 = change of DST
} date_time;

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
    printk("current time is: %d \n", last_time_update);
    current_time = last_time_update - base_time;
    printk("current time is: %d \n", current_time);
    return len;
}

BT_GATT_SERVICE_DEFINE(
    cts,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_CTS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_CTS_CURRENT_TIME,
        BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_WRITE,
        NULL,
        write_time,
        NULL),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_CTS_CURRENT_TIME,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        read_time,
        NULL,
        NULL),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_ENS_SETTINGS_CTE,
        BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_WRITE,
        NULL,
        write_time_epoch,
        NULL));