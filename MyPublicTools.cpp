// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

class XMLManager
{
public:

	XMLManager(const char* xmlPath)
	{
		InitializeSRWLock(&g_srwLock);//初始化文件锁
		InitializeCriticalSection(&g_cs);//初始化临界区
		m_xmlPath = xmlPath;
        //ofstream outfile;
        //outfile.open(m_xmlPath);
	}

	~XMLManager()
	{

		DeleteCriticalSection(&g_cs);//删除临界区
		ReleaseSRWLockExclusive(&g_srwLock);//释放写文件锁
		ReleaseSRWLockShared(&g_srwLock);//释放读文件锁
		delete this;
	}

	void ReadLock()
	{
		//读者申请读取文件
		AcquireSRWLockShared(&g_srwLock);//请求读文件锁
		EnterCriticalSection(&g_cs);//创建临界区
	}
	void ReadUnlock()
	{
		//读者结束读取文件
		ReleaseSRWLockShared(&g_srwLock);//释放读文件锁
		LeaveCriticalSection(&g_cs);//释放临界区
	}
	void WriteLock()
	{
		AcquireSRWLockExclusive(&g_srwLock);//请求写文件锁
		EnterCriticalSection(&g_cs);//创建临界区
	}
	void WriteUnlock()
	{
		ReleaseSRWLockExclusive(&g_srwLock);//释放写文件锁
		LeaveCriticalSection(&g_cs);//释放临界区
	}


	//XMLDocument* LoadXMLFile(const char* xmlPath);




private:

	CRITICAL_SECTION g_cs;
	SRWLOCK          g_srwLock;
	const char* m_xmlPath;
};



//class xiancheng1 : public Thread
//{
//public:
//    XMLManager* suo;
//    //重写Run;
//    void Run()
//    {
//        while (this->bRun)
//        {
//            suo = new XMLManager("卧槽.xml");
//            suo->ReadLock();
//            cout << "线程1" << endl;
//            XMLDocument* abc = LoadXMLFile("卧槽.xml");
//            XMLElement* root = abc->RootElement();
//            XMLElement* ddd = NULL;
//            SaveXMLFile(abc, "卧槽.xml");
//            Sleep(5000);
//        }
//    }
//};
//
//class xiancheng2 : public Thread
//{
//public:
//    XMLManager *suo;
//	//重写Run;
//	void Run()
//	{
//		while (this->bRun)
//		{
//            suo = new XMLManager("卧槽.xml");
//            suo->ReadLock();
//            cout << "线程2" << endl;
//            XMLDocument* abc = LoadXMLFile("卧槽.xml");
//            XMLElement* root = abc->RootElement();
//            XMLElement* ddd=NULL;
//            SaveXMLFile(abc, "卧槽.xml");
//            Sleep(5000);
//		}
//	}
//};

//例子,在主函数中实例化并重写run()方法
class xiancheng1 : public Thread
{
public:
    Thread* thread;
	//重写Run;
    xiancheng1(Thread* thread)
    {
        this->thread = thread;
    }
	void Run()
	{
		while (this->bRun)
		{
            WaitAnotherThreadExit(thread);
			printf("Hello thread1\n");
			Sleep(1000);

		}
	}
};

//例子,在主函数中实例化并重写run()方法
class xiancheng2 : public Thread
{
public:
	//重写Run;
	void Run()
	{
		int i = 0;
		while (i<10)
		{
			printf("Hello thread2\n");
			Sleep(1000);
			i++;
		}
	}
};






int main()
{   //https://blog.csdn.net/qq_40945965/article/details/86831178
    //https://www.cnblogs.com/happykoukou/p/6307257.html
    //https://www.cnblogs.com/whwywzhj/p/8478628.html
    // 
    //LockFile


    //XMLDocument* doc = new XMLDocument();
    //int nReturnCode = doc->LoadFile("123.xml");
    //XMLElement* pRoot = iXmlDoc->NewElement("Ewaybot");


    //CreateEmptyXMLFile("卧槽.xml", "卧槽");

    //XMLDocument* abc = LoadXMLFile("卧槽.xml");
    //XMLElement* root = abc->RootElement();
    //XMLElement* ddd=NULL;
    //BOOL qwe= FindXMLNode(root, "std", ddd);
    //BOOL qwe = FindXMLNode(root, "std", ddd, "qqq", "3211");


    //const char*  ab22[] = { "class","qqq" };
    //const char*  ab23[] = { "123","321" };


    //BOOL qwee = FindXMLNode(root, "std", ddd, "class", "123");
    //BOOL qweew = FindXMLNode(root, "std", ddd, ab22, ab23);

    //const char* test = NULL;
    //GetXMLNodeText(root, "std", test, ab22, ab23);

    //BOOL qwee = FindXMLNode(root, "name", ddd);
    //GetXMLNodeText(ddd, test);
    //map<string, string> mapAttribute = {};

    //GetXMLNodeAttribute(ddd, mapAttribute);

    //const char* ab22[] = { "class","qqq" };
    //const char* ab23[] = { "123","321" };
    //BOOL qwed = SetXMLNodeText(abc, "卧槽.xml", root, "ee","name", ab22, ab23);
    //BOOL qwed = SetXMLNodeText(abc, "卧槽.xml", root, "wwwwwwwwwwwwww","name");

    //abc->Print();

    //LoadXMLFile("卧槽.xml");


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
    //userNode->SetText("我v");

    //root->InsertEndChild(userNode);

    //doc->SaveFile("1111 - 副本.xml");



    //XMLElement* abc = queryUserNodeByName(root, "111");
    //const char* aaa=abc->GetText();




    //Thread* ww = new xiancheng2();;
    //xiancheng1 e(ww);
    //e.Start();
    //ww->Start();



    XMLDocument *doc = LoadXMLFile("卧槽.xml");
    XMLElement* root = doc->RootElement();
    ////XMLElement* userNode = doc->NewElement("User");

    XMLElement* ddd=NULL;
    XMLElement* eee = NULL;
    BOOL qwe= FindXMLNode(root, "std", eee);


    //map<string, string> abc = {};
    //auto pr = std::make_pair("TT", "");
    //abc.insert(pr);
    //SetXMLNodeAttribution(doc, "卧槽.xml", ddd, abc);

    //SetXMLNewNode(doc, "卧槽.xml", ddd, "我干", abc, "这是个新节点");



    map<const char*, const char*> adc = { {"class","123"}};
   auto pr = std::make_pair("qqq", "321");
   adc.insert(pr);

   FindXMLNode1(root, "std",ddd,adc);
   ddd;





    system("pause");
}

