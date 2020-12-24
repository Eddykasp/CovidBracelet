#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include "date_time.h"

static ssize_t read_time(
  struct bt_conn *conn, 
  const struct bt_gatt_attr *attr, 
  void *buf, 
  u16_t len, 
  u16_t offset)
{
  date_time time = get_date_time();
  return bt_gatt_attr_read(
    conn, 
    attr, 
    buf, 
    len, 
    offset, 
    &time, 
    sizeof(time)
  );
}

static ssize_t write_time(
  struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, 
  uint16_t len, 
  uint16_t offset,
	uint8_t flags)
{
  set_date_time(*(date_time*)buf);
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
    NULL
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_CTS_CURRENT_TIME,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_time,
    NULL,
    NULL
  )
);