#include "ens_records_api.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

static void notify_enabled_ens_records(const struct bt_gatt_attr* attr, uint16_t value);

void send_notification(uint8_t* records, uint8_t len);

uint16_t get_max_mtu();