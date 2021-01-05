#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include "ens_settings.h"

typedef struct {
  uint8_t data_retention_policy;// time in days until data is purged
  uint8_t temporary_key_length;// 1-16 octests integer
} ens_settings;

ens_settings settings = {22, 55};

static ssize_t read_setting(
  struct bt_conn *conn, 
  const struct bt_gatt_attr *attr, 
  void *buf, 
  u16_t len, 
  u16_t offset)
{
  return bt_gatt_attr_read(
    conn, 
    attr, 
    buf, 
    len, 
    offset, 
    attr->user_data, 
    sizeof(*attr->user_data)
  );
}

static ssize_t write_setting(
  struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, 
  uint16_t len, 
  uint16_t offset,
	uint8_t flags)
{
  memcpy(attr->user_data, buf, sizeof(uint8_t));
  return len;
}

BT_GATT_SERVICE_DEFINE(
  ens_settings_svc,
  BT_GATT_PRIMARY_SERVICE(BT_UUID_ENS_SETTINGS),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_DTP,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting,
    &settings.data_retention_policy
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_DTP,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting,
    NULL,
    &settings.data_retention_policy
  ),
    BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_TKL,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting,
    &settings.temporary_key_length
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_TKL,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting,
    NULL,
    &settings.temporary_key_length
  )
);