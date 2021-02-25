#include "ens_records_api.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

static void notify_enabled_ens_records(const struct bt_gatt_attr* attr, uint16_t value);
static void racp_response_indicate(const struct bt_gatt_attr* attr, u16_t value);

void send_notification(uint8_t* records, uint8_t len);
void send_racp_response();

uint16_t get_max_mtu();