/**
 * @file code_file_std.h
 * @author wwyyy (1046685883@qq.com)
 * @brief
 * @version 1.0
 * @date 2026-03-21
 *
 * @copyright Copyright (c) 2015-2026 oldking-ecu, All rights reserved
 *
 */
#ifndef CODE_FILE_STD_H__
#define CODE_FILE_STD_H__

#define NAME_CONCAT(name1, name2) name1##name2


#define MODULE_INS_CFG_TYPE(name) NAME_CONCAT(name, _InsCfgType)
#define MODULE_INS_CFG(name)      NAME_CONCAT(name, _InsCfg)
#define MODULE_INS_INF_TYPE(name) NAME_CONCAT(name, _InsInfType)
#define MODULE_INS_INF(name)      NAME_CONCAT(name, _InsInf)
#define MODULE_CFG_TYPE(name)     NAME_CONCAT(name, _CfgType)
#define MODULE_CFG(name)          NAME_CONCAT(name, _Conf)
#define MODULE_INF_TYPE(name)     NAME_CONCAT(name, _InfType)
#define MODULE_INF(name)          NAME_CONCAT(name, _Info)

#define MODULE_INIT_FUN(name)           NAME_CONCAT(name, _Init)
#define MODULE_GET_INSCFG_PTR_FUN(name) NAME_CONCAT(name, _GetInsCfgPtr)
#define MODULE_GET_INSINF_PTR_FUN(name) NAME_CONCAT(name, _GetInsInfPtr)
#define MODULE_MAIN_FUN(name)           NAME_CONCAT(name, _Main)

#define MODULE_USERINIT_FUN(name)   NAME_CONCAT(name, _UserInit)
#define MODULE_ENUM_NAME(name, ins) NAME_CONCAT(name, _##INS_##ins)
#define MODULE_ENUM_TYPE(name)      NAME_CONCAT(name, _##INS_ENUM_TYPE)


#endif    // CODE_FILE_STD_H__
