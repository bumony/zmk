/*
 * Copyright (c) 2023 Taisheng WANG <wstrn66@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef LPM009M360A_DISPLAY_DRIVER_H__
#define LPM009M360A_DISPLAY_DRIVER_H__

#include <zephyr/kernel.h>


#define LPM009M360A_CMD_NO_UPDATE           0x00
#define LPM009M360A_CMD_BLINKING_BLACK      0x10
#define LPM009M360A_CMD_BLINKING_INVERSION  0x14
#define LPM009M360A_CMD_BLINKING_WHITE      0x18
#define LPM009M360A_CMD_ALL_CLEAR           0x20
#define LPM009M360A_CMD_VCOM                0x40
#define LPM009M360A_CMD_UPDATE              0x80



#endif  /* LPM009M360A_DISPLAY_DRIVER_H__ */
