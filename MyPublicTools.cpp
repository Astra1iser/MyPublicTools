// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"




int createXML(const char* xmlPath)
{
    const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    XMLDocument doc;
    doc.Parse(declaration);//会覆盖xml所有内容

    //添加申明可以使用如下两行
    //XMLDeclaration* declaration=doc.NewDeclaration();
    //doc.InsertFirstChild(declaration);

    //XMLElement* root = doc.NewElement("DBUSER");
    //doc.InsertEndChild(root);

    return doc.SaveFile(xmlPath);
}



int main()
{   //https://www.cnblogs.com/happykoukou/p/6307257.html
    //XMLDocument* doc = new XMLDocument();
    //int nReturnCode = doc->LoadFile("123.xml");
    ////XMLElement* pRoot = iXmlDoc->NewElement("Ewaybot");


    ////createXML("123.xml");

    //XMLElement* root = doc->RootElement();//设置根节点 每个xml只有一个根节点

    //XMLElement* userNode = doc->NewElement("User1");
    //userNode->SetAttribute("Name", "111");
    //userNode->SetAttribute("Password ", "1111");
    //root->InsertEndChild(userNode);

    //XMLElement* userNode2 = doc->NewElement("User2");
    //userNode2->SetAttribute("Name", "222");
    //userNode2->SetAttribute("Password ", "2222");
    //root->InsertEndChild(userNode2);

    //doc->SaveFile("123.xml");
    //delete doc;


    

    //int a=DownLoadFile(L"https://baidu15641.com", L"123.bat");

    LPCTSTR abc = L"123";
    LPCTSTR abcd = L"456";
    LPCTSTR abcde = CombineLPCTSRT(abc ,abcd);




    system("pause");
}

