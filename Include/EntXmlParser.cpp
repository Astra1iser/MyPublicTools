#include "stdafx.h"
#include "EntXmlParser.h"

#pragma comment(lib, "comsuppw.lib")

CEntXmlNode CEntXmlNode::GetParentNode()
{
	if(!IsValid())
		return INVALID_XML_NODE;

	CEntXmlNode parent;
	try
	{
		if(FAILED(m_pNode->get_parentNode(&(parent.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}

	return parent;
}

CEntXmlNode CEntXmlNode::GetChildNode(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return INVALID_XML_NODE;

	CEntXmlNode node;
	try
	{
		if(FAILED(m_pNode->selectSingleNode(strNodePath,&(node.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}

	return node;
}

CEntXmlNodeList CEntXmlNode::GetChildNodeList(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return INVALID_XML_NODELIST;

	CEntXmlNodeList nodeList;
	try
	{
		if(FAILED(m_pNode->selectNodes(strNodePath,&(nodeList.m_pNodeList))))
			return INVALID_XML_NODELIST;
	}
	catch(...)
	{
		return INVALID_XML_NODELIST;
	}

	return nodeList;
}

CEntXmlNamedNodeMap CEntXmlNode::GetAttributeList()
{
	if(!IsValid())
		return INVALID_XML_NAMED_NODE_MAP;

	CEntXmlNamedNodeMap attrList;
	try
	{
		if(FAILED(m_pNode->get_attributes(&(attrList.m_pNodeList))))
			return INVALID_XML_NAMED_NODE_MAP;
	}
	catch(...)
	{
		return INVALID_XML_NAMED_NODE_MAP;
	}

	return attrList;
}

HRESULT CEntXmlNode::AppendChild(CEntXmlNode nodeToInsert)
{
	if(!IsValid() || !nodeToInsert.IsValid())
		return E_INVALIDARG;

	HRESULT hr = E_FAIL;
	try
	{
		hr = m_pNode->appendChild(nodeToInsert.m_pNode,NULL);
	}
	catch(...)
	{
		
	}

	return hr;
}

HRESULT CEntXmlNode::InsertChild(CEntXmlNode nodeToInsert,const _bstr_t& strInsertBefore)
{
	if(!IsValid() || !nodeToInsert.IsValid())
		return E_INVALIDARG;

	if(strInsertBefore.length() != 0)
	{
		CEntXmlNode nodeToInsertBefore = GetChildNode(strInsertBefore);
		if(!nodeToInsertBefore.IsValid())
			return E_INVALIDARG;

		HRESULT hr = E_FAIL;
		try
		{
			hr = m_pNode->insertBefore(nodeToInsert.m_pNode,(_variant_t)(IDispatch*)nodeToInsertBefore.m_pNode,NULL);
		}
		catch(...)
		{

		}

		return hr;
	}
	else
		return AppendChild(nodeToInsert);
}

HRESULT CEntXmlNode::RemoveChild(CEntXmlNode nodeToRemove)
{
	if(!IsValid() || !nodeToRemove)
		return E_INVALIDARG;

	HRESULT hr = E_FAIL;
	try
	{
		hr = m_pNode->removeChild(nodeToRemove.m_pNode,NULL);
	}
	catch(...)
	{

	}

	return hr;
}

HRESULT CEntXmlNode::RemoveChild(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return E_INVALIDARG;

	CEntXmlNode node = GetChildNode(strNodePath);
	if(!node)
		return E_INVALIDARG;

	HRESULT hr = E_FAIL;
	
	try
	{
		hr = m_pNode->removeChild(node.m_pNode,NULL);
	}
	catch(...)
	{

	}

	return hr;
}

BOOL CEntXmlNode::GetAttributeVal(const _bstr_t& strAttrName,_variant_t& val)
{
	if(!IsValid())
		return FALSE;

	try
	{
		CComPtr<IXMLDOMNamedNodeMap> pAttri;
		if(FAILED(m_pNode->get_attributes(&pAttri)) || !pAttri)
			return FALSE;

		CComPtr<IXMLDOMNode> pAttrNode;
		if(FAILED(pAttri->getNamedItem(strAttrName,&pAttrNode)) || !pAttrNode)
			return FALSE;

		if(FAILED(pAttrNode->get_nodeValue(&val)))
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CEntXmlNode::GetAttributeVal(const _bstr_t& strAttrName,_bstr_t& val)
{
	if(!IsValid())
		return FALSE;

	_variant_t valTmp;
	if(!GetAttributeVal(strAttrName,valTmp))
		return FALSE;

	if(valTmp.vt != VT_BSTR)
		return FALSE;

	val = valTmp.bstrVal;
	return TRUE;
}

BOOL CEntXmlNode::GetAttributeVal(const _bstr_t& strAttrName,int& val)
{
	if(!IsValid())
		return FALSE;

	_variant_t valTmp;
	if(!GetAttributeVal(strAttrName,valTmp))
		return FALSE;

	if(valTmp.vt != VT_BSTR)
		return FALSE;

	val = _ttoi((wchar_t*)(valTmp.bstrVal));
	return TRUE;
}

BOOL CEntXmlNode::SetAttributeVal(const _bstr_t& strAttrName,_bstr_t& val)
{
	if(!IsValid())
		return FALSE;

	try
	{
		CComPtr<IXMLDOMNamedNodeMap> pAttri;
		if(FAILED(m_pNode->get_attributes(&pAttri)) || !pAttri)
			return FALSE;

		CComPtr<IXMLDOMNode> pAttrNode;
		if(FAILED(pAttri->getNamedItem(strAttrName,&pAttrNode)) || !pAttrNode)
			return FALSE;

		if(FAILED(pAttrNode->put_nodeValue(_variant_t(val))))
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CEntXmlNode::GetNodeName(_bstr_t& val)
{
	if(!IsValid())
		return FALSE;

	BSTR tempVal;
	try
	{
		if(FAILED(m_pNode->get_nodeName(&tempVal)))
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}

	val = tempVal;
	return TRUE;
}

BOOL CEntXmlNode::GetNodeVal(_bstr_t& val)
{
	if(!IsValid())
		return FALSE;

	BSTR tempVal;
	try
	{
		if(FAILED(m_pNode->get_text(&tempVal)))
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}
	val = tempVal;
	return TRUE;
}

BOOL CEntXmlNode::GetNodeVal(int& val)
{
	if(!IsValid())
		return FALSE;

	_bstr_t valTmp;
	if(!GetNodeVal(valTmp))
		return FALSE;

	val = _ttoi((wchar_t*)(valTmp));
	return TRUE;
}

BOOL CEntXmlNode::SetNodeVal(_bstr_t& val)
{
	if(!IsValid())
		return FALSE;

	try
	{
		//if(FAILED(m_pNode->put_nodeValue(_variant_t(val))))
		if(FAILED(m_pNode->put_text(val)))
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
long CEntXmlNodeList::GetItemCount()
{
	if(!IsValid())
		return 0;

	if(m_lItemCount)
		return m_lItemCount;

	try
	{
		if(FAILED(m_pNodeList->get_length(&m_lItemCount)))
			m_lItemCount = 0;
	}
	catch(...)
	{
		m_lItemCount = 0;
	}

	return m_lItemCount;
}

CEntXmlNode CEntXmlNodeList::GetItem(int nIndex)
{
	if(!IsValid() || nIndex < 0 || nIndex >= GetItemCount())
		return INVALID_XML_NODE;

	CEntXmlNode node;
	try
	{
		if(FAILED(m_pNodeList->get_item(nIndex,&(node.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}

	return node;
}

//////////////////////////////////////////////////////////////////////////

long CEntXmlNamedNodeMap::GetItemCount()
{
	if(!IsValid())
		return 0;

	if(m_lItemCount)
		return m_lItemCount;

	try
	{
		if(FAILED(m_pNodeList->get_length(&m_lItemCount)))
			m_lItemCount = 0;
	}
	catch(...)
	{
		m_lItemCount = 0;
	}

	return m_lItemCount;
}

CEntXmlNode CEntXmlNamedNodeMap::GetItem(int nIndex)
{
	if(!IsValid() || nIndex < 0 || nIndex >= GetItemCount())
		return INVALID_XML_NODE;

	CEntXmlNode node;
	try
	{
		if(FAILED(m_pNodeList->get_item(nIndex,&(node.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}

	return node;
}

//////////////////////////////////////////////////////////////////////////
bool CEntXmlDocument::_Init()
{
	try
	{
		if(!m_pDoc && FAILED(CreateXMLDOMDocument(&m_pDoc)))
		{
			m_pDoc = NULL;
		}
	}
	catch(...)
	{
		m_pDoc = NULL;
	}

	return m_pDoc != NULL;	
}

bool CEntXmlDocument::IsValid()
{
	return _Init();
}

HRESULT CEntXmlDocument::LoadFile(const _bstr_t& strFilename)
{
	if(!IsValid())
		return E_FAIL;

	VARIANT_BOOL val = VARIANT_FALSE;
	HRESULT hRet = E_FAIL;
	try
	{
		hRet = m_pDoc->load(_variant_t(strFilename),&val);
	}
	catch(...)
	{
		hRet = E_FAIL;
	}

	return (val == VARIANT_FALSE) ? E_FAIL : hRet;
}

HRESULT CEntXmlDocument::LoadXml(const _bstr_t& strXml)
{
	if(!IsValid())
		return E_FAIL;

	VARIANT_BOOL val = VARIANT_FALSE;
	HRESULT hRet = E_FAIL;
	
	try
	{
		hRet = m_pDoc->loadXML(strXml,&val);
	}
	catch(...)
	{
		hRet = E_FAIL;
	}

	return (val == VARIANT_FALSE) ? E_FAIL : hRet;
}

HRESULT CEntXmlDocument::SaveToFile(const _bstr_t& strFilename)
{
	if(!IsValid())
		return E_FAIL;

	HRESULT hr = E_FAIL;
	try
	{
		hr = m_pDoc->save(_variant_t(strFilename));
	}
	catch(...)
	{
		hr = E_FAIL;
	}

	return hr;
}

HRESULT CEntXmlDocument::CreateNode(const _bstr_t& strNodename,CEntXmlNode& node)
{

	HRESULT hr = E_FAIL;
	try
	{
		CComPtr<IXMLDOMNode> spNewNode;
		hr = m_pDoc->createNode(CComVariant(NODE_ELEMENT),strNodename,NULL,&spNewNode);
		node.m_pNode = spNewNode ;
	}
	catch(...)
	{
		hr = E_FAIL;
	}

	return hr;
}

CEntXmlNode CEntXmlDocument::CloneNode(const CEntXmlNode& nodeToClone,BOOL bDeep /* = TRUE */)
{
	if(!IsValid() || !nodeToClone.IsValid())
		return INVALID_XML_NODE;

	CEntXmlNode nodeClone;
	VARIANT_BOOL valDeep = bDeep ? VARIANT_TRUE : VARIANT_FALSE;
	try
	{
		if(FAILED(nodeToClone.m_pNode->cloneNode(valDeep,&(nodeClone.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}
	return nodeClone;
}

CEntXmlNode CEntXmlDocument::GetChildNode(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return INVALID_XML_NODE;

	CEntXmlNode node;
	try
	{
		if(FAILED(m_pDoc->selectSingleNode(strNodePath,&(node.m_pNode))))
			return INVALID_XML_NODE;
	}
	catch(...)
	{
		return INVALID_XML_NODE;
	}

	return node;
}

CEntXmlNodeList CEntXmlDocument::GetChildNodeList(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return INVALID_XML_NODELIST;

	CEntXmlNodeList nodeList;
	try
	{
		if(FAILED(m_pDoc->selectNodes(strNodePath,&(nodeList.m_pNodeList))))
			return INVALID_XML_NODELIST;
	}
	catch(...)
	{
		return INVALID_XML_NODELIST;
	}

	return nodeList;
}

HRESULT CEntXmlDocument::RemoveChildNode(const _bstr_t& strNodePath)
{
	if(!IsValid())
		return E_FAIL;

	CEntXmlNode nodeToRemove = GetChildNode(strNodePath);
	if(!nodeToRemove)
		return E_FAIL;
	
	CEntXmlNode parent = nodeToRemove.GetParentNode();
	if(parent)
		return parent.RemoveChild(nodeToRemove);
	else
	{
		HRESULT hr = E_FAIL;
		try
		{
			hr = m_pDoc->removeChild(nodeToRemove.m_pNode,NULL);
		}
		catch(...)
		{
			hr = E_FAIL;
		}

		return hr;
	}
}

HRESULT CEntXmlDocument::CreateXMLDOMDocument(IXMLDOMDocument** ppDocument)
{
	static int s_iLastIndex	= -1;
	IXMLDOMDocument* pDoc = NULL;
	if(s_iLastIndex >= 0 && s_iLastIndex < sizeof(MSXML_CLSID) / sizeof(MSXML_CLSID[0]))
	{
		try
		{
			if(SUCCEEDED(::CoCreateInstance(MSXML_CLSID[s_iLastIndex], NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pDoc)))
			{
				*ppDocument = pDoc;
				return S_OK;
			}
		}
		catch (...)
		{
			pDoc = NULL;
		}
	}
	else
	{
		for(int i = 0; i < sizeof(MSXML_CLSID) / sizeof(MSXML_CLSID[0]); i++)
		{
			try
			{
				if(SUCCEEDED(::CoCreateInstance(MSXML_CLSID[i], NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pDoc)))
				{
					*ppDocument = pDoc;
					s_iLastIndex = i;
					return S_OK;
				}
			}
			catch(...)
			{
				pDoc = NULL;
			}
		}
	}

	for (int iTry = sizeof(MSXML_PATH) / sizeof(MSXML_PATH[0]) - 1; iTry >= 0; iTry--)
	{
		WCHAR szPath[MAX_PATH * 2];
		DWORD dwRet = ExpandEnvironmentStrings(MSXML_PATH[iTry], szPath, sizeof(szPath));

		if (dwRet == 0 || dwRet > sizeof(szPath))
			continue;

		HMODULE hMod = LoadLibrary_api(szPath);
		if (hMod == NULL)
			continue;

		typedef HRESULT (__stdcall* defDllRegisterServer)(void);
		defDllRegisterServer pDllRegisterServer	= (defDllRegisterServer)GetProcAddress(hMod,"DllRegisterServer");
		if (pDllRegisterServer == NULL)
		{
			FreeLibrary(hMod);
			continue;
		}

		try
		{
			if (FAILED(pDllRegisterServer()))
			{
				FreeLibrary(hMod);
				continue;
			}
		}
		catch(...)
		{
			FreeLibrary(hMod);
			continue;
		}
		FreeLibrary(hMod);

		pDoc = NULL;
		for (int i = sizeof(MSXML_CLSID) / sizeof(MSXML_CLSID[0]) - 1; i >= 0; i--)
		{
			try
			{
				HRESULT hr = ::CoCreateInstance(MSXML_CLSID[i], NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pDoc);
				if (FAILED(hr))
				{
					pDoc = NULL;
					continue;
				}

				//检测一下是否能够正常解析 xml
				VARIANT_BOOL vRet;
				std::wstring strTest = L"<doc><test>test</test></doc>";
				hr = pDoc->loadXML((LPTSTR)strTest.c_str(), &vRet);
				if (FAILED(hr) || vRet == VARIANT_FALSE)
				{
					pDoc->Release();
					pDoc = NULL;
					continue;
				}

				s_iLastIndex = i;
				*ppDocument = pDoc;
				return S_OK;
			}
			catch(...)
			{
				pDoc = NULL;
			}
		}
	}
	
	return E_FAIL;
}