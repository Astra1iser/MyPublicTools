//XML解析
#pragma once
#include <tinyxml2.h>//XML操作一次封装
using namespace tinyxml2;
#define XMLDocument tinyxml2::XMLDocument
class XmlManager
{

private:
	//xml文件指针
	XMLDocument* m_pDoc;
	//根节点指针
	XMLElement* m_pRoot;
	//文件路径
	string m_xmlPath;

	/*
	辅助函数
	刷新当前加载的文件指针和根节点指针
	*/
	BOOL PreaseInfo();


	/*
	辅助函数
	载入xml文件, 返回xml的XMLDocument* 指针, 这个指针可以用来继续对创建的xml进行读写操作
	参数:要加载的xml的路径
	*/
	XMLDocument* LoadXMLFile(string xmlPath);




public:

	XmlManager(string xmlPath);
	~XmlManager();

	/*
	创建一个且只有根节点的xml文件,可自定义根节点名称,返回创建的xml的XMLDocument*指针,
	这个指针可以用来继续对创建的xml进行读写操作,但我建议拿文件路径来重新实例化XmlManager类
	参数1:要创建的xml的路径
	参数2:根节点名称
	*/
	static XMLDocument* CreateEmptyXMLFile(string xmlPath, string rootNodeName);

	/*
	重新设置当前XmlManager对象加载的xml文件路径
	注意,在设置完后,请勿再使用之前得到的所有指针
	参数:要加载的新的xml的文件路径
	*/
	BOOL SetXMLPath(string newXmlPath);

	/*
	保存xml内容
	你可以选择手动保存,但不建议这么做
	*/
	BOOL SaveXMLFile(XMLDocument* doc = NULL, string xmlSavePath = "");

	/*
	读取XML声明
	参数1(出参):得到的字符
	参数2:没必要填
	*/
	BOOL GetXMLDeclaration(string& strDecl, XMLDocument* doc = NULL);

	/*
	根据"节点的名字"或者"节点的名字及其节点某些属性的值"来寻找节点
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的地址)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
	注意:对于既重名,属性也相同的那种节点,请填写第四个根节点参数代表其父节点
	参数1:如果查询到了返回的节点的地址,没查到则原引用的地址不变
	参数2:需要查询的节点名字
	参数3:要查询的节点的某些具体属性 例如: map<string, string> adc = { {"class","15班"}}; (也可以不填)
	参数4:需要查询的xml中某个节点(仅会遍历这个节点和其子节点)的指针
	*/
	BOOL FindXMLNode(XMLElement*& pNode, string nodeName, map<string, string> Attribution = {}, XMLElement* pRoot = NULL);

	/*
	根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取该节点文本
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
		4.节点指针
	参数1:如果查询到了则给参数1传值,没查到则传空
	参数2:需要查询的xml中某个节点(仅会遍历这个节点及子节点)的指针
	参数3:需要查询的节点名字(如果为空则只查询参数2的节点的文本)
	参数4:要查询的节点的某些具体属性 例如: map<const char*, const char*> adc = { {"class","15班"}}; (也可以不填)
	*/
	BOOL GetXMLNodeText(string& text, XMLElement* pRoot = NULL, string nodeName = "", map<string, string> Attribution = {});

	/*
	根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取节点属性
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
	参数1:如果查询到了返回属性的map<string, string>,没查到则原引用的内容不变
	参数2:需要查询的节点名字(如果为空则只查询参数1的节点的属性)
	参数3:要查询的节点的某些具体属性 例如: map<const char*, const char*> adc = { {"class","15班"}}; (也可以不填)
	参数4:需要查询的xml中某个节点(仅会遍历这个节点和子节点)的指针
	*/
	BOOL GetXMLNodeAttribute(map<string, string>& mapAttribute, string nodeName = "", map<string, string> Attribution = {}, XMLElement * pRoot = NULL);

	/*
	根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",更新节点的文本(没有文本会添加,有文本会更新)
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
		4.如果只有参数1和参数2且参数2不是NULL,则直接修改填入的节点指针指向的节点
	参数1:要设置(更新)的字符串
	参数2:节点指针,填入想要设置的节点或者其父节点,如果填NULL,则默认父节点是加载的xml的根节点,如果只有参数1和参数2且参数2不是NULL,则直接修改填入的节点指针指向的节点
	参数3:要修改的节点的名称,配合参数2来查找具体节点
	参数4:要修改的节点的属性,配合参数2来查找具体节点
	参数5:不需要改变
	参数6:不需要改变
	*/
	BOOL SetXMLNodeText(string text, XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {},XMLDocument * doc = NULL, string xmlSavePath= NULL);

	/*
	重命名节点
	参数1:节点的指针
	参数2:节点新名字
	*/
	BOOL RenameNode(XMLElement* pRoot, string newNodeName, XMLDocument* doc = NULL, string xmlSavePath = NULL);

	
	/*
	根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",更新节点的属性的值(没有的属性会添加,已经有的属性会更新)
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
		4.如果只有参数1和参数2且参数2不是NULL,则直接修改填入的节点指针指向的节点
	参数1:要设置(更新)的属性map
	参数2:节点指针,填入想要设置的节点或者其父节点,如果填NULL,则默认父节点是加载的xml的根节点,如果只有参数1和参数2且参数2不是NULL,则直接修改填入的节点指针指向的节点
	参数3:要修改的节点的名称,配合参数2来查找具体节点
	参数4:要修改的节点的属性,配合参数2来查找具体节点
	参数5:不需要改变
	参数6:不需要改变
	*/
	BOOL SetXMLNodeAttribution(map<string, string>& mapAttribute, XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath= NULL);


	/*
	根据"父节点指针"或者"父节点指针和节点名"或者"父节点指针和节点的名字及其节点某些属性的值",添加新子节点
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
		4.如果只有参数1和参数2且参数1不是NULL,则直接修改填入的节点指针指向的节点
	参数1:要添加到哪个父节点下,如果填NULL,则默认父节点是加载的xml的根节点(要更精确定位需要配合参数5,6),如果只有参数1和参数2且参数2不是NULL,则直接修改填入的节点指针指向的节点
	参数2:新节点名
	参数3:新节点属性
	参数4:新节点文本
	参数5:节点名,配合参数1用来作为定位条件
	参数6:节点属性,配合参数1用来作为定位条件
	参数7:不需要改变
	参数8,不需要改变
	*/
	BOOL SetXMLNewNode(XMLElement* pFatherRoot, string newNodeName, map<string, string>newMapAttribute = {}, string newText = "", string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath = NULL);

	/*
	27.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",删除父节点下的所有节点
	你可以选择如下搭配来寻找节点:
		1.节点名(若有重名节点则只会返回第一个节点的数据)
		2.节点名及节点属性
		3.节点名及节点属性及父节点指针
		4.如果只有参数1和参数2且参数1不是NULL,则直接修改填入的节点指针指向的节点
	参数1:要删除的节点的指针
	参数2:节点名,配合参数1用来作为定位条件
	参数3:节点属性,配合参数1用来作为定位条件
	参数4:不需要改变
	参数5:不需要改变
	*/
	BOOL DeleteXMLNode(XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath = NULL);


	/*
	删除父节点下的指定子节点
	参数1:父节点指针
	参数2:父节点下的子节点的指针
	*/
	BOOL DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode);
};