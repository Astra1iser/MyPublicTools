#include "stdafx.h"
#include "TxtReader.h"

TxtReader::TxtReader()
{
	m_file = 0;
	m_buf_len = 0;
	m_pos = 0;
	m_line_no = 0;
}

TxtReader::~TxtReader()
{
	Close();
}

void TxtReader::Close()
{
	if(m_file)
	{
		CloseHandle(m_file);
		m_file = 0;
	}

	m_buf_len = 0;
	m_pos = 0;

	m_line_no = 0;
}

BOOL TxtReader::Open(LPCTSTR szFile)
{
	Close();

	m_file = ::CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_file == INVALID_HANDLE_VALUE)
	{
		m_file = 0;
	}

	return IsValid();
}

BOOL TxtReader::ScanLine(char *line_buf, int line_len, int &ret)
{
	for(;m_pos<m_buf_len;)
	{
		char ch = m_buf[m_pos];
		m_pos ++;

		if(ch == '\r')
		{
		}
		else if(ch == '\n')
		{
			line_buf[ret] = 0;
			m_line_no ++;;
			return TRUE;
		}
		else
		{
			// ��ֹ������������������1��Ҳ����1�з���
			if(ret >= line_len - 2)
			{
				line_buf[ret] = 0;
				return TRUE;
			}

			line_buf[ret] = ch;
			ret ++;
		}
	}

	return FALSE;
}

BOOL TxtReader::ReadLine(char *line_buf, int line_len)
{
	line_buf[0] = 0;
	line_buf[line_len-1] = 0;

	if(!IsValid())
		return FALSE;


	// ���ȴӻ�����ȡ
	int ret = 0;
	BOOL enough = ScanLine(line_buf, line_len, ret);
	if(enough) return TRUE;


	// ����ȡ�Ĳ��������߻���Ϊ�գ����ʣ�ಿ�ָ��Ƶ�ǰ��
	int copy_size = m_buf_len - m_pos;
	assert(copy_size >= 0);

	if(copy_size > 0)
	{
		char tmp[sizeof(m_buf)];
		memcpy(tmp, m_buf + m_pos, copy_size);
		memcpy(m_buf, tmp, copy_size);
		m_buf_len = copy_size;
	}
	else
	{
		m_buf_len = 0;
	}
	m_pos = 0;
	

	// Ȼ���ȡ�ļ���䵽����
	{
		int read_size = sizeof(m_buf) - m_buf_len;
		int read_from = m_buf_len;
		DWORD dwReaded = 0;
		BOOL bRet = ::ReadFile(m_file, m_buf + read_from, read_size, &dwReaded, NULL);

		if(bRet == FALSE)
			return FALSE;

		m_pos = 0;
		m_buf_len += dwReaded;
	}


	// ��ɨһ��
	enough = ScanLine(line_buf, line_len, ret);
	if(enough) return TRUE;

	return FALSE;
}
