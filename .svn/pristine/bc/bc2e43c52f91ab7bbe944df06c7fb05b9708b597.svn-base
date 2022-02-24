#include "stdafx.h"
#include "About.h"
#include "Hyperlink.h"
#include "LSCryptShellExt.h"

extern HINSTANCE	g_hInst;
CHyperlink			g_HyperRegistry;
HICON				m_hIcon;


#ifdef SUPPORT_REGISTRY
HWND	m_hWnd;
#endif

INT_PTR CALLBACK AboutDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD		lw = LOWORD(wParam);
	TCHAR		szTitle[20]		= {0};
	TCHAR		szStatic[20]	= {0};
	TCHAR		szButn[20]		= {0};
	
	TCHAR	szIconPath[MAX_PATH] = {0};

	switch(msg)
	{
	case WM_INITDIALOG:

		m_hWnd = hwnd;
		//center window
		int			cx, cy;
		RECT		rect;

		cx = GetSystemMetrics(SM_CXSCREEN);
		cy = GetSystemMetrics(SM_CYSCREEN);


		lstrcpy(szIconPath,pExt->m_szDllDir);
		lstrcat(szIconPath,TEXT("EA£­Key3.0.ico"));

		if (GetFileAttributes(szIconPath) == (DWORD)INVALID_HANDLE_VALUE)
		{
			m_hIcon	= (HICON)LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_CRYPT));
		}
		else
		{
			m_hIcon	= (HICON)LoadImage(g_hInst,szIconPath,IMAGE_ICON,0,0,LR_LOADFROMFILE);
		}

#ifdef SUPPORT_REGISTRY
		HWND	hStatic;
		hStatic = GetDlgItem(hwnd,IDC_STATIC_INFO);
		if (pExt->m_bRegistered)
		{
			switch(PRIMARYLANGID(GetUserDefaultLangID()))
			{
			case LANG_ENGLISH:
				LoadString(g_hInst,IDS_DLG_ABOUT_STATIC_REG_EN,szStatic,sizeof(szStatic));
				LoadString(g_hInst,IDS_DLG_ABOUT_TITLE_EN,szTitle,sizeof(szTitle));
				LoadString(g_hInst,IDS_DLG_ABOUT_BUTN_OK_EN,szButn,sizeof(szButn));
				break;
			case LANG_CHINESE:
				LoadString(g_hInst,IDS_DLG_ABOUT_STATIC_REG_CH,szStatic,sizeof(szStatic));
				LoadString(g_hInst,IDS_DLG_ABOUT_TITLE_CH,szTitle,sizeof(szTitle));
				LoadString(g_hInst,IDS_DLG_ABOUT_BUTN_OK_CH,szButn,sizeof(szButn));
			    break;
			}
		} 
		else
		{
			switch(PRIMARYLANGID(GetUserDefaultLangID()))
			{
			case LANG_ENGLISH:
				LoadString(g_hInst,IDS_DLG_ABOUT_STATIC_UNREG_EN,szStatic,sizeof(szStatic));
				LoadString(g_hInst,IDS_DLG_ABOUT_TITLE_EN,szTitle,sizeof(szTitle));
				LoadString(g_hInst,IDS_DLG_ABOUT_BUTN_OK_EN,szButn,sizeof(szButn));
				break;
			case LANG_CHINESE:
				LoadString(g_hInst,IDS_DLG_ABOUT_STATIC_UNREG_CH,szStatic,sizeof(szStatic));
				LoadString(g_hInst,IDS_DLG_ABOUT_TITLE_CH,szTitle,sizeof(szTitle));
				LoadString(g_hInst,IDS_DLG_ABOUT_BUTN_OK_CH,szButn,sizeof(szButn));
				break;
			}
			CreateHyperlinkControls(hwnd);
		}

		SetDlgItemText(m_hWnd,IDC_STATIC_INFO,szStatic);
		SetWindowText(m_hWnd,szTitle);
		SetDlgItemText(m_hWnd,IDOK,szButn);
#else
		ShowWindow(hStatic,SW_HIDE);
#endif
		GetWindowRect(hwnd, &rect);
		SetWindowPos(hwnd, HWND_TOP, (cx - (rect.right - rect.left))/2, (cy - (rect.bottom - rect.top))/2, 
			0, 0, SWP_NOSIZE);
		break;
	case WM_DRAWITEM:
		OnDrawItem(wParam,lParam);
		break;
	case WM_COMMAND:
		if(lw == IDOK || lw == IDCANCEL)
		{
			EndDialog(hwnd, IDOK);
			return 1;
		}
		break;
	case WM_DESTROY:
		g_HyperRegistry.Destroy();
		break;
	default:
		break;
	}
	return 0;
}

void CreateHyperlinkControls( HWND hWnd )
{
	HYPERLINKSTRUCT hls = {0};
	HCURSOR			hCursor;
	TCHAR			szText[20] = {0};


	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{
	case LANG_ENGLISH:
		LoadString(g_hInst,IDS_DLG_ABOUT_HYPERLINK_REG_EN,szText,sizeof(szText));
		break;
	case LANG_CHINESE:
		LoadString(g_hInst,IDS_DLG_ABOUT_HYPERLINK_REG_CH,szText,sizeof(szText));
	    break;
	}
	hCursor			= LoadCursor( g_hInst, MAKEINTRESOURCE(IDC_MY_HAND));
	hls.bBold		= false;				// Bold font?
	hls.bUnderline	= true;				// Underlined font?
	hls.bUseBg		= false;			// Use opaque background?
	hls.bUseCursor	= true;				// Use custom cursor?
	hls.hCursor		= hCursor;
	hls.clrBack		= RGB(255,255,255);			// Fill this out if bUseBg is true
	hls.clrHilite	= RGB( 255, 0, 0 );	// Color of mouseover font
	hls.clrText		= RGB( 0, 0, 255 );	// Color of regular font
	hls.coordX		= 110;					// X-axis coordinate of upper left corner
	hls.coordY		= 118;					// Y-axis coordinate of upper left corner
	hls.ctlID		= IDC_INPUT_SERINAL;					// Set this to use GetDlgItem on this control
	hls.fontsize	= 14;					// Size of font used
	hls.height		= 18;					// Height of control
	hls.hWndParent	= hWnd;              // Handle to parent window
	hls.pFn			= OnRegistry;		// Function pointer to function that is called when link is clicked
	hls.szCaption	= szText; // Caption
	hls.szFontName	= TEXT("Arial");			// Font face name
	hls.width		= 100;				// Width of control

	if( !g_HyperRegistry.Create( &hls ) )
	{
		MessageBox( 0, TEXT("Failed to create hyperlink 1."), TEXT("Fatal error!"), MB_OK | MB_ICONSTOP );
	}
}

void OnRegistry(void)
{
	/*
	CInputSerinalDlg	dlg(m_hWnd);

	dlg.m_pExpired		= NULL;
	dlg.m_pRegistered	= &pExt->m_bRegistered;
	dlg.m_pV3Key		= &pExt->m_V3FileKey;

	dlg.DoModal();
	*/
	pExt->RunRegister(pExt->m_szKeyFilePath,TEXT("LSCrypt"));
	SendMessage(m_hWnd,WM_CLOSE,0,0);
}

void OnDrawItem(WPARAM wParam,LPARAM lParam)
{
	int					nIDCtl				= (int)wParam;
	LPDRAWITEMSTRUCT	lpDrawItemStruct	= (LPDRAWITEMSTRUCT)lParam; 

	HWND	hIconWnd;
	HDC		hIconDC;
	RECT	rect;

	if (nIDCtl == IDC_ICON)
	{
		hIconDC		=lpDrawItemStruct->hDC;
		hIconWnd	= GetDlgItem(m_hWnd,IDC_ICON);
		GetClientRect(hIconWnd,&rect);

		/*
		hIconWnd = GetDlgItem(m_hWnd,IDC_ICON);
		hIconDC	 = GetDC(hIconWnd);
		GetClientRect(hIconWnd,&rect);
		*/
		DrawIcon(hIconDC,rect.left,rect.top,m_hIcon);

		/*
		GetObject(m_hBitmapLogo,sizeof(BMPINFO),&BMPINFO);
		hMemDC	= CreateCompatibleDC(hStaticDC);
		HBITMAP	hOldBitmap = (HBITMAP)SelectObject(hMemDC,m_hBitmapLogo);
		StretchBlt(hStaticDC,rectStatic.left,rectStatic.top,
			rectStatic.right-rectStatic.left,
			rectStatic.bottom-rectStatic.top,hMemDC,
			0,0,BMPINFO.bmWidth,BMPINFO.bmHeight,SRCCOPY);
		SelectObject(hMemDC,hOldBitmap);
		*/
	}
}