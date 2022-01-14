#pragma once

// 按行读取文本，无内存分配操作
// 支持另外一个程序正在写日志（例如HTTP服务器），本程序同时顺序读取
// 调用本工具时，不要每次都重新打开日志，除非日期发生变化，且无数据可读

/*
 *exp:

	TxtReader test;
	test.Open(L"D:\\MyPublicTools\\MyPublicTools\\Include\\TxtReader.h");
	char line_buf[TXT_READER_MAX_BUF] = {0};
	test.ReadLine(line_buf);
	test.ReadLine(line_buf);
	test.ReadLine(line_buf);
	test.ReadLine(line_buf);

*/


// 内部buffer长度
const int TXT_READER_MAX_BUF	= 64*1024;

class TxtReader
{
public:
	TxtReader();
	~TxtReader();

	BOOL Open(LPCTSTR szFile);
	BOOL IsValid() { return m_file != 0;}
	void Close();
	
	// 读取一整行，返回FALSE表示到达文件末尾，并且没有读到内容。
	// buf大小建议使用TXT_READER_MAX_BUF+1
	// line_len 建议使用 TXT_READER_MAX_BUF
	// 如果读满了buf，也没遇到换行符，会返回读到的东西
	// 如果文件末尾不满一行，本函数不会返回半截的东西，而是返回FALSE
	BOOL ReadLine(char *line_buf, int line_len = TXT_READER_MAX_BUF);

private:
	char	m_buf[TXT_READER_MAX_BUF];
	int		m_pos;
	int		m_buf_len;
	HANDLE	m_file;
	int		m_line_no;		// 用于调试时，确定是第几行

	BOOL	ScanLine(char *line_buf, int line_len, int &ret);
};
