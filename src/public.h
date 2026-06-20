/**
 * @file public.h
 * @author wwyyy (1046685883@qq.com)
 * @link https://gitee.com/oldking-ecu
 * @brief
 * @version 1.0
 * @date 2026-04-19
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#ifndef PUBLIC_H__
#define PUBLIC_H__
#include "type.h"


/**
 * @brief 编译检查符号大小,超过指定大小时编译错误
 * 
 */
#define STATIC_ASSERT_SIZE(sym, val) typedef char __STATIC_ASSERT_NAME_##sym[ (sizeof(sym) <= (val)) ? 1 : -1 ]


#define NUM2VOIDP(num)  ((void *)num)
#define ARRAY_NUM(arr)  (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define MATH_MIN(A, B)        (((A) > (B)) ? (B) : (A))
#define MATH_MAX(A, B)        (((A) < (B)) ? (B) : (A))
#define MATH_SAT(X, MIN, MAX) (MATH_MAX(MATH_MIN((X), (MAX)), (MIN)))
#define MATH_ABS(A, B)        (((A) > (B)) ? ((A) - (B)) : ((B) - (A)))
#define MATH_ABS_VAR(A)       (((A) > 0) ? (A) : (-(A)))
#define MATH_DIV_ROUND(A, B)  (((A) >= 0) ? ((((A) + ((B) / 2)) / (B))) : (((A) - ((B) / 2)) / (B)))    // 四舍五入整除算法，除数必须大于0

// get memory address bit n
#define GMA_BYTE_BIT(Addr, Bit) ((((uint8 *)(Addr))[(Bit) / 8] >> ((Bit) % 8)) & 0x01)
// set memory address bit n
#define SMA_BYTE_BIT(Addr, Bit) (((uint8 *)(Addr))[(Bit) / 8] |= (0x01 << ((Bit) % 8)))
// clear memory address bit n
#define CMA_BYTE_BIT(Addr, Bit) (((uint8 *)(Addr))[(Bit) / 8] &= ((uint8)(~(0x01 << ((Bit) % 8)))))

// 数值限幅
#define MATH_NUM_LIMIT(num, min, max) \
	do {                              \
		if (num < (min)) {            \
			num = (min);              \
		} else if (num > (max)) {     \
			num = (max);              \
		}                             \
	} while (0)
// 数值+Step(不对齐Step)
#define MATH_NUM_ADD_LIMIT(num, step, maxlimt) \
	do {                                       \
		if (num + step <= maxlimt) {           \
			num = ((num) + (step));            \
		} else {                               \
			num = maxlimt;                     \
		}                                      \
	} while (0)
// 数值-Step(不对齐Step)
#define MATH_NUM_SUB_LIMIT(num, sub, minlimt) \
	do {                                      \
		if ((num) >= (sub) + (minlimt)) {     \
			(num) = ((num) - (sub));          \
		} else {                              \
			(num) = (minlimt);                \
		}                                     \
	} while (0)
// 数值+Step(对齐Step)
#define MATH_NUM_ADD_ALIGN_LIMIT(num, step, maxlimt)  \
	do {                                              \
		if (num + step <= maxlimt) {                  \
			num = ((num) + (step)) / (step) * (step); \
		} else {                                      \
			num = maxlimt;                            \
		}                                             \
	} while (0)
// 数值-Step(不对齐Step)
#define MATH_NUM_SUB_ALIGN_LIMIT(num, step, minlimt)    \
	do {                                                \
		if ((num) >= (step) + (minlimt)) {              \
			(num) = ((num) - (step)) / (step) * (step); \
		} else {                                        \
			(num) = (minlimt);                          \
		}                                               \
	} while (0)


/**
 * @brief 判断内存是否为特定值
 *
 * @param src: 内存地址
 * @param cmpValue: 比较值
 * @param size: 内存大小
 * @return uint8: TRUE: 内存全部为 cmpValue，FALSE: 内存不全为 cmpValue
 */
uint8 MemChk(const uint8 *src, uint8 cmpValue, uint16 size);

/**
 * @brief 将单个 ascii 字符 ('0'~'f') 转为半字节 (0x0~0xf)
 *
 * @param ascii: ascii 字符
 * @return uint8: 转换后的半字节值
 */
uint8 AsciiToHalfHex(uint8 ascii);

/**
 * @brief 将两个 ascii 字符表示的十六进制（如 "1A"）转为一个字节
 *
 * @param ascii: 指向两个 ascii 字符的指针
 * @return uint8: 转换后的字节值
 */
uint8 AsciiToHex(const uint8 *ascii);

/**
 * @brief 将 2n 字节 ascii 转成 n 字节 hex（"1A2b" -> 0x1A 0x2B）
 *
 * @param asciis: 输入 ascii 缓冲
 * @param asciiSize: ascii 字节数（必须为偶数）
 * @param hexs: 输出 hex 缓冲
 */
uint8 AsciisToHexs(const uint8 *asciis, uint16 asciiSize, uint8 *hexs);

/**
 * @brief 计算一段 ascii 格式十六进制数字的和（按字节求和并截断到 8bit）
 *
 * @param asciis: ascii 缓冲
 * @param asciiSize: 缓冲长度
 * @return uint8: 8bit 校验和
 */
uint8 CalcSumAsciis(const uint8 *asciis, uint16 asciiSize);

/**
 * @brief 将单个 hex 字节转换为两个 ascii 字符（如 0x1A -> "1A"）
 *
 * @param hex: 输入字节
 * @param ascii: 输出 2 字节 ascii 缓冲（至少 2 字节）
 */
void HexToAscii(uint8 hex, uint8 *ascii);

/**
 * @brief 将多个 hex 字节转换为 ascii 字符串（每字节占 2 ascii）
 *
 * @param hexs: 输入 hex 缓冲
 * @param hexSize: hex 字节数
 * @param asciis: 输出 ascii 缓冲（长度应为 hexSize*2）
 */
void HexsToAsciis(const uint8 *hexs, uint16 hexSize, uint8 *asciis);

/**
 * @brief 计算若干字节的和（溢出保留 8bit）
 *
 * @param src: 输入缓冲
 * @param size: 字节数
 * @return uint8: 8bit 校验和
 */
uint8 CalcSum(const uint8 *src, uint16 size);

/**
 * @brief 计算若干字节的异或值
 *
 * @param src: 输入缓冲
 * @param size: 字节数
 * @return uint8: 异或结果
 */
uint8 CalcXor(const uint8 *src, uint16 size);

/**
 * @brief 将内存中 2 字节按小端方式组合为 uint16
 *
 * @param src: 指向 2 字节数据
 * @return uint16: 组合后的值
 */
uint16 Combine2BytesLittle(const uint8 *src);
uint16 Combine2BytesBig(const uint8 *src);
uint32 Combine4BytesLittle(const uint8 *src);
uint32 Combine4BytesBig(const uint8 *src);
uint64 Combine8BytesLittle(const uint8 *src);
uint64 Combine8BytesBig(const uint8 *src);

/**
 * @brief 将 uint16 以小端方式切分为 2 字节
 *
 * @param num: 输入 uint16
 * @param dst: 输出 2 字节缓冲
 */
void Split2BytesLittle(uint16 num, uint8 *dst);
void Split2BytesBig(uint16 num, uint8 *dst);
void Split4BytesLittle(uint32 num, uint8 *dst);
void Split4BytesBig(uint32 num, uint8 *dst);
void Split8BytesLittle(uint64 num, uint8 *dst);
void Split8BytesBig(uint64 num, uint8 *dst);

/**
 * @brief CRC-32/MPEG-校验,多项式0x04C11DB7,初始值0xFFFFFFFF,结果异或0xFFFFFFFF，输入反转，输出反转
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint32: CRC32 值
 */
uint32 Crc32BlockCalc(const void *buff, uint32 size);
void Crc32StreamInit(void);
uint32 Crc32StreamCalc(const void *buff, uint32 size);
uint32 Crc32StreamResult(void);

/**
 * @brief 多项式x^16 + x^15 + x^2 + 1(0x8005),初始值0xFFFF,结果异或0x0000,输入数据反转，输出数据反转
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16ModbusBlockCalc(const void *buff, uint32 size);
void Crc16ModbusStreamInit(void);
uint16 Crc16ModbusStreamCalc(const void *buff, uint32 size);

/**
 * @brief 多项式x^16 + x^12 + x^5 + 1(0x1021),初始值0x0000,结果异或0x0000
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16XmodemBlockCalc(const void *buff, uint32 size);
void Crc16XmodemStreamInit(void);
uint16 Crc16XmodemStreamCalc(const void *buff, uint32 size);

/**
 * @brief 多项式x^16 + x^12 + x^5 + 1(0x1021),初始值0xFFFF,结果异或0x0000
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16CcitFalseBlockCalc(const void *buff, uint32 size);
void Crc16CcitFalseStreamInit(void);
uint16 Crc16CcitFalseStreamCalc(const void *buff, uint32 size);

/**
 * @brief 多项式x^8+x^4+x^3+x^2+1(SAE J1850),初始值0xFF,结果异或0x00
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint8: CRC8 值
 */
uint8 Crc8BlockCalc(const void *buff, uint32 size);
void Crc8StreamInit(void);
uint8 Crc8StreamCalc(const void *buff, uint32 size);

/**
 * @brief 判断是否已超时（Start 到 Now 的间隔是否 >= invt）
 *
 * @param start: 起始时间戳
 * @param now: 当前时间戳
 * @param invt: 期望间隔
 * @return uint8: TRUE: 超时到达，FALSE: 未到达
 */
uint8 CheckTimeout(uint32 start, uint32 now, uint32 invt);

/**
 * @brief 用于计算两个数字之间的间隔,一般用于定时器计算间隔时间
 *
 * @param start:
 * @param now:
 * @return uint32: 间隔
 */
uint32 CalcTimInvt(uint32 start, uint32 now);

/**
 * @brief CAN FD 字节长度转换为 DLC 码
 *
 * @param size: 字节数
 * @return uint8: DLC 码
 */
uint8 CanfdBytes2Dlc(uint8 size);

/**
 * @brief CAN FD DLC 码转换为数据字节长度
 *
 * @param dlc: DLC 码
 * @return uint8: 数据字节数
 */
uint8 CanfdDlc2Bytes(uint8 dlc);

/**
 * @brief 在 u8/i8 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx8(const uint8 *arr, uint32 size, uint8 refNum);

/**
 * @brief 在 u16/i16 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx16(const uint16 *arr, uint32 size, uint16 refNum);

/**
 * @brief 在 u32/i32 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx32(const uint32 *arr, uint32 size, uint32 refNum);

/**
 * @brief 对 u8/i8 数值二进制加 1，达到最大值后回到 1
 *
 * @param value: 指向值的指针
 */
void UpdateRollCount08(uint8 *value);
void UpdateRollCount16(uint16 *value);
void UpdateRollCount32(uint32 *value);

/**
 * @brief 在按升序排列的 uint32 表中查找 refValue 所在的范围索引
 *
 * @param array: 升序数组
 * @param arrSize: 数组长度
 * @param refValue: 待查找的值
 * @param idxMin: 输出范围最小索引
 * @param idxMax: 输出范围最大索引
 * @return uint8: 0 成功，其他失败
 */
uint8 LookupArryRangeInc32(const uint32 array[], uint16 arrSize, uint32 refValue, uint16 *idxMin, uint16 *idxMax);

/**
 * @brief 判断是否为 2 的幂
 *
 * @param n: 输入值
 * @return uint8: TRUE/ FALSE
 */
uint8 IsPowOf2(uint32 n);

/**
 * @brief 向下对齐到最近的 2 的幂
 *
 * @param n: 输入值
 * @return uint32: 对齐后的值
 */
uint32 RounddownPowOf2(uint32 n);

/**
 * @brief 向上对齐到最近的 2 的幂
 *
 * @param n: 输入值
 * @return uint32: 对齐后的值
 */
uint32 RoundupPowOf2(uint32 n);

/**
 * @brief 通过下拉测量上拉阻值计算
 *
 * @param resUp: 上拉电阻
 * @param resDown: 下拉电阻
 * @param volt: 测量电压
 * @param vRef: 参考电压
 * @return uint32: 计算得到的上拉阻值
 */
uint32 ResistorCalcUp(uint32 resUp, uint32 resDown, uint16 volt, uint16 vRef);

/**
 * @brief 通过上拉测量下拉阻值计算
 *
 * @param resUp: 上拉电阻
 * @param resDown: 下拉电阻
 * @param volt: 测量电压
 * @param vRef: 参考电压
 * @return uint32: 计算得到的下拉阻值
 */
uint32 ResistorCalcDown(uint32 resUp, uint32 resDown, uint16 volt, uint16 vRef);

/**
 * @brief 根据并联后阻值计算并联电阻中的另一个阻值
 *
 * @param parRes: 并联后的阻值
 * @param res1: 已知的并联电阻
 * @return uint32: 另一个并联电阻值
 */
uint32 ResistorCalcPara(uint32 parRes, uint32 res1);

/**
 * @brief 线性映射
 *
 * @param input: 输入值
 * @param imin: 输入最小值
 * @param imax: 输入最大值
 * @param omin: 输出最小值
 * @param omax: 输出最大值
 * @return int32: 映射后的输出值
 */
int32 LinearMap_I32(int32 input, int32 imin, int32 imax, int32 omin, int32 omax);

typedef enum {
	FILTER_AVG_ELEM_U8 = 0x01,
	FILTER_AVG_ELEM_S8 = 0x81,
	FILTER_AVG_ELEM_U16 = 0x02,
	FILTER_AVG_ELEM_S16 = 0x82,
	FILTER_AVG_ELEM_U32 = 0x04,
	FILTER_AVG_ELEM_S32 = 0x84,
} FILTERAVG_ELEMENT_TYPE;
typedef struct {
	void *Buffer;
	uint16 Depth;
	uint16 ArrNum;
	FILTERAVG_ELEMENT_TYPE ElementType;
	uint16 Head;
	uint16 Count;
	uint8 InitOK;
} FilterAvgType;
/**
 * @brief 支持对数组元素类型进行去极值平均滤波
 *
 * @param filter: 滤波器指针
 * @param buffer: 缓冲区指针，空间为 depth * eNum * sizeof(eType)
 * @param depth: 缓冲区深度
 * @param eNum: 元素数量(数组长度,对于单个成员，长度为1)
 * @param eType: 元素类型
 * @return uint8:
 */
uint8 FilterAvg_Init(FilterAvgType *filter, void *buffer, uint16 depth, uint16 eNum, FILTERAVG_ELEMENT_TYPE eType);
/**
 * @brief 更新均值滤波器
 *
 * @param filter: 滤波器指针
 * @param element: 输入元素指针
 * @param oelement: 输出元素指针
 */
void FilterAvg_Updata(FilterAvgType *filter, const void *element, void *oelement);

#endif    // PUBLIC_H__
