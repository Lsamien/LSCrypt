//MainDlg.h

#pragma once

#include <windows.h>
#include <commctrl.h>
#include "scrypt.h"


#define PASSWORD_MODE_ENCRYPT			1	//加密输入密码模式
#define PASSWORD_MODE_DECRYPT			2	//解密输入密码模式

INT_PTR CALLBACK PasswordDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CPasswordDlg
{
	int		m_nMode;
	char	m_password[MAX_AES_PASSWORD_LEN + 1];
	HBITMAP		m_hBitmapLogo;
public:
	HWND	m_hWnd;
	HWND	m_hWndParent;
public:
	CPasswordDlg(int nMode, HWND hwndParent = NULL);
	~CPasswordDlg();
protected:
	virtual bool OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
	void CenterWindow();
public:
	virtual INT_PTR DoModal();
	void	OnDrawItem(WPARAM wParam,LPARAM	lParam);
	virtual bool PreTranslateMessage(MSG* pMsg);
	void SetItemText(int nIDDlgItem, const wchar_t *text);
	char *GetPassword(void) { return m_password; }
};