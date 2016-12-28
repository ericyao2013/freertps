/*
 * bma222.h
 *
 *  Created on: 28 déc. 2016
 *      Author: micky
 */

#ifndef COMMON_BMA222_H_
#define COMMON_BMA222_H_

#include "stdint.h"

#define BMA222_DEV_ADDR 0x18
#define BMA222_CHIP_ID_REG 0x00

#define BMA222_ACC_DATA_X_NEW (0x2)
#define BMA222_ACC_DATA_X     (0x3)
#define BMA222_ACC_DATA_Y_NEW (0x4)
#define BMA222_ACC_DATA_Y     (0x5)
#define BMA222_ACC_DATA_Z_NEW (0x6)
#define BMA222_ACC_DATA_Z     (0x7)

int16_t readXData();
int16_t readYData();
int16_t readZData();

#endif /* COMMON_BMA222_H_ */
