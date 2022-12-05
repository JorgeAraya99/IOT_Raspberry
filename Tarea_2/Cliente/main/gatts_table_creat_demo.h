/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Attributes State Machine */
enum
{
    IDX_SVC,
    IDX_CHAR_Status,
    IDX_CHAR_VAL_Status,
    IDX_CHAR_CFG_Status,

    IDX_CHAR_IDprotocol,
    IDX_CHAR_VAL_IDprotocol,

    IDX_CHAR_BMISampling,
    IDX_CHAR_VAL_BMISampling,

    IDX_CHAR_AccSensibility,
    IDX_CHAR_VAL_AccSensibility,

    IDX_CHAR_GyroSensibility,
    IDX_CHAR_VAL_GyroSensibility,

    IDX_CHAR_BMESampling,
    IDX_CHAR_VAL_BMESampling,

    IDX_CHAR_DiscontinuosTime,
    IDX_CHAR_VAL_DiscontinuosTime,

    IDX_CHAR_PortTcp,
    IDX_CHAR_VAL_PortTcp,

    IDX_CHAR_PortUdp,
    IDX_CHAR_VAL_PortUdp,

    IDX_CHAR_HostIpAddr,
    IDX_CHAR_VAL_HostIpAddr,

    IDX_CHAR_SSIDX,
    IDX_CHAR_VAL_SSIDX,

    IDX_CHAR_Pass,
    IDX_CHAR_VAL_Pass,

    HRS_IDX_NB,
};
