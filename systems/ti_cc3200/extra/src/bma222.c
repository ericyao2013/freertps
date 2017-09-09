/*
 * bma222.c
 *
 *  Created on: 28 déc. 2016
 *      Author: micky
 */
#include "stdlib.h"
#include "string.h"
#include "common.h"
#include "i2c_if.h"
#include "bma222.h"

#define RETERR_IF_TRUE(condition) {if(condition) return FAILURE;}
#define RET_IF_ERR(Func)          {int iRetVal = (Func); \
                                   if (SUCCESS != iRetVal) \
                                     return  iRetVal;}

int
ProcessReadRegCommand(uint8_t ucDevAddr, uint8_t ucRegOffset, uint8_t *aucRdDataBuf, uint8_t ucRdLen)
{
//    unsigned char aucRdDataBuf[256];

    //
    // Write the register address to be read from.
    // Stop bit implicitly assumed to be 0.
    //
    RET_IF_ERR(I2C_IF_Write(ucDevAddr,&ucRegOffset,1,0));

    //
    // Read the specified length of data
    //
    RET_IF_ERR(I2C_IF_Read(ucDevAddr, &aucRdDataBuf[0], ucRdLen));

    return SUCCESS;
}

int16_t readXData()
{
  unsigned char aucRdDataBuf[256];
  int result = ProcessReadRegCommand(BMA222_DEV_ADDR, BMA222_ACC_DATA_X, aucRdDataBuf , 1);
  return aucRdDataBuf[0];
}

int16_t readYData()
{
  unsigned char aucRdDataBuf[256];
  int result = ProcessReadRegCommand(BMA222_DEV_ADDR, BMA222_ACC_DATA_Y, aucRdDataBuf , 1);
  return aucRdDataBuf[0];
}

int16_t readZData()
{
  unsigned char aucRdDataBuf[256];
  int result = ProcessReadRegCommand(BMA222_DEV_ADDR, BMA222_ACC_DATA_Z, aucRdDataBuf , 1);
  return aucRdDataBuf[0];
}
