#ifndef _CORRECT_INSTALLED_PATCHES_H_
#define _CORRECT_INSTALLED_PATCHES_H_

#include <vector>
#include <map>
using namespace std;

class CCorrectInstalledPatches
{
public:
	CCorrectInstalledPatches();
	~CCorrectInstalledPatches();
public:
	bool GetInstalledPatches(std::map<DWORD,CString>& wmiResult);
};

#endif//_CORRECT_INSTALLED_PATCHES_H_