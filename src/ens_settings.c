#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include "ens_settings.h"

//0 = not supported
typedef struct {
  uint8_t data_retention_policy;    // 0-14 time in days until data is purged
  uint8_t temporary_key_length;     // 0-16 octests integer
  uint16_t maximum_key_duration;    // 0-1440(24h) minutes 
  uint8_t ens_advertisement_length; // 0-29 number of octests in ens advertisement field of the ens advertisement record
  uint8_t maximum_advertisement_duration;     // 1-255 minutes
  uint8_t scan_on_time;             // 1-255 (multiple of 50 ms)  (suggestes 0x06)
  uint16_t scan_off_time;           // 0x0001 - 0xFFFF duration between scanns for ens advertisement (suggested 0x03C = 1 minute)
  uint16_t minimum_advertisement_interval;    // advertising interval when the server broadcasts ENS packets (suggestest 0x0140)
  uint16_t maximum_advertisement_interval;    // must be => minimum (suggestes 0x01B0)
  uint8_t self_pause_resume;        // 0x00 = Disabled; 0x01 = Enabled
} ens_settings;

//Initialize with suggested settings
ens_settings settings = {14, 16, 1440, 29, 10, 300, 60, 200, 270, 0};

static ssize_t read_setting8(
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
    sizeof(uint8_t)
  );
}

static ssize_t write_setting8(
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

static ssize_t read_setting16(
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
    sizeof(uint16_t)
  );
}

static ssize_t write_setting16(
  struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, 
  uint16_t len, 
  uint16_t offset,
	uint8_t flags)
{
  memcpy(attr->user_data, buf, sizeof(uint16_t));
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
    write_setting8,
    &settings.data_retention_policy
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_DTP,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.data_retention_policy
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_TKL,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting8,
    &settings.temporary_key_length
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_TKL,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.temporary_key_length
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MKD,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting16,
    &settings.maximum_key_duration
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MKD,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting16,
    NULL,
    &settings.maximum_key_duration
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_EAL,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting8,
    &settings.ens_advertisement_length
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_EAL,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.ens_advertisement_length
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MDA,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting8,
    &settings.maximum_advertisement_duration
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MDA,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.maximum_advertisement_duration
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SOnTime,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting8,
    &settings.scan_on_time
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SOnTime,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.scan_on_time
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SOffTime,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting16,
    &settings.scan_off_time
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SOffTime,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting16,
    NULL,
    &settings.scan_off_time
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MinAI,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting16,
    &settings.minimum_advertisement_interval
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MinAI,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting16,
    NULL,
    &settings.minimum_advertisement_interval
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MaxAI,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting16,
    &settings.maximum_advertisement_interval
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_MaxAI,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting16,
    NULL,
    &settings.maximum_advertisement_interval
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SPR,
    BT_GATT_CHRC_WRITE,
    BT_GATT_PERM_WRITE,
    NULL,
    write_setting8,
    &settings.self_pause_resume
  ),
  BT_GATT_CHARACTERISTIC(
    BT_UUID_ENS_SETTINGS_SPR,
    BT_GATT_CHRC_READ,
    BT_GATT_PERM_READ,
    read_setting8,
    NULL,
    &settings.self_pause_resume
  )
);