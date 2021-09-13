#pragma once

/// 注意下面两文件的路径，必须在工程包含
//注意 本页面仅支持VS2017(VC141)及以上版本




//CString strSkylarJson_Software = L"";
//if (m_bSoftWareUpdSucess && QeDownloadOption::IsNeedToDownloadSoftware())
//{
//	CString strSkylarJson = strSkylarDir + _T("\\download\\update\\virus.json");
//	if (PathFileExists(strSkylarJson))
//	{
//		Json::Value jvContent;
//		if (CJsonHelper::LoadJson(strSkylarJson, jvContent))
//		{
//			CString csLegVer = L"";
//			Json::Value jvData;
//			if (CJsonHelper::GetJsonObject(jvContent, "data", jvData))
//			{
//				Json::Value jvInfo;
//				if (CJsonHelper::GetJsonObject(jvData, "update_info", jvInfo))
//				{
//					csLegVer = CJsonHelper::GetJsonValueString(L"version", jvInfo, L"");
//				}
//			}
//			if (!csLegVer.IsEmpty())
//			{
//				CString strSkylarJsonNew = strSkylarJson.Left(strSkylarJson.ReverseFind(L'.'));
//				strSkylarJsonNew = strSkylarJsonNew + L"_" + csLegVer + L".json";//strSkylarJsonNew = strSkylarJsonNew + L"-" + csLegVer + L".json";
//				strSkylarJson_Software = strSkylarJsonNew;
//				if (!PathFileExists(strSkylarJsonNew))
//				{
//					//旧的virus.json改名为virus-xxxx.json(xxxx为旧版本号)
//					if (FALSE == ::MoveFile(strSkylarJson, strSkylarJsonNew))
//					{
//						WRITE_LOG(_T("把文件从[%s]移动到[%s]失败，LastErr: [%d]"), strSkylarJson, strSkylarJsonNew, GetLastError());
//						return TreeList_Update_State_Failed;
//					}
//				}
//			}
//		}
//	}
//
//
//	//加载最新的virus.json取出软件库数据
//	//复制旧版本virus-xxxxx.json到virus.json
//	//修改virus.json中的软件库数据到取出的最新数据
//	//保存
//	CString software_rule_new_md5 = L"";
//	CString software_rule_new_sha1 = L"";
//	CString software_rule_new_url = L"";
//	CString software_rule_new_version = L"";
//
//	CString software_rule_paid_new_md5 = L"";
//	CString software_rule_paid_new_sha1 = L"";
//	CString software_rule_paid_new_url = L"";
//	CString software_rule_paid_new_version = L"";
//	//获取最新文件中的软件库数据
//	if (PathFileExists(strUpdateDir + _T("\\virus.json")))
//	{
//		Json::Value jvContent;
//		if (CJsonHelper::LoadJson(strUpdateDir + _T("\\virus.json"), jvContent))
//		{
//			Json::Value jvData;
//			if (CJsonHelper::GetJsonObject(jvContent, "data", jvData))
//			{
//				Json::Value jvInfo;
//				if (CJsonHelper::GetJsonObject(jvData, "update_info", jvInfo))
//				{
//					Json::Value jvmod;
//					if (CJsonHelper::GetJsonObject(jvInfo, "mod", jvmod))
//					{
//						Json::Value software_rule_new;
//						if (CJsonHelper::GetJsonObject(jvmod, "software_rule", software_rule_new))
//						{
//							software_rule_new_md5 = CJsonHelper::GetJsonValueString(L"md5", software_rule_new, L"");
//							software_rule_new_sha1 = CJsonHelper::GetJsonValueString(L"sha1", software_rule_new, L"");
//							software_rule_new_url = CJsonHelper::GetJsonValueString(L"url", software_rule_new, L"");
//							software_rule_new_version = CJsonHelper::GetJsonValueString(L"version", software_rule_new, L"");
//						}
//						Json::Value software_rule_paid_new;
//						if (CJsonHelper::GetJsonObject(jvmod, "software_rule_paid", software_rule_paid_new))
//						{
//							software_rule_paid_new_md5 = CJsonHelper::GetJsonValueString(L"md5", software_rule_paid_new, L"");
//							software_rule_paid_new_sha1 = CJsonHelper::GetJsonValueString(L"sha1", software_rule_paid_new, L"");
//							software_rule_paid_new_url = CJsonHelper::GetJsonValueString(L"url", software_rule_paid_new, L"");
//							software_rule_paid_new_version = CJsonHelper::GetJsonValueString(L"version", software_rule_paid_new, L"");
//						}
//					}
//				}
//			}
//		}
//	}
//
//
//	//拷贝下载的virus.json到控制台目录
//	if (FALSE == UTILITY::MyCopyFile(strSkylarJson_Software, strSkylarDir + _T("\\download\\update\\virus.json"), FALSE))
//	{
//		WRITE_LOG(_T("文件从[%s]拷贝到[%s]失败, lasterr=%d"), strSkylarJson_Software + _T("\\virus.json"),
//			strSkylarDir + _T("\\download\\update\\virus.json"), GetLastError());
//
//		return TreeList_Update_State_Failed;
//	}
//	else
//	{
//		//修改旧virus.json的内容
//		if (PathFileExists(strSkylarDir + _T("\\download\\update\\virus.json")))
//		{
//			Json::Value jvContent;
//			if (CJsonHelper::LoadJson(strSkylarDir + _T("\\download\\update\\virus.json"), jvContent))
//			{
//				Json::Value jvData;
//				if (CJsonHelper::GetJsonObject(jvContent, "data", jvData))
//				{
//					Json::Value jvInfo;
//					if (CJsonHelper::GetJsonObject(jvData, "update_info", jvInfo))
//					{
//						Json::Value jvmod;
//						if (CJsonHelper::GetJsonObject(jvInfo, "mod", jvmod))
//						{
//							Json::Value software_rule_new;
//							if (CJsonHelper::GetJsonObject(jvmod, "software_rule", software_rule_new))
//							{
//								CJsonHelper::WriteJsonValueString(L"md5", software_rule_new, software_rule_new_md5);
//								CJsonHelper::WriteJsonValueString(L"sha1", software_rule_new, software_rule_new_sha1);
//								CJsonHelper::WriteJsonValueString(L"url", software_rule_new, software_rule_new_url);
//								CJsonHelper::WriteJsonValueString(L"version", software_rule_new, software_rule_new_version);
//							}
//							Json::Value software_rule_paid_new;
//							if (CJsonHelper::GetJsonObject(jvmod, "software_rule_paid", software_rule_paid_new))
//							{
//								CJsonHelper::WriteJsonValueString(L"md5", software_rule_paid_new, software_rule_paid_new_md5);
//								CJsonHelper::WriteJsonValueString(L"sha1", software_rule_paid_new, software_rule_paid_new_sha1);
//								CJsonHelper::WriteJsonValueString(L"url", software_rule_paid_new, software_rule_paid_new_url);
//								CJsonHelper::WriteJsonValueString(L"version", software_rule_paid_new, software_rule_paid_new_version);
//							}
//
//							jvContent["data"]["update_info"]["mod"]["software_rule"] = software_rule_new;
//							jvContent["data"]["update_info"]["mod"]["software_rule_paid"] = software_rule_paid_new;
//
//							CJsonHelper::WriteJson(strSkylarDir + _T("\\download\\update\\virus.json"), jvContent);
//						}
//					}
//				}
//			}
//		}
//
//		//复制新文件并改名virus-xxxx.json，xxxx代表病毒库版本号
//		{
//			//CString strSkylarJson = strSkylarDir + _T("\\download\\update\\virus.json");
//			CString strSkylarJson = strUpdateDir + _T("\\virus.json");
//			CString strSkylarJsonsoftware = strSkylarDir + _T("\\download\\update\\virus.json");
//			if (PathFileExists(strSkylarJson))
//			{
//				Json::Value jvContent;
//				if (CJsonHelper::LoadJson(strSkylarJson, jvContent))
//				{
//					CString csLegVer = L"";
//					Json::Value jvData;
//					if (CJsonHelper::GetJsonObject(jvContent, "data", jvData))
//					{
//						Json::Value jvInfo;
//						if (CJsonHelper::GetJsonObject(jvData, "update_info", jvInfo))
//						{
//							csLegVer = CJsonHelper::GetJsonValueString(L"version", jvInfo, L"");
//
//						}
//					}
//					if (!csLegVer.IsEmpty())
//					{
//						CString strSkylarJsonNew = strSkylarJsonsoftware.Left(strSkylarJsonsoftware.ReverseFind(L'.'));
//						strSkylarJsonNew = strSkylarJsonNew + L"_" + csLegVer + L".json";//strSkylarJsonNew = strSkylarJsonNew + L"-" + csLegVer + L".json";
//						if (FALSE == ::CopyFile(strSkylarJson, strSkylarJsonNew, false))
//						{
//							WRITE_LOG(_T("把文件从[%s]拷贝到[%s]失败，LastError: [%d]"), strSkylarJson, strSkylarJsonNew, GetLastError());
//							return TreeList_Update_State_Failed;
//						}
//						else
//							WRITE_LOG(_T("把文件从[%s]拷贝到[%s]成功!"), strSkylarJson, strSkylarJsonNew);
//
//						//Additional Codes for Push Data to Server by Send Http Request
//						if (m_SyncData.UpdateVirusJson(strSkylarDir + _T("\\download\\update\\virus.json")))
//							WRITE_LOG(_T("软件库入库失败!"));
//						else
//							WRITE_LOG(_T("软件库入库成功!"));
//					}
//				}
//			}
//		}













#include <fstream>
#include <string>

using namespace std;

#include "json/json.h"

//#pragma comment(lib,"json_vc90_libmt-s-0.10.6_x64.lib")
#ifndef _WIN64
	#if (defined _WINDOWS) || (defined WIN32)
		#ifdef _DEBUG
			#if _MSC_VER == 1500
				#pragma comment(lib,"json_vc90_libmt-sgd-0.10.6.lib")
			#else if _MSC_VER >= 1900
				#if _MSC_VER == 1900
					#pragma comment(lib,"json_vc140_libmt-sgd-0.10.6.lib")
				#else if _MSC_VER > 1900
					#pragma comment(lib,"json_vc141_libmt-sgd-0.10.6.lib")
				#endif
			#endif
		#else
			#if _MSC_VER == 1500
				#pragma comment(lib,"json_vc90_libmt-s-0.10.6.lib")
			#else if _MSC_VER >= 1900
					#if _MSC_VER == 1900
						#pragma comment(lib,"json_vc140_libmt-s-0.10.6.lib")
					#else if _MSC_VER > 1900
						#pragma comment(lib,"json_vc141_libmt-s-0.10.6.lib")
					#endif
			#endif
		#endif	//endif _DEBUG
	#endif	//endif _WINDOWS || WIN32
#else
	#ifdef _DEBUG
		#if _MSC_VER == 1500
			#pragma comment(lib,"json_vc90_libmt-sgd-0.10.6_x64.lib")
		#else if _MSC_VER >= 1900
			#if _MSC_VER == 1900
				#pragma comment(lib,"json_vc140_libmt-sgd-0.10.6_x64.lib")
			#else if _MSC_VER > 1900
				#pragma comment(lib,"json_vc141_libmt-sgd-0.10.6_x64.lib")
			#endif
		#endif
	#else
		#if _MSC_VER == 1500
			#pragma comment(lib,"json_vc90_libmt-s-0.10.6_x64.lib")
		#else if _MSC_VER >= 1900
			#if _MSC_VER == 1900
				#pragma comment(lib,"json_vc140_libmt-s-0.10.6_x64.lib")
			#else if _MSC_VER > 1900
				#pragma comment(lib,"json_vc141_libmt-s-0.10.6_x64.lib")
			#endif
		#endif
	#endif	//endif _DEBUG
#endif	//endif WIN64

class CJsonHelper
{
public:

	static bool LoadJson(LPCTSTR lpFile, Json::Value& jvRoot)
	{
		bool bLoadSucc = false;

		if (PathFileExists(lpFile) == FALSE)
		{
			return bLoadSucc;
		}

		try
		{
			Json::Reader _reader;

			jvRoot = Json::Value(Json::nullValue);

			std::ifstream _config;

			_config.open(lpFile, ios::in);
			bLoadSucc = _reader.parse(_config, jvRoot);
		}
		catch(...)
		{
			return false;
		}

		return bLoadSucc;
	}

	static bool LoadJson(const std::string &strDoc, Json::Value& jvRoot)
	{
		bool bLoadSucc = false;

		if (strDoc.empty())
		{
			return bLoadSucc;
		}

		try
		{
			Json::Reader _reader;

			jvRoot = Json::Value(Json::nullValue);

			bLoadSucc = _reader.parse(strDoc, jvRoot);
		}
		catch(...)
		{
			return false;
		}

		return bLoadSucc;
	}

	static bool WriteJson(LPCTSTR lpFile, const Json::Value &jvRoot)
	{
		if (lpFile == NULL)
			return false;

		try
		{
			Json::StyledWriter writer;
			std::ofstream _config;

			std::string strBuff = writer.write(jvRoot);

			_config.open(lpFile, ios::out);
			_config.write(strBuff.c_str(), strBuff.size());
			_config.flush();
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	static bool WriteJson(const Json::Value &jvRoot,CStringA& strJsonA)
	{
		try
		{
			Json::StyledWriter writer;

			std::string strBuff = writer.write(jvRoot);
			strJsonA = strBuff.c_str();
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	static INT GetJsonValueINT(LPCTSTR lpKey, const Json::Value& jvRoot, DWORD dwDefault = 0)
	{
		try
		{
			if (jvRoot.isNull() || !jvRoot.isObject())
			{
				return dwDefault;
			}

			if (lpKey == NULL ||  (!jvRoot.isMember(CStringA(lpKey))))
			{
				return dwDefault;
			}

			Json::Value _value = jvRoot[CStringA(lpKey)];
			if (!_value.isInt())
			{
				return dwDefault;
			}

			return _value.asInt();
		}
		catch(...)
		{
			return dwDefault;
		}
	}

	static INT GetJsonValueBOOL(LPCTSTR lpKey, const Json::Value& jvRoot, DWORD dwDefault = 0)
	{
		try
		{
			if (jvRoot.isNull() || !jvRoot.isObject())
			{
				return dwDefault;
			}

			if (lpKey == NULL ||  (!jvRoot.isMember(CStringA(lpKey))))
			{
				return dwDefault;
			}

			Json::Value _value = jvRoot[CStringA(lpKey)];
			if (!_value.isBool())
			{
				return dwDefault;
			}

			return _value.asBool();
		}
		catch(...)
		{
			return dwDefault;
		}
	}
	
	
	static CString GetJsonValueString(LPCTSTR lpKey, const Json::Value& jvRoot, CString strDefault = CString())
	{
		try
		{
			if (jvRoot.isNull() || !jvRoot.isObject())
			{
				return strDefault;
			}

			if (lpKey == NULL || (!jvRoot.isMember(CStringA(lpKey))))
			{
				return strDefault;
			}	

			Json::Value _value = jvRoot[CStringA(lpKey)];
			if (_value.isNull() || !_value.isString())
			{
				return strDefault;
			}

			return CString(CA2T(_value.asCString(),CP_UTF8));
		}
		catch(...) 
		{
			return strDefault;
		}
	}

	static CStringA GetJsonValueStringA(LPCSTR lpKey, const Json::Value& jvRoot, CStringA strDefault = CStringA())
	{
		try
		{
			if (jvRoot.isNull() || !jvRoot.isObject())
			{
				return strDefault;
			}

			if (lpKey == NULL || (!jvRoot.isMember(lpKey)))
			{
				return strDefault;
			}

			Json::Value _value = jvRoot[lpKey];
			if (_value.isNull() || !_value.isString())
			{
				return strDefault;
			}

			LPCSTR lpText = _value.asCString();
			return CStringA(lpText);
		}
		catch(...)
		{
			return strDefault;
		}
	}
	
	static BOOL GetJsonValueArray(LPCSTR lpKey, const Json::Value& jvRoot, Json::Value& jvArray)
	{
		try
		{
			if (jvRoot.isNull() || !jvRoot.isObject())
			{
				return FALSE;
			}

			if (lpKey == NULL || (!jvRoot.isMember(lpKey)))
			{
				return FALSE;
			}

			jvArray = jvRoot[lpKey];
			if (jvArray.isNull() || !jvArray.isArray())
			{
				return FALSE;
			}

			return TRUE;
		}
		catch(...)
		{
			return FALSE;
		}
	}

	static bool WriteJsonValueInt(LPCTSTR lpKey, Json::Value& jvRoot, INT nValue)
	{
		try
		{
			if (lpKey == NULL)
				return false;

		//if (!jvRoot[CStringA(lpKey)].isArray())
			jvRoot[CStringA(lpKey)] = nValue;
					

			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	static bool WriteJsonValueString(LPCTSTR lpKey, Json::Value& jvRoot, LPCTSTR lpValue)
	{
		try
		{
			if (lpKey == NULL)
				return false;

			//if (!jvRoot[CStringA(lpKey)].isArray())
				jvRoot[CStringA(lpKey)] = (LPCSTR)CStringA(lpValue);

			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	static bool GetJsonObject(const Json::Value &jsParent, LPCSTR lpKey, Json::Value &jsObject)
	{
		try
		{
			if (jsParent.isNull() || !jsParent.isObject() || lpKey==NULL)
			{
				return false;
			}

			jsObject = Json::nullValue;

			if (jsParent.isMember(lpKey))
			{
				jsObject = jsParent[lpKey];
			}

			return !jsObject.isNull();
		}
		catch(...)
		{
			return false;
		}
	}
};
