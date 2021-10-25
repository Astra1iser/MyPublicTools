//XML����
#pragma once
#include <tinyxml2.h>//XML����һ�η�װ
using namespace tinyxml2;
#define XMLDocument tinyxml2::XMLDocument
class XmlManager
{

private:
	//xml�ļ�ָ��
	XMLDocument* m_pDoc;
	//���ڵ�ָ��
	XMLElement* m_pRoot;
	//�ļ�·��
	string m_xmlPath;

	/*
	��������
	ˢ�µ�ǰ���ص��ļ�ָ��͸��ڵ�ָ��
	*/
	BOOL PreaseInfo();


	/*
	��������
	����xml�ļ�, ����xml��XMLDocument* ָ��, ���ָ��������������Դ�����xml���ж�д����
	����:Ҫ���ص�xml��·��
	*/
	XMLDocument* LoadXMLFile(string xmlPath);




public:

	XmlManager(string xmlPath);
	~XmlManager();

	/*
	����һ����ֻ�и��ڵ��xml�ļ�,���Զ�����ڵ�����,���ش�����xml��XMLDocument*ָ��,
	���ָ��������������Դ�����xml���ж�д����,���ҽ������ļ�·��������ʵ����XmlManager��
	����1:Ҫ������xml��·��
	����2:���ڵ�����
	*/
	static XMLDocument* CreateEmptyXMLFile(string xmlPath, string rootNodeName);

	/*
	�������õ�ǰXmlManager������ص�xml�ļ�·��
	ע��,���������,������ʹ��֮ǰ�õ�������ָ��
	����:Ҫ���ص��µ�xml���ļ�·��
	*/
	BOOL SetXMLPath(string newXmlPath);

	/*
	����xml����
	�����ѡ���ֶ�����,����������ô��
	*/
	BOOL SaveXMLFile(XMLDocument* doc = NULL, string xmlSavePath = "");

	/*
	��ȡXML����
	����1(����):�õ����ַ�
	����2:û��Ҫ��
	*/
	BOOL GetXMLDeclaration(string& strDecl, XMLDocument* doc = NULL);

	/*
	����"�ڵ������"����"�ڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"��Ѱ�ҽڵ�
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ�ĵ�ַ)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
	ע��:���ڼ�����,����Ҳ��ͬ�����ֽڵ�,����д���ĸ����ڵ���������丸�ڵ�
	����1:�����ѯ���˷��صĽڵ�ĵ�ַ,û�鵽��ԭ���õĵ�ַ����
	����2:��Ҫ��ѯ�Ľڵ�����
	����3:Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<string, string> adc = { {"class","15��"}}; (Ҳ���Բ���)
	����4:��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ�����ӽڵ�)��ָ��
	*/
	BOOL FindXMLNode(XMLElement*& pNode, string nodeName, map<string, string> Attribution = {}, XMLElement* pRoot = NULL);

	/*
	����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ýڵ��ı�
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
		4.�ڵ�ָ��
	����1:�����ѯ�����������1��ֵ,û�鵽�򴫿�
	����2:��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ㼰�ӽڵ�)��ָ��
	����3:��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����2�Ľڵ���ı�)
	����4:Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<const char*, const char*> adc = { {"class","15��"}}; (Ҳ���Բ���)
	*/
	BOOL GetXMLNodeText(string& text, XMLElement* pRoot = NULL, string nodeName = "", map<string, string> Attribution = {});

	/*
	����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ڵ�����
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
	����1:�����ѯ���˷������Ե�map<string, string>,û�鵽��ԭ���õ����ݲ���
	����2:��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ������)
	����3:Ҫ��ѯ�Ľڵ��ĳЩ�������� ����: map<const char*, const char*> adc = { {"class","15��"}}; (Ҳ���Բ���)
	����4:��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ӽڵ�)��ָ��
	*/
	BOOL GetXMLNodeAttribute(map<string, string>& mapAttribute, string nodeName = "", map<string, string> Attribution = {}, XMLElement * pRoot = NULL);

	/*
	����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",���½ڵ���ı�(û���ı������,���ı������)
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
		4.���ֻ�в���1�Ͳ���2�Ҳ���2����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����1:Ҫ����(����)���ַ���
	����2:�ڵ�ָ��,������Ҫ���õĽڵ�����丸�ڵ�,�����NULL,��Ĭ�ϸ��ڵ��Ǽ��ص�xml�ĸ��ڵ�,���ֻ�в���1�Ͳ���2�Ҳ���2����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����3:Ҫ�޸ĵĽڵ������,��ϲ���2�����Ҿ���ڵ�
	����4:Ҫ�޸ĵĽڵ������,��ϲ���2�����Ҿ���ڵ�
	����5:����Ҫ�ı�
	����6:����Ҫ�ı�
	*/
	BOOL SetXMLNodeText(string text, XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {},XMLDocument * doc = NULL, string xmlSavePath= NULL);

	/*
	�������ڵ�
	����1:�ڵ��ָ��
	����2:�ڵ�������
	*/
	BOOL RenameNode(XMLElement* pRoot, string newNodeName, XMLDocument* doc = NULL, string xmlSavePath = NULL);

	
	/*
	����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",���½ڵ�����Ե�ֵ(û�е����Ի����,�Ѿ��е����Ի����)
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
		4.���ֻ�в���1�Ͳ���2�Ҳ���2����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����1:Ҫ����(����)������map
	����2:�ڵ�ָ��,������Ҫ���õĽڵ�����丸�ڵ�,�����NULL,��Ĭ�ϸ��ڵ��Ǽ��ص�xml�ĸ��ڵ�,���ֻ�в���1�Ͳ���2�Ҳ���2����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����3:Ҫ�޸ĵĽڵ������,��ϲ���2�����Ҿ���ڵ�
	����4:Ҫ�޸ĵĽڵ������,��ϲ���2�����Ҿ���ڵ�
	����5:����Ҫ�ı�
	����6:����Ҫ�ı�
	*/
	BOOL SetXMLNodeAttribution(map<string, string>& mapAttribute, XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath= NULL);


	/*
	����"���ڵ�ָ��"����"���ڵ�ָ��ͽڵ���"����"���ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",������ӽڵ�
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
		4.���ֻ�в���1�Ͳ���2�Ҳ���1����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����1:Ҫ��ӵ��ĸ����ڵ���,�����NULL,��Ĭ�ϸ��ڵ��Ǽ��ص�xml�ĸ��ڵ�(Ҫ����ȷ��λ��Ҫ��ϲ���5,6),���ֻ�в���1�Ͳ���2�Ҳ���2����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����2:�½ڵ���
	����3:�½ڵ�����
	����4:�½ڵ��ı�
	����5:�ڵ���,��ϲ���1������Ϊ��λ����
	����6:�ڵ�����,��ϲ���1������Ϊ��λ����
	����7:����Ҫ�ı�
	����8,����Ҫ�ı�
	*/
	BOOL SetXMLNewNode(XMLElement* pFatherRoot, string newNodeName, map<string, string>newMapAttribute = {}, string newText = "", string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath = NULL);

	/*
	27.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ",ɾ�����ڵ��µ����нڵ�
	�����ѡ�����´�����Ѱ�ҽڵ�:
		1.�ڵ���(���������ڵ���ֻ�᷵�ص�һ���ڵ������)
		2.�ڵ������ڵ�����
		3.�ڵ������ڵ����Լ����ڵ�ָ��
		4.���ֻ�в���1�Ͳ���2�Ҳ���1����NULL,��ֱ���޸�����Ľڵ�ָ��ָ��Ľڵ�
	����1:Ҫɾ���Ľڵ��ָ��
	����2:�ڵ���,��ϲ���1������Ϊ��λ����
	����3:�ڵ�����,��ϲ���1������Ϊ��λ����
	����4:����Ҫ�ı�
	����5:����Ҫ�ı�
	*/
	BOOL DeleteXMLNode(XMLElement* pRoot, string nodeName = "", map<string, string> Attribution = {}, XMLDocument* doc = NULL, string xmlSavePath = NULL);


	/*
	ɾ�����ڵ��µ�ָ���ӽڵ�
	����1:���ڵ�ָ��
	����2:���ڵ��µ��ӽڵ��ָ��
	*/
	BOOL DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode);
};