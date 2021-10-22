//Base公共库的实现
#pragma once
#include <XMLManager.h>
#include <tinyxml2.cpp>
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
	if ((doc->SaveFile(xmlSavePath)))
	{
		return FALSE;
	}

	//delete doc;
	return TRUE;
}


BOOL XmlManager::GetXMLDeclaration(XMLDocument* doc, string& strDecl)
{
	if (NULL == doc)
		return FALSE;
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
	return false;
}


BOOL XmlManager::FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, map<const char*, const char*> Attribution)
{

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
		if (FindXMLNode(p, nodeName, pNode, Attribution))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL XmlManager::GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot)
	{
		return FALSE;
	}

	if ("" == nodeName)
	{
		text = pRoot->GetText();
		return TRUE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
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


BOOL XmlManager::GetXMLNodeAttribute(XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot)
	{
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
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
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


BOOL XmlManager::SetXMLNodeText(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string text, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
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
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
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


BOOL XmlManager::SetXMLNodeAttribution(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
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
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
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


BOOL XmlManager::SetXMLNewNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* newNodeName, map<string, string>newMapAttribute, const char* newText, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}
	if ("" == nodeName)
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pRoot->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
			}
			if ("" != newText)
			{
				SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pNode->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
			}
			if ("" != newText)
			{
				SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
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


BOOL XmlManager::DeleteXMLNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* delNodeName, const string nodeName, map<const char*, const char*> Attribution)
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
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
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