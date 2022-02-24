//scrypt.h
#pragma once

#include <windows.h>

#define LSC_ERR_SUCCESS							0
#define LSC_ERR_SPACE_NOT_ENOUGH				1
#define LSC_ERR_INPUT_FILE_NOT_FOUND			2
#define LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS		3
//LSC_ERR_DECRYPT_FILE_FORMAT解密文件格式错误, 通常是解密时, 输入的文件是未加密过的或密码错误
#define LSC_ERR_DECRYPT_FILE_FORMAT				4
//被加密的文件与默认的扩展名相同
#define LSC_ERR_FILE_IS_DEFAULT_EXT				5
#define LSC_ERR_FILE_IS_NOT_DEFAULT_EXT			6
#define LSC_ERR_TASK_ABORT						7
#define LSC_ERR_SYSTEM							30

#define MB400		419430400
#define MB100		104857600
#define MB80		83886080
#define MB40		41943040
#define MB20		20971520
#define MB10		10485760
//BUFFER SIZE
#define MB2			2097152	
#define MB1			1048576
#define KB512		524288
#define KB256		262144
#define KB128		131072
#define KB64		65536
#define KB8			8192

//默认加密后的文件名后缀
#define DEFAULT_EXT_A				"LSC"
#define DEFAULT_EXT_W				L"LSC"

#if defined(UNICODE) || defined(_UNICODE)
#define DEFAULT_EXT					DEFAULT_EXT_W
#else
#define DEFAULT_EXT					DEFAULT_EXT_A
#endif

//回调事件nEvent参数
//进度指示, 此时param1 是当前处理的文件总的大小, param2 是已完成的大小
#define EN_DE_CALLBACK_EVENT_PROGRESS			1
//此时param1指示了LSC_开头的错误信息
#define EN_DE_CALLBACK_EVENT_ERROR				2		//目前未使用
//回调应答
//pfnEn_DeCallback的返回值
#define EN_DE_CALLBACK_EVENT_CONTINUE			10		//忽略当前错误
#define EN_DE_CALLBACK_EVENT_IGNORE				11		//忽略当前错误
#define EN_DE_CALLBACK_EVENT_ABORT				12		//取消任务



typedef DWORD (WINAPI *pfnEn_DeCallback)(DWORD nEvent, unsigned __int64 param1, unsigned __int64 param2);

#define MAX_AES_PASSWORD_LEN					32

//#ifndef _static_lib_en_decrypt_
//#include "UFileCrypt.h"
//	#ifdef _DEBUG
//		#pragma comment(lib, "../bin/en_decrypt.lib")
//	#else
//		#pragma comment(lib, "../bin/en_decrypt_d.lib")
//	#endif
//#endif