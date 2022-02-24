//MainDlg.h

#pragma once

#include <windows.h>
#include <commctrl.h>
#include "FileList.h"
#include "scrypt.h"


INT_PTR CALLBACK MainDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//错误处理消息
#define UM_EN_DE_FILE_ERROR					(WM_USER + 10)
#define UM_SET_FILE_TOTAL					(WM_USER + 11)
#define UM_SET_CURRENT_FILE					(WM_USER + 12)
#define UM_SET_DONE							(WM_USER + 13)
#define UM_SET_CURRENT_FILE_PROGRESS		(WM_USER + 14)
#define UM_ERROR_HANDLE						(WM_USER + 15)
#define UM_TAST_COMPLETE					(WM_USER + 16)
#define UM_MONITOR_THREAD_EXIT				(WM_USER + 17)

#define TASK_MSG_ABORT			7777
//#define TASK_MSG_PAUSE			7778
//#define TASK_MSG_CONTINUE		7779
#define DIR_ENCRYPT			1
#define DIR_DECRYPT			2


class CMainDlg
{
	CFileList		*m_pFileList;
	int				m_nTotalFiles;
	char			m_password[MAX_AES_PASSWORD_LEN + 1];
	int				m_en_de;		//加密? 解密?
	HANDLE			m_hThread;
	DWORD			m_nThreadId;
	bool			m_bCanExit;
public:
	HWND			m_hWnd;
	HWND			m_hWndParent;
	wchar_t			m_FilePath[MAX_PATH];
	wchar_t			m_OutFilePath[MAX_PATH];
public:
	CMainDlg(int en_de, HWND hwndParent = NULL);
	~CMainDlg();
protected:
	void CenterWindow();
	bool OnError(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnOK();
	void ExitDlg(int nCodeExit);
	virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
	virtual bool OnInitDialog();
	void AbortTask(void);
	void OnTaskComplete();
	void OnMonitorThreadExit(WPARAM wParam, LPARAM lParam);
public:
	virtual bool PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR DoModal();
public:
	void SetCurrentFile(LPCWSTR lpFile);	
	void SetCurrentProgressRange(int range);
	void SetTotalProgressRange(int range);
	int SetCurrentProgressPos(int nPos);
	int SetTotalProgressPos(int nPos);
	int SetTotalFile(int nFiles);
	void SetFilesOfDone(int nFiles);
	void SetItemText(int nIDDlgItem, const wchar_t *text);
	void SetPassword(char *password);
//do task
public:
	void SetTask(CFileList *pFileList);
	//task thread proc
	static DWORD WINAPI DoTask(LPVOID lpParam);
	static DWORD WINAPI taskCallback(DWORD nEvent, unsigned __int64 param1, unsigned __int64 param2);
	static int GetLastSystemErrorText(wchar_t *errorText, int nBufferSize, DWORD dwLastError);
	static DWORD WINAPI MonitorProc(LPVOID lpParam);
};