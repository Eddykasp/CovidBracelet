#include "ens_settings.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>

ens_settings current_ens_settings = {
    .data_retention_policy          = 1,
    .temporary_key_length           = 2,
    .maximum_key_duration           = 3,
    .ens_advertisement_length       = 4,
    .maximum_advertisement_duration = 5,
    .scan_on_time                   = 6,
    .scan_off_time                  = 7,
    .minimum_advertisement_interval = 8,
    .maximum_advertisement_interval = 9,
    .self_pause_resume              = 10,
};

void ens_settings_pack(const ens_settings* setting_to_unpack, uint8_t* setting_bytes)
{
    setting_bytes[0] = setting_to_unpack->data_retention_policy;
    setting_bytes[1] = setting_to_unpack->temporary_key_length;

    setting_bytes[2] = (setting_to_unpack->maximum_key_duration & 0x00ff) >> 0;
    setting_bytes[3] = (setting_to_unpack->maximum_key_duration & 0xff00) >> 8;

    setting_bytes[4] = setting_to_unpack->ens_advertisement_length;
    setting_bytes[5] = setting_to_unpack->maximum_advertisement_duration;
    setting_bytes[6] = setting_to_unpack->scan_on_time;

    setting_bytes[7]  = (setting_to_unpack->scan_off_time & 0x00ff) >> 0;
    setting_bytes[8]  = (setting_to_unpack->scan_off_time & 0xff00) >> 8;
    setting_bytes[9]  = (setting_to_unpack->minimum_advertisement_interval & 0x00ff) >> 0;
    setting_bytes[10] = (setting_to_unpack->minimum_advertisement_interval & 0xff00) >> 8;

    setting_bytes[11] = (setting_to_unpack->maximum_advertisement_interval & 0x00ff) >> 0;
    setting_bytes[12] = (setting_to_unpack->maximum_advertisement_interval & 0xff00) >> 8;

    setting_bytes[13] = setting_to_unpack->self_pause_resume;
}

void ens_settings_unpack(const uint8_t* setting_bytes, ens_settings* settings)
{
    settings->data_retention_policy          = setting_bytes[0];
    settings->temporary_key_length           = setting_bytes[1];
    settings->maximum_key_duration           = (setting_bytes[3] << 8) | setting_bytes[2];
    settings->ens_advertisement_length       = setting_bytes[4];
    settings->maximum_advertisement_duration = setting_bytes[5];
    settings->scan_on_time                   = setting_bytes[6];
    settings->scan_off_time                  = (setting_bytes[8] << 8) | setting_bytes[7];
    settings->minimum_advertisement_interval = (setting_bytes[10] << 8) | setting_bytes[9];
    settings->maximum_advertisement_interval = (setting_bytes[12] << 8) | setting_bytes[11];
    settings->self_pause_resume              = setting_bytes[13];
}
