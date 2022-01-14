#pragma once

/*
* 文件加解密,其实加解密都能反过来用,解密其实也是一种加密,注意加解密一定要用同一密钥,不然解密直接GG
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

	//加密文件
	int Encrypt(LPCTSTR lpcszFile);

	//解密文件
	int Decrypt(LPCTSTR lpcszFile);

	//异或
	int XOREncrypt(LPBYTE pbyContent, DWORD dwLen, BYTE byKey);

	//设置密钥
	void SetByKey(BYTE byKey) { m_byKey = byKey;}

private:
	BYTE m_byKey;
};
