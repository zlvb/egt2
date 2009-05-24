/*
+-----------------------------------------------------------+
|
|	filename:	egpt.h
|	created:	05/09/2007
|	author:		Zhang Li
|
|	purpose:	文件夹加密类实现
|
+-----------------------------------------------------------+
|	文件夹加密工具 (http://jonlee.cnblogs.com)
+-----------------------------------------------------------+
*/

#ifndef egpt_h__
#define egpt_h__

#pragma once
#include <afx.h>
#include <fstream>
#pragma warning(disable: 4800)
#pragma warning(disable: 4267)

class FileEncryptor
{
public:

	FileEncryptor();
	~FileEncryptor();

	//> 加密一个文件夹
	//> szDirName 文件夹路径
	void EncryptDir(char *szDirName);

	//> 解密一个文件夹
	//> szDirName 文件夹路径
	void DecryptDir(char *szDirName);

private:

	//> 加密一个文件
	//> szFilename 文件路径
	void _Encrypt(char *szFilename);

	//> 解密一个文件
	//> szFilename 文件路径
	void _Decrypt(char *szFilename);

	//> 处理一个文件夹（加密或解密）
	//> mode	true	加密
	//			false	解谜
	void _ProcDir(char *szDirName, bool mode);

	//一个文件是否加过密
	bool _IsEncrypted(char *szFileName);

	//是否是缩略图文件
	bool _IstmDB(char *szFileName);


	std::ifstream m_isHead;
	size_t m_iHeadlength;
	char *m_szHeadbuffer;
};


#endif // egpt_h__
