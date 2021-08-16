// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"




//继承Thread;
class Example : public Thread
{
public:
	//重写Run;
	void Run()
	{
		while (this->bRun)
		{
			printf("Hello thread1\n");
			Sleep(1000);
		}
	}
};



int main()
{   
    //HMODULE  hDll = LoadLibrary(GetAnyRunPathFile(L"7z.dll"));

    //wcout.imbue(std::locale("chs"));
    //CString abc = L"";

    //wcout << abc.GetString()<<endl;

    //SetReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path", L"1234",0);
    //auto t = GetIntReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");
    //wcout << get<1>(t) << endl;
    //DeleteReg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MyProgram.exe", L"Path");


   /* cout << IsAdmin();*/

    //double a[] = {7,5,2,4,2.1};
	//int aa= DoublePointerRemoveValue(a,2);
	//
	////int ValueNumber = FindValueInList(2.1, a);
	//
	//a;



    //for (int i = 0; i < 4; i++)
    //    cout << a[i];


	//int a[] = { 7,5,2,4 };
	//int temp, j, mid;
	//for (int i = 0; i < 4; i++)
	//{
	//	temp = a[i];
	//	int low = 0;
	//	int high = i - 1;
	//	while (low <= high)
	//	{
	//		mid = (low + high) / 2;
	//		if (a[mid] > temp)
	//			high = mid - 1;
	//		else
	//			low = mid + 1;
	//	}
	//	for (j = i - 1; j >= low; j--)
	//	{
	//		a[j + 1] = a[j];
	//	}
	//	a[low] = temp;
	//}


	//for (int i = 0; i < 4; i++)
	//	cout << a[i];




	//int a[] = { 0,1,2,3,4,5,6,7,8,9 }, k;
	//int low = 0, high = 10 - 1, mid, find = 0;
	//printf("请输入欲查找的值：\n");
	//scanf_s("%d", &k);
	//while (low <= high)
	//{
	//	mid = (low + high) / 2;
	//	if (a[mid] == k)
	//	{
	//		printf("找到位置为：%d\n", mid + 1); find = 1;
	//	}
	//	if (a[mid] > k)
	//		high = mid - 1;
	//	else
	//		low = mid + 1;
	//}
	//if (!find) printf(" % d未找到\n", k);




	//int ddd =StartPrograme("cmd");



	Example e;
	//启动线程;
	e.Start();

	getchar();
	return 0;




    system("pause");
}

