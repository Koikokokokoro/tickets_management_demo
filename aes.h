#include "SharedFunc.h"
#include <openssl/aes.h>

using namespace std;

#ifndef __AES_H__
#define __AES_H__

#define OS_LINUX	0x00
//#define	OS_WINDOWS	0x01

#define TYPE_OS		OS_WINDOWS	//windows下使用

#if TYPE_OS == OS_WINDOWS
//去掉编译警告
#define _CRT_NONSTDC_NO_DEPRECATE 
//#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#endif

#define AES_BIT		256//192//256
#define AES_KEYLEN  AES_BIT/8

 //错误码定义
enum SysErrCode {
	ErrNone = 0,
	ErrSrcNotExist,
	ErrSrcOpenFail,
	ErrDstOpenFail,
	ErrWriteFail,
	ErrAesFileFormat
};

class aes {
public:
	enum AESBIT {
		AES_256 = 256,
		AES_192 = 192,
		AES_128 = 128,
	};

	int aesBit;
	int aesKeyLen;
	unsigned char* myKey;

	/**
	  * @brief aes构造函数
	  */
	aes();

	/**
	  * @brief aes析构函数
	  */
	~aes();

	/**
	  * @brief 设置加密key
	  */
	int setKey(unsigned char* key, AESBIT bit);

	/**
	  * @brief 加密输入缓冲内容
	  */
	int encryptBuf(unsigned char* inbuf, int inbuflen, unsigned char* outbuf);

	/**
	  * @brief 解密输入缓冲内容
	  */
	int decryptBuf(unsigned char* inbuf, int inbuflen, unsigned char* outbuf);

	//int SaveEncryptedList(const vector<BusTimetable>& timetableData, const char* filename);
	//int LoadDecryptedList(vector<BusTimetable>& timetableData, const char* filename);

	int SaveEncryptedAdminInfo(const vector<pair<string, string>>& adminInfo, const char* filename);
	int LoadDecryptedAdminInfo(vector<pair<string, string>>& adminInfo, const char* filename);
};

#endif