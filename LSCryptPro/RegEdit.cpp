#include "RegEdit.h"
#include <shlobj.h>

/*
关联/取消 扩展名 assocex 

*/
BOOL AssocEx(LPCTSTR assocex,BOOL bAssoc)
{
	CRegKey		rk;
	HKEY		hKeyRoot;
	TCHAR		szModule[MAX_PATH];
	TCHAR		value[MAX_PATH];
	TCHAR		szSubKey[MAX_PATH]={0};
	TCHAR		extion[MAX_PATH]={0};
	TCHAR		extionname[MAX_PATH]={0};
	TCHAR		extionexp[MAX_PATH]={0};
	TCHAR		extionetext[MAX_PATH]={0};
	TCHAR		*pEnd = szSubKey;
	int			len = 0;
	bool		ok = false;

	GetModuleFileName(NULL, szModule, MAX_PATH);
	_stprintf_s(extion,MAX_PATH,_T(".%s"),assocex);
	_stprintf_s(extionname,MAX_PATH,_T("%sFile"),assocex);
	_stprintf_s(extionexp,MAX_PATH,_T("%s File"),assocex);

	if(!IsAdmin())
	{
		hKeyRoot = HKEY_CURRENT_USER;
		lstrcpyn(szSubKey, _T("Software\\Classes"), MAX_PATH);
		len = lstrlen(szSubKey);
		pEnd = szSubKey + len;
		if(szSubKey[len - 1] != _T('\\'))
		{
			szSubKey[len] = _T('\\');
			len++;
			szSubKey[len] = _T('\0');
			pEnd++;
		}
	}else
	{

		hKeyRoot = HKEY_CLASSES_ROOT;
	}

	if(bAssoc)
	{
		//关联
		lstrcpy(pEnd, extion);
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			//rk.SetValue(_T("LSCFile"),NULL);
			rk.SetStringValue(NULL,extionname);
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key %s fail\n",extion);
			goto END_ASSOC;
		}
		lstrcpy(pEnd, extionname);
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			rk.SetStringValue(NULL,extionexp);
			//rk.SetValue(_T("LSC File"));
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'%s\' fail\n",extionname);
			goto END_ASSOC;
		}

		_stprintf_s(extionexp,MAX_PATH,_T("%s\\DefaultIcon"),extionname);
		lstrcpy(pEnd, extionexp);
		//lstrcpy(pEnd, _T("LSCFile\\DefaultIcon"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			value[0] = _T('\"');
			value[1] = 0;
			lstrcat(value, szModule);
			lstrcat(value, _T("\",0"));
			//rk.SetValue(value);
			rk.SetStringValue(NULL,value);
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'%s\\DefaultIcon\' fail\n",extionname);
			goto END_ASSOC;
		}
		_stprintf_s(extionexp,MAX_PATH,_T("%s\\Shell\\open\\Command"),extionname);
		lstrcpy(pEnd, extionexp);
		//lstrcpy(pEnd, _T("LSCFile\\Shell\\open\\Command"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			value[0] = _T('\"');
			value[1] = 0;
			lstrcat(value, szModule);
			lstrcat(value, _T("\" \"%1\""));
			//rk.SetValue(value);
			rk.SetStringValue(NULL,value);
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'%s\'\n",extionexp);
			goto END_ASSOC;
		}

	}
	else
	{

		//不关联
		lstrcpy(pEnd, extionname);
		if(RegDelnodeRecurse(hKeyRoot, szSubKey))
		{
			DOutPut(L"delete register node \'%s\' success\n",extionname);
		}
		else
		{
			DOutPut(L"delete register node \'%s\' failed\n",extionname);
		}

		lstrcpy(pEnd, extion);
		if(RegDelnodeRecurse(hKeyRoot, szSubKey))
		{
			DOutPut(L"delete register node \'%s\' success\n",extionname);
		}
		else
		{
			DOutPut(L"delete register node \'%s\' failed\n",extion);
		}

	}
	//通告关联改变SHCNE_ASSOCCHANGED
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	ok = true;

END_ASSOC:

	return ok;
}
BOOL AssocExCheck(LPCTSTR assocex)
{
	CRegKey		rk;
	HKEY		hKeyRoot;
	TCHAR		szSubKey[MAX_PATH]={0};
	TCHAR		extion[MAX_PATH]={0};
	TCHAR		*pEnd = szSubKey;
	int			len = 0;
	bool		ok = false;

	_stprintf_s(extion,MAX_PATH,_T(".%s"),assocex);

	if(!IsAdmin())
	{
		hKeyRoot = HKEY_CURRENT_USER;
		lstrcpyn(szSubKey, _T("Software\\Classes"), MAX_PATH);
		len = lstrlen(szSubKey);
		pEnd = szSubKey + len;
		if(szSubKey[len - 1] != _T('\\'))
		{
			szSubKey[len] = _T('\\');
			len++;
			szSubKey[len] = _T('\0');
			pEnd++;
		}
	}else
	{

		hKeyRoot = HKEY_CLASSES_ROOT;
	}
	lstrcpy(pEnd, extion);
	if(ERROR_SUCCESS == rk.Open(hKeyRoot, szSubKey))
	{
		ok = true;
		rk.Close();
	}
	return ok;
}

BOOL AssocUFCCheck(HKEY hKeyRoot, LPCTSTR lpSubKey)
{
	CRegKey		rk;;
	TCHAR		szSubKey[MAX_PATH]={0};
	TCHAR		*pEnd = szSubKey;
	int			len = 0;
	bool		ok = false;


	if(lpSubKey)
	{
		lstrcpyn(szSubKey, lpSubKey, MAX_PATH);
		len = lstrlen(szSubKey);
		pEnd = szSubKey + len;
		if(szSubKey[len - 1] != _T('\\'))
		{
			szSubKey[len] = _T('\\');
			len++;
			szSubKey[len] = _T('\0');
			pEnd++;
		}
	}
	lstrcpy(pEnd, _T(".LSC"));
	if(ERROR_SUCCESS == rk.Open(hKeyRoot, szSubKey))
	{
		ok = true;
		rk.Close();
	}
	return ok;
}
BOOL AssocUFCEx(HKEY hKeyRoot, LPCTSTR lpSubKey, bool bAssoc)
{
	CRegKey		rk;
	TCHAR		szModule[MAX_PATH];
	TCHAR		value[MAX_PATH];
	TCHAR		szSubKey[MAX_PATH]={0};
	TCHAR		*pEnd = szSubKey;
	int			len = 0;
	bool		ok = false;

	GetModuleFileName(NULL, szModule, MAX_PATH);

	if(lpSubKey)
	{
		lstrcpyn(szSubKey, lpSubKey, MAX_PATH);
		len = lstrlen(szSubKey);
		pEnd = szSubKey + len;
		if(szSubKey[len - 1] != _T('\\'))
		{
			szSubKey[len] = _T('\\');
			len++;
			szSubKey[len] = _T('\0');
			pEnd++;
		}
	}

	if(bAssoc)
	{
		//关联
		lstrcpy(pEnd, _T(".LSC"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			//rk.SetValue(_T("LSCFile"),NULL);
			rk.SetStringValue(NULL,_T("LSCFile"));
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key .LSC fail\n");
			goto END_ASSOC;
		}
		lstrcpy(pEnd, _T("LSCFile"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			rk.SetStringValue(NULL,_T("LSC File"));
			//rk.SetValue(_T("LSC File"));
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'LSCFile\' fail\n");
			goto END_ASSOC;
		}

		lstrcpy(pEnd, _T("LSCFile\\DefaultIcon"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			value[0] = _T('\"');
			value[1] = 0;
			lstrcat(value, szModule);
			lstrcat(value, _T("\",0"));
			//rk.SetValue(value);
			rk.SetStringValue(NULL,value);
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'LSCFile\\DefaultIcon\' fail\n");
			goto END_ASSOC;
		}
		lstrcpy(pEnd, _T("LSCFile\\Shell\\open\\Command"));
		if(ERROR_SUCCESS == rk.Create(hKeyRoot, szSubKey))
		{
			value[0] = _T('\"');
			value[1] = 0;
			lstrcat(value, szModule);
			lstrcat(value, _T("\" \"%1\""));
			//rk.SetValue(value);
			rk.SetStringValue(NULL,value);
			rk.Close();
		}
		else
		{
			DOutPut(L"create sub key \'LSCFile\\Shell\\open\\Command\n");
			goto END_ASSOC;
		}

	}
	else
	{

		//不关联
		lstrcpy(pEnd, _T("LSCFile"));
		if(RegDelnodeRecurse(hKeyRoot, szSubKey))
		{
			DOutPut(L"delete register node \'LSCFile\' success\n");
		}
		else
		{
			DOutPut(L"delete register node \'LSCFile\' failed\n");
		}

		lstrcpy(pEnd, _T(".LSC"));
		if(RegDelnodeRecurse(hKeyRoot, szSubKey))
		{
			DOutPut(L"delete register node \'.LSC\' success\n");
		}
		else
		{
			DOutPut(L"delete register node \'.LSC\' failed\n");
		}

/*
		//不关联
		lstrcpy(value, _T("LSCFile"));
		if(RegDelnodeRecurse(hKeyRoot, value))
		{
			print(L"delete regiter node \'LSCFile\' success\n");
		}
		else
		{
			print(L"delete regiter node \'LSCFile\' failed\n");
		}

		lstrcpy(value, _T(".LSC"));
		if(RegDelnodeRecurse(hKeyRoot, value))
		{
			print(L"delete regiter node \'.LSC\' success\n");
		}
		else
		{
			print(L"delete regiter node \'.LSC\' failed\n");
		}
*/
	}
	//通告关联改变SHCNE_ASSOCCHANGED
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	ok = true;

END_ASSOC:

	return ok;
}


//*************************************************************
//
//  RegDelnodeRecurse()
//
//  Purpose:    Deletes a registry key and all it's subkeys / values.
//
//  Parameters: hKeyRoot    -   Root key
//              lpSubKey    -   SubKey to delete
//
//  Return:     TRUE if successful.
//              FALSE if an error occurs.
//
//*************************************************************

BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey)
{
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;
	
    // First, see if we can delete the key without having
    // to recurse.
	
    lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	
    if (lResult == ERROR_SUCCESS) 
        return TRUE;
	
    lResult = RegOpenKeyEx (hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	
    if (lResult != ERROR_SUCCESS) 
    {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            //printf("Key not found.\n");
            return TRUE;
        } 
        else {
            //printf("Error opening key.\n");
            return FALSE;
        }
    }
	
    // Check for an ending slash and add one if it is missing.
	
    lpEnd = lpSubKey + lstrlen(lpSubKey);
	
    if (*(lpEnd - 1) != TEXT('\\')) 
    {
        *lpEnd =  TEXT('\\');
        lpEnd++;
        *lpEnd =  TEXT('\0');
    }
	
    // Enumerate the keys
	
    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &ftWrite);
	
    if (lResult == ERROR_SUCCESS) 
    {
        do {
			
            lstrcpy (lpEnd, szName);
			
            if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
                break;
            }
			
            dwSize = MAX_PATH;
			
            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
				NULL, NULL, &ftWrite);
			
        } while (lResult == ERROR_SUCCESS);
    }
	
    lpEnd--;
    *lpEnd = TEXT('\0');
	
    RegCloseKey (hKey);
	
    // Try again to delete the key.
	
    lResult = RegDeleteKey(hKeyRoot, lpSubKey);
	
    if (lResult == ERROR_SUCCESS) 
        return TRUE;
	
    return FALSE;
}
