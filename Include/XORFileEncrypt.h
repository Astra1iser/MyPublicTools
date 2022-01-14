#pragma once

/*
* �ļ��ӽ���,��ʵ�ӽ��ܶ��ܷ�������,������ʵҲ��һ�ּ���,ע��ӽ���һ��Ҫ��ͬһ��Կ,��Ȼ����ֱ��GG
* exp:
	XORFileEncrypt test;
	test.SetByKey(999);
	test.Encrypt(L"D:\\Dbgview.exe");
	test.Decrypt(L"D:\\Dbgview.exe");
*/
class XORFileEncrypt
{
public:
	XORFileEncrypt(void);
	~XORFileEncrypt(void);

	//�����ļ�
	int Encrypt(LPCTSTR lpcszFile);

	//�����ļ�
	int Decrypt(LPCTSTR lpcszFile);

	//���
	int XOREncrypt(LPBYTE pbyContent, DWORD dwLen, BYTE byKey);

	//������Կ
	void SetByKey(BYTE byKey) { m_byKey = byKey;}

private:
	BYTE m_byKey;
};
