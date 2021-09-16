// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include <iostream>



int main()
{   //https://blog.csdn.net/qq_40945965/article/details/86831178
    //https://www.cnblogs.com/happykoukou/p/6307257.html
    //https://www.cnblogs.com/whwywzhj/p/8478628.html


    CHttpDownload abc;
    abc.DownloadToFile(L"https://tools.skylarqa.com/test/yuanchunming/csv数据处理.zip?download=true",L"C:\\Users\\yuanchunming01\\Desktop\\MyPublicTools\\123.html");
}