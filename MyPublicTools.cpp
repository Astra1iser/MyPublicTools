// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"







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


    cout << IsAdmin();

    system("pause");
}

