#pragma once

class MyOperFile
{

public:
	/*
	* @funcn							--- FileIsEixst
	* @brief							--- 判断文件是否存在	
	* @param strFilePath				--- 文件或文件夹的路径
	* @return 
	*							   FALSE--- 文件不存在
	*							   TRUE	--- 文件存在
	*/
	static BOOL FileIsEixst(IN const std::wstring& strFilePath);

	/*
	* @funcn							--- GetFileSize
	* @brief							--- 获取文件的大小	
	* @param strFilePath				--- 文件的绝对路径
	* @param uFileSize					--- 得到的文件的大小
	* @return 
	*							   FALSE--- 失败，可能原因是文件打开失败；或，获取文件大小失败
	*							   TRUE	--- 文件存在
	*/
	static int GetFileSize(IN const std::wstring& strFilePath, OUT size_t& uFileSize);

	/*
	* @funcn							--- ReadFileData
	* @brief							--- 读取文件	
	* @param strFilePath				--- 需要读取的文件的绝对路径
	* @param nMode						--- 编码方式，1-ansi 2-utf8 3-unicode
	* @param strFileData				--- 获取的文件的数据
	* @return 
	*							   FALSE--- 读文件失败
	*							   TRUE	--- 读文件成功
	*/
	static int ReadFileData(IN const std::wstring& strFilePath, OUT int& nMode, OUT std::wstring& strFileData);
	static int ReadFileData(IN const std::wstring& strFilePath, IN BYTE* pData, int nSize, int& nFileSize);
	//static int ReadFileData(IN const std::wstring& strFilePath, OUT std::wstring& strFileData);

	/*
	* @funcn							--- WriteFileData
	* @brief							--- 写入文件	
	* @param strFilePath				--- 需要写入的文件的绝对路径
	* @param nMode						--- 编码方式，1-ansi 2-utf8 3-unicode
	* @param strFileData				--- 需要写入的文件的数据
	* @return 
	*							   FALSE--- 写文件失败
	*							   TRUE	--- 读文件成功
	*/
	static int WriteFileData(IN const std::wstring& strFilePath, IN int nMode, IN std::wstring strFileData);

	static int WriteFileData(IN const std::wstring& strFilePath, IN BYTE* pData, int nSize);

	/*
	* @funcn							--- DelteFile
	* @brief							--- 删除文件，可以设定是否重启删除	
	* @param strFilePath				--- 需要删除的文件的绝对路径
	* @param bRebootMoveIfFail			--- 设定是否重启后删除
	* @return 
	*							   FALSE--- 删除失败，或设置重启失败
	*							   TRUE	--- 删除成功，或设置重启成功
	*/
	static int DelteFile(IN const std::wstring& strFilePath, IN BOOL bRebootMoveIfFail = TRUE);

	/*
	* @funcn							--- FileCopy
	* @brief							--- 拷贝文件	
	* @param strSrcFilePath				--- 源文件的路径
	* @param strDesFilePath				--- 要拷贝到的文件路径
	* @param bFailIfExists				--- 设定当文件存在时，是否覆盖
	* @return 
	*							   FALSE--- 文件拷贝失败
	*							   TRUE	--- 文件拷贝成功
	*/
	static int FileCopy(IN const std::wstring& strSrcFilePath, IN const std::wstring& strDesFilePath, IN BOOL bFailIfExists);
};
