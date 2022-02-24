//MainDlg.cpp

#include "stdafx.h"
#include "ProgressDlg.h"
//#include "resource.h"
#include <wchar.h>

#include "LSFileCrypt.h"
#include "LSCryptShellExt.h"

extern HINSTANCE g_hInst;
HWND	ghwnd = NULL;			//进度指示对话框的窗口句柄, 主要是taskCallback时使用
bool	gbAbort = false;

CProgressDlg::CProgressDlg(int en_de, HWND hwndParent)
{
	m_en_de = en_de;
	m_hWnd = NULL;
	m_hWndParent = hwndParent;
	m_hThread = NULL;
	m_nThreadId = 0;
	memset(m_password, 0, sizeof(m_password));
	memset(m_FilePath, 0, sizeof(m_FilePath));
	memset(m_OutFilePath, 0, sizeof(m_OutFilePath));
	m_bCanExit = false;
}

CProgressDlg::~CProgressDlg()
{
#if defined(_DEBUG) || defined(Release_check)
	OutputDebugStringW(L"CProgressDlg::~CProgressDlg()\n");
#endif
	gbAbort = false;

}


void CProgressDlg::CenterWindow()
{
	int			cx, cy;
	RECT		rect;

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, HWND_TOP, (cx - (rect.right - rect.left))/2, (cy - (rect.bottom - rect.top))/2, 
		0, 0, SWP_NOSIZE);
}



void CProgressDlg::OnCancel()
{
	//::PostThreadMessage(m_nThreadId, TASK_MSG_PAUSE, 0, 0);
	TCHAR		szText[50] = {0};

	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{
	case LANG_ENGLISH:
		LoadString(g_hInst,IDS_DLG_MAIN_EXIT_EN,szText,sizeof(szText));
		break;
	case LANG_CHINESE:
		LoadString(g_hInst,IDS_DLG_MAIN_EXIT_CH,szText,sizeof(szText));
		break;
	}
	SuspendThread(m_hThread);
	if(IDYES == ::MessageBoxW(m_hWnd, szText, 
		PROJECT_NAME_W, MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2))
	{
		gbAbort = true;
		ResumeThread(m_hThread);
		Sleep(1);
		::PostThreadMessage(m_nThreadId, TASK_MSG_ABORT, 0, 0);		
		m_bCanExit = true;
		ExitDlg(IDCANCEL);
		return;
	}
	//::PostThreadMessage(m_nThreadId, TASK_MSG_CONTINUE, 0, 0);
	ResumeThread(m_hThread);
}


void CProgressDlg::OnOK()
{
	ExitDlg(IDOK);
}



void CProgressDlg::ExitDlg(int nCodeExit)
{
	DWORD		nThreadExitCode = 0;
	HANDLE		hMonitor = NULL;
	if(m_bCanExit)
	{
		GetExitCodeThread(m_hThread, &nThreadExitCode);
		if(STILL_ACTIVE == nThreadExitCode)
		{
			//WaitForSingleObject(m_hThread, 6000);	//BUG造成主线程和工作线程相互阻塞, 导致工作线程无法取消
			//GetExitCodeThread(m_hThread, &nThreadExitCode);
			//if(STILL_ACTIVE == nThreadExitCode)
			//{
			//	OutputDebugStringW(L"Terminate thread\n");
			//	::TerminateThread(m_hThread, 0xFFFFFFFF);
			//}
			//开启新的线程监视
			hMonitor = ::CreateThread(NULL, 0, MonitorProc, (LPVOID)this,0, NULL);
			::CloseHandle(hMonitor);
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"Begin monitor thread\n");
#endif
		}
		else
		{
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"work thread already has been exit, not start monitor thread\n");
#endif
			OnMonitorThreadExit(nCodeExit, 0);
		}
	}
}


bool CProgressDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD		lw = LOWORD(wParam);

	if(IDCANCEL == lw)
	{
		//m_bCanExit = true;
		OnCancel();
		return true;
	}
	else if(IDOK == lw)
	{
		//m_bCanExit = true;
		//OnOK();
		return true;
	}
	return false;
}


bool CProgressDlg::OnInitDialog()
{
	HWND		hwnd = NULL;
	//password dialog test

	TCHAR		szCurrentFile[20]		= {0};
	TCHAR		szCurrentProgress[20]	= {0};
	TCHAR		szTotalProgress[20]		= {0};
	TCHAR		szButnCancel[20]		= {0};


	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{
	case LANG_ENGLISH:
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC1_EN,szCurrentFile,sizeof(szCurrentFile));
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC_CURRENT_PROGRESS_EN,szCurrentProgress,sizeof(szCurrentProgress));
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC_TOTAL_PROGRESS_EN,szTotalProgress,sizeof(szTotalProgress));
		LoadString(g_hInst,IDS_DLG_PROGRESS_BUTN_CANCEL_EN,szButnCancel,sizeof(szButnCancel));
		break;
	case LANG_CHINESE:
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC1_CH,szCurrentFile,sizeof(szCurrentFile));
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC_CURRENT_PROGRESS_CH,szCurrentProgress,sizeof(szCurrentProgress));
		LoadString(g_hInst,IDS_DLG_PROGRESS_STATIC_TOTAL_PROGRESS_CH,szTotalProgress,sizeof(szTotalProgress));
		LoadString(g_hInst,IDS_DLG_PROGRESS_BUTN_CANCEL_CH,szButnCancel,sizeof(szButnCancel));
		break;
	}


	//SetDlgItemText(m_hWnd,IDC_STATIC1,szCurrentFile);
	//SetDlgItemText(m_hWnd,IDC_STATIC_CURRENT_PROGRESS,szCurrentProgress);
	//SetDlgItemText(m_hWnd,IDC_STATIC_TOTAL_PROGRESS,szTotalProgress);
	//SetDlgItemText(m_hWnd,IDCANCEL,szButnCancel);


	hwnd = GetDlgItem(m_hWnd, IDC_PROGRESS_SINGLE);
	::SendMessage(hwnd, PBM_SETRANGE32, 0, 100);

	//hwnd = GetDlgItem(m_hWnd, IDC_PROGRESS_TOTAL);
	//::SendMessage(hwnd, PBM_SETRANGE32, 0, 100);

	CenterWindow();

	ghwnd = m_hWnd;
	//开始任务
	m_hThread = ::CreateThread(NULL, 0, DoTask, (LPVOID)this, 0, &m_nThreadId);
	return true;
}


void CProgressDlg::AbortTask(void)
{
	::PostThreadMessageW(m_nThreadId, TASK_MSG_ABORT, 0, 0);
	::PostThreadMessageW(m_nThreadId, TASK_MSG_ABORT, 0, 0);
}


void CProgressDlg::OnTaskComplete()
{
#if defined(_DEBUG) || defined(Release_check)
	OutputDebugStringW(L"CProgressDlg::OnTaskComplete()\n");
#endif
	m_bCanExit = true;
	ExitDlg(IDOK);
}


void CProgressDlg::OnMonitorThreadExit(WPARAM wParam, LPARAM lParam)
{
	::CloseHandle(m_hThread);
	m_hThread = NULL;
	m_nThreadId = 0;
	//EndDialog(m_hWnd, wParam);
	//m_hWnd = NULL;
	::SendMessage(m_hWnd, WM_CLOSE, wParam, 0);
}

bool CProgressDlg::PreTranslateMessage(MSG* pMsg)
{
	WORD lw = LOWORD(pMsg->wParam);

	switch(pMsg->message)
	{
	case WM_INITDIALOG:
		m_hWnd = pMsg->hwnd;
		OnInitDialog();
		return true;
	case WM_COMMAND:
		if(OnCommand(pMsg->wParam, pMsg->lParam)) return true;
		break;	
	case UM_SET_FILE_TOTAL:
		//SetTotalFile((int)pMsg->wParam);
		return true;
	case UM_EN_DE_FILE_ERROR:
		//return OnError(pMsg->wParam, pMsg->lParam);
	case UM_SET_CURRENT_FILE:
		//SetCurrentFile((LPCWSTR)pMsg->lParam);
		return true;
	case UM_SET_DONE:
		//SetTotalProgressPos((int)pMsg->wParam);
		return true;
	case UM_SET_CURRENT_FILE_PROGRESS:
		SetCurrentProgressPos((int)pMsg->wParam);
		return true;
	case UM_ERROR_HANDLE:
		OnError(pMsg->wParam, pMsg->lParam);
		return true;
	case UM_TAST_COMPLETE:
		OnTaskComplete();
		return true;
	case UM_MONITOR_THREAD_EXIT:
		OnMonitorThreadExit(pMsg->wParam, pMsg->lParam);
		return true;
	default:
		break;
	}
	return false;
}


INT_PTR CProgressDlg::DoModal()
{
	//判断构造的有效性
	if(DIR_ENCRYPT != m_en_de && DIR_DECRYPT != m_en_de) return IDCANCEL;

	return DialogBoxParamW(g_hInst, MAKEINTRESOURCE(IDD_PROGRESS_SINGLE), m_hWndParent, 
		(DLGPROC)ProgressDialogProc, (LPARAM)this);

}



int CProgressDlg::SetCurrentProgressPos(int nPos)
{
	HWND		hwndProgress = NULL;

	hwndProgress = GetDlgItem(m_hWnd, IDC_PROGRESS_SINGLE);
	::SendMessage(hwndProgress, PBM_SETPOS, nPos, 0);
	return nPos;
}




void CProgressDlg::SetPassword(char *password)
{
	lstrcpynA(m_password, password, MAX_AES_PASSWORD_LEN + 1);
}

DWORD CProgressDlg::DoTask(LPVOID lpParam)
{
	MSG				msg={0};
	CProgressDlg		*_this = (CProgressDlg *)lpParam;
	CFileList		*pFileList = NULL;
	HWND			hwnd = NULL;
	LSFileCrypt		ufc;
	//list<wchar_t *> folderList, fileList;
	//list<wchar_t *>::iterator	iter;
	//list<wchar_t *>::size_type	size;
	//list<wchar_t *> *pFileLists = NULL;

	wchar_t			*pCurrentDir = NULL;		//当前目录
	wchar_t			*pCurDirEnd = NULL;			//指向当前目录的尾部
	wchar_t			*pTemp = NULL;				//临时BUFFER
	//wchar_t			*pFileIn;					//将要被加密的文件名
	int				en_de;						//加密还是解密?
	//int				nFiles;
	//int				nFolders;
	//int				nTotal = 0;						//总的文件数量
	//int				nDone = 0;						//已经完成的文件数量
	//int				index;
	DWORD			error;		//错误信息



	if(!_this) return 0xffffffff;

	//build a message queue
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);


	hwnd = _this->m_hWnd;
	en_de = _this->m_en_de;
	//pFileList = _this->m_pFileList;

	//设置UI文件总数
	//::SendMessage(hwnd, UM_SET_FILE_TOTAL, 1, 0);

	//设置回调函数
	ufc.SetCallback(taskCallback);

	//::SendMessageW(hwnd, UM_SET_CURRENT_FILE, 0, (LPARAM)_this->m_FilePath);
	if(DIR_ENCRYPT == en_de)
	{
		error = ufc.Encrypt(_this->m_password, _this->m_FilePath, NULL, true, true,_this->m_OutFilePath,MAX_PATH);
	}
	else
	{
		error = ufc.Decrypt(_this->m_password, _this->m_FilePath, NULL, true, true,_this->m_OutFilePath,MAX_PATH);
	}
	//::SendMessageW(hwnd, UM_SET_DONE, 1, 0);

	//错误处理
	if(LSC_ERR_SYSTEM == error)
	{
		::SendMessageW(hwnd, UM_ERROR_HANDLE, (WPARAM)error, (LPARAM)ufc.GetLastSystemError());
	}
	else
	{
		::SendMessageW(hwnd, UM_ERROR_HANDLE, (WPARAM)error, (LPARAM)_this->m_FilePath);
	}
	//peek message
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	::PostMessageW(hwnd, UM_TAST_COMPLETE, 0, 0);

	OutputDebugStringW(L"work thread exit\n");
	return 0;
}



DWORD CProgressDlg::taskCallback(DWORD nEvent, unsigned __int64 param1, unsigned __int64 param2)
{
	DWORD		ret;

	if(EN_DE_CALLBACK_EVENT_PROGRESS == nEvent)
	{
		double		dbPercent;

		dbPercent = (double)param2/(double)param1;
		::SendMessageW(ghwnd, UM_SET_CURRENT_FILE_PROGRESS, (WPARAM)(dbPercent * 100), 0);
	}

	ret = gbAbort ? EN_DE_CALLBACK_EVENT_ABORT : EN_DE_CALLBACK_EVENT_CONTINUE;

	return ret;
}



DWORD WINAPI CProgressDlg::MonitorProc(LPVOID lpParam)
{
	CProgressDlg		*_this = (CProgressDlg *)lpParam;
	HANDLE			hWorkThread = NULL;
	DWORD			nExitCode;
	int				nTimes = 10;
	DWORD			nThreadExitCode = 0;

	hWorkThread = _this->m_hThread;

	do
	{
		Sleep(500);
		GetExitCodeThread(hWorkThread, &nThreadExitCode);
	}
	while(STILL_ACTIVE == nThreadExitCode && nTimes > 0);

	if(nTimes <= 0)
	{
		//终止线程
		OutputDebugStringW(L"Terminate thread\n");
		::TerminateThread(hWorkThread, 0xFFFFFFFF);
		nExitCode = IDCANCEL;
	}
	else
	{
		nExitCode = IDOK;
	}
	::PostMessage(_this->m_hWnd, UM_MONITOR_THREAD_EXIT, (WPARAM)nExitCode, 0);

	return nExitCode;
}

//------------------------------------------------------------------------------------
INT_PTR CALLBACK ProgressDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD		lw = LOWORD(wParam);
	static CProgressDlg	*pDialog = NULL;
	MSG			_msg = {0};

	_msg.hwnd = hwnd;
	_msg.lParam = lParam;
	_msg.message = msg;
	_msg.wParam = wParam;

	if(pDialog && pDialog->m_hWnd)
	{
		if(pDialog->PreTranslateMessage(&_msg)) return 1;
	}
	switch(msg)
	{
	case WM_INITDIALOG:
		pDialog = (CProgressDlg *)lParam;
		if(pDialog) 
		{
			if(pDialog->PreTranslateMessage(&_msg)) return 1;
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		EndDialog(hwnd, wParam);
		if(pDialog)
		{
			pDialog->m_hWnd = NULL;
		}
		pDialog = NULL;
		OutputDebugStringW(L"MainDialog closed\n");
		//return 1;
		break;
	default:
		break;
	}
	return 0;
}

bool CProgressDlg::OnError(WPARAM wParam, LPARAM lParam)
{
	wchar_t		*pBuffer = NULL;

	TCHAR		szFileNotExist[50] = {0};
	TCHAR		szCurrentFile[50]  = {0};
	TCHAR		szDecryptError[50] = {0};

	switch(wParam)
	{
	case LSC_ERR_SUCCESS:
		//nothing
		break;
	case LSC_ERR_SPACE_NOT_ENOUGH:
		break;
	case LSC_ERR_INPUT_FILE_NOT_FOUND:
		pBuffer = new wchar_t[MAX_PATH + 64];
		lstrcat(szCurrentFile,(wchar_t *)lParam);
		lstrcat(szCurrentFile,szFileNotExist);
		//swprintf_s(pBuffer, MAX_PATH + 64, L"当前文件:\n\n%s\n\n不存在", (wchar_t *)lParam);
		::MessageBoxW(m_hWnd, szCurrentFile, PROJECT_NAME_W, MB_ICONWARNING);
		//忽略并继续
		break;
	case LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS:
		//跳过现存的文件
		//pBuffer = new wchar_t[MAX_PATH + 64];
		//swprintf_s(pBuffer, MAX_PATH + 64, L"目标文件", (wchar_t *)lParam);
		//暂时忽略这个错误, 内部直接跳过
		break;
	case LSC_ERR_DECRYPT_FILE_FORMAT:
		//解密文件格式错误, 通常是解密时, 输入的文件是未加密过的或密码错误
		::MessageBoxW(m_hWnd,szDecryptError, PROJECT_NAME, MB_ICONWARNING);
		//取消任务
		AbortTask();
		break;
	case LSC_ERR_FILE_IS_DEFAULT_EXT:
		//encrypt
		//忽略并继续
		break;
	case LSC_ERR_FILE_IS_NOT_DEFAULT_EXT:
		//decrypt
		//忽略并继续
		break;
	case LSC_ERR_SYSTEM:
		//pBuffer = new wchar_t[256];
		//GetLastSystemErrorText(pBuffer, 256, (DWORD)lParam);
		//::MessageBoxW(m_hWnd, pBuffer, PROJECT_NAME, MB_ICONERROR);
		AbortTask();
		break;
	default:
		break;
	}

	delete [] pBuffer;
	pBuffer = NULL;

	return true;
}
