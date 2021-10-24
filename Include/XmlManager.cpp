
#pragma once
#include <XMLManager.h>
#include <tinyxml2.cpp>


XmlManager::XmlManager(const char* xmlPath)
{
	m_xmlPath = xmlPath;

	m_pDoc = LoadXMLFile(m_xmlPath);
	if (NULL != m_pDoc)
	{
		m_pRoot = m_pDoc->RootElement();
	}
}

XmlManager::~XmlManager()
{

}

BOOL XmlManager::SetXMLPath(const char* newXmlPath)
{
	m_xmlPath = newXmlPath;
	m_pDoc = LoadXMLFile(m_xmlPath);
	if (!m_pDoc)
	{
		m_pRoot = m_pDoc->RootElement();
		if (!m_pRoot)
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

XMLDocument* XmlManager::CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName)
{
	const char* declaration = "<?xml version=\"1.0\" encoding=\"GBK\" standalone=\"yes\"?>";
	XMLDocument* doc = new XMLDocument;
	doc->Parse(declaration);//会覆盖xml所有内容

	//添加申明可以使用如下两行
	//XMLDeclaration* declaration=doc.NewDeclaration();
	//doc.InsertFirstChild(declaration);

	//XMLElement* rootNode = doc->NewElement(G2U(rootNodeName));
	XMLElement* rootNode = doc->NewElement(rootNodeName);
	doc->InsertEndChild(rootNode);

	if (!(doc->SaveFile(xmlPath)))
	{
		return NULL;
	}

	return doc;
}


XMLDocument* XmlManager::LoadXMLFile(const char* xmlPath)
{
	XMLDocument* doc = new XMLDocument;
	int res = doc->LoadFile(xmlPath);
	if (res != 0)
	{
		return NULL;
	}
	return doc;
}


BOOL XmlManager::SaveXMLFile(XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	if (NULL == xmlSavePath)
	{
		xmlSavePath = m_xmlPath;
	}

	if ((doc->SaveFile(xmlSavePath)))
	{
		return FALSE;
	}

	//delete doc;
	return TRUE;
}



BOOL XmlManager::GetXMLDeclaration(string& strDecl, XMLDocument* doc)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	XMLNode* decl = doc->FirstChild();
	if (NULL != decl)
	{
		XMLDeclaration* declaration = decl->ToDeclaration();
		if (NULL != declaration)
		{
			strDecl = declaration->Value();
			return true;
		}
	}
	strDecl = "";
	return FALSE;
}


BOOL XmlManager::FindXMLNode(XMLElement*& pNode, const string nodeName, map<const char*, const char*> Attribution, XMLElement* pRoot)
{
	if (NULL == pRoot)
	{
		pRoot = m_pRoot;
 		if (NULL == pRoot)
			return FALSE;
	}

	const char* value = pRoot->Value();
	if (strcmp(pRoot->Value(), nodeName.c_str()) == 0)
	{
		if (0 != Attribution.size())
		{
			int j = 0;
			for (auto it = Attribution.begin(); it != Attribution.end(); ++it)
			{
				string source = pRoot->Attribute(it->first);
				string target = it->second;
				if (source == target)
				{
					j++;
					if (j == Attribution.size())
					{
						pNode = pRoot;
						return TRUE;
					}
				}
			}
		}
		else
		{
			pNode = pRoot;
			return TRUE;
		}
	}

	XMLElement* p = pRoot;

	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		if (FindXMLNode(pNode, nodeName, Attribution, p))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL XmlManager::GetXMLNodeText(const char*& text, XMLElement* pRoot, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot && "" == nodeName)
		return FALSE;
	if (!pRoot)
	{
		pRoot = m_pRoot;
		if(!pRoot)
			return FALSE;
	}

	if ("" == nodeName)
	{
		text = pRoot->GetText();
		return TRUE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pRoot))
	{
		if (NULL != pNode)
		{
			text = pNode->GetText();
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}


BOOL XmlManager::GetXMLNodeAttribute(map<string, string>& mapAttribute, XMLElement* pRoot, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot && "" == nodeName)
		return FALSE;
	if (!pRoot)
	{
		pRoot = m_pRoot;
		if (!pRoot)
			return FALSE;
	}

	if ("" == nodeName)
	{
		const XMLAttribute* pAttr = NULL;
		for (pAttr = pRoot->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
		{
			string name = pAttr->Name();
			string value = pAttr->Value();
			mapAttribute.insert(make_pair(name, value));
		}
		return TRUE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pRoot))
	{
		if (NULL != pNode)
		{
			const XMLAttribute* pAttr = NULL;
			for (pAttr = pNode->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
			{
				string name = pAttr->Name();
				string value = pAttr->Value();
				mapAttribute.insert(make_pair(name, value));
			}
		}
		return true;
	}
	return false;
}


BOOL XmlManager::SetXMLNodeText(const string text, XMLElement* pRoot, const string nodeName, map<const char*, const char*> Attribution, XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	if (NULL == xmlSavePath)
	{
		xmlSavePath = m_xmlPath;
	}

	if (!pRoot && ""==nodeName)
		return FALSE;

	if (!pRoot)
	{
		pRoot = m_pRoot;
		if (!pRoot)
			return FALSE;
	}

	if ("" == nodeName)
	{
		try
		{

			XMLNode* pText = pRoot->LastChild();
			if (NULL != pText)
			{
				pText->SetValue(text.c_str());
			}
			else
			{
				//XMLText* pText = doc->NewText(text.c_str());
				//pRoot->InsertFirstChild(pText);
				pRoot->InsertNewText(text.c_str());
			}
		}
		catch (...)
		{
			//XMLText* pText = doc->NewText(text.c_str());
			//pRoot->InsertFirstChild(pText);
			pRoot->InsertNewText(text.c_str());
		}
		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pRoot))
	{
		try
		{
			XMLNode* pText = pNode->LastChild();
			if (NULL != pText)
			{
				pText->SetValue(text.c_str());
				//pNode->InsertAfterChild(pText, pText);
			}
			else
			{
				//XMLText* pText = doc->NewText(text.c_str());
				//pNode->InsertEndChild(pText);
				pNode->InsertNewText(text.c_str());
			}
		}
		catch (...)
		{
			/*XMLText* pText = doc->NewText(text.c_str());
			pNode->InsertFirstChild(pText);*/
			pNode->InsertNewText(text.c_str());
		}
		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL XmlManager::SetXMLNodeAttribution(map<string, string>& mapAttribute, XMLElement* pRoot, const string nodeName, map<const char*, const char*> Attribution, XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	if (NULL == xmlSavePath)
	{
		xmlSavePath = m_xmlPath;
	}

	if (!pRoot && "" == nodeName)
		return FALSE;

	if (!pRoot)
	{
		pRoot = m_pRoot;
		if (!pRoot)
			return FALSE;
	}

	if ("" == nodeName)
	{
		const XMLAttribute* pAttr = pRoot->FirstAttribute();

		if (NULL == pAttr)
		{
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if ("" != it->first.c_str())
				{
					if ("" != it->second.c_str())
					{
						pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}

		char* strName = NULL;
		for (; pAttr != NULL; pAttr = pAttr->Next())
		{
			strName = const_cast<char*>(pAttr->Name());
			map<string, string> isnofind = {};
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if (strName == it->first)
				{
					if ("" != it->second.c_str())
					{
						pRoot->SetAttribute(strName, it->second.c_str());
					}
					else
					{
						pRoot->DeleteAttribute(strName);
					}
				}
				else
				{
					auto pr = std::make_pair(it->first, it->second);
					isnofind.insert(pr);
				}

			}

			if (0 != isnofind.size())
			{
				for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
				{

					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pRoot))
	{
		const XMLAttribute* pAttr = pNode->FirstAttribute();

		if (NULL == pAttr)
		{
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if ("" != it->first.c_str())
				{
					if ("" != it->second.c_str())
					{
						pNode->SetAttribute(it->first.c_str(), it->second.c_str());
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}

		char* strName = NULL;
		for (; pAttr != NULL; pAttr = pAttr->Next())
		{
			strName = const_cast<char*>(pAttr->Name());
			map<string, string> isnofind = {};
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if (strName == it->first)
				{
					if ("" != it->second.c_str())
					{
						pNode->SetAttribute(strName, it->second.c_str());
					}
					else
					{
						pNode->DeleteAttribute(strName);
					}
				}
				else
				{
					auto pr = std::make_pair(it->first, it->second);
					isnofind.insert(pr);
				}

			}

			if (0 != isnofind.size())
			{
				for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
				{

					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pNode->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL XmlManager::SetXMLNewNode(XMLElement* pFatherRoot, const char* newNodeName, map<string, string>newMapAttribute, const char* newText, const string nodeName, map<const char*, const char*> Attribution, XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	if (NULL == xmlSavePath)
	{
		xmlSavePath = m_xmlPath;
	}

	if (!pFatherRoot && "" == nodeName)
		return FALSE;

	if (!pFatherRoot)
	{
		pFatherRoot = m_pRoot;
		if (!pFatherRoot)
			return FALSE;
	}

	if ("" == nodeName)
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pFatherRoot->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(newMapAttribute, pNewNode, NULL, {}, doc, xmlSavePath);
			}
			if ("" != newText)
			{
				SetXMLNodeText(newText, pNewNode, NULL, {}, doc, xmlSavePath);
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pFatherRoot))
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pNode->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(newMapAttribute, pNewNode, NULL, {}, doc, xmlSavePath);
			}
			if ("" != newText)
			{
				SetXMLNodeText(newText, pNewNode, NULL, {}, doc, xmlSavePath);
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}


BOOL XmlManager::RenameNode(XMLElement* pRoot, const char* newNodeName, XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == doc)
	{
		doc = m_pDoc;
		if (NULL == doc)
			return FALSE;
	}

	if (NULL == xmlSavePath)
	{
		xmlSavePath = m_xmlPath;
	}
	if (NULL == pRoot || NULL == newNodeName)
		return FALSE;


	pRoot->SetName(newNodeName);

	if (SaveXMLFile(doc, xmlSavePath))
	{
		return TRUE;
	}
	return FALSE;
}




BOOL XmlManager::DeleteXMLNode(XMLElement* pRoot, const string nodeName, map<const char*, const char*> Attribution, XMLDocument* doc, const char* xmlSavePath)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}
	if ("" == nodeName)
	{
		//pRoot->DeleteChild(NodeName); //删除指定节点
		pRoot->DeleteChildren();//删除节点的所有孩子节点


		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	XMLElement* pNode = NULL;
	if (FindXMLNode(pNode, nodeName, Attribution, pRoot))
	{

		//pRoot->DeleteChild(NodeName); //删除指定节点
		pNode->DeleteChildren();//删除节点的所有孩子节点


		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL XmlManager::DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode)
{
	fatherNode->DeleteChild(childrenNode); //删除指定节点
	return TRUE;
}