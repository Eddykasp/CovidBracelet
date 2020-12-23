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

//test function not sure how and where to actually trigger this
static ssize_t log_notify(struct bt_conn *conn, const struct bt_gatt_attr *attr, u16_t len)
{
  return bt_gatt_notify(conn, attr, attr->user_data, len);
}

// tempkeylist read teststring
// expand characteristic later to be read/write
static ssize_t read_str(struct bt_conn *conn, const struct bt_gatt_attr *attr, 
                        void *buf, u16_t len, u16_t offset)
{
  return bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, 
                            strlen(attr->user_data));
}

BT_GATT_SERVICE_DEFINE(wens_svc,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_WENS),
  BT_GATT_CHARACTERISTIC(BT_UUID_WENS_LOG,
                          BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE,
                          NULL,NULL,NULL),
  BT_GATT_CHARACTERISTIC(BT_UUID_WENS_TEMPKEYLIST,
                          BT_GATT_CHRC_READ, BT_GATT_PERM_READ,
                          read_str, NULL, "TESTVAlUE"));