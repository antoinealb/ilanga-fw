/*
 * Copyright 2017 Antoine Albertelli <antoine@antoinealb.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "bsp/bsp.h"
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "bleprph.h"
#include "main.h"

/**
 * The vendor specific sensor test has the following characteristics:
 *
 * * Chip ID read: This characteristic reads the chip id of the veml 6075 chip.
 */

/* 11a8564d-e4a9-4bc8-8775-140c25d30783 */
static const ble_uuid128_t gatt_svr_svc_sensor_test_uuid =
    BLE_UUID128_INIT(0x83, 0x07, 0xd3, 0x25, 0x0c, 0x14, 0x75, 0x87,
                     0xc8, 0x4b, 0xa9, 0xe4, 0x4d, 0x56, 0xa8, 0x11);

/* 0b08ccd6-27a1-4439-98b4-36878efafd7a */
static const ble_uuid128_t gatt_svr_svc_sensor_test_chipid_uuid =
    BLE_UUID128_INIT(0x7a, 0xfd, 0xfa, 0x8e, 0x87, 0x36, 0xb4, 0x98,
                     0x39, 0x44, 0xa1, 0x27, 0xd6, 0xcc, 0x08, 0x0b);

/* cc7e9436-df5d-47c6-be2d-ee1490931b78 */
static const ble_uuid128_t gatt_svr_svc_sensor_test_uva_uuid =
    BLE_UUID128_INIT(0x78,
                     0x1b,
                     0x93,
                     0x90,
                     0x14,
                     0xee,
                     0x2d,
                     0xbe,
                     0xc6,
                     0x47,
                     0x5d,
                     0xdf,
                     0x36,
                     0x94,
                     0x7e,
                     0xcc);


/* bfdaa81e-8456-41f0-96f4-082e5cee2f00 */
static const ble_uuid128_t gatt_svr_svc_sensor_test_uvb_uuid =
    BLE_UUID128_INIT(0x00,
                     0x2f,
                     0xee,
                     0x5c,
                     0x2e,
                     0x08,
                     0xf4,
                     0x96,
                     0xf0,
                     0x41,
                     0x56,
                     0x84,
                     0x1e,
                     0xa8,
                     0xda,
                     0xbf);

static int gatt_svr_chr_access_sensor_test_chipid(uint16_t conn_handle,
                                                  uint16_t attr_handle,
                                                  struct ble_gatt_access_ctxt *ctxt,
                                                  void *arg)
{
    int rc;

    /* TODO: Read chip id instead. */
    uint16_t chipid = veml6075_read_chip_id(&uv_sensor);
    chipid = htobe16(chipid);

    rc = os_mbuf_append(ctxt->om, &chipid, sizeof chipid);
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
}

static int gatt_svr_chr_access_sensor_test_uva(uint16_t conn_handle,
                                               uint16_t attr_handle,
                                               struct ble_gatt_access_ctxt *ctxt,
                                               void *arg)
{
    int rc;

    uint16_t uv = veml6075_read_uva(&uv_sensor);
    uv = htobe16(uv);

    rc = os_mbuf_append(ctxt->om, &uv, sizeof uv);
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
}

static int gatt_svr_chr_access_sensor_test_uvb(uint16_t conn_handle,
                                               uint16_t attr_handle,
                                               struct ble_gatt_access_ctxt *ctxt,
                                               void *arg)
{
    int rc;

    uint16_t uv = veml6075_read_uvb(&uv_sensor);
    uv = htobe16(uv);

    rc = os_mbuf_append(ctxt->om, &uv, sizeof uv);
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
}



static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /*** Service: Sensor test. */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_svr_svc_sensor_test_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]) {
            {
                /*** Characteristic: Chip ID. */
                .uuid = &gatt_svr_svc_sensor_test_chipid_uuid.u,
                .access_cb = gatt_svr_chr_access_sensor_test_chipid,
                .flags = BLE_GATT_CHR_F_READ,
            }, {
                /*** Characteristic: UVA. */
                .uuid = &gatt_svr_svc_sensor_test_uva_uuid.u,
                .access_cb = gatt_svr_chr_access_sensor_test_uva,
                .flags = BLE_GATT_CHR_F_READ,
            }, {
                /*** Characteristic: UVB. */
                .uuid = &gatt_svr_svc_sensor_test_uvb_uuid.u,
                .access_cb = gatt_svr_chr_access_sensor_test_uvb,
                .flags = BLE_GATT_CHR_F_READ,
            }, {


                0, /* No more characteristics in this service. */
            }
        },
    },

    {
        0,         /* No more services. */
    },
};

int
gatt_svr_init(void)
{
    int rc;

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}
