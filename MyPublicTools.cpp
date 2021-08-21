﻿// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"




XMLElement* queryUserNodeByName(XMLElement* root, const string& userName)
{

    XMLElement* userNode = root->FirstChildElement();
    while (userNode != NULL)
    {
        try
        {
            if (userNode->Attribute("T") == userName)
                break;
            throw "Unknown Exception";
        }
        catch(...)
        {
            cout << "异常了";
        }
        
        userNode = userNode->NextSiblingElement();//下一个兄弟节点
    }
    return userNode;
}







int main()
{   //https://www.cnblogs.com/happykoukou/p/6307257.html
    //XMLDocument* doc = new XMLDocument();
    //int nReturnCode = doc->LoadFile("123.xml");
    //XMLElement* pRoot = iXmlDoc->NewElement("Ewaybot");


    CreateEmptyXMLFile("卧槽.xml", "卧槽");

    XMLDocument* abc = LoadXMLFile("卧槽.xml");
    abc->Print();



    //添加申明可以使用如下两行
    //XMLDeclaration* declaration=doc.NewDeclaration();
    //doc.InsertFirstChild(declaration);

 







    //XMLElement* root = abc->RootElement();//设置根节点 每个xml只有一个根节点


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

    //LPCTSTR abc = L"123";
    //LPCTSTR abcd = L"456";
    //LPCTSTR abcde = CombineLPCTSRT(abc ,abcd);


    //XMLDocument *doc = LoadXMLFile("stu_info.xml");
    //XMLElement* root = doc->RootElement();
    //XMLElement* userNode = doc->NewElement("User");
    //userNode->SetAttribute("dd", "qq");
    //userNode->SetText("我v操");

    //root->InsertEndChild(userNode);

    //doc->SaveFile("1111 - 副本.xml");



    //XMLElement* abc = queryUserNodeByName(root, "111");
    //const char* aaa=abc->GetText();



    system("pause");
}

