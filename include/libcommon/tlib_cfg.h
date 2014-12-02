#ifndef _TLIB_CFG_H_

#define _TLIB_CFG_H_



#define CFG_STRING    (int)1

#define CFG_INT        (int)2

#define CFG_LONG        (int)3

#define CFG_DOUBLE    (int)4



void TLib_Cfg_GetConfig(const char *sConfigFilePath, ...);



#endif

