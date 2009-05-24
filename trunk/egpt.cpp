/*
+-----------------------------------------------------------+
|
|	filename:	egpt.cpp
|	created:	05/09/2007
|	author:		Zhang Li
|
|	purpose:	文件夹加密类声明
|
+-----------------------------------------------------------+
*/

#include "egpt.h"

/*>--------------------------------------------------------------<*/
/*>	构造
/*>--------------------------------------------------------------<*/
FileEncryptor::FileEncryptor()
{
	m_isHead.open("entSvc.dat", std::ios_base::binary);
	m_isHead.seekg(0,std::ios_base::end);
	m_iHeadlength = m_isHead.tellg();
	m_isHead.seekg(0,std::ios_base::beg);
	m_szHeadbuffer = new char [m_iHeadlength];
	m_isHead.read(m_szHeadbuffer,m_iHeadlength);
}
/*>--------------------------------------------------------------<*/
/*>	析构
/*>--------------------------------------------------------------<*/
FileEncryptor::~FileEncryptor()
{
	m_isHead.close();
}

/*>--------------------------------------------------------------<*/
/*>	加密一个文件夹
/*>--------------------------------------------------------------<*/
void FileEncryptor::EncryptDir( char *szDirName )
{
	_ProcDir(szDirName,true);
}
/*>--------------------------------------------------------------<*/
/*>	解密一个文件夹
/*>--------------------------------------------------------------<*/
void FileEncryptor::DecryptDir( char *szDirName )
{
	_ProcDir(szDirName,false);
}
/*>--------------------------------------------------------------<*/
/*>	加密一个文件
/*>--------------------------------------------------------------<*/
void FileEncryptor::_Encrypt( char *szFilename )
{
	if (_IsEncrypted(szFilename) || _IstmDB(szFilename))
	{
		return;
	}

	//读入文件
	std::ifstream is;
	is.open(szFilename,std::ios_base::binary);
	is.seekg (0, std::ios_base::end);
	size_t length = is.tellg();
	is.seekg (0, std::ios_base::beg);
	char *buffer = new char[length];
	is.read(buffer,length);
	is.close();

	//写入文件
	std::ofstream os(szFilename,std::ios_base::binary);
	os.seekp(0,std::ios_base::beg);
	os.write(m_szHeadbuffer, m_iHeadlength);
	os.write(buffer,length);
	os.close();
	delete[] buffer;

	//文件改名
	char newExt[] = ".{s}.jpg";
	char newfileName[256] = {0};
	strcpy(newfileName, szFilename);
	strcat(newfileName,newExt);
	rename(szFilename,newfileName);	
	printf("%s\t\tOK\n",szFilename);
}
/*>--------------------------------------------------------------<*/
/*>	解密一个文件
/*>--------------------------------------------------------------<*/
void FileEncryptor::_Decrypt( char *szFilename )
{
	if (!_IsEncrypted(szFilename))
	{
		return;
	}

	//读入文件
	std::ifstream is;
	is.open(szFilename,std::ios_base::binary);
	is.seekg (0, std::ios_base::end);
	size_t length = is.tellg();
	is.seekg (0, std::ios_base::beg);
	is.seekg(m_iHeadlength, std::ios_base::cur);

	if(length > m_iHeadlength)
	{
		char *buffer = new char[length - m_iHeadlength];
		is.read(buffer,length - m_iHeadlength);
		is.close();

		//写入文件
		std::ofstream os(szFilename,std::ios_base::binary);
		os.seekp(0,std::ios_base::beg);
		os.write(buffer,length - m_iHeadlength);
		os.close();
		delete[] buffer;
	}

	//文件改名
	char newfileName[256] = {0};
	int str_len = strlen(szFilename);
	memcpy(newfileName, szFilename, str_len - 15);
	rename(szFilename,newfileName);
	printf("%s\t\tOK\n",szFilename);
}
/*>--------------------------------------------------------------<*/
/*>	处理一个文件夹（加密或解密）
/*> mode  true	加密
/*>		  false	解谜
/*>--------------------------------------------------------------<*/
void FileEncryptor::_ProcDir( char *szDirName, bool mode )
{
	CFileFind tempFind; 

	char sTempFileFind[256] ;

	sprintf(sTempFileFind,"%s\\*.*",szDirName); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
	while (IsFinded) 
	{ 
		IsFinded = tempFind.FindNextFile(); 

		if (!tempFind.IsDots()) 
		{ 
			char sFoundFileName[256]; 
			strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(256)); 

			if (tempFind.IsDirectory()) 
			{ 
				char sTempDir[256]; 
				sprintf(sTempDir,"%s\\%s",szDirName,sFoundFileName); 
				_ProcDir(sTempDir,mode); 
			} 
			else 
			{ 
				char sTempFileName[256]; 
				sprintf(sTempFileName,"%s\\%s",szDirName,sFoundFileName); 
				if (mode)
				{
					_Encrypt(sTempFileName); 
				}
				else
				{
					_Decrypt(sTempFileName);
				}

			} 
		} 
	} 
	tempFind.Close(); 
}
/*>--------------------------------------------------------------<*/
/*>	一个文件是否加过密
/*>--------------------------------------------------------------<*/
bool FileEncryptor::_IsEncrypted( char *szFileName )
{
	size_t length = strlen(szFileName);
	if (length < 15)
	{
		return false;
	}

	char temp[16] = {0};
	strcpy(temp, &(szFileName[length - 15]));
	return strcmp(temp, ".{ILoveUBB}.jpg") == 0;
}
/*>--------------------------------------------------------------<*/
/*>	是否是缩略图文件
/*>--------------------------------------------------------------<*/
bool FileEncryptor::_IstmDB( char *szFileName )
{
	size_t length = strlen(szFileName);
	if (length < 9)
	{
		return false;
	}

	char temp[10] = {0};
	strcpy(temp, &(szFileName[length - 9]));
	return strcmp(temp, "Thumbs.db") == 0;
}