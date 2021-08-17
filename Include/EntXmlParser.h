#ifndef _ENT_XML_PARSER_H_
#define _ENT_XML_PARSER_H_

/*
	XML解析封装类

	使用方法：可参考gtest用例：skylar_gpub\test_case\src\utils\gtest_EntXmlParser
*/

#include <MsXml.h>
#include <comutil.h>

class CEntXmlNamedNodeMap;
class CEntXmlNodeList;
class CEntXmlNode
{
public:
	friend class CEntXmlDocument;
	friend class CEntXmlNodeList;
	friend class CEntXmlNamedNodeMap;

	CEntXmlNode() { m_pNode = NULL; }
	//CEntXmlNode(CComPtr<IXMLDOMNodeList> node) { m_pNode = node; }
	operator bool() { return IsValid(); }
	bool operator ! () { return !IsValid(); }
	bool IsValid() const{ return m_pNode.p != NULL; }

	CEntXmlNode GetChildNode(const _bstr_t& strNodePath);
	CEntXmlNode GetParentNode();
	CEntXmlNodeList GetChildNodeList(const _bstr_t& strNodePath);
	CEntXmlNamedNodeMap GetAttributeList();

	HRESULT AppendChild(CEntXmlNode nodeToInsert);
	//如果strInsertBefore为空，就等同于appendChild
	HRESULT InsertChild(CEntXmlNode nodeToInsert,const _bstr_t& strInsertBefore);
	HRESULT RemoveChild(CEntXmlNode nodeToRemove);
	HRESULT RemoveChild(const _bstr_t& strNodePath);

	BOOL GetAttributeVal(const _bstr_t& strAttrName,_variant_t& val);
	BOOL GetAttributeVal(const _bstr_t& strAttrName,_bstr_t& val);
	BOOL GetAttributeVal(const _bstr_t& strAttrName,int& val);
	BOOL SetAttributeVal(const _bstr_t& strAttrName,_bstr_t& val);

	BOOL GetNodeVal(_bstr_t& val);
	BOOL GetNodeVal(int& val);
	BOOL SetNodeVal(_bstr_t& val);
	BOOL GetNodeName(_bstr_t& val);

private:
	CComPtr<IXMLDOMNode> m_pNode;
};

class CEntXmlNodeList
{
public:
	friend class CEntXmlNode;
	friend class CEntXmlDocument;
	CEntXmlNodeList() {  m_pNodeList = NULL; m_lItemCount = 0;}

	operator bool() { return IsValid(); }
	bool operator ! () { return !IsValid(); }

	bool IsValid() const{ return  m_pNodeList.p != NULL; }
	long GetItemCount();
	CEntXmlNode GetItem(int nIndex);

private:
	long m_lItemCount;
	CComPtr<IXMLDOMNodeList> m_pNodeList;
};

// 属性列表
class CEntXmlNamedNodeMap
{
public:
	friend class CEntXmlNode;
	friend class CEntXmlDocument;
	CEntXmlNamedNodeMap() {  m_pNodeList = NULL; m_lItemCount = 0;}

	operator bool() { return IsValid(); }
	bool operator ! () { return !IsValid(); }

	bool IsValid() const{ return  m_pNodeList.p != NULL; }
	long GetItemCount();
	CEntXmlNode GetItem(int nIndex);

private:
	long m_lItemCount;
	CComPtr<IXMLDOMNamedNodeMap> m_pNodeList;
};

class CEntXmlDocument
{
public:
	CEntXmlDocument() { CoInitialize(NULL);  m_pDoc = NULL; _Init(); }
	~CEntXmlDocument() { m_pDoc.Release(); m_pDoc = NULL; CoUninitialize(); }

	operator bool() { return IsValid(); }
	bool operator ! () { return !IsValid(); }

	bool IsValid();
	HRESULT LoadFile(const _bstr_t& strFilename);	
	HRESULT LoadXml(const _bstr_t& strXml);	
	HRESULT SaveToFile(const _bstr_t& strFilename);
	HRESULT CreateNode(const _bstr_t& strNodename,CEntXmlNode& node);

	CEntXmlNode CloneNode(const CEntXmlNode& nodeToClone,BOOL bDeep = TRUE);

	//按层次路径获取xml节点，若相同路径有多个，返回第一个节点，支持XPATH格式的xml路径
	CEntXmlNode GetChildNode(const _bstr_t& strNodePath);			
	CEntXmlNodeList GetChildNodeList(const _bstr_t& strNodePath);	
	HRESULT RemoveChildNode(const _bstr_t& strNodePath);

	static HRESULT CreateXMLDOMDocument(IXMLDOMDocument** ppDocument);

protected:
	bool _Init();

private:
	CComPtr<IXMLDOMDocument> m_pDoc;
};


// 目前为了减少兼容性的问题，所以确定了只创建1.0版本的对象,所以把别的版本都注掉了
const CLSID MSXML_CLSID[]  = {
	{ 0x2933BF90, 0x7B36, 0x11D2, { 0xB2, 0x0E, 0x00, 0xC0, 0x4F, 0x98, 0x3E, 0x60 } }		// 1.0
	//{ 0xF6D90F11, 0x9C73, 0x11D3, { 0xB3, 0x2E, 0x00, 0xC0, 0x4F, 0x99, 0x0B, 0xB4 } },	// 2.0
	//{ 0xF5078F1B, 0xC551, 0x11D3, { 0x89, 0xB9, 0x00, 0x00, 0xF8, 0x1F, 0xE2, 0x21 } },	// 2.6
	//{ 0xF5078F32, 0xC551, 0x11D3, { 0x89, 0xB9, 0x00, 0x00, 0xF8, 0x1F, 0xE2, 0x21 } },	// 3.0
	//{ 0x88D969C0, 0xF192, 0x11D4, { 0xA6, 0x5F, 0x00, 0x40, 0x96, 0x32, 0x51, 0xE5 } },	// 4.0
	//{ 0x88D969E5, 0xF192, 0x11D4, { 0xA6, 0x5F, 0x00, 0x40, 0x96, 0x32, 0x51, 0xE5 } },	// 5.0
	//{ 0x88D96A05, 0xF192, 0x11D4, { 0xA6, 0x5F, 0x00, 0x40, 0x96, 0x32, 0x51, 0xE5 } }	// 6.0
};

const LPCWSTR MSXML_PATH[]   = {
	L"%SystemRoot%\\System32\\msxml.dll",
	L"%SystemRoot%\\System32\\msxml2.dll",
	L"%SystemRoot%\\System32\\msxml3.dll",
	L"%SystemRoot%\\System32\\msxml4.dll",
	L"%CommonProgramFiles%\\Microsoft Shared\\OFFICE11\\msxml5.dll",
	L"%SystemRoot%\\System32\\msxml6.dll",
};

const CEntXmlNode INVALID_XML_NODE; 
const CEntXmlNodeList INVALID_XML_NODELIST;
const CEntXmlNamedNodeMap INVALID_XML_NAMED_NODE_MAP;





#endif