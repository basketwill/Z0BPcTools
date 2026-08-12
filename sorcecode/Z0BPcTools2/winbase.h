#ifndef _CWinBase_h
#define _CWinBase_h

// define useful macros from WindowsX.h
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))
#endif

/************************************************************************/
/* gdi                                                                     */
/************************************************************************/
typedef void* HGDIOBJ;
class CGDI_Data
{
public:
	// Constructor
	CGDI_Data() : hGDIObject(0), Count(1L), IsManagedObject(FALSE) {}

	HGDIOBJ hGDIObject;
	LONG	Count;
	bool	IsManagedObject;

};

// Forward declarations
class CPoint;
class CRect;


/////////////////////////////////////////
// Definition of the CSize class
// This class can be used to replace the SIZE structure
class CSize : public SIZE
{
public:
	CSize()								{ cx = 0; cy = 0; }
	CSize(int CX, int CY)				{ cx = CX; cy = CY; }
	CSize(SIZE sz)						{ cx = sz.cx; cy = sz.cy; }
	CSize(POINT pt)						{ cx = pt.x;  cy = pt.y; }
	CSize(DWORD dw)						{ cx = (short)LOWORD(dw); cy = (short)HIWORD(dw); }
	void SetSize(int CX, int CY)		{ cx = CX; cy = CY; }

	// Operators
	operator LPSIZE()					{ return this; }
	BOOL operator == (SIZE sz) const	{ return (cx == sz.cx && cy == sz.cy); }
	BOOL operator != (SIZE sz) const	{ return (cx != sz.cx || cy != sz.cy); }
	void operator += (SIZE sz)			{ cx += sz.cx; cy += sz.cy; }
	void operator -= (SIZE sz)			{ cx -= sz.cx; cy -= sz.cy; }

	// Operators returning CSize
	CSize operator - () const			{ return CSize (-cx, -cy); }
	CSize operator + (SIZE sz) const	{ return CSize (cx + sz.cx, cy + sz.cy); }
	CSize operator - (SIZE sz) const	{ return CSize (cx - sz.cx, cy - sz.cy); }

	// Operators returning CPoint
	CPoint operator + (POINT point) const;
	CPoint operator - (POINT point) const;

	// Operators returning CRect
	CRect operator + (LPCRECT prc) const;
	CRect operator - (LPCRECT prc) const;
};


/////////////////////////////////////////
// Definition of the CPoint class
// This class can be used to replace the POINT structure
class CPoint : public POINT
{
public:
	CPoint()							{ x = 0; y = 0; }
	CPoint(int X, int Y)				{ x = X; y = Y; }
	CPoint(POINT pt)					{ x = pt.x ; y = pt.y; }
	CPoint(POINTS pts)					{ x = pts.x; y = pts.y; }
	CPoint(SIZE sz)						{ x = sz.cx; y = sz.cy; }
	CPoint(LPARAM dwPos)				{ x = GET_X_LPARAM(dwPos); y = GET_Y_LPARAM(dwPos); }

	void Offset(int dx, int dy)			{ x += dx; y += dy; }
	void Offset(POINT pt)				{ x += pt.x; y += pt.y; }
	void Offset(SIZE sz)				{ x += sz.cx; y += sz.cy; }
	void SetPoint(int X, int Y)			{ x = X; y = Y; }

	// Operators
	operator LPPOINT()					{ return this; }
	BOOL operator == (POINT pt) const	{ return ((x == pt.x) && (y == pt.y)); }
	BOOL operator != (POINT pt) const	{ return ((x != pt.x) || (y != pt.y)); }
	void operator += (SIZE sz)			{ x += sz.cx; y += sz.cy; }
	void operator -= (SIZE sz)			{ x -= sz.cx; y -= sz.cy; }
	void operator += (POINT pt)			{ x += pt.x; y += pt.y; }
	void operator -= (POINT pt)			{ x -= pt.x; y -= pt.y; }

	// Operators returning CPoint
	CPoint operator - () const			{ return CPoint(-x, -y); }
	CPoint operator + (SIZE sz) const	{ return CPoint(x + sz.cx, y + sz.cy); }
	CPoint operator - (SIZE sz) const	{ return CPoint(x - sz.cx, y - sz.cy); }
	CPoint operator + (POINT pt) const	{ return CPoint(x + pt.x, y + pt.y); }
	CPoint operator - (POINT pt) const	{ return CPoint(x - pt.x, y - pt.y); }

	// Operators returning CRect
	CRect operator + (LPCRECT prc) const;
	CRect operator - (LPCRECT prc) const;
};


/////////////////////////////////////////
// Definition of the CRect class
// This class can be used to replace the RECT structure.
class CRect : public RECT
{
public:
	CRect()										{ left = top = right = bottom = 0; }
	CRect(int l, int t, int r, int b)			{ left = l; top = t; right = r; bottom = b; }
	CRect(const RECT& rc)						{ left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }
	CRect(POINT pt, SIZE sz)					{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }
	CRect(POINT topLeft, POINT bottomRight)		{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

	BOOL CopyRect(LPCRECT prc)					{ return ::CopyRect(this, prc); }
	BOOL DeflateRect(int x, int y)				{ return ::InflateRect(this, -x, -y); }
	BOOL DeflateRect(SIZE size)					{ return ::InflateRect(this, -size.cx, -size.cy); }
	void DeflateRect(LPCRECT prc)				{ left += prc->left; top += prc->top; right -= prc->right; bottom -= prc->bottom; }
	void DeflateRect(int l, int t, int r, int b){ left += l; top += t; right -= r; bottom -= b; }
	BOOL EqualRect(LPRECT prc) const			{ return ::EqualRect(prc, this); }
	BOOL InflateRect(int dx, int dy)			{ return ::InflateRect(this, dx, dy); }
	BOOL InflateRect(SIZE sz)					{ return ::InflateRect(this, sz.cx, sz.cy); }
	void InflateRect(LPCRECT prc)				{ left -= prc->left; top -= prc->top; right += prc->right; bottom += prc->bottom; }
	void InflateRect(int l, int t, int r, int b){ left -= l; top -= t; right += r; bottom += b; }
	BOOL IntersectRect(LPCRECT prc1, LPCRECT prc2)	{ return ::IntersectRect(this, prc1, prc2); }
	BOOL IsRectEmpty() const					{ return ::IsRectEmpty(this);}
	BOOL IsRectNull() const						{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
	CRect MulDiv(int nMult, int nDiv) const		{ return CRect ((left * nMult) / nDiv, (top * nMult) / nDiv,
		(right * nMult) / nDiv, (bottom * nMult) / nDiv); }
	void NormalizeRect()						{ int nTemp; if (left > right) { nTemp = left; left = right; right = nTemp; }
	if (top > bottom) { nTemp = top; top = bottom; bottom = nTemp; } }
	BOOL OffsetRect(int dx, int dy)				{ return ::OffsetRect(this, dx, dy); }
	BOOL OffsetRect(POINT pt)					{ return ::OffsetRect(this, pt.x, pt.y); }
	BOOL OffsetRect(SIZE size)					{ return ::OffsetRect(this, size.cx, size.cy); }
	BOOL PtInRect(POINT pt) const				{ return ::PtInRect(this, pt); }
	BOOL SetRect(int l, int t, int r, int b)	{ return ::SetRect(this, l, t, r, b); }
	BOOL SetRect(POINT TopLeft, POINT BtmRight)	{ return ::SetRect(this, TopLeft.x, TopLeft.y, BtmRight.x, BtmRight.y); }
	BOOL SetRectEmpty()							{ return ::SetRectEmpty(this); }
	BOOL SubtractRect(LPCRECT prc1, LPCRECT prc2)	{ return ::SubtractRect(this, prc1, prc2); }
	BOOL UnionRect(LPCRECT prc1, LPCRECT prc2)	{ return ::UnionRect(this, prc1, prc2); }

	// Reposition rectangle
	void MoveToX (int x)						{ right = Width() + x; left = x; }
	void MoveToY (int y)						{ bottom = Height() + y; top = y; }
	void MoveToXY (int x, int y)				{ MoveToX(x); MoveToY(y); }
	void MoveToXY (POINT pt)					{ MoveToX (pt.x); MoveToY (pt.y); }

	// Attributes
	int Height() const							{ return bottom - top; }
	int Width() const							{ return right - left; }
	CSize Size() const							{ return CSize(Width(), Height()); }
	CPoint CenterPoint() const					{ return CPoint((left + right) / 2, (top + bottom) / 2); }
	CPoint TopLeft() const						{ return CPoint(left, top); }
	CPoint BottomRight() const					{ return CPoint(right, bottom); }

	// operators
	operator LPRECT()							{ return this; }
	operator LPCRECT() const					{ return this; }
	BOOL operator == (const RECT& rc) const		{ return ::EqualRect(this, &rc); }
	BOOL operator != (const RECT& rc) const		{ return !::EqualRect(this, &rc); }
	void operator += (POINT pt)					{ ::OffsetRect(this, pt.x, pt.y); }
	void operator += (SIZE size)				{ ::OffsetRect(this, size.cx, size.cy); }
	void operator += (LPCRECT prc)				{ ::InflateRect(this, prc->right - prc->left, prc->bottom - prc->top); }
	void operator -= (LPCRECT prc)				{ ::InflateRect(this, prc->left - prc->right, prc->top - prc->bottom); }
	void operator -= (POINT pt)					{ ::OffsetRect(this, -pt.x, -pt.y); }
	void operator -= (SIZE sz)					{ ::OffsetRect(this, -sz.cx, -sz.cy); }
	void operator &= (const RECT& rc)			{ ::IntersectRect(this, this, &rc); }
	void operator |= (const RECT& rc)			{ ::UnionRect(this, this, &rc); }

	// Operators returning CRect
	CRect operator + (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, pt.x, pt.y); return rc; }
	CRect operator - (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, -pt.x, -pt.y); return rc; }
	CRect operator + (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, sz.cx, sz.cy); return rc; }
	CRect operator - (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, -sz.cx, -sz.cy); return rc; }
	CRect operator + (LPRECT prc) const			{ CRect rc1(*this); rc1.InflateRect(prc); return rc1; }
	CRect operator - (LPRECT prc) const			{ CRect rc1(*this); rc1.DeflateRect(prc); return rc1; }
	CRect operator & (const RECT& rc) const		{ CRect rc1; ::IntersectRect(&rc1, this, &rc); return rc1; }
	CRect operator | (const RECT& rc) const		{ CRect rc1; ::UnionRect(&rc1, this, &rc); return rc1; }
};

// CSize member function definitions
inline CPoint CSize::operator + (POINT pt) const	{ return CPoint(pt) + *this; }
inline CPoint CSize::operator - (POINT pt) const	{ return CPoint(pt) - *this; }
inline CRect CSize::operator + (LPCRECT prc) const	{ return CRect(*prc) + *this; }
inline CRect CSize::operator - (LPCRECT prc) const	{ return CRect(*prc) - *this; }

// CPoint member function definitions
inline CRect CPoint::operator + (LPCRECT prc) const	{ return CRect(*prc) + *this; }
inline CRect CPoint::operator - (LPCRECT prc) const	{ return CRect(*prc) - *this; }


////////////////////////////////////////
// Global Functions
//	
inline CPoint GetCursorPos()
{
	CPoint pt;
	::GetCursorPos(&pt);
	return pt;
}

class CDC;

class CGDIObject
{
public:
	inline CGDIObject()
	{
		m_pData = new CGDI_Data;
	}

	inline CGDIObject(const CGDIObject& rhs)
	{
		m_pData = rhs.m_pData;
		InterlockedIncrement(&m_pData->Count);
	}

	virtual ~CGDIObject()
	{
		Release();
	}

	inline CGDIObject& operator = (const CGDIObject& rhs)
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	void operator = (const HGDIOBJ hObject)
	{
		Attach(hObject);
	}

	inline void	Attach(HGDIOBJ hObject)
	{
		if (hObject != m_pData->hGDIObject)
		{
			// Release any existing GDI object
			if (m_pData->hGDIObject != 0)
			{
				Release();
				m_pData = new CGDI_Data;
			}

			if (hObject)
			{				
				m_pData->hGDIObject = hObject;				
			}
		}
	}

	inline void	DeleteObject()
	{
		if (m_pData->hGDIObject != 0)
		{			
			::DeleteObject(m_pData->hGDIObject);
			m_pData->hGDIObject = 0;
		}
	}

	inline HGDIOBJ Detach()
	{
		HGDIOBJ hObject = m_pData->hGDIObject;
		
		m_pData->hGDIObject = 0;

		if (m_pData->Count > 0)
		{
			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				delete m_pData;
			}
		}

		m_pData = new CGDI_Data;
		return hObject;
	}

	inline HGDIOBJ GetHandle() const
	{
		return m_pData->hGDIObject;
	}

	inline int	GetObject(int nCount, LPVOID pObject) const
	{
		return ::GetObject(m_pData->hGDIObject, nCount, pObject);
	}

protected:
	void	Release()
	{
		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hGDIObject != NULL)
			{
				if (m_pData->IsManagedObject)
				{
					::DeleteObject(m_pData->hGDIObject);
				}
			}

			delete m_pData;
			m_pData = 0;
		}
	}

	void	SetManaged(bool IsManaged) const { m_pData->IsManagedObject = IsManaged; }

private:
	BOOL	RemoveFromMap()
	{
		return TRUE;
	}

	CGDI_Data*  m_pData;
};

class CFont : public CGDIObject
{
public:
	inline CFont()
	{

	}

	inline CFont(HFONT hFont)
	{
		Attach(hFont);
	}

	inline CFont(const LOGFONT& LogFont)
	{
		try
		{
			CreateFontIndirect(LogFont);
		}
		catch(...)
		{
			Release();	// Cleanup
			throw;		// Rethrow;
		}
	}

	inline operator HFONT() const
	{
		return static_cast<HFONT>(GetHandle());
	}

	inline virtual ~CFont()
	{

	}

	// Create methods
	inline HFONT CreateFontIndirect(const LOGFONT& LogFont)
	{
		HFONT hFont = ::CreateFontIndirect(&LogFont);
		if (hFont == 0)
			return hFont;

		Attach(hFont);
		SetManaged(true);
		return hFont;
	}

	inline HFONT CreatePointFont(
		int nPointSize, 
		LPTSTR lpszFaceName,
		HDC hdc = NULL,
		BOOL IsBold = FALSE, 
		BOOL IsItalic = FALSE)
	{
		LOGFONT logFont;
		ZeroMemory(&logFont, sizeof(LOGFONT));
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = nPointSize;

		lstrcpyn(logFont.lfFaceName, lpszFaceName, LF_FACESIZE);

		if (IsBold)
			logFont.lfWeight = FW_BOLD;
		if (IsItalic)
			logFont.lfItalic = (BYTE)TRUE;

		return CreatePointFontIndirect(logFont, hdc);
	}

	HFONT CreatePointFontIndirect(const LOGFONT& LogFont, HDC hdc = NULL)
	{		
		HDC hDC1 = (hdc != NULL) ? hdc : ::GetDC(HWND_DESKTOP);
		
		// convert nPointSize to logical units based on hDC
		LOGFONT logFont = LogFont;

#ifndef _WIN_W
		POINT pt = { 0, 0 };
		pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), logFont.lfHeight, 720);
		::DPtoLP(hDC1, &pt, 1);

		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);

		logFont.lfHeight = -abs(pt.y - ptOrg.y);
#else // CE specific
		// DP and LP are always the same on CE
		logFont.lfHeight = -abs(((::GetDeviceCaps(hDC1, LOGPIXELSY)* logFont.lfHeight)/ 720));
#endif // _WIN_W
		
		return CreateFontIndirect (logFont);
	}

#ifndef _WIN_W
	inline HFONT CreateFont(int nHeight, int nWidth, int nEscapement,
		int nOrientation, int nWeight, DWORD dwItalic, DWORD dwUnderline,
		DWORD dwStrikeOut, DWORD dwCharSet, DWORD dwOutPrecision,
		DWORD dwClipPrecision, DWORD dwQuality, DWORD dwPitchAndFamily,
		LPCTSTR lpszFacename)
	{
		HFONT hFont = ::CreateFont(nHeight, nWidth, nEscapement,
			nOrientation, nWeight, dwItalic, dwUnderline, dwStrikeOut,
			dwCharSet, dwOutPrecision, dwClipPrecision, dwQuality,
			dwPitchAndFamily, lpszFacename);

		if (hFont == 0)
			return hFont;

		Attach(hFont);
		SetManaged(true);
		return hFont;
	}
#endif // #ifndef _WIN_W

	// Attributes
	LOGFONT GetLogFont() const
	{
		LOGFONT LogFont;
		ZeroMemory(&LogFont, sizeof(LOGFONT));
		::GetObject(GetHandle(), sizeof(LOGFONT), &LogFont);
		return LogFont;
	}
};



class CBitmap : public CGDIObject
{

public:
	inline CBitmap()
	{

	}

	inline CBitmap(HBITMAP hBitmap)
	{
		Attach(hBitmap);
	}

	inline CBitmap(LPTSTR lpszName)
	{
		LoadBitmap(lpszName);
	}

	inline CBitmap(int nID)
	{
		LoadBitmap(nID);
	}

	inline operator HBITMAP() const
	{
		return static_cast<HBITMAP>(GetHandle());
	}

	inline virtual ~CBitmap()
	{

	}

	// Create and load methods
	inline BOOL LoadBitmap(LPTSTR lpszName)
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, lpszName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if (hBitmap != 0)
		{
			Attach(hBitmap);
			SetManaged(true);
		}

		return (0 != hBitmap);	// boolean expression
	}

	inline BOOL LoadBitmap(int nID)
	{
		return LoadBitmap(MAKEINTRESOURCE(nID));
	}

	inline BOOL LoadImage(LPTSTR lpszName, UINT fuLoad)
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, lpszName, IMAGE_BITMAP, 0, 0, fuLoad);
		if (hBitmap != 0)
		{
			Attach(hBitmap);
			SetManaged(true);
		}
		return (0 != hBitmap);	// boolean expression
		
	}

	inline BOOL LoadImage(UINT nID, UINT fuLoad)
	{
		return LoadImage(MAKEINTRESOURCE(nID), fuLoad);
	}

	inline BOOL LoadOEMBitmap(UINT nIDBitmap)
	{
		HBITMAP hBitmap = ::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap));
		if (hBitmap != 0)
		{
			Attach( ::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap)) );
			SetManaged(true);
		}
		return (0 != hBitmap);	// boolean expression
	}

	inline HBITMAP CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitsPerPixel, LPCVOID lpBits)
	{
		HBITMAP hBitmap = ::CreateBitmap(nWidth, nHeight, nPlanes, nBitsPerPixel, lpBits);
		if (hBitmap == 0)
			return NULL;

		Attach(hBitmap);
		SetManaged(true);
		return hBitmap;
	}

	inline HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight)
	{
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, nWidth, nHeight);
		if (hBitmap == 0)
		{
			return NULL;
		}

		Attach(hBitmap);
		SetManaged(true);
		return hBitmap;
	}

	inline HBITMAP CreateDIBSection(HDC hdc, const LPBITMAPINFO pbmi, UINT uColorUse, LPVOID* ppvBits, HANDLE hSection, DWORD dwOffset)
	{
		HBITMAP hBitmap = ::CreateDIBSection(hdc, pbmi, uColorUse, ppvBits, hSection, dwOffset);
		Attach(hBitmap);
		SetManaged(true);
		return hBitmap;
	}

#ifndef _WIN32_WCE
	inline HBITMAP CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER* pbmih, DWORD dwInit, LPCVOID pbInit, const LPBITMAPINFO pbmi, UINT uColorUse)
	{
		HBITMAP hBitmap = ::CreateDIBitmap(hdc, pbmih, dwInit, pbInit, pbmi, uColorUse);
		Attach(hBitmap);
		SetManaged(true);
		return hBitmap;
	}
	//HBITMAP CreateMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
	inline HBITMAP CreateBitmapIndirect(const BITMAP& Bitmap)
	{
		HBITMAP hBitmap = ::CreateBitmapIndirect(&Bitmap);
		if (hBitmap == 0)
			return hBitmap;

		Attach(hBitmap);
		SetManaged(true);
		return hBitmap;
	}
	
	//void TintBitmap (int cRed, int cGreen, int cBlue);
	inline int GetDIBits(HDC hdc, UINT uStartScan, UINT cScanLines,  LPVOID pBits, LPBITMAPINFO pbmi, UINT uColorUse) const
	{
		return ::GetDIBits(hdc, (HBITMAP)GetHandle(), uStartScan, cScanLines,  pBits, pbmi, uColorUse);
	}
	inline int SetDIBits(HDC hdc, UINT uStartScan, UINT cScanLines, LPCVOID pBits, const LPBITMAPINFO pbmi, UINT uColorUse) const
	{
		return ::SetDIBits(hdc, (HBITMAP)GetHandle(), uStartScan, cScanLines, pBits, pbmi, uColorUse);
	}

	CSize GetBitmapDimensionEx() const
	{
		CSize Size;
		::GetBitmapDimensionEx((HBITMAP)GetHandle(), &Size);
		return Size;
	}
	CSize SetBitmapDimensionEx(int nWidth, int nHeight) const
	{
		CSize Size;
		::SetBitmapDimensionEx((HBITMAP)GetHandle(), nWidth, nHeight, Size);
		return Size;
	}
#endif // !_WIN32_WCE

	// Attributes
	inline BITMAP GetBitmapData() const
	{
		BITMAP bmp;
		ZeroMemory(&bmp, sizeof(BITMAP));
		::GetObject(GetHandle(), sizeof(BITMAP), &bmp);
		return bmp;
	}
};

class CBrush : public CGDIObject
{
public:
	inline CBrush()
	{

	}

	inline CBrush(HBRUSH hBrush)
	{
		Attach(hBrush);
	}

	inline CBrush(COLORREF crColor)
	{
		try
		{
			CreateSolidBrush(crColor);
		}
		catch(...)
		{
			Release();	// Cleanup
			throw;		// Rethrow
		}
	}

	inline operator HBRUSH() const
	{
		return static_cast<HBRUSH>(GetHandle());
	}

	inline virtual ~CBrush()
	{

	}

	inline HBRUSH CreateSolidBrush(COLORREF crColor)
	{
		HBRUSH hBrush = ::CreateSolidBrush(crColor);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

	inline HBRUSH CreatePatternBrush(HBITMAP hBitmap)
	{
		HBRUSH hBrush = ::CreatePatternBrush(hBitmap);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

	inline LOGBRUSH GetLogBrush() const
	{
		LOGBRUSH LogBrush;
		ZeroMemory(&LogBrush, sizeof(LOGBRUSH));
		::GetObject (GetHandle(), sizeof(LOGBRUSH), &LogBrush);
		return LogBrush;
	}

#ifndef _WIN_W
	inline HBRUSH CreateHatchBrush(int nIndex, COLORREF crColor)
	{
		HBRUSH hBrush = ::CreateHatchBrush(nIndex, crColor);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

	HBRUSH CreateBrushIndirect(const LOGBRUSH& LogBrush)
	{
		HBRUSH hBrush = ::CreateBrushIndirect(&LogBrush);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

	HBRUSH CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
	{
		HBRUSH hBrush = ::CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

	HBRUSH CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT nUsage)
	{
		HBRUSH hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
		if (hBrush == 0)
			return hBrush;

		Attach(hBrush);
		SetManaged(true);
		return hBrush;
	}

#endif // !defined(_WIN_W)

};

class CPen : public CGDIObject
{
public:
	CPen();
	CPen(HPEN hPen);
	CPen(int nPenStyle, int nWidth, COLORREF crColor);
#ifndef _WIN_W
	CPen(int nPenStyle, int nWidth, const LOGBRUSH& LogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
#endif // !_WIN_W
	operator HPEN() const;
	virtual ~CPen();

	HPEN CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
	HPEN CreatePenIndirect(const LOGPEN& LogPen);
	LOGPEN GetLogPen() const;

#ifndef _WIN_W
	HPEN ExtCreatePen(int nPenStyle, int nWidth, const LOGBRUSH& LogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL) ;
	EXTLOGPEN GetExtLogPen() const;
#endif // !_WIN_W

};

class CPalette : public CGDIObject
{
public:
	CPalette();
	CPalette(HPALETTE hPalette);
	operator HPALETTE() const;
	virtual ~CPalette();

	// Create methods
	HPALETTE CreatePalette(LPLOGPALETTE lpLogPalette);

#ifndef _WIN_W
	HPALETTE CreateHalftonePalette(HDC hdc);
#endif // !_WIN_W

	// Attributes
	int GetEntryCount() const;
	UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
	UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;

	// Operations
#ifndef _WIN_W
	BOOL ResizePalette(UINT nNumEntries) const;
	void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
#endif // !_WIN_W

	UINT GetNearestPaletteIndex (COLORREF crColor) const;

};

inline CPalette::CPalette()
{
}

inline CPalette::CPalette(HPALETTE hPalette)
{
	Attach(hPalette);
}

inline CPalette::operator HPALETTE() const
{
	return static_cast<HPALETTE>(GetHandle());
}

inline CPalette::~CPalette ()
{
}

inline HPALETTE CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
	// Creates a logical palette from the information in the specified LOGPALETTE structure.
{
	HPALETTE hPalette = ::CreatePalette (lpLogPalette);
	if (hPalette == 0)
		return hPalette;

	Attach(hPalette);
	SetManaged(true);
	return hPalette;
}

#ifndef _WIN_W
inline HPALETTE CPalette::CreateHalftonePalette(HDC hdc)
	// Creates a halftone palette for the specified device context (DC).
{
	HPALETTE hPalette = ::CreateHalftonePalette(hdc);
	if (hPalette == 0)
		return hPalette;

	Attach(hPalette);
	::RealizePalette(hdc);
	SetManaged(true);
	return hPalette;
}
#endif // !_WIN_W

inline int CPalette::GetEntryCount() const
	// Retrieve the number of entries in the palette.
{
	WORD nEntries = 0;
	::GetObject(GetHandle(), sizeof(WORD), &nEntries);
	return static_cast<int>(nEntries);
}

inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	// Retrieves a specified range of palette entries from the palette.
{
	return ::GetPaletteEntries((HPALETTE)GetHandle(), nStartIndex, nNumEntries, lpPaletteColors);
}

inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	// Sets RGB (red, green, blue) color values and flags in a range of entries in the palette.
{
	return ::SetPaletteEntries((HPALETTE)GetHandle(), nStartIndex, nNumEntries, lpPaletteColors);
}

#ifndef _WIN_W
inline void CPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	// Replaces entries in the palette.
{
	::AnimatePalette((HPALETTE)GetHandle(), nStartIndex, nNumEntries, lpPaletteColors);
}

inline BOOL CPalette::ResizePalette(UINT nNumEntries) const
	//  Increases or decreases the size of the palette based on the specified value.
{
	return ::ResizePalette((HPALETTE)GetHandle(), nNumEntries);
}
#endif // !_WIN32_WCE

inline UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const
	// Retrieves the index for the entry in the palette most closely matching a specified color value.
{
	return ::GetNearestPaletteIndex((HPALETTE)GetHandle(), crColor);
}


inline CPen::CPen()
{
}

inline CPen::CPen(HPEN hPen)
{
	Attach(hPen);
}

inline CPen::CPen(int nPenStyle, int nWidth, COLORREF crColor)
{
	try
	{
		CreatePen(nPenStyle, nWidth, crColor);
	}
	catch(...)
	{
		Release();	// Cleanup
		throw;		// Rethrow
	}
}

#ifndef _WIN_W
inline CPen::CPen(int nPenStyle, int nWidth, const LOGBRUSH& LogBrush, int nStyleCount /*= 0*/, const DWORD* lpStyle /*= NULL*/)
{
	try
	{
		Attach(::ExtCreatePen(nPenStyle, nWidth, &LogBrush, nStyleCount, lpStyle));
	}
	catch(...)
	{
		Release();	// Cleanup
		throw;		// Rethrow
	}
}
#endif // !_WIN_W

inline CPen::operator HPEN () const
{
	return static_cast<HPEN>(GetHandle());
}

inline CPen::~CPen()
{
}

inline HPEN CPen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
	// Creates a logical pen that has the specified style, width, and color.
{
	HPEN hPen = ::CreatePen(nPenStyle, nWidth, crColor);
	Attach(hPen);
	SetManaged(true);
	return hPen;
}

inline HPEN CPen::CreatePenIndirect(const LOGPEN& LogPen)
	// Creates a logical pen that has the style, width, and color specified in a structure.
{
	HPEN hPen = ::CreatePenIndirect(&LogPen);
	Attach(hPen);
	SetManaged(true);
	return hPen;
}

inline LOGPEN CPen::GetLogPen() const
{
	// Retrieves the LOGPEN struct that specifies the pen's style, width, and color.
	LOGPEN LogPen;
	ZeroMemory(&LogPen, sizeof(LOGPEN));
	::GetObject(GetHandle(), sizeof(LOGPEN), &LogPen);
	return LogPen;
}

#ifndef _WIN_W
inline HPEN CPen::ExtCreatePen(int nPenStyle, int nWidth, const LOGBRUSH& LogBrush, int nStyleCount /* = 0*/, const DWORD* lpStyle /*= NULL*/)
	// Creates a logical cosmetic or geometric pen that has the specified style, width, and brush attributes.
{
	HPEN hPen = ::ExtCreatePen(nPenStyle, nWidth, &LogBrush, nStyleCount, lpStyle);
	Attach(hPen);
	SetManaged(true);
	return hPen;
}

inline EXTLOGPEN CPen::GetExtLogPen() const
	// Retrieves the EXTLOGPEN struct that specifies the pen's style, width, color and brush attributes.
{
	EXTLOGPEN ExLogPen;
	ZeroMemory(&ExLogPen, sizeof(EXTLOGPEN));
	::GetObject(GetHandle(), sizeof(EXTLOGPEN), &ExLogPen);
	return ExLogPen;
}
#endif // !_WIN_W



class CRgn : public CGDIObject
{
public:
	inline CRgn()
	{

	}

	inline CRgn(HRGN hRgn)
	{
		Attach(hRgn);
	}

	inline operator HRGN() const
	{
		return static_cast<HRGN>(GetHandle());
	}

	inline virtual ~CRgn ()
	{

	}

	// Create methods
	inline HRGN CreateRectRgn(int x1, int y1, int x2, int y2)
	{
		HRGN hRgn = ::CreateRectRgn(x1, y1, x2, y2);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}

	inline HRGN CreateRectRgnIndirect(const RECT& rc)
	{
		HRGN hRgn = ::CreateRectRgnIndirect(&rc);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}

	inline HRGN CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	{
		HRGN hRgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}

#ifndef _WIN_W
	inline HRGN CreateEllipticRgn(int x1, int y1, int x2, int y2)
	{
		HRGN hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}

	inline HRGN CreateEllipticRgnIndirect(const RECT& rc)
	{
		HRGN hRgn = ::CreateEllipticRgnIndirect(&rc);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}

	inline HRGN CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
	{
		HRGN hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}
	inline HRGN CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	{
		HRGN hRgn = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}
	inline HRGN CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		HRGN hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}
	inline HRGN CreateFromPath(HDC hDC)
	{
		HRGN hRgn = ::PathToRegion(hDC);
		if (hRgn == 0)
			return hRgn;

		Attach(hRgn);
		SetManaged(true);
		return hRgn;
	}
#endif // !_WIN32_WCE

	// Operations
	inline void SetRectRgn(int x1, int y1, int x2, int y2) const
	{
		::SetRectRgn((HRGN)GetHandle(), x1, y1, x2, y2);
	}

	inline void SetRectRgn(const RECT& rc) const
	{
		::SetRectRgn((HRGN)GetHandle(), rc.left, rc.top, rc.right, rc.bottom);
	}

	inline int CombineRgn(HRGN hrgnSrc1, HRGN hrgnSrc2, int nCombineMode) const
	{
		return ::CombineRgn((HRGN)GetHandle(), hrgnSrc1, hrgnSrc2, nCombineMode);
	}

	inline int CombineRgn(HRGN hrgnSrc, int nCombineMode) const
	{
		return ::CombineRgn((HRGN)GetHandle(), (HRGN)GetHandle(), hrgnSrc, nCombineMode);
	}

	inline int CopyRgn(HRGN hrgnSrc) const
	{
		return ::CombineRgn((HRGN)GetHandle(), hrgnSrc, NULL, RGN_COPY);
	}

	inline BOOL EqualRgn(HRGN hRgn) const
	{
		return ::EqualRgn((HRGN)GetHandle(), hRgn);
	}

	inline int OffsetRgn(int x, int y) const
	{
		return ::OffsetRgn((HRGN)GetHandle(), x, y);
	}

	inline int OffsetRgn(POINT& pt) const
	{
		return ::OffsetRgn((HRGN)GetHandle(), pt.x, pt.y);
	}

	inline int GetRgnBox(RECT& rc) const
	{
		return ::GetRgnBox((HRGN)GetHandle(), &rc);
	}

	inline BOOL PtInRegion(int x, int y) const
	{
		return ::PtInRegion((HRGN)GetHandle(), x, y);
	}

	inline BOOL PtInRegion(POINT& pt) const
	{
		return ::PtInRegion((HRGN)GetHandle(), pt.x, pt.y);
	}

	inline BOOL RectInRegion(const RECT& rc) const
	{
		return ::RectInRegion((HRGN)GetHandle(), &rc);
	}

	inline int GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
	{
		return static_cast<int>(::GetRegionData((HRGN)GetHandle(), nDataSize, lpRgnData));
	}
};


class CDC
{
public:
	inline CDC()
	{
		m_hDC = NULL;
	}

	inline CDC(HDC hDC, HWND hWnd = 0)
	{
		Attach(hDC, hWnd);
	}

	inline CDC(const CDC& rhs)
	{
		Attach(rhs);
	}

	inline virtual ~CDC()
	{
		//Release();
	}

	inline operator HDC() const { return m_hDC; }
	
	inline CDC& operator = (const CDC& rhs)
	{
		if (this != &rhs)
		{
			Release();
			m_hDC = rhs.m_hDC;
		}

		return *this;
	}

	inline void Attach(HDC hDC, HWND hWnd = 0)
	{
		if (hDC != m_hDC)
		{
			if (m_hDC)
			{
				Release();
			}

			if (hDC)
			{
				m_hDC = hDC;				
			}
		}
	}

	inline void Destroy()
	{
		if (m_hDC != 0)
		{			
			if (!::DeleteDC(m_hDC))
					::ReleaseDC(NULL, m_hDC);

			m_hDC = 0;
		}
	}

	inline HDC  Detach()
	{
		HDC hDC = m_hDC;
		
		m_hDC = 0;

		return hDC;
	}

	inline HDC GetHDC() const { return m_hDC; }

	inline BOOL RestoreDC(int nSavedDC) const
	{
		return ::RestoreDC(m_hDC, nSavedDC);
	}

	int SaveDC() const
	{
		return ::SaveDC(m_hDC);
	}

	HGDIOBJ SelectObject(HGDIOBJ hObject) const
	{
		return ::SelectObject(m_hDC, hObject);
	}

	HBITMAP SelectObject(HBITMAP hBitmap) const
	{
		return (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	HBRUSH SelectObject(HBRUSH hBrush) const
	{
		return (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	HFONT SelectObject(HFONT hFont) const
	{
		return (HFONT)::SelectObject(m_hDC, hFont);
	}

	HPEN SelectObject(HPEN hPen) const
	{
		return (HPEN)::SelectObject(m_hDC, hPen);
	}

	int SelectObject(HRGN hRgn) const
	{
		return (int)(INT_PTR)::SelectObject(m_hDC, hRgn);
	}

	void FillSolidRect(LPCRECT lpRect, COLORREF clr)const;

#ifndef _WIN_W
	inline void operator = (const HDC hDC)
	{
		Attach(hDC);
	}
#endif

	// Initialization
	inline HDC CreateCompatibleDC(HDC hdcSource)
	{
		HDC hDC = ::CreateCompatibleDC(hdcSource);

		if (hDC == NULL)
			return hDC;

		m_hDC = hDC;
		
		return hDC;
	}

	inline HDC CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE* pInitData)
	{
		HDC hDC = ::CreateDC(lpszDriver, lpszDevice, lpszOutput, pInitData);

		if (hDC == NULL)
			return hDC;

		m_hDC = hDC;
		
		return hDC;
	}

	inline int GetDeviceCaps(int nIndex) const
	{
		return ::GetDeviceCaps(m_hDC, nIndex);
	}

	// Create Bitmaps
	inline void CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, LPCVOID pvColors)
		// Creates a bitmap and selects it into the device context.
	{
	
		CBitmap bitmap;
		bitmap.CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		SelectObject(bitmap);
	}

	inline CBitmap CreateCompatibleBitmap(HDC hdc, int cx, int cy)
	{
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, cx, cy);
		return SelectObject(hBitmap);
	}

	
	inline CBitmap DetachBitmap()
	{
		CBitmap bitmap;
		bitmap.CreateBitmap(1, 1, 1, 1, 0);

		CBitmap OldBitmap = SelectObject(bitmap);
	
		return OldBitmap;
	}

	inline BITMAP  GetBitmapData() const
	{
		HBITMAP hbm = (HBITMAP)::GetCurrentObject(m_hDC, OBJ_BITMAP);
		BITMAP bm;
		ZeroMemory(&bm, sizeof(BITMAP));
		::GetObject(hbm, sizeof(bm), &bm);
		return bm;
	}

	inline HBITMAP GetCurrentBitmap() const
	{
		return static_cast<HBITMAP>(::GetCurrentObject(m_hDC, OBJ_BITMAP));
	}

	inline BOOL LoadBitmap(UINT nID)
	{
		return LoadBitmap(MAKEINTRESOURCE(nID));
	}

	inline BOOL LoadBitmap(LPTSTR lpszName)
	{
		CBitmap bitmap;
		BOOL IsLoaded = bitmap.LoadBitmap(lpszName);

		if (IsLoaded)
		{
			SelectObject(bitmap);
		}

		return IsLoaded;
	}

	inline BOOL LoadImage(UINT nID, UINT fuLoad)
	{
		return LoadImage(nID, fuLoad);
	}

	inline BOOL LoadImage(LPTSTR lpszName, UINT fuLoad)
	{
		CBitmap bitmap;
		BOOL IsLoaded = bitmap.LoadImage(lpszName, fuLoad);

		if (IsLoaded)
		{
			SelectObject(bitmap);
		}

		return IsLoaded;
	}

	inline BOOL LoadOEMBitmap(UINT nIDBitmap) // for OBM_/OCR_/OIC
	{
		CBitmap bitmap;
		BOOL IsLoaded = bitmap.LoadOEMBitmap(nIDBitmap);

		if (IsLoaded)
		{
			SelectObject(bitmap);
		}

		return IsLoaded;
	}

#ifndef _WIN_W
	inline void CreateBitmapIndirect(const BITMAP& Bitmap)
	{
		CBitmap bitmap;
		bitmap.CreateBitmapIndirect(Bitmap);
		SelectObject(bitmap);
	}

	inline void CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, LPCVOID lpbInit,
		const LPBITMAPINFO pbmi, UINT fuUsage)
	{
		CBitmap bitmap;
		bitmap.CreateDIBitmap(hdc, &bmih, fdwInit, lpbInit, pbmi, fuUsage);
		SelectObject(bitmap);
	}
	
#endif

	// Create Brushes
	inline void CreatePatternBrush(HBITMAP hBitmap)
	{
		CBrush brush;
		brush.CreatePatternBrush(hBitmap);
		SelectObject(brush);
	}

	inline void CreateSolidBrush(COLORREF rbg)
	{
		CBrush brush;
		brush.CreateSolidBrush(rbg);
		SelectObject(brush);
		
	}

	inline HBRUSH GetCurrentBrush() const
	{
		return static_cast<HBRUSH>(::GetCurrentObject(m_hDC, OBJ_BRUSH));
	}

	inline LOGBRUSH GetLogBrush() const
	{
		HBRUSH hBrush = (HBRUSH)::GetCurrentObject(m_hDC, OBJ_BRUSH);
		LOGBRUSH lBrush;
		ZeroMemory(&lBrush, sizeof(LOGBRUSH));
		::GetObject(hBrush, sizeof(lBrush), &lBrush);
		return lBrush;
	}

#ifndef _WIN_W
	inline void CreateBrushIndirect(const LOGBRUSH& LogBrush)
	{
		CBrush brush;
		brush.CreateBrushIndirect(LogBrush);
		SelectObject(brush);
	}

	inline void CreateHatchBrush(int fnStyle, COLORREF rgb)
	{
		CBrush brush;
		brush.CreateHatchBrush(fnStyle, rgb);
		SelectObject(brush);
	}

	inline void CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
	{
		CBrush brush;
		brush.CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
		SelectObject(brush);
	}

	inline void CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT iUsage)
	{
		CBrush brush;
		brush.CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		SelectObject(brush);
	}
#endif

	// Create Fonts
	inline void CreateFontIndirect(const LOGFONT& lf)
	{
		CFont font;
		font.CreateFontIndirect(lf);
		SelectObject(font);
	}

	inline HFONT GetCurrentFont() const
	{
		return static_cast<HFONT>(::GetCurrentObject(m_hDC, OBJ_FONT));
	}

	inline LOGFONT GetLogFont() const
	{
		HFONT hFont = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
		LOGFONT lFont;
		ZeroMemory(&lFont, sizeof(LOGFONT));
		::GetObject(hFont, sizeof(lFont), &lFont);
		return lFont;
	}


	HPALETTE SelectPalette(HPALETTE hPalette, BOOL ForceBkgnd)
	{
		return static_cast<HPALETTE>(::SelectPalette(m_hDC, hPalette, ForceBkgnd));
	}
	
	// Create Pens
	//void CreatePen(int nStyle, int nWidth, COLORREF rgb);
// 	void CreatePenIndirect(const LOGPEN& LogPen)
// 	{
// 		CPen pen;
// 		pen.CreatePenIndirect(LogPen);
// 		SelectObject(pen);
// 	}

	inline HPEN GetCurrentPen() const
	{
		return static_cast<HPEN>(::GetCurrentObject(m_hDC, OBJ_PEN));
	}

	inline LOGPEN GetLogPen() const
	{
		HPEN hPen = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
		LOGPEN lPen;
		ZeroMemory(&lPen, sizeof(LOGPEN));
		::GetObject(hPen, sizeof(lPen), &lPen);
		return lPen;
	}

	// Retrieve and Select Stock Objects
	inline HGDIOBJ GetStockObject(int nIndex) const
	{
		return ::GetStockObject(nIndex);
	}

	inline HGDIOBJ SelectStockObject(int nIndex) const
	{
		HGDIOBJ hStockObject = ::GetStockObject(nIndex);
		return SelectObject(hStockObject);
	}

	// Create Regions
	inline int CreateRectRgn(int left, int top, int right, int bottom)
	{
		CRgn rgn;
		rgn.CreateRectRgn(left, top, right, bottom);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

	inline int CreateRectRgnIndirect(const RECT& rc)
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(rc);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

	inline int CreateFromData(const XFORM* Xform, DWORD nCount, const RGNDATA* pRgnData)
	{
		CRgn rgn;
		rgn.CreateFromData(Xform, nCount, pRgnData);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

#ifndef _WIN_W
	inline int CreateEllipticRgn(int left, int top, int right, int bottom)
	{
		CRgn rgn;
		rgn.CreateEllipticRgn(left, top, right, bottom);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

	inline int CreateEllipticRgnIndirect(const RECT& rc)
	{
		CRgn rgn;
		rgn.CreateEllipticRgnIndirect(rc);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

	inline int CreatePolygonRgn(LPPOINT ppt, int cPoints, int fnPolyFillMode)
	{
		CRgn rgn;
		rgn.CreatePolygonRgn(ppt, cPoints, fnPolyFillMode);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

	int CreatePolyPolygonRgn(LPPOINT ppt, LPINT pPolyCounts, int nCount, int fnPolyFillMode)
	{
		CRgn rgn;
		rgn.CreatePolyPolygonRgn(ppt, pPolyCounts, nCount, fnPolyFillMode);
		int Complexity = SelectClipRgn(rgn);
		
		return Complexity;
	}

#endif

	// Point and Line Drawing Functions
	inline CPoint GetCurrentPosition() const
	{
		CPoint pt;
		::MoveToEx(m_hDC, 0, 0, &pt);
		::MoveToEx(m_hDC, pt.x, pt.y, NULL);
		return pt;
	}

	inline CPoint MoveTo(int x, int y) const
	{
		return ::MoveToEx(m_hDC, x, y, NULL);
	}

	inline CPoint MoveTo(POINT pt) const
	{
		return ::MoveToEx(m_hDC, pt.x, pt.y, NULL);
	}

	inline BOOL LineTo(int x, int y) const
	{
		return ::LineTo(m_hDC, x, y);
	}

	inline BOOL LineTo(POINT pt) const
	{
		return ::LineTo(m_hDC, pt.x, pt.y);
	}

	inline COLORREF GetPixel(int x, int y) const
	{
		return ::GetPixel(m_hDC, x, y);
	}

	inline COLORREF GetPixel(POINT pt) const
	{
		return ::GetPixel(m_hDC, pt.x, pt.y);
	}

	inline COLORREF SetPixel(int x, int y, COLORREF crColor) const
	{
		return ::SetPixel(m_hDC, x, y, crColor);
	}

	inline COLORREF SetPixel(POINT pt, COLORREF crColor) const
	{
		return ::SetPixel(m_hDC, pt.x, pt.y, crColor);
	}

	inline int SetROP2(int iDrawMode) const
	{
		return ::SetROP2(m_hDC, iDrawMode);
	}

#ifndef _WIN_W
	inline BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	{
		return ::Arc(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL Arc(RECT& rc, POINT ptStart, POINT ptEnd) const
	{
		return ::Arc(m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	{
		return ::ArcTo(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL ArcTo(RECT& rc, POINT ptStart, POINT ptEnd) const
	{
		return ::ArcTo (m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle) const
	{
		return ::AngleArc(m_hDC, x, y, nRadius, fStartAngle, fSweepAngle);
	}

	inline BOOL CloseFigure() const
	{
		return ::CloseFigure(m_hDC);
	}

	inline int  GetROP2() const
	{
		return ::GetROP2(m_hDC);
	}

	inline int  GetArcDirection() const
	{
		return ::GetArcDirection(m_hDC);
	}

	inline int  SetArcDirection(int nArcDirection) const
	{
		return ::SetArcDirection(m_hDC, nArcDirection);
	}

	inline BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount) const
	{
		return ::PolyDraw(m_hDC, lpPoints, lpTypes, nCount);
	}

	inline BOOL Polyline(LPPOINT lpPoints, int nCount) const
	{
		return ::Polyline(m_hDC, lpPoints, nCount);
	}

	inline BOOL PolyPolyline(const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount) const
	{
		return ::PolyPolyline(m_hDC, lpPoints, lpPolyPoints, nCount);
	}

	inline BOOL PolylineTo(const POINT* lpPoints, int nCount) const
	{
		return ::PolylineTo(m_hDC, lpPoints, nCount);
	}

	inline BOOL PolyBezier(const POINT* lpPoints, int nCount) const
	{
		return ::PolyBezier(m_hDC, lpPoints, nCount);
	}

	inline BOOL PolyBezierTo(const POINT* lpPoints, int nCount) const
	{
		return ::PolyBezierTo(m_hDC, lpPoints, nCount );
	}

	inline BOOL SetPixelV(int x, int y, COLORREF crColor) const
	{
		return ::SetPixelV(m_hDC, x, y, crColor);
	}

	inline BOOL SetPixelV(POINT pt, COLORREF crColor) const
	{
		return ::SetPixel(m_hDC, pt.x, pt.y, crColor);
	}
#endif

	// Shape Drawing Functions
	inline void DrawFocusRect(const RECT& rc) const
	{
		::DrawFocusRect(m_hDC, &rc);
	}

	inline BOOL Ellipse(int x1, int y1, int x2, int y2) const
	{
		return ::Ellipse(m_hDC, x1, y1, x2, y2);
	}

	inline BOOL Ellipse(const RECT& rc) const
	{
		return ::Ellipse(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL Polygon(LPPOINT lpPoints, int nCount) const
	{
		return ::Polygon(m_hDC, lpPoints, nCount);
	}

	inline BOOL Rectangle(int x1, int y1, int x2, int y2) const
	{
		return ::Rectangle(m_hDC, x1, y1, x2, y2);
	}

	inline BOOL Rectangle(const RECT& rc) const
	{
		return ::Rectangle(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL RoundRect(int x1, int y1, int x2, int y2, int nWidth, int nHeight) const
	{
		return ::RoundRect(m_hDC, x1, y1, x2, y2, nWidth, nHeight);
	}

	inline BOOL RoundRect(const RECT& rc, int nWidth, int nHeight) const
	{
		return ::RoundRect(m_hDC, rc.left, rc.top, rc.right, rc.bottom, nWidth, nHeight );
	}

#ifndef _WIN32_WCE
	inline BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	{
		return ::Chord(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL Chord(const RECT& rc, POINT ptStart, POINT ptEnd) const
	{
		return ::Chord(m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	{
		return ::Pie(m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL Pie(const RECT& rc, POINT ptStart, POINT ptEnd) const
	{
		return ::Pie(m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount) const
	{
		return ::PolyPolygon(m_hDC, lpPoints, lpPolyCounts, nCount);
	}
#endif

	// Fill and Image Drawing functions
	inline BOOL DrawEdge(const RECT& rc, UINT nEdge, UINT nFlags) const
	{
		return ::DrawEdge(m_hDC, (LPRECT)&rc, nEdge, nFlags);
	}

	inline BOOL DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth,
		UINT istepIfAniCur, HBRUSH hFlickerFreeDraw, UINT diFlags) const
	{
		return ::DrawIconEx(m_hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hFlickerFreeDraw, diFlags);
	}

	inline BOOL DrawFrameControl(const RECT& rc, UINT nType, UINT nState) const
	{
		return ::DrawFrameControl(m_hDC, (LPRECT)&rc, nType, nState);
	}

	inline BOOL FillRect(const RECT& rc, HBRUSH hBrush) const
	{
		return static_cast<BOOL>(::FillRect(m_hDC, &rc, hBrush));
	}

	inline BOOL FillRgn(HRGN hRgn, HBRUSH hBrush) const
	{
		return ::FillRgn(m_hDC, hRgn, hBrush);
	}

	inline BOOL InvertRect(const RECT& rc) const
	{
		return ::InvertRect( m_hDC, &rc);
	}

	inline void SolidFill(COLORREF Color, const RECT& rc) const;

#if (WINVER >= 0x0410)
	inline BOOL GradientFill(PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode) const
	{
		{
			return ::GradientFill(m_hDC, pVertex, nVertex, pMesh, nMesh, ulMode);
		}
	}
#endif

#ifndef _WIN_W
	inline BOOL DrawIcon(int x, int y, HICON hIcon) const
	{
		return ::DrawIcon(m_hDC, x, y, hIcon);
	}

	inline BOOL DrawIcon(POINT pt, HICON hIcon) const
	{
		return ::DrawIcon(m_hDC, pt.x, pt.y, hIcon);
	}

	inline BOOL FrameRect(const RECT& rc, HBRUSH hBrush) const
	{
		return static_cast<BOOL>(::FrameRect(m_hDC, &rc, hBrush));
	}

	inline BOOL FrameRgn(HRGN hRgn, HBRUSH hBrush, int nWidth, int nHeight) const
	{
		return static_cast<BOOL>(::FrameRgn(m_hDC, hRgn, hBrush, nWidth, nHeight));
	}

	inline int  GetPolyFillMode() const
	{
		return ::GetPolyFillMode(m_hDC);
	}

	inline BOOL PaintRgn(HRGN hRgn) const
	{
		return static_cast<BOOL>(::PaintRgn(m_hDC, hRgn));
	}

	inline int  SetPolyFillMode(int iPolyFillMode) const
	{
		return ::SetPolyFillMode(m_hDC, iPolyFillMode);
	}

#endif

	// Bitmap Functions
	inline BOOL BitBlt(int x, int y, int nWidth, int nHeight, HDC hdcSrc, int xSrc, int ySrc, DWORD dwRop) const
	{
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, hdcSrc, xSrc, ySrc, dwRop);
	}

	void DrawBitmap(int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask) const;

	inline BOOL MaskBlt(int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc,
		int nXSrc, int nYSrc, HBITMAP hbmMask, int xMask, int  yMask,
		DWORD dwRop) const
	{
		return ::MaskBlt(m_hDC, nXDest, nYDest, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, hbmMask, xMask, yMask, dwRop);
	}

	inline BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop) const
	{
		return ::PatBlt(m_hDC, x, y, nWidth, nHeight, dwRop);
	}

	inline int  StretchDIBits(int XDest, int YDest, int nDestWidth, int nDestHeight,
		int XSrc, int YSrc, int nSrcWidth, int nSrcHeight,
		LPCVOID pBits, const LPBITMAPINFO pbmi, UINT iUsage, DWORD dwRop) const
	{
		return ::StretchDIBits(m_hDC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, pBits, pbmi, iUsage, dwRop);
	}

	inline BOOL StretchBlt(int x, int y, int nWidth, int nHeight, HDC hdcSrc,
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
		DWORD dwRop) const
	{
		return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, hdcSrc, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
	}

#ifndef _WIN_W
	inline BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType) const
	{
		return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType );
	}

	inline BOOL FloodFill(int x, int y, COLORREF crColor) const
	{
		return ::FloodFill(m_hDC, x, y, crColor);
	}

	inline int  GetDIBits(HBITMAP hBitmap, UINT uStartScan, UINT cScanLines, LPVOID pBits,
		LPBITMAPINFO pbi, UINT uUsage) const
	{
		return ::GetDIBits(m_hDC, hBitmap, uStartScan, cScanLines, pBits, pbi, uUsage);
	}

	inline int  GetStretchBltMode() const
	{
		return ::GetStretchBltMode(m_hDC);
	}

	inline int  SetDIBits(HBITMAP hBitmap, UINT uStartScan, UINT cScanLines, LPCVOID pBits,
		LPBITMAPINFO pbmi, UINT fuColorUse) const
	{
		return ::SetDIBits(m_hDC, hBitmap, uStartScan, cScanLines, pBits, pbmi, fuColorUse);
	}

	inline int  SetStretchBltMode(int iStretchMode) const
	{
		return ::SetStretchBltMode(m_hDC, iStretchMode);
	}

#if (WINVER >= 0x0410)
	inline BOOL TransparentBlt(int x, int y, int nWidth, int nHeight, HDC hdcSrc,
		int xSrc, int ySrc, int nWidthSrc, int nHeightSrc,
		UINT crTransparent) const
	{
		return ::TransparentBlt(m_hDC, x, y, nWidth, nHeight, hdcSrc, xSrc, ySrc, nWidthSrc, nHeightSrc, crTransparent);
	}
#endif
#endif

	// Brush Functions
#if (_WIN32_WINNT >= 0x0500)
	inline COLORREF GetDCBrushColor() const;
	inline COLORREF SetDCBrushColor(COLORREF crColor) const;
#endif

	// Font Functions
#ifndef _WIN_W
	inline DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID pvBuffer,  DWORD cbData) const
	{
		return ::GetFontData(m_hDC, dwTable, dwOffset, pvBuffer, cbData);
	}

	inline DWORD GetFontLanguageInfo() const
	{
		return ::GetFontLanguageInfo(m_hDC);
	}

	inline DWORD GetGlyphOutline(UINT uChar, UINT uFormat, LPGLYPHMETRICS pgm, DWORD cbBuffer,
		LPVOID pvBuffer, const MAT2* lpmat2) const
	{
		return ::GetGlyphOutline(m_hDC, uChar, uFormat, pgm, cbBuffer, pvBuffer, lpmat2);
	}

	inline DWORD GetKerningPairs(DWORD nNumPairs, LPKERNINGPAIR pkrnpair) const
	{
		return ::GetKerningPairs(m_hDC, nNumPairs, pkrnpair);
	}

	inline DWORD SetMapperFlags(DWORD dwFlag) const
	{
		return ::SetMapperFlags(m_hDC, dwFlag);
	}

#endif

	// Palette and color functions
	inline HPALETTE GetCurrentPalette() const
	{

	}

	inline COLORREF GetNearestColor(COLORREF crColor) const;
	inline void RealizePalette() const;

#ifndef _WIN_W
	inline BOOL GetColorAdjustment(LPCOLORADJUSTMENT pCA) const;
	inline BOOL SetColorAdjustment(const COLORADJUSTMENT* pCA) const;
	inline BOOL UpdateColors() const;
#endif

#ifndef _WIN_W
	// Pen Functions
	inline BOOL GetMiterLimit(PFLOAT peLimit) const
	{
		return ::GetMiterLimit(m_hDC, peLimit);
	}

	inline BOOL SetMiterLimit(FLOAT eNewLimit, PFLOAT peOldLimit) const
	{
		return ::SetMiterLimit(m_hDC, eNewLimit, peOldLimit);
	}
#endif

	// Clipping and Region Functions
	inline int  ExcludeClipRect(int Left, int Top, int Right, int BottomRect) const
	{
		return ::ExcludeClipRect(m_hDC, Left, Top, Right, BottomRect);
	}

	inline int  ExcludeClipRect(const RECT& rc) const
	{
		return ::ExcludeClipRect(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline int  GetClipBox(RECT& rc) const
	{
		return ::GetClipBox(m_hDC, &rc);
	}

	inline int  IntersectClipRect(int Left, int Top, int Right, int Bottom) const
	{
		return ::IntersectClipRect(m_hDC, Left, Top, Right, Bottom);
	}

	inline int  IntersectClipRect(const RECT& rc) const
	{
		return ::IntersectClipRect(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL RectVisible(const RECT& rc) const
	{
		return ::RectVisible (m_hDC, &rc);
	}

	inline int  SelectClipRgn(HRGN hRgn) const
	{
		return ::SelectClipRgn(m_hDC, hRgn);
	}

#ifndef _WIN_W
	inline BOOL BeginPath() const
	{
		return ::BeginPath(m_hDC);
	}

	inline BOOL EndPath() const
	{
		return ::EndPath(m_hDC);
	}

	inline int  ExtSelectClipRgn(HRGN hRgn, int fnMode) const
	{
		return ::ExtSelectClipRgn(m_hDC, hRgn, fnMode);
	}

	inline BOOL FlattenPath() const
	{
		return ::FlattenPath(m_hDC);
	}

	inline int	 GetPath(POINT* pPoints, BYTE* pTypes, int nCount) const
	{
		return ::GetPath(m_hDC, pPoints, pTypes, nCount);
	}

	inline int  OffsetClipRgn(int nXOffset, int nYOffset) const
	{
		return ::OffsetClipRgn (m_hDC, nXOffset, nYOffset);
	}

	inline BOOL PtVisible(int X, int Y) const
	{
		return ::PtVisible (m_hDC, X, Y);
	}

	inline BOOL SelectClipPath(int nMode) const
	{
		return ::SelectClipPath(m_hDC, nMode);
	}

	inline BOOL WidenPath() const
	{
		return ::WidenPath(m_hDC);
	}

#endif

	// Co-ordinate Functions
#ifndef _WIN_W
	inline BOOL DPtoLP(LPPOINT lpPoints, int nCount)  const
	{
		return ::DPtoLP(m_hDC, lpPoints, nCount);
	}

	inline BOOL DPtoLP(RECT& rc)  const
	{
		return ::DPtoLP(m_hDC, (LPPOINT)&rc, 2);
	}

	inline BOOL LPtoDP(LPPOINT lpPoints, int nCount)  const
	{
		return ::LPtoDP(m_hDC, lpPoints, nCount);
	}

	inline BOOL LPtoDP(RECT& rc)  const
	{
		return ::LPtoDP(m_hDC, (LPPOINT)&rc, 2);
	}

#endif

	// Layout Functions
	inline DWORD GetLayout() const
	{
		return ::GetLayout(m_hDC);
	}

	inline DWORD SetLayout(DWORD dwLayout) const
	{
		return ::SetLayout(m_hDC, dwLayout);
	}

	// Mapping functions
#ifndef _WIN_W
	inline int  GetMapMode() const
	{
		return ::GetMapMode(m_hDC);
	}

	inline int  SetMapMode(int nMapMode) const
	{
		return ::SetMapMode(m_hDC, nMapMode);
	}

	inline BOOL GetViewportOrgEx(LPPOINT lpPoint)  const
	{
		return ::GetViewportOrgEx(m_hDC, lpPoint);
	}

	inline BOOL SetViewportOrgEx(int x, int y, LPPOINT lpPoint = NULL) const
	{
		return ::SetViewportOrgEx(m_hDC, x, y, lpPoint);
	}

	inline BOOL SetViewportOrgEx(POINT point, LPPOINT lpPointRet = NULL) const
	{
		return SetViewportOrgEx(point.x, point.y, lpPointRet);
	}

	inline BOOL OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint = NULL) const
	{
		return ::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, lpPoint);
	}

	inline BOOL GetViewportExtEx(LPSIZE lpSize)  const
	{
		return ::GetViewportExtEx(m_hDC, lpSize);
	}
	inline BOOL SetViewportExtEx(int x, int y, LPSIZE lpSize) const
	{
		return ::SetViewportExtEx(m_hDC, x, y, lpSize);
	}
	inline BOOL SetViewportExtEx(SIZE size, LPSIZE lpSizeRet) const
	{
		return SetViewportExtEx(size.cx, size.cy, lpSizeRet);
	}
	inline BOOL ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const
	{
		return ::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}
	inline BOOL GetWindowExtEx(LPSIZE lpSize)  const
	{
		return ::GetWindowExtEx(m_hDC, lpSize);
	}
	inline BOOL SetWindowExtEx(int x, int y, LPSIZE lpSize) const
	{
		return ::SetWindowExtEx(m_hDC, x, y, lpSize);
	}
	inline BOOL SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const
	{
		return SetWindowExtEx(size.cx, size.cy, lpSizeRet);
	}

	inline BOOL ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const
	{
		return ::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}
	inline BOOL GetWindowOrgEx(LPPOINT lpPoint)  const
	{
		return ::GetWindowOrgEx(m_hDC, lpPoint);
	}
	inline BOOL SetWindowOrgEx(int x, int y, LPPOINT lpPoint) const
	{
		return ::SetWindowOrgEx(m_hDC, x, y, lpPoint);
	}
	inline BOOL SetWindowOrgEx(POINT point, LPPOINT lpPointRet) const
	{
		return SetWindowOrgEx(point.x, point.y, lpPointRet);
	}
	inline BOOL OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint) const
	{
		return ::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, lpPoint);
	}
#endif

	// MetaFile Functions
#ifndef _WIN_W
	inline BOOL PlayMetaFile(HMETAFILE hMF) const
	{
		return ::PlayMetaFile(m_hDC, hMF);
	}
	inline BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT& rcBounds) const
	{
		return ::PlayEnhMetaFile(m_hDC, hEnhMetaFile, &rcBounds);
	}
#endif

	// Printer Functions
	inline int StartDoc(LPDOCINFO lpDocInfo) const
	{
		return ::StartDoc(m_hDC, lpDocInfo);
	}
	inline int EndDoc() const
	{
		return ::EndDoc(m_hDC);
	}
	inline int StartPage() const
	{
		return ::StartPage(m_hDC);
	}
	inline int EndPage() const
	{
		return ::EndPage(m_hDC);
	}
	inline int AbortDoc() const
	{
		return ::AbortDoc(m_hDC);
	}

	inline int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const
	{
		return ::SetAbortProc(m_hDC, lpfn);
	}

	// Text Functions
	inline int   DrawText(LPTSTR lpszString, int nCount, RECT& rc, UINT nFormat) const
	{
		return ::DrawText(m_hDC, lpszString, nCount, &rc, nFormat );
	}

	// Text Functions
	inline int   DrawTextA(const char* lpszString, int nCount, RECT& rc, UINT nFormat) const
	{
		return ::DrawTextA(m_hDC, lpszString, nCount, &rc, nFormat );
	}

	inline BOOL  ExtTextOut(int x, int y, UINT nOptions, const RECT& rc, LPCTSTR lpszString, int nCount = -1, LPINT lpDxWidths = NULL) const
	{
		return ::ExtTextOut(m_hDC, x, y, nOptions, &rc, lpszString, nCount, lpDxWidths );
	}

	inline COLORREF GetBkColor() const
	{
		return ::GetBkColor(m_hDC);
	}

	inline int   GetBkMode() const
	{
		return ::GetBkMode(m_hDC);
	}

	inline UINT  GetTextAlign() const
	{
		return ::GetTextAlign(m_hDC);
	}

	inline int   GetTextFace(int nCount, LPTSTR lpszFacename) const
	{
		return ::GetTextFace(m_hDC, nCount, lpszFacename);
	}

	inline COLORREF GetTextColor() const
	{
		return ::GetTextColor(m_hDC);
	}

	inline BOOL  GetTextMetrics(TEXTMETRIC& Metrics) const
	{
		return ::GetTextMetrics(m_hDC, &Metrics);
	}

	inline COLORREF SetBkColor(COLORREF crColor) const
	{
		return ::SetBkColor(m_hDC, crColor);
	}

	inline int   SetBkMode(int iBkMode) const
	{
		return ::SetBkMode(m_hDC, iBkMode);
	}

	inline UINT  SetTextAlign(UINT nFlags) const
	{
		return ::SetTextAlign(m_hDC, nFlags);
	}

	inline COLORREF SetTextColor(COLORREF crColor) const
	{
		return ::SetTextColor(m_hDC, crColor);
	}

#ifndef _WIN_W
	inline int   DrawTextEx(LPTSTR lpszString, int nCount, RECT& rc, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams) const
	{
		return ::DrawTextEx(m_hDC, lpszString, nCount, &rc, nFormat, lpDTParams);
	}

	inline DWORD GetCharacterPlacement(LPCTSTR pString, int nCount, int nMaxExtent,
		LPGCP_RESULTS pResults, DWORD dwFlags) const
	{
		return ::GetCharacterPlacement(m_hDC, pString, nCount, nMaxExtent, pResults, dwFlags);
	}

	inline BOOL  GetCharABCWidths(UINT uFirstChar, UINT uLastChar, LPABC pABC) const
	{
		return::GetCharABCWidths(m_hDC, uFirstChar, uLastChar, pABC);
	}

	inline BOOL  GetCharWidth(UINT iFirstChar, UINT iLastChar, float* pBuffer) const
	{
		return ::GetCharWidthFloat(m_hDC, iFirstChar, iLastChar, pBuffer);
	}

	inline CSize GetTabbedTextExtent(LPTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
	{
		DWORD dwSize = ::GetTabbedTextExtent(m_hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions );
		CSize sz(dwSize);
		return sz;
	}

	inline int   GetTextCharacterExtra() const
	{
		return ::GetTextCharacterExtra(m_hDC);
	}

	inline CSize GetTextExtentPoint32(LPTSTR lpszString, int nCount) const
	{
		CSize sz;
		::GetTextExtentPoint32(m_hDC, lpszString, nCount, &sz);
		return sz;
	}
	
	inline BOOL  GrayString(HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight) const
	{
		return ::GrayString(m_hDC, hBrush, lpOutputFunc, lpData, nCount, x, y, nWidth, nHeight);
	}

	inline int   SetTextCharacterExtra(int nCharExtra) const
	{
		return ::SetTextCharacterExtra(m_hDC, nCharExtra);
	}
	inline int   SetTextJustification(int nBreakExtra, int nBreakCount) const
	{
		return ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
	}

	inline CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) const
	{
		DWORD dwSize = ::TabbedTextOut(m_hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin );
		CSize sz(dwSize);
		return sz;
	}

	inline BOOL  TextOut(int x, int y, LPCTSTR lpszString, int nCount = -1) const
	{
		if (nCount == -1)
			nCount = lstrlen (lpszString);

		return ::TextOut(m_hDC, x, y, lpszString, nCount);
	}

#if (_WIN32_WINNT >= 0x0500) && !defined(__GNUC__)
	inline BOOL  GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC pABC) const
	{
		return ::GetCharABCWidthsI(m_hDC, giFirst, cgi, pgi, pABC);
	}

	inline BOOL  GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, int* pBuffer) const
	{
		return ::GetCharWidthI(m_hDC, giFirst, cgi, pgi, pBuffer);
	}
#endif // (_WIN32_WINNT >= 0x0500) && !defined(__GNUC__)
#endif  // _WIN32_WCE

protected:
	void Release()
	{	
		Destroy();		
	}

	//void SetManaged(bool IsManaged) { m_pData->IsManagedHDC = IsManaged; }

protected:
	HDC	 m_hDC;			// The HDC belonging to this CDC;
};

class CMemDC : public CDC
{
public:
	inline CMemDC(HDC hdc)
	{
		try
		{
			CreateCompatibleDC(hdc);
		}
		catch(...)
		{
			Release();	// Cleanup
			throw;		// Rethrow
		}
	}

	inline virtual ~CMemDC() {}
};

inline void CDC::DrawBitmap(int x, int y, int cx, int cy, HBITMAP hBitmap, COLORREF clrMask) const
{

	CMemDC dcImage(*this);
	dcImage.SetBkColor(clrMask);
	::SelectObject(dcImage,(HGDIOBJ)hBitmap);

	// Create the Mask memory DC
	CMemDC dcMask(*this);
	dcMask.CreateBitmap(cx, cy, 1, 1, NULL);
	dcMask.BitBlt(0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

	// Mask the image to 'this' DC
	BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
	BitBlt(x, y, cx, cy, dcMask, 0, 0, SRCAND);
	BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);

}

class CClientDC : public CDC
{
public:
	CClientDC(HWND hWnd)
	{
		if (0 == hWnd) hWnd = GetDesktopWindow();
		
		try
		{
			HDC hDC = ::GetDC(hWnd);
			if (hDC != 0)
				Attach(hDC, hWnd);
		}
		catch(...)
		{
			Release();	// Cleanup
			throw;		// Rethrow
		}
	}

	virtual ~CClientDC() {}
};

class CPaintDC : public CDC
{
public:
	CPaintDC(HWND hWnd) : m_hWndPaint(hWnd)
	{
		try
		{
			HDC hDC = ::BeginPaint(hWnd, &m_ps);
			if (hDC != 0)
				Attach(hDC, hWnd);
			
		}
		catch(...)
		{
			//Release();	// Cleanup
			throw;		// Rethrow
		}
	}

	virtual ~CPaintDC()
	{
		::EndPaint(m_hWndPaint, &m_ps);
		if (m_hDC)
		{
			//::ReleaseDC(m_hWndPaint, m_hDC);
			m_hDC = 0;
		}
	}

private:
	HWND m_hWndPaint;
	PAINTSTRUCT m_ps;
};

class CWindowDC : public CDC
{
public:
	CWindowDC(HWND hWnd)
	{
		if (0 == hWnd) hWnd = GetDesktopWindow();
		
		try
		{
			HDC hDC = ::GetWindowDC(hWnd);
			if (hDC != 0)
				Attach(hDC, hWnd);
		}
		catch(...)
		{
			Release();	// Cleanup
			throw;		// Rethrow
		}

	}
	virtual ~CWindowDC() {}
};

class CClientRect
{
public:
	CClientRect()
	{

	}

	CClientRect(HWND hWnd)
	{
		::GetClientRect(hWnd,&m_rcClient);
	}

	~CClientRect()
	{
		m_rcClient.left = 0;
		m_rcClient.top = 0;
		m_rcClient.right = 0;
		m_rcClient.bottom = 0;
	}

	operator RECT&()
	{
		return m_rcClient;
	}

	operator RECT*()
	{
		return &m_rcClient;
	}

	ULONG Width()
	{
		return m_rcClient.right - m_rcClient.left;
	}

	ULONG Height()
	{
		return m_rcClient.bottom - m_rcClient.top;
	}

	LONG& Left()
	{
		return m_rcClient.left;
	}

	LONG& Right()
	{
		return m_rcClient.right;
	}

	LONG& Top()
	{
		return m_rcClient.top;
	}

	LONG& Bottom()
	{
		return m_rcClient.bottom;
	}

private:
	RECT m_rcClient;
};


#define mag_map virtual

#define afx_msg virtual

#define MAX_MSG_COUNT WM_USER + 0x200

#define MAP_MSG_BEGIN(T) LRESULT  T::WinBaseProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){\
	T* pthis = (T*)( GetWindowLongPtr(hwnd, 0));\
	if (uMsg == WM_NCCREATE)\
	{\
		T::PreOnNcCreate(hwnd,wParam,lParam);\
	}\
	if (pthis)\
	{\
		/*if ( uMsg >= 0 && uMsg < MAX_MSG_COUNT)*/\
		{\
			\
			/*pWinBaseMsg pBaseMsg = pthis->GetMsgCallBack(uMsg);*/\
			/*if (pBaseMsg)*/\
			{\
				BOOL bDef = FALSE;\
				BOOL bResult = pthis->CallWinBaseMsg(uMsg,wParam,lParam,bDef);\
				if ( bDef)\
				{\
					return bResult;\
				}\
			}\
		}\
		\
		/*return pthis->DefWinProc(hwnd, uMsg, wParam, lParam);*/\
	}\
	\
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);\
}\
void T::BeginMapMsg(){\
	
#define MAP_MSG_END() }

#define DECLARE_MSG_MAP() public: \
	virtual void BeginMapMsg();\
	static LRESULT CALLBACK WinBaseProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

#define ON_MESSAGE( a,b) /*m_WinBaseMsg[a] = (pWinBaseMsg)&b;*/

template<typename T>
class CWinBase
{
public:
	typedef BOOL (CWinBase<T>::*pWinBaseMsg)( WPARAM wp,LPARAM lp,BOOL& bDef);
	CWinBase(void)
		:m_hWnd(NULL)
		,m_hInst(NULL)
	{		
		//ZeroMemory(m_WinBaseMsg,sizeof(pWinBaseMsg)*MAX_MSG_COUNT);
	}

	virtual ~CWinBase(void)
	{

	}

	operator HWND()
	{
		return m_hWnd;
	}

	virtual BOOL Create(
					HWND hWnd,
					HINSTANCE hInst,
					UINT nId,
					LPTSTR lpClassName,
					LPTSTR lpText = NULL,
					DWORD dwStyle = WS_VISIBLE | WS_CHILD |	WS_TABSTOP 
					)
	{
		LONG Error = Register(hInst,lpClassName);

		if(Error)
		{
			BeginMapMsg();

			m_hWnd = CreateWindow(		
				lpClassName,
				lpText,
				dwStyle,
				0,
				0,
				1,
				1,
				hWnd,
				(HMENU)nId,
				hInst,
				0);

			if (m_hWnd)
			{
				SetWindowLongPtr(m_hWnd,0, (LONG_PTR)this);
				return TRUE;
			}
		}

		return FALSE;
	}

	void ShowWindow( int CmdShow)
	{
		::ShowWindow(m_hWnd,CmdShow);
	}

	BOOL ShowScrollBar(int wBar,BOOL bShow)
	{
		return ::ShowScrollBar(m_hWnd,wBar,bShow);
	}

	BOOL SetScrollRange( int nBar,int nMinPos,int nMaxPos, BOOL bRedraw = FALSE)
	{
		SCROLLINFO si = {0};

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE;
		si.nMin = nMinPos;
		si.nMax = nMaxPos;
		return ::SetScrollInfo(m_hWnd, nBar, &si,bRedraw);
		//return ::SetScrollRange(m_hWnd,nBar,nMinPos,nMaxPos,bRedraw);
	}

	BOOL InvalidateRect(BOOL bRedraw = FALSE)
	{
		return ::InvalidateRect(m_hWnd,NULL,bRedraw);
	}

	pWinBaseMsg GetMsgCallBack(UINT uMsg)
	{
		return m_WinBaseMsg[uMsg];
	}

#define CALL_MSG(msg,fun) \
	 if(uMsg == msg){\
	      return fun(wp,lp,bDef);\
	 }

	BOOL CallWinBaseMsg(UINT uMsg,WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;

		CALL_MSG(WM_PAINT,OnPaint)
		CALL_MSG(WM_NCPAINT,OnNcPaint)
		CALL_MSG(WM_NCLBUTTONDOWN,OnNcLButtonDown)
		CALL_MSG(WM_NCCALCSIZE,OnNCClassSize)
		CALL_MSG(WM_SIZE,OnSize)
		CALL_MSG(WM_ERASEBKGND,OnEraseBkgnd)
		CALL_MSG(WM_MOUSEMOVE,OnMouseMove)
		CALL_MSG(WM_LBUTTONDOWN,OnLButtonDown)
		CALL_MSG(WM_LBUTTONDBLCLK,OnLButtonDBClick)
		CALL_MSG(WM_LBUTTONUP,OnLButtonUp)
		CALL_MSG(WM_RBUTTONDOWN,OnRButtonDown)
			//ON_MESSAGE(WM_RBUTTONUP,CDynamicView::OnRButtonUp)
		CALL_MSG(WM_KEYDOWN,OnKeyDown)
		CALL_MSG(WM_MOUSELEAVE,OnMouseLeave)
		CALL_MSG(WM_TIMER,OnTimer)
		CALL_MSG(WM_KILLFOCUS,OnKillFocus)
		CALL_MSG(WM_SETFOCUS,OnSetFocus)
		CALL_MSG(WM_HSCROLL,OnHScroll)
		CALL_MSG(WM_VSCROLL,OnVScroll)
		CALL_MSG(WM_MOUSEWHEEL,OnMouseWheel)
		CALL_MSG(WM_MOUSEHWHEEL,OnMouseHWheel)
		
		return 0;
	}

public:
	static BOOL PreOnNcCreate( HWND hWnd, WPARAM wp,LPARAM lp )
	{
		return 0;
	}

// 	static
// 	LRESULT
// 	CALLBACK
// 	WinBaseProc(
// 			HWND hwnd,
// 			UINT uMsg,
// 			WPARAM wParam, 
// 			LPARAM lParam)
// 	{
// 		//T* pthis = static_cast<T*>(this);
// 
// 		T* pthis = static_cast<T*>( GetWindowLongPtr(hwnd, 0));
// 
// 		if (pthis)
// 		{
// 			if ( uMsg >= 0 && uMsg < MAX_MSG_COUNT)
// 			{
// 				if (uMsg == WM_NCCREATE)
// 				{
// 					*this = hwnd;
// 				}
// 
// 				pWinBaseMsg pBaseMsg = pthis->GetMsgCallBack(uMsg);
// 				if (pBaseMsg)
// 				{
// 					BOOL bDef = FALSE;
// 					BOOL bResult = pthis->CallWinBaseMsg(uMsg,wParam,lParam,bDef);
// 					if ( bDef)
// 					{
// 						return bResult;
// 					}
// 				}
// 			}
// 
// 			pthis->DefWinProc(hwnd, uMsg, wParam, lParam);
// 		}
// 
// 		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
// 	}

protected:
	virtual
	LRESULT 
	DefWinProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam, 
		LPARAM lParam)
	{
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	afx_msg BOOL OnNcCreate( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnPaint( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnNCClassSize( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnNcPaint( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnNcLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnSize( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnEraseBkgnd( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnMouseMove( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnMouseLeave( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnTimer( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnKillFocus( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnSetFocus( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnLButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnLButtonDBClick( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnLButtonUp( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnRButtonDown( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnKeyDown( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnVScroll( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnHScroll( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnMouseWheel( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	afx_msg BOOL OnMouseHWheel( WPARAM wp,LPARAM lp,BOOL& bDef)
	{
		bDef = FALSE;
		return 0;
	}

	virtual void BeginMapMsg()
	{

	}

protected:
	//pWinBaseMsg m_WinBaseMsg[MAX_MSG_COUNT];
protected:
	HWND		m_hWnd;
	HINSTANCE	m_hInst;

private:
	LRESULT Register(
		HINSTANCE hInstance,
		LPTSTR lpClassName)
	{
		WNDCLASS wc = { 0 };

		wc.hInstance = hInstance;
		wc.lpfnWndProc = T::WinBaseProc;
		wc.style	   = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.cbWndExtra = sizeof(T*);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = lpClassName;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		ULONG dwError = RegisterClass(&wc);

		if (!dwError)
		{
			if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
			{
				return TRUE;
			}

			return 0;
		}

		return TRUE;;
	}
	
};

#endif