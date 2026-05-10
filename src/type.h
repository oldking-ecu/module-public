/**
 * @file type.h
 * @author wwyyy (1046685883@qq.com)
 * @link https://gitee.com/oldking-ecu
 * @brief
 * @version 1.0
 * @date 2026-04-19
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#ifndef __TYPE_H__
#define __TYPE_H__

/**
 * @brief 没定义MCU_CUSTOM_TYPES时，使用本文类型和常量，否则使用MCU_CUSTOM_TYPES中定义的类型和常量
 *
 */
#ifdef USE_MCU_CUSTOM_TYPES
	#include "mcu_custom_types.h"
#else
// 定义类型
typedef unsigned char boolean;
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed long int32;
typedef unsigned long uint32;
typedef unsigned long long uint64;
typedef signed long long int64;

	#ifndef TRUE    // true/false
		#define TRUE 1
	#endif
	#ifndef FALS
		#define FALS 0
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif
	#ifndef HIH    // high/low
		#define HIH 1
	#endif
	#ifndef LOW
		#define LOW 0
	#endif
#endif    // USE_MCU_CUSTOM_TYPES


#ifndef FLIP    // flip/noflip
	#define FLIP 1
#endif
#ifndef NFLP
	#define NFLP 0
#endif

#ifndef NULL
	#define NULL ((void *)0)
#endif

#define CANFD_MSG_ATTTR_EXT (0x01U)
#define CANFD_MSG_ATTTR_RTR (0x02U)
#define CANFD_MSG_ATTTR_FD  (0x04U)
#define CANFD_MSG_ATTTR_BRS (0x08U)
typedef struct {
	uint32 Id;
	uint8 Attribute;    // bit0扩展帧;bit1远程帧;bit2Canfd;bit3Brs
	uint8 Size;
	uint8 Data[64];
} Canfd_MsgType;


#endif    //__TYPE_H__
