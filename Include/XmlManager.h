//XML解析
#pragma once
#include <tinyxml2.h>//XML操作一次封装
using namespace tinyxml2;
#define XMLDocument tinyxml2::XMLDocument
class XmlManager
{
public:

	XmlManager();
	~XmlManager();

	//17.创建一个且只有根节点的xml文件,可自定义根节点名称,返回创建的xml的XMLDocument*指针,这个指针可以用来继续对创建的xml进行读写操作
	//参数1:要创建的xml的路径
	//参数2:根节点名称
	XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName);


	//18.载入xml文件,返回xml的XMLDocument*指针,这个指针可以用来继续对创建的xml进行读写操作
	//参数:要加载的xml的路径
	XMLDocument* LoadXMLFile(const char* xmlPath);


	//19.保存xml内容,此方法不同于SaveFile()之处在于此处如果保存成功,则释放doc指针并返回TRUE,若保存失败,则返回FALSE但此时不释放doc指针
	BOOL SaveXMLFile(XMLDocument* doc, const char* xmlSavePath);


	//20.读取XML声明
	BOOL GetXMLDeclaration(XMLDocument* doc, string& strDecl);


	//21.根据"节点的名字"或者"节点的名字及其节点某些属性的值"来寻找节点(此重载针对多属性的node进行精确定位,注意参数4和参数5有一一对应关系)(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4)
	//参数2 需要查询的节点名字
	//参数3 如果查询到了返回的节点的地址,没查到则原引用的地址不变
	//参数4 要查询的节点的某些具体属性 例如: map<const char*, const char*> adc = { {"class","15班"}}; (也可以不填)
	BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, map<const char*, const char*> Attribution = {});


	//22.根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取该节点文本(此重载针对多属性的node进行精确定位,注意参数4和参数5有一一对应关系)(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4)
	//参数2 如果查询到了返回的文字的const char* 指针,没查到则原引用的地址不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的文本)
	//参数4 要查询的节点的某些具体属性 例如: map<const char*, const char*> adc = { {"class","15班"}}; (也可以不填)
	BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//23.根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取节点属性(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4)
	//参数2 如果查询到了返回属性的map<string, string>,没查到则原引用的内容不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的属性)(请务必遵循一个xml中同名节点的属性名都相同的结构来构造xml,因为此方法只会返回查找到的第一个同名节点的属性名和值,如果你想获取某些同名节点的属性构成的话)
	//参数4 要查询的节点的某些具体属性 例如: map<const char*, const char*> adc = { {"class","15班"}}; (也可以不填)
	BOOL GetXMLNodeAttribute(XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//24.根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",更新节点的文本(没有文本会添加,有文本会更新)
	BOOL SetXMLNodeText(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string text, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//25.根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",更新节点的属性的值(没有的属性会添加,已经有的属性会更新)
	BOOL SetXMLNodeAttribution(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//26.根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",添加新节点
	BOOL SetXMLNewNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* newNodeName, map<string, string>newMapAttribute = {}, const char* newText = "", const string nodeName = "", map<const char*, const char*> Attribution = {});


	//27.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值",删除父节点下的所有节点
	BOOL DeleteXMLNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* delNodeName, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//27.2删除父节点下的指定子节点
	BOOL DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode);
};