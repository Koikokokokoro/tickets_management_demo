#include "pch.h"
#include "aes.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <stdlib.h>

#if TYPE_OS == OS_WINDOWS
#include <io.h>
#define lstat		_stati64
#define	stat		_stati64
#else
#include <sys/vfs.h>
#endif // TYPE_OS == OS_WINDOWS
#include "C:/Program Files/OpenSSL-Win64/include/openssl/evp.h"

using namespace std;

static const unsigned int READ_BUF_LEN = 1024 * 1024;
#if AES_BIT == 256
unsigned char gAESKEY[AES_KEYLEN] = { 0x45,0x4d,0x5e,0xdf,0x8c,0xae,0x53,0x23,
								   0xc4,0xed,0xac,0x99,0xaa,0x78,0x29,0x45,
								   0x11,0x54,0x36,0x65,0x45,0x88,0xac,0xea,
								   0xde,0xdd,0xea,0xeb,0xbd,0x8b,0x9a,0xb5 };
#elif AES_BIT == 192
unsigned char gAESKEY[AES_KEYLEN] = { 0x85,0x4d,0x55,0xdf,0x8c,0xae,0x53,0x23,
								   0xca,0xed,0xac,0x99,0xaa,0x76,0x29,0x45,
								   0x11,0x54,0x31,0x65,0x45,0x28,0xac,0xea };

#elif AES_BIT == 128
unsigned char gAESKEY[AES_KEYLEN] = { 0x85,0x4d,0x5e,0xdf,0x8c,0x5e,0x53,0x23,
								   0xca,0xed,0xac,0x97s,0xaa,0x78,0x29,0x45 };
#endif

/**
  * @brief 构造函数
  */
aes::aes()
{
	aesBit = AES_BIT;
	aesKeyLen = AES_KEYLEN;
	myKey = new unsigned char[aesKeyLen];
	memcpy(myKey, gAESKEY, aesKeyLen);
}

/**
  * @brief 析构函数
  */
aes::~aes()
{
	delete[] myKey;
}

/**
  * @brief 设置密钥和加密位数
  * @para1 key 密钥
  * @para2 bit 加密位数
  * @expl
  * @return 0:成功 -1:失败
  */
int aes::setKey(unsigned char* key, AESBIT bit)
{
	aesBit = bit;
	aesKeyLen = bit / 8;
	delete[] myKey;
	myKey = new unsigned char[aesKeyLen];
	memcpy(myKey, key, aesKeyLen);
	return 0;
}


/**
  * @brief 加密输入缓冲内容
  * @para1 inbuf 输入缓冲
  * @para2 inbuflen 输入缓冲大小
  * @para3 outbuf 输出缓冲
  * @expl
  * @return 加密后的字节数
  */
int aes::encryptBuf(unsigned char* inbuf, int inbuflen, unsigned char* outbuf)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();  // 创建加密上下文
	if (!ctx) return -1;

	int len, ciphertext_len;
	const int block_size = 16;  // AES 块大小
	unsigned char iv[block_size] = { 0 };  // 初始化IV为0

	// 初始化加密操作
	if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, myKey, iv) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}

	// 加密数据
	if (EVP_EncryptUpdate(ctx, outbuf, &len, inbuf, inbuflen) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	ciphertext_len = len;

	// 处理最后的填充块
	if (EVP_EncryptFinal_ex(ctx, outbuf + len, &len) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	ciphertext_len += len;

	// 清理
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;  // 返回加密后的长度
}


/**
  * @brief 解密输入缓冲内容
  * @para1 inbuf 输入缓冲
  * @para2 inbuflen 输入缓冲大小
  * @para3 outbuf 输出缓冲
  * @expl
  * @return 加密后的字节数
  */
int aes::decryptBuf(unsigned char* inbuf, int inbuflen, unsigned char* outbuf)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();  // 创建解密上下文
	if (!ctx) return -1;

	int len, plaintext_len;
	unsigned char iv[16] = { 0 };  // 初始化IV为0

	// 初始化解密操作
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, myKey, iv) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}

	// 解密数据
	if (EVP_DecryptUpdate(ctx, outbuf, &len, inbuf, inbuflen) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	plaintext_len = len;

	// 处理最后的填充块
	if (EVP_DecryptFinal_ex(ctx, outbuf + len, &len) != 1)
	{
		EVP_CIPHER_CTX_free(ctx);
		return -1;
	}
	plaintext_len += len;

	// 清理
	EVP_CIPHER_CTX_free(ctx);
	return plaintext_len;  // 返回解密后的长度
}



/**
  * @brief AES文件加密
  * @para1 src_path 源文件,未加密的文件
  * @para2 dst_path 目的文件,加密文件
  * @expl
  * @return 0:成功 其他:失败
  */
//int aes::encryptFile(const char* src_path, const char* dst_path)
//{
//
//	AES_KEY aeskey;
//	struct stat src_stat;
//	int read_fd, write_fd, ret, add_tail = 0;
//	long long read_file_size;
//	int read_size, remain;
//	int block, block_remain;
//	unsigned char* inbuf = new unsigned char[READ_BUF_LEN + 32];
//	unsigned char* outbuf = new unsigned char[READ_BUF_LEN + 32];
//
//	/* 获取文件属性 */
//	if (lstat(src_path, &src_stat) != 0)
//		return ErrSrcNotExist;
//
//	read_file_size = src_stat.st_size;
//
//	if (read_file_size % 16 == 0 || read_file_size == 0)
//		add_tail = 1;
//
//
//	read_fd = open(src_path, O_RDONLY | O_BINARY);
//	if (read_fd < 0)
//		return ErrSrcOpenFail;
//
//
//	write_fd = open(dst_path, O_CREAT | O_WRONLY | O_BINARY);
//	if (write_fd < 0) {
//		close(read_fd);
//		return ErrDstOpenFail;
//	}
//
//	AES_set_encrypt_key(myKey, aesBit, &aeskey);
//
//	while (read_file_size > 0) {
//		read_size = read(read_fd, inbuf, READ_BUF_LEN);
//		block = read_size / 16;
//		block_remain = read_size % 16;
//		for (int i = 0; i < block; i++) {
//			AES_encrypt(&inbuf[i * 16], &outbuf[i * 16], &aeskey);
//		}
//		if (block_remain != 0) {
//			remain = 32 - block_remain;
//			for (int i = block_remain; i < 16; i++)
//				inbuf[i + block * 16] = remain;
//			AES_encrypt(&inbuf[block * 16], &outbuf[block * 16], &aeskey);
//			block++;
//			for (int i = 0; i < 16; i++)
//				inbuf[i + block * 16] = remain;
//			AES_encrypt(&inbuf[block * 16], &outbuf[block * 16], &aeskey);
//			block++;
//		}
//		ret = write(write_fd, outbuf, block * 16);
//		if (ret < 0)
//			goto END;
//		read_file_size -= read_size;
//	}
//
//	if (add_tail == 1) {
//		for (int k = 0; k < 2; k++) {
//			memset(inbuf, 32, 16);
//			AES_encrypt(inbuf, outbuf, &aeskey);
//			ret = write(write_fd, outbuf, 16);
//			if (ret < 0)
//				goto END;
//		}
//	}
//
//END:
//	close(read_fd);
//	close(write_fd);
//
//	delete[] inbuf;
//	delete[] outbuf;
//	if (ret < 0)
//		return ErrWriteFail;
//
//	return ErrNone;
//}

/**
  * @brief AES文件解密
  * @para1 src_path 源文件,解密前的密文
  * @para2 dst_path 目的文件,解密后的明文
  * @expl
  * @return 0:成功 其他:失败
  */
//int aes::decryptFile(const char* src_path, const char* dst_path)
//{
//	AES_KEY aeskey;
//	struct stat src_stat;
//	int read_fd, write_fd, ret, same = 0, write_len;
//	long long read_file_size;
//	int read_size, block;
//	unsigned char* inbuf = new unsigned char[READ_BUF_LEN + 32];
//	unsigned char* outbuf = new unsigned char[READ_BUF_LEN + 32];
//	unsigned char data;
//	char* dstfileDir = NULL;
//
//	/* 获取文件属性 */
//	if (lstat(src_path, &src_stat) != 0)
//		return ErrSrcNotExist;
//
//	read_file_size = src_stat.st_size;
//
//	/* 判断加密文件长度是否合法,正常为16的倍数 */
//	if (read_file_size % 16 != 0 || read_file_size == 0)
//		return ErrAesFileFormat;
//
//	read_fd = open(src_path, O_RDONLY | O_BINARY);
//	if (read_fd < 0)
//		return ErrSrcOpenFail;
//
//	/* 删除已存在的目的文件 */
//	unlink(dst_path);
//
//	write_fd = open(dst_path, O_CREAT | O_WRONLY | O_BINARY);
//	if (write_fd < 0) {
//		close(read_fd);
//		return ErrDstOpenFail;
//	}
//
//	AES_set_decrypt_key(myKey, aesBit, &aeskey);
//
//	while (read_file_size > 0) {
//		read_size = read(read_fd, inbuf, READ_BUF_LEN);
//
//		block = read_size / 16;
//
//		for (int i = 0; i < block; i++)
//			AES_decrypt(&inbuf[i * 16], &outbuf[i * 16], &aeskey);
//
//		write_len = read_size;
//
//		if (read_size == read_file_size) {
//			data = outbuf[read_size - 1];
//			for (int i = 0; i < 32; i++) {
//				if (data == outbuf[read_size - 1 - i]) {
//					same++;
//				}
//				else {
//					break;
//				}
//			}
//			if (same > 0 && same >= data) {
//				write_len = read_size - data;
//			}
//		}
//		ret = write(write_fd, outbuf, write_len);
//		if (ret < 0)
//			goto END;
//		read_file_size -= read_size;
//	}
//END:
//	close(read_fd);
//	close(write_fd);
//
//	delete[] inbuf;
//	delete[] outbuf;
//
//	if (ret < 0)
//		return ErrWriteFail;
//	return ErrNone;
//}
//int aes::SaveEncryptedList(const vector<BusTimetable>& timetableData, const char* filename)
//{
//	// 将车次信息序列化到内存缓冲区
//	ostringstream oss;
//	CString cstrdepartureTime;
//	string strdepartureTime;
//	for (const auto& timetable : timetableData)
//	{
//		cstrdepartureTime.Format("%d:%d", timetable.departureTime.GetHour(), timetable.departureTime.GetMinute());
//		strdepartureTime = cstrdepartureTime.GetBuffer();
//
//		oss << timetable.busNumber << ","
//			<< strdepartureTime << ","
//			<< timetable.startStation << ","
//			<< timetable.endStation << ","
//			<< timetable.travelTime << ","
//			<< timetable.price << ","
//			<< timetable.capacity << ","
//			<< timetable.soldTickets << "\n";
//	}
//	string plainData = oss.str();
//	int plainDataLen = plainData.length();
//
//	// 加密缓冲区内容
//	vector<unsigned char> encryptedData(plainDataLen + AES_BLOCK_SIZE);
//	int encryptedDataLen = encryptBuf(reinterpret_cast<unsigned char*>(&plainData[0]), plainDataLen, &encryptedData[0]);
//
//	// 将加密数据写入文件
//	ofstream outFile(filename, ios::binary | ios::trunc);
//	if (!outFile.is_open())
//		return ErrDstOpenFail;
//
//	outFile.write(reinterpret_cast<char*>(&encryptedData[0]), encryptedDataLen);
//	outFile.close();
//
//	return ErrNone;
//}
//
//int aes::LoadDecryptedList(vector<BusTimetable>& timetableData, const char* filename)
//{
//	// 读取加密文件内容到内存
//	ifstream inFile(filename, ios::binary);
//	if (!inFile.is_open())
//		return ErrSrcOpenFail;
//
//	vector<unsigned char> encryptedData((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
//	inFile.close();
//
//	int encryptedDataLen = encryptedData.size();
//	vector<unsigned char> decryptedData(encryptedDataLen + AES_BLOCK_SIZE);
//
//	// 解密文件内容
//	int decryptedDataLen = decryptBuf(&encryptedData[0], encryptedDataLen, &decryptedData[0]);
//
//	// 将解密后的数据转成字符串
//	string plainData(reinterpret_cast<char*>(&decryptedData[0]), decryptedDataLen);
//
//	// 解析解密后的数据
//	istringstream iss(plainData);
//	string line;
//	timetableData.clear();
//
//	while (getline(iss, line))
//	{
//		istringstream lineStream(line);
//		string token;
//		BusTimetable timetable;
//		if (getline(lineStream, token, ',')) timetable.busNumber = token.c_str();
//		if (getline(lineStream, token, ','))
//		{
//			int ny, nmm, nd, nh, nm;
//			CTime currentTime = CTime::GetCurrentTime();
//			ny = currentTime.GetYear();
//			nmm = currentTime.GetMonth();
//			nd = currentTime.GetDay();
//			istringstream iss(token);
//			string str;
//			char interval = ':';
//			iss >> nh >> interval >> nm;
//			CTime nt(ny, nmm, nd, nh, nm, 0);
//			timetable.departureTime = nt;
//		}
//		if (getline(lineStream, token, ',')) timetable.startStation = token.c_str();
//		if (getline(lineStream, token, ',')) timetable.endStation = token.c_str();
//		if (getline(lineStream, token, ',')) timetable.travelTime = token.c_str();
//		if (getline(lineStream, token, ',')) timetable.price = atoi(token.c_str());
//		if (getline(lineStream, token, ',')) timetable.capacity = stoi(token);
//		if (getline(lineStream, token, ',')) timetable.soldTickets = stoi(token);
//
//		timetableData.push_back(timetable);
//	}
//
//	return ErrNone;
//}

int aes::SaveEncryptedAdminInfo(const std::vector<std::pair<std::string, std::string>>& adminInfo, const char* filename)
{
	// 将管理员用户信息序列化到内存缓冲区
	std::ostringstream oss;
	for (const auto& admin : adminInfo)
	{
		oss << admin.first << "," << admin.second << "\n";  // username,password
	}
	std::string plainData = oss.str();
	int plainDataLen = plainData.length();

	// 加密缓冲区内容
	std::vector<unsigned char> encryptedData(plainDataLen + 32);  // 加密数据缓冲区
	int encryptedDataLen = encryptBuf(reinterpret_cast<unsigned char*>(&plainData[0]), plainDataLen, &encryptedData[0]);

	// 将加密数据写入文件
	std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
	if (!outFile.is_open())
		return ErrDstOpenFail;

	outFile.write(reinterpret_cast<char*>(&encryptedData[0]), encryptedDataLen);
	outFile.close();

	return ErrNone;
}


int aes::LoadDecryptedAdminInfo(std::vector<std::pair<std::string, std::string>>& adminInfo, const char* filename)
{
	// 读取加密文件内容到内存
	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile.is_open())
		return ErrSrcOpenFail;

	std::vector<unsigned char> encryptedData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
	inFile.close();

	int encryptedDataLen = encryptedData.size();
	std::vector<unsigned char> decryptedData(encryptedDataLen + 32);  // 解密数据缓冲区

	// 解密文件内容
	int decryptedDataLen = decryptBuf(&encryptedData[0], encryptedDataLen, &decryptedData[0]);

	// 将解密后的数据转成字符串
	std::string plainData(reinterpret_cast<char*>(&decryptedData[0]), decryptedDataLen);

	// 解析解密后的数据，格式：username,password
	std::istringstream iss(plainData);
	std::string line;
	adminInfo.clear();

	while (std::getline(iss, line))
	{
		std::istringstream lineStream(line);
		std::string username, password;
		std::getline(lineStream, username, ',');
		std::getline(lineStream, password, ',');

		adminInfo.push_back(std::make_pair(username, password));
	}

	return ErrNone;
}

