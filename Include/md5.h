/*
* ����򵥽���һ�¾����÷���

1.�����ַ�����MD5ֵ

����Ĵ�������ַ���"abc"��MD5ֵ����cout�����
	MD5 md5;
	md5.update("abc");
	cout << md5.toString() << endl;
	//���߸��򵥵�
	cout << MD5("abc").toString() << endl;

2.�����ļ���MD5ֵ

����Ĵ�������ı��ļ�"D:\test.txt"��MD5ֵ����cout���������Ƕ������ļ��򿪵�ʱ��ǵ�Ҫָ��ios::binaryģʽ��������Ҫע���������������ļ�������ڣ���������ڼ���ǰ���ж���ifstream��״̬��
	MD5 md5;
	md5.update(ifstream("D:\\test.txt"));
	cout << md5.toString() << endl;
	//���߸��򵥵�
	cout << MD5(ifstream("D:\\test.txt")).toString() << endl;

���濴�����Դ���:


	void PrintMD5(const string &str, MD5 &md5) {
		cout << "MD5(\"" << str << "\") = " << md5.toString() << endl;
	}
	
	int main() {
	
		MD5 md5;
		md5.update("");
		PrintMD5("", md5);
	
		md5.update("a");
		PrintMD5("a", md5);
	
		md5.update("bc");
		PrintMD5("abc", md5);
	
		md5.update("defghijklmnopqrstuvwxyz");
		PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);
	
		md5.reset();
		md5.update("message digest");
		PrintMD5("message digest", md5);
	
		md5.reset();
		md5.update(ifstream("D:\\test.txt"));
		PrintMD5("D:\\test.txt", md5);
	
		return 0;
	}

���Խ����
MD5("") = d41d8cd98f00b204e9800998ecf8427e
MD5("a") = 0cc175b9c0f1b6a831c399e269772661
MD5("abc") = 900150983cd24fb0d6963f7d28e17f72
MD5("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
MD5("message digest") = f96b697d7cb7938d525a2f31aaf161d0
MD5("D:\test.txt") = 7ac66c0f148de9519b8bd264312c4d64

*/

#ifndef MD5_H
#define MD5_H

#include <string>
#include <fstream>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
	MD5();
	MD5(const void* input, size_t length);
	MD5(const string& str);
	MD5(ifstream& in);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	const byte* digest();
	string toString();
	void reset();

private:
	void update(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32* input, byte* output, size_t length);
	void decode(const byte* input, uint32* output, size_t length);
	string bytesToHexString(const byte* input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);

private:
	uint32 _state[4];	/* state (ABCD) */
	uint32 _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};

#endif /*MD5_H*/
