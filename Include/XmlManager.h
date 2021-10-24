//XML����
#pragma once
#include <tinyxml2.h>//XML����һ�η�װ
using namespace tinyxml2;
#define XMLDocument tinyxml2::XMLDocument
class XmlManager
{
public:

	XmlManager(const char* xmlPath);
	~XmlManager();
	XMLDocument* m_pDoc;
	XMLElement* m_pRoot;
	const char* m_xmlPath;


	//17.����һ����ֻ�и��ڵ��xml�ļ�,���Զ�����ڵ�����,���ش�����xml��XMLDocument*ָ��,���ָ��������������Դ�����xml���ж�д����
	//����1:Ҫ������xml��·��
	//����2:���ڵ�����
	XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName);


	//18.����xml�ļ�,����xml��XMLDocument*ָ��,���ָ��������������Դ�����xml���ж�д����
	//����:Ҫ���ص�xml��·��
	XMLDocument* LoadXMLFile(const char* xmlPath);

	BOOL SetXMLPath(const char* newXmlPath);

	//19.����xml����,�˷�����ͬ��SaveFile()֮�����ڴ˴��������ɹ�,���ͷ�docָ�벢����TRUE,������ʧ��,�򷵻�FALSE����ʱ���ͷ�docָ��
	BOOL SaveXMLFile(XMLDocument* doc = NULL, const char* xmlSavePath = NULL);

	//20.��ȡXML����
	BOOL GetXMLDeclaration(string& strDecl, XMLDocument* doc = NULL);


	//21.����"�ڵ������"����"�ڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"��Ѱ�ҽڵ�(��������Զ����Ե�node���о�ȷ��λ,ע�����4�Ͳ���5��һһ��Ӧ��ϵ)(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4)
	//����2 ��Ҫ��ѯ�Ľڵ�����
	//����3 �����ѯ���˷��صĽڵ�ĵ�ַ,û�鵽��ԭ���õĵ�ַ����
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<const char*, const char*> adc = { {"class","15��"}}; (Ҳ���Բ���)
	BOOL FindXMLNode(XMLElement*& pNode, const string nodeName, map<const char*, const char*> Attribution = {}, XMLElement* pRoot = NULL);


	//22.����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ýڵ��ı�(��������Զ����Ե�node���о�ȷ��λ,ע�����4�Ͳ���5��һһ��Ӧ��ϵ)(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4)
	//����2 �����ѯ���˷��ص����ֵ�const char* ָ��,û�鵽��ԭ���õĵ�ַ����
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ���ı�)
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<const char*, const char*> adc = { {"class","15��"}}; (Ҳ���Բ���)
	BOOL GetXMLNodeText(const char*& text, XMLElement* pRoot, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//23.����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ڵ�����(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4)
	//����2 �����ѯ���˷������Ե�map<string, string>,û�鵽��ԭ���õ����ݲ���
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ������)(�������ѭһ��xml��ͬ���ڵ������������ͬ�Ľṹ������xml,��Ϊ�˷���ֻ�᷵�ز��ҵ��ĵ�һ��ͬ���ڵ����������ֵ,��������ȡĳЩͬ���ڵ�����Թ��ɵĻ�)
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<const char*, const char*> adc = { {"class","15��"}}; (Ҳ���Բ���)
	BOOL GetXMLNodeAttribute(map<string, string>& mapAttribute, XMLElement* pRoot, const string nodeName = "", map<const char*, const char*> Attribution = {});


	//24.����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",���½ڵ���ı�(û���ı������,���ı������)
	BOOL SetXMLNodeText(const string text, XMLElement* pRoot, const string nodeName = "", map<const char*, const char*> Attribution = {},XMLDocument * doc = NULL, const char* xmlSavePath= NULL);

	BOOL RenameNode(XMLElement* pRoot, const char* newNodeName, XMLDocument* doc = NULL, const char* xmlSavePath = NULL);

	//25.����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",���½ڵ�����Ե�ֵ(û�е����Ի����,�Ѿ��е����Ի����)
	BOOL SetXMLNodeAttribution(map<string, string>& mapAttribute, XMLElement* pRoot, const string nodeName = "", map<const char*, const char*> Attribution = {}, XMLDocument* doc = NULL, const char* xmlSavePath= NULL);


	//26.����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",����½ڵ�
	BOOL SetXMLNewNode(XMLElement* pFatherRoot, const char* newNodeName, map<string, string>newMapAttribute = {}, const char* newText = "", const string nodeName = "", map<const char*, const char*> Attribution = {}, XMLDocument* doc = NULL, const char* xmlSavePath = NULL);


	//27.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",ɾ�����ڵ��µ����нڵ�
	BOOL DeleteXMLNode(XMLElement* pRoot, const string nodeName = "", map<const char*, const char*> Attribution = {}, XMLDocument* doc = NULL, const char* xmlSavePath = NULL);


	//27.2ɾ�����ڵ��µ�ָ���ӽڵ�
	BOOL DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode);
};