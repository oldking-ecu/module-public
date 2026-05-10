/**
 * @file public.c
 * @author wwyyy (1046685883@qq.com)
 * @link https://gitee.com/oldking-ecu
 * @brief
 * @version 1.0
 * @date 2026-04-20
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 * @fileid method
 * @filehash 2.1C451AE417D0C7D94223006B8121CA3E6DF28CA6F672A5F094D25B365637630F
 * @timestamp 2026-04-20T21:33:07
 */
#include "public.h"
#include "string.h"


/**
 * @brief 判断内存是否为特定值
 *
 * @param src: 内存地址
 * @param cmpValue: 比较值
 * @param size: 内存大小
 * @return uint8: TRUE: 内存全部为 cmpValue，FALSE: 内存不全为 cmpValue
 */
uint8 MemChk(const uint8 *src, uint8 cmpValue, uint16 size)
{
	uint16 i;
	for (i = 0; i < size; i++) {
		if (src[i] != cmpValue)
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief 将单个 ascii 字符 ('0'~'f') 转为半字节 (0x0~0xf)
 *
 * @param ascii: ascii 字符
 * @return uint8: 转换后的半字节值
 */
uint8 AsciiToHalfHex(uint8 ascii)
{
	if (ascii >= '0' && ascii <= '9')
		return ascii - '0';
	else if (ascii >= 'A' && ascii <= 'F')
		return ascii - 'A' + 10;
	else if (ascii >= 'a' && ascii <= 'f')
		return ascii - 'a' + 10;
	else
		return 0;
}

/**
 * @brief 将两个 ascii 字符表示的十六进制（如 "1A"）转为一个字节
 *
 * @param ascii: 指向两个 ascii 字符的指针
 * @return uint8: 转换后的字节值
 */
uint8 AsciiToHex(const uint8 *ascii)
{
	uint8 hex;
	hex = AsciiToHalfHex(ascii[0]);
	hex <<= 4;
	hex |= AsciiToHalfHex(ascii[1]);
	return hex;
}

/**
 * @brief 将 2n 字节 ascii 转成 n 字节 hex（"1A2b" -> 0x1A 0x2B）
 *
 * @param asciis: 输入 ascii 缓冲
 * @param asciiSize: ascii 字节数（必须为偶数）
 * @param hexs: 输出 hex 缓冲
 */
void AsciisToHexs(const uint8 *asciis, uint16 asciiSize, uint8 *hexs)
{
	uint16 i, j;
	for (i = 0, j = 0; i < asciiSize; i++, j += 2) {
		hexs[i] = AsciiToHex(asciis + j);
	}
}

/**
 * @brief 计算一段 ascii 格式十六进制数字的和（按字节求和并截断到 8bit）
 *
 * @param asciis: ascii 缓冲
 * @param asciiSize: 缓冲长度
 * @return uint8: 8bit 校验和
 */
uint8 CalcSumAsciis(const uint8 *asciis, uint16 asciiSize)
{
	uint8 sum = 0;
	while (asciiSize--) {
		sum += AsciiToHex(asciis);
		asciis += 2;
	}
	return sum;
}

/**
 * @brief 将单个 hex 字节转换为两个 ascii 字符（如 0x1A -> "1A"）
 *
 * @param hex: 输入字节
 * @param ascii: 输出 2 字节 ascii 缓冲（至少 2 字节）
 */
void HexToAscii(uint8 hex, uint8 *ascii)
{
	ascii[0] = (hex >> 4 & 0x0F) > 9 ? (hex >> 4 & 0x0F) - 10 + 'A' : (hex >> 4 & 0x0F) + '0';
	ascii[1] = (hex & 0x0F) > 9 ? (hex & 0x0F) - 10 + 'A' : (hex & 0x0F) + '0';
}

/**
 * @brief 将多个 hex 字节转换为 ascii 字符（每字节占 2 ascii）
 *
 * @param hexs: 输入 hex 缓冲
 * @param hexSize: hex 字节数
 * @param asciis: 输出 ascii 缓冲（长度应为 hexSize*2）
 */
void HexsToAsciis(const uint8 *hexs, uint16 hexSize, uint8 *asciis)
{
	uint16 i, j;
	for (i = 0, j = 0; i < hexSize; i++, j += 2) {
		HexToAscii(hexs[i], asciis + j);
	}
}

/**
 * @brief 计算若干字节的和（溢出保留 8bit）
 *
 * @param src: 输入缓冲
 * @param size: 字节数
 * @return uint8: 8bit 校验和
 */
uint8 CalcSum(const uint8 *src, uint16 size)
{
	uint8 sum = 0;
	while (size--) {
		sum += *src++;
	}
	return sum;
}

/**
 * @brief 计算若干字节的异或值
 *
 * @param src: 输入缓冲
 * @param size: 字节数
 * @return uint8: 异或结果
 */
uint8 CalcXor(const uint8 *src, uint16 size)
{
	uint8 xor = 0;
	while (size--) {
		xor ^= *src++;
	}
	return xor;
}

/**
 * @brief 将内存中 2 字节按小端方式组合为 uint16
 *
 * @param src: 指向 2 字节数据
 * @return uint16: 组合后的值
 */
uint16 Combine2BytesLittle(const uint8 *src)
{
	uint16 temp;
	temp = src[1];
	temp <<= 8;
	temp |= src[0];
	return temp;
}
uint16 Combine2BytesBig(const uint8 *src)
{
	uint16 temp;
	temp = src[0];
	temp <<= 8;
	temp |= src[1];
	return temp;
}
uint32 Combine4BytesLittle(const uint8 *src)
{
	uint32 temp;
	temp = src[3];
	temp <<= 8;
	temp |= src[2];
	temp <<= 8;
	temp |= src[1];
	temp <<= 8;
	temp |= src[0];
	return temp;
}
uint32 Combine4BytesBig(const uint8 *src)
{
	uint32 temp;
	temp = src[0];
	temp <<= 8;
	temp |= src[1];
	temp <<= 8;
	temp |= src[2];
	temp <<= 8;
	temp |= src[3];
	return temp;
}
uint64 Combine8BytesLittle(const uint8 *src)
{
	uint64 temp;
	temp = src[7];
	temp <<= 8;
	temp |= src[6];
	temp <<= 8;
	temp |= src[5];
	temp <<= 8;
	temp |= src[4];
	temp <<= 8;
	temp |= src[3];
	temp <<= 8;
	temp |= src[2];
	temp <<= 8;
	temp |= src[1];
	temp <<= 8;
	temp |= src[0];
	return temp;
}
uint64 Combine8BytesBig(const uint8 *src)
{
	uint64 temp;
	temp = src[0];
	temp <<= 8;
	temp |= src[1];
	temp <<= 8;
	temp |= src[2];
	temp <<= 8;
	temp |= src[3];
	temp <<= 8;
	temp |= src[4];
	temp <<= 8;
	temp |= src[5];
	temp <<= 8;
	temp |= src[6];
	temp <<= 8;
	temp |= src[7];
	return temp;
}
void Split2BytesLittle(uint16 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[0] = (uint8)num;
		num >>= 8;
		dst[1] = (uint8)num;
	}
}
void Split2BytesBig(uint16 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[1] = (uint8)num;
		num >>= 8;
		dst[0] = (uint8)num;
	}
}
void Split4BytesLittle(uint32 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[0] = (uint8)num;
		num >>= 8;
		dst[1] = (uint8)num;
		num >>= 8;
		dst[2] = (uint8)num;
		num >>= 8;
		dst[3] = (uint8)num;
	}
}
void Split4BytesBig(uint32 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[3] = (uint8)num;
		num >>= 8;
		dst[2] = (uint8)num;
		num >>= 8;
		dst[1] = (uint8)num;
		num >>= 8;
		dst[0] = (uint8)num;
	}
}
void Split8BytesLittle(uint64 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[0] = (uint8)num;
		num >>= 8;
		dst[1] = (uint8)num;
		num >>= 8;
		dst[2] = (uint8)num;
		num >>= 8;
		dst[3] = (uint8)num;
		num >>= 8;
		dst[4] = (uint8)num;
		num >>= 8;
		dst[5] = (uint8)num;
		num >>= 8;
		dst[6] = (uint8)num;
		num >>= 8;
		dst[7] = (uint8)num;
	}
}
void Split8BytesBig(uint64 num, uint8 *dst)
{
	if (NULL != dst) {
		dst[7] = (uint8)num;
		num >>= 8;
		dst[6] = (uint8)num;
		num >>= 8;
		dst[5] = (uint8)num;
		num >>= 8;
		dst[4] = (uint8)num;
		num >>= 8;
		dst[3] = (uint8)num;
		num >>= 8;
		dst[2] = (uint8)num;
		num >>= 8;
		dst[1] = (uint8)num;
		num >>= 8;
		dst[0] = (uint8)num;
	}
}

static const uint32 Crc32Tab[256] = {
	0x00000000u, 0x77073096u, 0xEE0E612Cu, 0x990951BAu, 0x076DC419u, 0x706AF48Fu, 0xE963A535u, 0x9E6495A3u,
	0x0EDB8832u, 0x79DCB8A4u, 0xE0D5E91Eu, 0x97D2D988u, 0x09B64C2Bu, 0x7EB17CBDu, 0xE7B82D07u, 0x90BF1D91u,
	0x1DB71064u, 0x6AB020F2u, 0xF3B97148u, 0x84BE41DEu, 0x1ADAD47Du, 0x6DDDE4EBu, 0xF4D4B551u, 0x83D385C7u,
	0x136C9856u, 0x646BA8C0u, 0xFD62F97Au, 0x8A65C9ECu, 0x14015C4Fu, 0x63066CD9u, 0xFA0F3D63u, 0x8D080DF5u,
	0x3B6E20C8u, 0x4C69105Eu, 0xD56041E4u, 0xA2677172u, 0x3C03E4D1u, 0x4B04D447u, 0xD20D85FDu, 0xA50AB56Bu,
	0x35B5A8FAu, 0x42B2986Cu, 0xDBBBC9D6u, 0xACBCF940u, 0x32D86CE3u, 0x45DF5C75u, 0xDCD60DCFu, 0xABD13D59u,
	0x26D930ACu, 0x51DE003Au, 0xC8D75180u, 0xBFD06116u, 0x21B4F4B5u, 0x56B3C423u, 0xCFBA9599u, 0xB8BDA50Fu,
	0x2802B89Eu, 0x5F058808u, 0xC60CD9B2u, 0xB10BE924u, 0x2F6F7C87u, 0x58684C11u, 0xC1611DABu, 0xB6662D3Du,
	0x76DC4190u, 0x01DB7106u, 0x98D220BCu, 0xEFD5102Au, 0x71B18589u, 0x06B6B51Fu, 0x9FBFE4A5u, 0xE8B8D433u,
	0x7807C9A2u, 0x0F00F934u, 0x9609A88Eu, 0xE10E9818u, 0x7F6A0DBBu, 0x086D3D2Du, 0x91646C97u, 0xE6635C01u,
	0x6B6B51F4u, 0x1C6C6162u, 0x856530D8u, 0xF262004Eu, 0x6C0695EDu, 0x1B01A57Bu, 0x8208F4C1u, 0xF50FC457u,
	0x65B0D9C6u, 0x12B7E950u, 0x8BBEB8EAu, 0xFCB9887Cu, 0x62DD1DDFu, 0x15DA2D49u, 0x8CD37CF3u, 0xFBD44C65u,
	0x4DB26158u, 0x3AB551CEu, 0xA3BC0074u, 0xD4BB30E2u, 0x4ADFA541u, 0x3DD895D7u, 0xA4D1C46Du, 0xD3D6F4FBu,
	0x4369E96Au, 0x346ED9FCu, 0xAD678846u, 0xDA60B8D0u, 0x44042D73u, 0x33031DE5u, 0xAA0A4C5Fu, 0xDD0D7CC9u,
	0x5005713Cu, 0x270241AAu, 0xBE0B1010u, 0xC90C2086u, 0x5768B525u, 0x206F85B3u, 0xB966D409u, 0xCE61E49Fu,
	0x5EDEF90Eu, 0x29D9C998u, 0xB0D09822u, 0xC7D7A8B4u, 0x59B33D17u, 0x2EB40D81u, 0xB7BD5C3Bu, 0xC0BA6CADu,
	0xEDB88320u, 0x9ABFB3B6u, 0x03B6E20Cu, 0x74B1D29Au, 0xEAD54739u, 0x9DD277AFu, 0x04DB2615u, 0x73DC1683u,
	0xE3630B12u, 0x94643B84u, 0x0D6D6A3Eu, 0x7A6A5AA8u, 0xE40ECF0Bu, 0x9309FF9Du, 0x0A00AE27u, 0x7D079EB1u,
	0xF00F9344u, 0x8708A3D2u, 0x1E01F268u, 0x6906C2FEu, 0xF762575Du, 0x806567CBu, 0x196C3671u, 0x6E6B06E7u,
	0xFED41B76u, 0x89D32BE0u, 0x10DA7A5Au, 0x67DD4ACCu, 0xF9B9DF6Fu, 0x8EBEEFF9u, 0x17B7BE43u, 0x60B08ED5u,
	0xD6D6A3E8u, 0xA1D1937Eu, 0x38D8C2C4u, 0x4FDFF252u, 0xD1BB67F1u, 0xA6BC5767u, 0x3FB506DDu, 0x48B2364Bu,
	0xD80D2BDAu, 0xAF0A1B4Cu, 0x36034AF6u, 0x41047A60u, 0xDF60EFC3u, 0xA867DF55u, 0x316E8EEFu, 0x4669BE79u,
	0xCB61B38Cu, 0xBC66831Au, 0x256FD2A0u, 0x5268E236u, 0xCC0C7795u, 0xBB0B4703u, 0x220216B9u, 0x5505262Fu,
	0xC5BA3BBEu, 0xB2BD0B28u, 0x2BB45A92u, 0x5CB36A04u, 0xC2D7FFA7u, 0xB5D0CF31u, 0x2CD99E8Bu, 0x5BDEAE1Du,
	0x9B64C2B0u, 0xEC63F226u, 0x756AA39Cu, 0x026D930Au, 0x9C0906A9u, 0xEB0E363Fu, 0x72076785u, 0x05005713u,
	0x95BF4A82u, 0xE2B87A14u, 0x7BB12BAEu, 0x0CB61B38u, 0x92D28E9Bu, 0xE5D5BE0Du, 0x7CDCEFB7u, 0x0BDBDF21u,
	0x86D3D2D4u, 0xF1D4E242u, 0x68DDB3F8u, 0x1FDA836Eu, 0x81BE16CDu, 0xF6B9265Bu, 0x6FB077E1u, 0x18B74777u,
	0x88085AE6u, 0xFF0F6A70u, 0x66063BCAu, 0x11010B5Cu, 0x8F659EFFu, 0xF862AE69u, 0x616BFFD3u, 0x166CCF45u,
	0xA00AE278u, 0xD70DD2EEu, 0x4E048354u, 0x3903B3C2u, 0xA7672661u, 0xD06016F7u, 0x4969474Du, 0x3E6E77DBu,
	0xAED16A4Au, 0xD9D65ADCu, 0x40DF0B66u, 0x37D83BF0u, 0xA9BCAE53u, 0xDEBB9EC5u, 0x47B2CF7Fu, 0x30B5FFE9u,
	0xBDBDF21Cu, 0xCABAC28Au, 0x53B39330u, 0x24B4A3A6u, 0xBAD03605u, 0xCDD70693u, 0x54DE5729u, 0x23D967BFu,
	0xB3667A2Eu, 0xC4614AB8u, 0x5D681B02u, 0x2A6F2B94u, 0xB40BBE37u, 0xC30C8EA1u, 0x5A05DF1Bu, 0x2D02EF8Du};
static uint32 Crc32Seed = 0xFFFFFFFF;
/**
 * @brief CRC-32/MPEG-校验,多项式0x04C11DB7,初始值0xFFFFFFFF,结果异或0xFFFFFFFF，输入反转，输出反转
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint32: CRC32 值
 */
uint32 Crc32BlockCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	uint32 seed = 0xFFFFFFFF;
	while (size--) {
		seed = (seed >> 8u) ^ Crc32Tab[(seed ^ *buf++) & 0xFF];
	}
	return seed ^ 0xFFFFFFFF;
}
void Crc32StreamInit(void)
{
	Crc32Seed = 0xFFFFFFFF;
}
uint32 Crc32StreamCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	while (size--) {
		Crc32Seed = (Crc32Seed >> 8) ^ Crc32Tab[(Crc32Seed ^ *buf++) & 0xFF];
	}
	return Crc32Seed ^ 0xFFFFFFFF;
}
uint32 Crc32StreamResult(void)
{
	return Crc32Seed ^ 0xFFFFFFFF;
}

static const uint16 Crc16Tab8005[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};
static uint16 Crc16ModbusSeed = 0xFFFF;
/**
 * @brief 多项式x^16 + x^15 + x^2 + 1(0x8005),初始值0xFFFF,结果异或0x0000,输入数据反转，输出数据反转
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16ModbusBlockCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	uint16 seed = 0xFFFF;
	while (size--) {
		seed = (seed >> 8) ^ Crc16Tab8005[(seed ^ *buf++) & 0xFF];
	}
	return seed;
}
void Crc16ModbusStreamInit(void)
{
	Crc16ModbusSeed = 0xFFFF;
}
uint16 Crc16ModbusStreamCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	while (size--) {
		Crc16ModbusSeed = (Crc16ModbusSeed >> 8) ^ Crc16Tab8005[(Crc16ModbusSeed ^ *buf++) & 0xFF];
	}
	return Crc16ModbusSeed;
}

static const uint16 Crc16Tab1021[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
static uint16 Crc16XmodemSeed = 0;
/**
 * @brief 多项式x^16 + x^12 + x^5 + 1(0x1021),初始值0x0000,结果异或0x0000
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16XmodemBlockCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	uint16 seed = 0;
	while (size--) {
		seed = (seed >> 8) ^ Crc16Tab1021[(seed ^ *buf++) & 0xFF];
	}
	return seed;
}
void Crc16XmodemStreamInit(void)
{
	Crc16XmodemSeed = 0;
}
uint16 Crc16XmodemStreamCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	while (size--) {
		Crc16XmodemSeed = (Crc16XmodemSeed >> 8) ^ Crc16Tab1021[(Crc16XmodemSeed ^ *buf++) & 0xFF];
	}
	return Crc16XmodemSeed;
}

/**
 * @brief 多项式x^16 + x^12 + x^5 + 1(0x1021),初始值0xFFFF,结果异或0x0000
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint16: CRC16 值
 */
uint16 Crc16CcitFalseBlockCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	uint16 Seed = 0xFFFF;
	while (size--) {
		Seed = (Seed << 8) ^ Crc16Tab1021[((Seed >> 8) ^ *buf++) & 0xFF];
	}
	return Seed;
}
static uint16 Crc16CcitFalseSeed = 0;
void Crc16CcitFalseStreamInit(void)
{
	Crc16CcitFalseSeed = 0xFFFF;
}
uint16 Crc16CcitFalseStreamCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	while (size--) {
		Crc16CcitFalseSeed = (Crc16CcitFalseSeed << 8) ^ Crc16Tab1021[((Crc16CcitFalseSeed >> 8) ^ *buf++) & 0xFF];
	}
	return Crc16CcitFalseSeed;
}

static const uint8 Crc8Tab[] = {
	0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53,
	0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
	0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E,
	0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
	0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4,
	0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
	0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19,
	0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
	0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40,
	0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
	0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D,
	0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
	0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7,
	0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
	0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A,
	0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
	0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75,
	0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
	0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8,
	0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
	0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2,
	0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
	0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F,
	0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
	0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66,
	0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
	0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB,
	0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
	0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1,
	0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
	0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C,
	0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4};
static uint8 Crc8Seed = 0xFF;
/**
 * @brief 多项式x^8+x^4+x^3+x^2+1(SAE J1850),初始值0xFF,结果异或0x00
 *
 * @param buff: 输入数据指针
 * @param size: 数据长度字节
 * @return uint8: CRC8 值
 */
uint8 Crc8BlockCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	uint8 seed = 0xFF;
	while (size--) {
		seed = Crc8Tab[seed ^ *buf++];
	}
	return seed;
}
void Crc8StreamInit(void)
{
	Crc8Seed = 0xFF;
}
uint8 Crc8StreamCalc(const void *buff, uint32 size)
{
	const uint8 *buf = buff;
	while (size--) {
		Crc8Seed = Crc8Tab[Crc8Seed ^ *buf++];
	}
	return Crc8Seed;
}

/**
 * @brief 判断是否已超时（Start 到 Now 的间隔是否 >= invt）
 *
 * @param start: 起始时间戳
 * @param now: 当前时间戳
 * @param invt: 期望间隔
 * @return uint8: TRUE: 超时到达，FALSE: 未到达
 */
uint8 CheckTimeout(uint32 start, uint32 now, uint32 invt)
{
	if ((uint32)(start + invt) >= start) {    // 未溢出
		if ((now >= (uint32)(start + invt)) || (now < start))
			return TRUE;
	} else {    // 溢出
		if ((now < start) && (now >= (uint32)(start + invt)))
			return TRUE;
	}
	return FALSE;
}

/**
 * @brief 用于计算两个数字之间的间隔,一般用于定时器计算间隔时间
 *
 * @param start: 起始时间戳
 * @param now: 当前时间戳
 * @return uint32: 间隔
 */
uint32 CalcTimInvt(uint32 start, uint32 now)
{
	if (now >= start) {
		return (now - start);
	} else {
		return (uint32)(0xFFFFFFFF) - start + now + 1;
	}
}

/**
 * @brief CAN FD 字节长度转换为 DLC 码
 *
 * @param size: 字节数
 * @return uint8: DLC 码
 */
uint8 CanfdBytes2Dlc(uint8 size)
{
	static const uint8 BytestoDLC[65] = {
		0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U,
		/* 9 to 12 payload have DLC Code 12 Bytes */
		9, 9, 9, 9,
		/* 13 to 16 payload have DLC Code 16 Bytes */
		0xA, 0xA, 0xA, 0xA,
		/* 17 to 20 payload have DLC Code 20 Bytes */
		0xB, 0xB, 0xB, 0xB,
		/* 21 to 24 payload have DLC Code 24 Bytes */
		0xC, 0xC, 0xC, 0xC,
		/* 25 to 32 payload have DLC Code 32 Bytes */
		0xD, 0xD, 0xD, 0xD,
		0xD, 0xD, 0xD, 0xD,
		/* 33 to 48 payload have DLC Code 48 Bytes */
		0xE, 0xE, 0xE, 0xE,
		0xE, 0xE, 0xE, 0xE,
		0xE, 0xE, 0xE, 0xE,
		0xE, 0xE, 0xE, 0xE,
		/* 49 to 64 payload have DLC Code 64 Bytes */
		0xF, 0xF, 0xF, 0xF,
		0xF, 0xF, 0xF, 0xF,
		0xF, 0xF, 0xF, 0xF,
		0xF, 0xF, 0xF, 0xF};
	if (size >= ARRAY_SIZE(BytestoDLC))
		return 0;
	else
		return BytestoDLC[size];
}

/**
 * @brief CAN FD DLC 码转换为数据字节长度
 *
 * @param dlc: DLC 码
 * @return uint8: 数据字节数
 */
uint8 CanfdDlc2Bytes(uint8 dlc)
{
	static const uint8 DLCtoBytes[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
	return DLCtoBytes[dlc & 0xF];
}

/**
 * @brief 在 u8/i8 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx8(const uint8 *arr, uint32 size, uint8 refNum)
{
	uint32 i;
	for (i = 0; i < size; i++) {
		if (arr[i] == refNum) {
			return i;
		}
	}
	return i;
}

/**
 * @brief 在 u16/i16 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx16(const uint16 *arr, uint32 size, uint16 refNum)
{
	uint32 i;
	for (i = 0; i < size; i++) {
		if (arr[i] == refNum) {
			return i;
		}
	}
	return i;
}

/**
 * @brief 在 u32/i32 数组中查找第一次出现 refNum 的索引
 *
 * @param arr: 数组指针
 * @param size: 数组长度
 * @param refNum: 要查找的值
 * @return uint32: 索引（未找到返回 size）
 */
uint32 LookupArryIdx32(const uint32 *arr, uint32 size, uint32 refNum)
{
	uint32 i;
	for (i = 0; i < size; i++) {
		if (arr[i] == refNum) {
			return i;
		}
	}
	return i;
}

/**
 * @brief 对 u8/i8 数值二进制加 1，达到最大值后回到 1
 *
 * @param value: 指向值的指针
 */
void UpdateRollCount08(uint8 *value)
{
	if (0xFF == value[0]) {
		value[0] = 1;
	} else {
		value[0]++;
	}
}

/**
 * @brief 对 u16/i16 数值二进制加 1，达到最大值后回到 1
 *
 * @param value: 指向值的指针
 */
void UpdateRollCount16(uint16 *value)
{
	if (0xFFFF == value[0]) {
		value[0] = 1;
	} else {
		value[0]++;
	}
}

/**
 * @brief 对 u32/i32 数值二进制加 1，达到最大值后回到 1
 *
 * @param value: 指向值的指针
 */
void UpdateRollCount32(uint32 *value)
{
	if (0xFFFFFFFF == value[0]) {
		value[0] = 1;
	} else {
		value[0]++;
	}
}

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
uint8 LookupArryRangeInc32(const uint32 array[], uint16 arrSize, uint32 refValue, uint16 *idxMin, uint16 *idxMax)
{
	uint16 mid, start, end;
	if (array == NULL || arrSize == 0 || idxMin == NULL || idxMax == NULL) {
		return 1;
	}
	start = 0;
	end = arrSize - 1;
	if (refValue < array[start] || refValue > array[end]) {
		return 1;
	}
	if (refValue == array[start]) {
		*idxMin = start;
		*idxMax = start;
	} else if (refValue == array[end]) {
		*idxMin = end;
		*idxMax = end;
	} else {
		while (end - start > 1) {
			mid = ((uint32)start + end) / 2;
			if (refValue > array[mid]) {
				start = mid;
			} else if (refValue < array[mid]) {
				end = mid;
			} else {
				*idxMin = mid;
				*idxMax = mid;
				return 1;
			}
		}
		*idxMin = start;
		*idxMax = end;
	}
	return 0;
}

/**
 * @brief 判断是否为 2 的幂
 *
 * @param n: 输入值
 * @return uint8: TRUE/ FALSE
 */
uint8 IsPowOf2(uint32 n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

/**
 * @brief 向下对齐到最近的 2 的幂
 *
 * @param n: 输入值
 * @return uint32: 对齐后的值
 */
uint32 RounddownPowOf2(uint32 n)
{
	if (n < 2)
		return n;
	if (n & n - 1)    // 已经对齐
		return n;
#if 0
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return (n + 1) >> 1;
#else
	uint32 count = 0;
	while (n) {
		count++;
		n >>= 1;
	}
	// 返回2^(count-1)
	return 1U << (count - 1);
#endif
}

/**
 * @brief 向上对齐到最近的 2 的幂
 *
 * @param n: 输入值
 * @return uint32: 对齐后的值
 */
uint32 RoundupPowOf2(uint32 n)
{
	if (n == 0)
		return 1;
	if (n & n - 1)    // 已经对齐
		return n;
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}

/**
 * @brief 通过下拉测量上拉阻值计算
 *
 * @param resUp: 上拉电阻
 * @param resDown: 下拉电阻
 * @param volt: 测量电压
 * @param vRef: 参考电压
 * @return uint32: 计算得到的上拉阻值
 */
uint32 ResistorCalcUp(uint32 resUp, uint32 resDown, uint16 volt, uint16 vRef)
{
	uint32 rVal;
	if (volt >= vRef || !volt) {
		return 0;
	}
	rVal = (uint32)(vRef - volt) * resDown;
	rVal = (rVal + (volt >> 1)) / volt;
	if (rVal > resUp) {
		rVal -= resUp;
	} else {
		rVal = 0;
	}
	return rVal;
}

/**
 * @brief 通过上拉测量下拉阻值计算
 *
 * @param resUp: 上拉电阻
 * @param resDown: 下拉电阻
 * @param volt: 测量电压
 * @param vRef: 参考电压
 * @return uint32: 计算得到的下拉阻值
 */
uint32 ResistorCalcDown(uint32 resUp, uint32 resDown, uint16 volt, uint16 vRef)
{
	uint32 rVal, div1, div2;
	if (volt >= vRef) {
		volt = vRef - 1;
	}
	div1 = resUp * ((uint32)volt);
	div2 = vRef - volt;
	rVal = (div1 + (div2 >> 1)) / div2;
	if (rVal >= resDown) {
		rVal = rVal - resDown;
	} else {
		rVal = 0;
	}
	return rVal;
}

/**
 * @brief 根据并联后阻值计算并联电阻中的另一个阻值
 *
 * @param parRes: 并联后的阻值
 * @param res1: 已知的并联电阻
 * @return uint32: 另一个并联电阻值
 */
uint32 ResistorCalcPara(uint32 parRes, uint32 res1)
{
	uint64 res2;

	if (res1 <= parRes) {
		return (uint32)-1;
	} else {
		res2 = ((uint64)parRes * res1 + ((res1 - parRes) >> 1)) / (res1 - parRes);
		if (res2 >= (uint32)-1) {
			return (uint32)-1;
		} else {
			return (uint32)res2;
		}
	}
}

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
int32 LinearMap_I32(int32 input, int32 imin, int32 imax, int32 omin, int32 omax)
{
	int64 irange, orange;
	if (imin == imax)
		return ((int64)omin + omax) / 2;
	if (input < imin)
		input = imin;
	if (input > imax)
		input = imax;
	irange = imax - imin;
	orange = omax - omin;
	return omin + MATH_DIV_ROUND((input - imin) * orange, irange);
}

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
uint8 FilterAvg_Init(FilterAvgType *filter, void *buffer, uint16 depth, uint16 eNum, FILTERAVG_ELEMENT_TYPE eType)
{
	if (NULL == filter || NULL == buffer || depth < 3)
		return 0xFF;
	filter->Buffer = buffer;
	filter->Depth = depth;
	filter->ArrNum = eNum;
	filter->ElementType = eType;
	filter->Head = 0;
	filter->Count = 0;
	filter->InitOK = 1;
	return 0;
}
#define _FILTER_ELEMENT_(baseType)                                                                       \
	do {                                                                                                 \
		if (filter->Count < filter->Depth) { /*滤波器缓存未满，只做均值滤波*/              \
			for (i = 0; i < filter->ArrNum; i++) {                                                       \
				inValue_##baseType = (baseType *)filter->Buffer + i;                                     \
				sum = 0;                                                                                 \
				for (j = 0; j < filter->Count; j++) {                                                    \
					tmp = inValue_##baseType[j * filter->ArrNum];                                        \
					sum += tmp;                                                                          \
				}                                                                                        \
				outValue_##baseType[i] = (baseType)(MATH_DIV_ROUND(sum, filter->Count));                 \
			}                                                                                            \
		} else { /*进行去极值滑动平均滤波*/                                                   \
			for (i = 0; i < filter->ArrNum; i++) {                                                       \
				inValue_##baseType = (baseType *)filter->Buffer + i;                                     \
				sum = max = min = inValue_##baseType[0];                                                 \
				for (j = 1; j < filter->Depth; j++) {                                                    \
					tmp = inValue_##baseType[j * filter->ArrNum];                                        \
					sum += tmp;                                                                          \
					if (tmp > max) {                                                                     \
						max = tmp;                                                                       \
					} else if (tmp < min) {                                                              \
						min = tmp;                                                                       \
					}                                                                                    \
				}                                                                                        \
				outValue_##baseType[i] = (baseType)(MATH_DIV_ROUND(sum - max - min, filter->Count - 2)); \
			}                                                                                            \
		}                                                                                                \
	} while (0)

/**
 * @brief 更新均值滤波器
 *
 * @param filter: 滤波器指针
 * @param element: 输入元素指针
 * @param oelement: 输出元素指针
 */
void FilterAvg_Updata(FilterAvgType *filter, const void *element, void *oelement)
{
	uint16 i, j;
	int64 max, min, sum, tmp;
	uint8 *outValue_uint8 = (uint8 *)oelement, *inValue_uint8;
	int8 *outValue_int8 = (int8 *)oelement, *inValue_int8;
	uint16 *outValue_uint16 = (uint16 *)oelement, *inValue_uint16;
	int16 *outValue_int16 = (int16 *)oelement, *inValue_int16;
	uint32 *outValue_uint32 = (uint32 *)oelement, *inValue_uint32;
	int32 *outValue_int32 = (int32 *)oelement, *inValue_int32;
	if (NULL == filter || NULL == element)
		return;
	if (!filter->InitOK) {
		return;
	}
	memcpy((char *)filter->Buffer + filter->Head * filter->ArrNum * (filter->ElementType & 0x0F), element, filter->ArrNum * (filter->ElementType & 0x0F));
	filter->Head = (filter->Head + 1) % filter->Depth;
	if (filter->Count < filter->Depth) {
		filter->Count++;
	}
	if (NULL != oelement) {
		if (filter->ElementType == FILTER_AVG_ELEM_U16) {
			_FILTER_ELEMENT_(uint16);
		} else if (filter->ElementType == FILTER_AVG_ELEM_S16) {
			_FILTER_ELEMENT_(int16);
		} else if (filter->ElementType == FILTER_AVG_ELEM_U32) {
			_FILTER_ELEMENT_(uint32);
		} else if (filter->ElementType == FILTER_AVG_ELEM_S32) {
			_FILTER_ELEMENT_(int32);
		} else if (filter->ElementType == FILTER_AVG_ELEM_U8) {
			_FILTER_ELEMENT_(uint8);
		} else if (filter->ElementType == FILTER_AVG_ELEM_S8) {
			_FILTER_ELEMENT_(int8);
		}
	}
}