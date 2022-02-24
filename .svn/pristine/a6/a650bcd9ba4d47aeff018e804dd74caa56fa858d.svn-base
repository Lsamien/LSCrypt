
// Hyperlink.cpp -- Implementation of CHyperlink class
// Ryan Lederman -- ryan@winprog.org

#include "Hyperlink.h"

CHyperlink::CHyperlink()
{
	OutputDebugStringW(L"CHyperlink");
}
CHyperlink::~CHyperlink()
{
	OutputDebugStringW(L"~CHyperlink");


}


bool CHyperlink::Create( PHYPERLINKSTRUCT phs )
{
	int iFontBold = 0;
	int iFontUnderline = 0;
	int iHighlightFontUnderline = 0;

	if( !IsWindow( phs->hWndParent ) )
		return false;

	if( phs->width <= 0 || phs->height <= 0 )
		return false;

	if( !phs->szFontName )
		return false;

	m_hWnd = ::CreateWindow( TEXT("STATIC"), L"", WS_CHILD | WS_VISIBLE, phs->coordX, phs->coordY, phs->width, phs->height,
		phs->hWndParent, (HMENU)phs->ctlID, m_hInstance, NULL );

	if( m_hWnd == NULL )
		return false;

	if( ::SetProp( m_hWnd, TEXT("CLASSPOINTER"), (HANDLE)this ) == 0 )
		return false;
#ifndef GWL_WNDPROC
#define GWL_WNDPROC         (-4)
#endif
	SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)HyperlinkProc );

	phs->bBold ? iFontBold = FW_BOLD : iFontBold = FW_NORMAL;

	m_hFont = ::CreateFont( phs->fontsize ,0,0,0,iFontBold,0, phs->bUnderline,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH, phs->szFontName );

	if( m_hFont == NULL )
		return false;

	if( phs->bUseBg )
	{
		m_hBrushBackground = CreateSolidBrush( phs->clrBack );
		m_bUseBg = true;
	}
	else
	{	
		m_bUseBg = false;
	}

	m_bUseCursor = phs->bUseCursor; 

	if( m_bUseCursor )
		m_hCursor = phs->hCursor;

	m_rgbForeground = phs->clrText;
	m_rgbHighlight  = phs->clrHilite;
	m_rgbBackground = phs->clrBack;

	if( phs->pFn )
		m_pFn = phs->pFn;

	m_szBuffer = new TCHAR[lstrlen( phs->szCaption ) + 1];

	if( m_szBuffer == NULL )
		return false;

	memset( m_szBuffer, 0, lstrlen( phs->szCaption ) );
	lstrcpy( m_szBuffer, phs->szCaption );

	m_iLastState = -1;

	return true;
}

LRESULT CALLBACK CHyperlink::HyperlinkProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	long pThisClass = (long)::GetProp( hWnd, TEXT("CLASSPOINTER") );

	switch( msg )
	{
	case WM_PAINT: 
		((CHyperlink*)pThisClass)->OnDraw(); 
		break;
	case WM_MOUSEMOVE: ((CHyperlink*)pThisClass)->OnMouseMove(); break;
	case WM_LBUTTONDOWN: 
		if( ((CHyperlink*)pThisClass)->m_pFn )
			((CHyperlink*)pThisClass)->m_pFn();
		break;
	case WM_CAPTURECHANGED:
		((CHyperlink*)pThisClass)->OnMouseMove(); 
		break;
	case WM_SETCURSOR:
		if( ((CHyperlink*)pThisClass)->m_bUseCursor )
		{
			((CHyperlink*)pThisClass)->m_hStockCursor = SetCursor( ((CHyperlink*)pThisClass)->m_hCursor );
			return true;
		}
		return false;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void CHyperlink::OnDraw( void )
{
	PAINTSTRUCT ps = {0};
	HDC			dc;
	RECT		winRect = {0};

	dc = BeginPaint( m_hWnd, &ps );
	GetClientRect( m_hWnd, &winRect );

	if( m_bUseBg )
	{
		FillRect( dc, &winRect, m_hBrushBackground );
		SetBkColor( dc, m_rgbBackground );
	}
	else
	{
		SetBkMode( dc, TRANSPARENT );

	}

	SelectObject( dc, m_hFont );

	if( m_bMouseOver )
	{
		SetTextColor( dc, m_rgbHighlight );
	}
	else
	{
		SetTextColor( dc, m_rgbForeground );
	}

	::DrawTextEx( dc, m_szBuffer, lstrlen( m_szBuffer ), &winRect, DT_VCENTER | DT_CENTER, NULL );

	EndPaint( m_hWnd, &ps );
}

void CHyperlink::OnMouseMove()
{
	RECT  winRect = {0};
	POINT pt	  = {0};

	GetCursorPos( &pt );

	GetWindowRect( m_hWnd, &winRect );

	if( PtInRect( &winRect, pt ) )
	{
		SetCapture( m_hWnd );
		m_bMouseOver = true;

		if( m_iLastState != S_MOUSEOVER )
		{
			InvalidateRect( m_hWnd, NULL, TRUE );
			UpdateWindow( m_hWnd );
		}
		m_iLastState = S_MOUSEOVER;
	}
	else
	{
		ReleaseCapture();
		m_bMouseOver = false;

		if( m_iLastState != S_MOUSENOTOVER )
		{
			InvalidateRect( m_hWnd, NULL, TRUE );
			UpdateWindow( m_hWnd );
		}
		m_iLastState = S_MOUSENOTOVER;
	}
}

void CHyperlink::Destroy( void )
{
	if (m_hFont)
	{
		DeleteObject( m_hFont );
	}
	if (m_hBrushBackground)
	{
		DeleteObject( m_hBrushBackground );
	}
	DestroyWindow( m_hWnd );
	if( m_bUseCursor )
		DeleteObject( m_hCursor );

	if( m_szBuffer )
		delete [] m_szBuffer;
}

