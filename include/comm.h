#ifndef __LS_INCLUDE_COMM_H______
#define __LS_INCLUDE_COMM_H______

#include <memory>
#include <tchar.h>
#include <stdio.h> 
#include <stdarg.h>
#include <Windows.h>
#include <string>

#ifndef MAX_PATH
#define MAX_PATH          260
#endif 

#define MAX_USER_PATH			4096

#if _DEBUG || OUTPUTUSER
	#define DOutPutW WinDebugUserPutStringW
	#define DOutPutA WinDebugUserPutStringA

	#if _UNICODE
		#define DOutPutFile DOutPutFileW
		#define DOutPut DOutPutW
		#define DOutPutFileW WinDebugUserPutStringW(L"Line = %d ; File = %s" , __LINE__ , __FILEW__)
	#else
		#define DOutPutFile DOutPutFileA
		#define DOutPut DOutPutA
		#define DOutPutFileA WinDebugUserPutStringA("Line = %d ; File = %s" , __LINE__ , __FILE__)
	#endif
#else
	#define DOutPutW //DebugUserPutStringW
	#define DOutPutA //DebugUserPutStringA
	#define DOutPut //DebugUserPutStringA

	#define DOutPutFile
	#define DOutPutFileW //WinDebugUserPutStringW(L"Line = %d ; File = %s" , __LINE__ , __FILEW__)
	#define DOutPutFileA //WinDebugUserPutStringA("Line = %d ; File = %s" , __LINE__ , __FILE__)
#endif
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;

static void WinDebugUserPutStringW(const wchar_t * szFormat,...)
{
	wchar_t* 				szPutstr = NULL ;
	
	va_list					szList;
	int						szSize = 0;

	va_start(szList , szFormat);

	szPutstr = (wchar_t *)malloc( MAX_USER_PATH );

	vswprintf_s(szPutstr,MAX_PATH,szFormat,szList);
	
	OutputDebugStringW(szPutstr);		

	va_end(szList); 
	free(szPutstr);	
}

static void WinDebugUserPutStringA(const char * szFormat,...)
{
	va_list					szList;
	int						szSize = 0;
	char* 					szPutstr;
	va_start(szList,szFormat);

	szSize = _vscprintf(szFormat,szList) + 1;

	szPutstr = (char *)malloc( szSize * sizeof(char) );

	vsprintf_s(szPutstr,szSize,szFormat,szList);
	OutputDebugStringA(szPutstr);

	va_end(szList); 
	free(szPutstr);	
}
static BOOL IsRunasAdmin()
{
	BOOL bElevated = FALSE;
	HANDLE hToken = NULL;

	// Get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return FALSE;

	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;

	// Retrieve token elevation information
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			bElevated = tokenEle.TokenIsElevated;
		}
	}

	CloseHandle(hToken);
	return bElevated;
}
//判断用户权限
static BOOL IsAdmin(void)
{
    HANDLE                   hAccessToken;
    BYTE                     *InfoBuffer = NULL;
    PTOKEN_GROUPS            ptgGroups;
    DWORD                    dwInfoBufferSize;
    PSID                     psidAdministrators;
    SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;
    UINT                     i;
    BOOL                     bRet = FALSE;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hAccessToken))
       goto cleanup;
    
    InfoBuffer = new BYTE[1024];
    if(!InfoBuffer)
       goto cleanup;

    bRet = GetTokenInformation(hAccessToken,
                               TokenGroups,
                               InfoBuffer,
                               1024,
                               &dwInfoBufferSize);

    CloseHandle( hAccessToken );

    if(!bRet)
       goto cleanup;

    if(!AllocateAndInitializeSid(&siaNtAuthority,
                                 2,
                                 SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS,
                                 0,0,0,0,0,0,
                                 &psidAdministrators))
       goto cleanup;

    bRet = FALSE;

    ptgGroups = (PTOKEN_GROUPS)InfoBuffer;

    for(i=0;i<ptgGroups->GroupCount;i++)
    {
        if(EqualSid(psidAdministrators,ptgGroups->Groups[i].Sid))
        {
            bRet = TRUE;
            break;
        }
    }

    FreeSid(psidAdministrators);

cleanup:

    if (InfoBuffer)
        delete InfoBuffer;

    return bRet;
}




#endif //__LS_INCLUDE_COMM_H______