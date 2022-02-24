#ifndef _LS_REGEDIT_H__
#define _LS_REGEDIT_H__

#include <Windows.h>
#include <atlbase.h>
#include <dbt.h>
#include "comm.h"


BOOL AssocEx(LPCTSTR assocex,BOOL bAssoc = TRUE);
BOOL AssocExCheck(LPCTSTR assocex);
BOOL AssocUFCEx(HKEY hKeyRoot, LPCTSTR lpSubKey, bool bAssoc);
BOOL AssocUFCCheck(HKEY hKeyRoot, LPCTSTR lpSubKey);

BOOL RegDelnodeRecurse (HKEY hKeyRoot, LPTSTR lpSubKey);

#endif //_LS_REGEDIT_H__