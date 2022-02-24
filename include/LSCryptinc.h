#ifndef _LS_CRYPT_H__
#define _LS_CRYPT_H__
#include "scrypt.h"

#define CRYPT_LIB_NAME "LSCrypt.lib"
#define CRYPT_DLL_NAME TEXT("LSCrypt.dll")


//x64 system

typedef DWORD (WINAPI *pfnDllDecrypt)(char *password, const wchar_t *file, pfnEn_DeCallback pCallback,
									  LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError);
typedef DWORD (WINAPI *pfnDllEncrypt)(char *password, const wchar_t *file, pfnEn_DeCallback pCallback,
									  LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError);
typedef DWORD (WINAPI *pfnDllOpenCrypt)(const wchar_t *file);

typedef void (WINAPI *pfnDllClearReg)();
typedef HRESULT (WINAPI *pfnDllUnregisterServer)();
typedef HRESULT (WINAPI *pfnDllRegisterServer)();
typedef BOOL (WINAPI * pfnDLLCheckeRegister)();


static HMODULE					g_hmod;
static pfnDllDecrypt			fnDllDecrypt;
static pfnDllEncrypt			fnDllEncrypt;
static pfnDllOpenCrypt			fnDllOpenCrypt;
static pfnDllClearReg			fnDllClearReg;

static pfnDllUnregisterServer	fnDllUnregisterServer;
static pfnDllRegisterServer		fnDllRegisterServer;
static pfnDLLCheckeRegister		fnDLLCheckeRegister;

static bool LoadDLL()
{
		g_hmod = LoadLibrary(CRYPT_DLL_NAME);
		if (g_hmod!=INVALID_HANDLE_VALUE)
		{
			fnDllDecrypt = (pfnDllDecrypt)GetProcAddress(g_hmod,"DllDecrypt");
			fnDllEncrypt = (pfnDllEncrypt)GetProcAddress(g_hmod,"DllEncrypt");
			fnDllOpenCrypt = (pfnDllOpenCrypt)GetProcAddress(g_hmod,"DllOpenCrypt");
			fnDllClearReg = (pfnDllClearReg)GetProcAddress(g_hmod,"DllClearReg");
			fnDllUnregisterServer = (pfnDllUnregisterServer)GetProcAddress(g_hmod,"DllUnregisterServer");
			fnDllRegisterServer = (pfnDllRegisterServer)GetProcAddress(g_hmod,"DllRegisterServer");
			fnDLLCheckeRegister = (pfnDLLCheckeRegister)GetProcAddress(g_hmod,"DLLCheckeRegister");
			return true;
		}
	return false;
}

static void UnLoadDLL()
{
	if (g_hmod)
	{
		FreeLibrary(g_hmod);
	}
}

//#pragma comment (lib,CRYPT_LIB_NAME)
//#pragma message ("Automatically linking with " CRYPT_LIB_NAME)
//
//HRESULT __declspec(dllimport) DllCanUnloadNow(void);
//HRESULT __declspec(dllimport) DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
//HRESULT __declspec(dllimport) DllRegisterServer(void);
//HRESULT __declspec(dllimport) DllUnregisterServer(void);
//void __declspec(dllimport) DllClearReg(void);
//DWORD __declspec(dllimport) DllDecrypt(const char *password, const wchar_t *file, pfnEn_DeCallback pCallback, 
//						LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError);
//DWORD __declspec(dllimport) DllEncrypt(const char *password, const wchar_t *file, pfnEn_DeCallback pCallback, 
//						LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError);

#endif //_LS_CRYPT_H__