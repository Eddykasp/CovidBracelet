/* 
 * Copyright (c) 2020 Max Kasperowski
 * 
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>

#include "wens_definitions.h"
#include "wens_types.h"

// Initialize with suggested settings
ens_settings settings = {14, 16, 1440, 29, 10, 300, 60, 200, 270, 0};

// test function not sure how and where to actually trigger this
// should be called from within RACP handlers
// still need to figure out segmentation
static ssize_t log_notify(struct bt_conn *conn, const struct bt_gatt_attr *attr, u16_t len)
{
  return bt_gatt_notify(conn, attr, attr->user_data, len);
}

// tempkeylist read teststring
// expand characteristic later to be read/write
// should read N tempkey_timestamp_pairs (N between 1 and 30)
static ssize_t read_str(struct bt_conn *conn, const struct bt_gatt_attr *attr, 
                        void *buf, u16_t len, u16_t offset)
{
  return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, 
                            strlen(attr->user_data));
}

static ssize_t read(struct bt_conn *conn, const struct bt_gatt_attr *attr, 
                        void *buf, u16_t len, u16_t offset)
{
  return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, 
                            strlen(attr->user_data));
}

static ssize_t write(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                        const void *buf, u16_t len, uint16_t offset, uint8_t flags)
{
  memcpy(attr->user_data, buf, len);
  return len;
}

BT_GATT_SERVICE_DEFINE(wens_svc,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_WENS),
  BT_GATT_CHARACTERISTIC(BT_UUID_WENS_LOG,
                          BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE,
                          NULL,NULL,NULL),
  BT_GATT_CHARACTERISTIC(BT_UUID_WENS_TEMPKEYLIST,
                          BT_GATT_CHRC_READ, BT_GATT_PERM_READ,
                          read_str, NULL, "TESTVAlUE"),
  BT_GATT_CHARACTERISTIC(BT_UUID_WENS_SETTINGS,
                          BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ,
                          BT_GATT_PERM_WRITE,
                          read,write,&settings));