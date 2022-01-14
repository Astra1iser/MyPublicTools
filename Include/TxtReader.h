#pragma once

// ���ж�ȡ�ı������ڴ�������
// ֧������һ����������д��־������HTTP����������������ͬʱ˳���ȡ
// ���ñ�����ʱ����Ҫÿ�ζ����´���־���������ڷ����仯���������ݿɶ�

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


// �ڲ�buffer����
const int TXT_READER_MAX_BUF	= 64*1024;

class TxtReader
{
public:
	TxtReader();
	~TxtReader();

	BOOL Open(LPCTSTR szFile);
	BOOL IsValid() { return m_file != 0;}
	void Close();
	
	// ��ȡһ���У�����FALSE��ʾ�����ļ�ĩβ������û�ж������ݡ�
	// buf��С����ʹ��TXT_READER_MAX_BUF+1
	// line_len ����ʹ�� TXT_READER_MAX_BUF
	// ���������buf��Ҳû�������з����᷵�ض����Ķ���
	// ����ļ�ĩβ����һ�У����������᷵�ذ�صĶ��������Ƿ���FALSE
	BOOL ReadLine(char *line_buf, int line_len = TXT_READER_MAX_BUF);

private:
	char	m_buf[TXT_READER_MAX_BUF];
	int		m_pos;
	int		m_buf_len;
	HANDLE	m_file;
	int		m_line_no;		// ���ڵ���ʱ��ȷ���ǵڼ���

	BOOL	ScanLine(char *line_buf, int line_len, int &ret);
};
