// dib.cpp : implementation file
//

#include "edtdefines.h"

// #include "edtdisplayscale.h"
#include "MsWindowPaint.h"

#include "stdio.h"

#include "ImageWindowData.h"

/////////////////////////////////////////////////////////////////////////////
// Global references

/////////////////////////////////////////////////////////////////////////////
// DIB Macros
//

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.
//
#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

// Dib Header Marker - used in writing DIBs to files
//
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'BLUE')

//
// ALERT: rainbow colors stuff -- ported from X windows C code
//
#define makecolor(r,g,b) (ULONG)((((ULONG)r << 16) | ((ULONG)g << 8) | (ULONG)b))
#define White makecolor(255,255,255)
#define Red makecolor(255,0,0)
#define Orange makecolor(255,165,0)
#define Yellow makecolor(255,255,0)
#define Green makecolor(0,255,0)
#define Aquamarine makecolor(127,255,212)
#define Blue makecolor(0,0,255)
#define Indigo makecolor(138,43,226)	/* blue violet */
#define Violet makecolor(238,130,238)
#define Black makecolor(0,0,0)
#define RED(x) ((float)(((x >> 16) & 0xff)))
#define GREEN(x) ((float)(((x >> 8) & 0xff)))
#define BLUE(x) ((float)(((x) & 0xff)))
#define CM_RAINBOW 1

#define RAINBOW_COLORS 7
ULONG colorspread[RAINBOW_COLORS+1] = {
    Black,
    Blue,
    Aquamarine,
    Green,
    Yellow,
    Orange,
    Red,
    White,
} ;
//
// end of ALERT: code
//

/////////////////////////////////////////////////////////////////////////////
// CMsWindowPaint class implementation

/////////////////////////////////////////////////////////////////////////////
// CMsWindowPaint construction and destruction
//
CMsWindowPaint::CMsWindowPaint()
{

    m_pBMI = NULL;
    m_lBMI = 0;
    m_pPal = NULL;
    m_lPal = 0;
    m_hPal = NULL;

    //	m_bDisplayable = true;
    //	m_bInverted = true;

    m_pMonochromeLut = NULL;

    m_bStretchData = FALSE;

    m_bWaitForVSync = FALSE;
}


CMsWindowPaint::~CMsWindowPaint()
{
    Destroy();

}

/////////////////////////////////////////////////////////////////////////////
// CMsWindowPaint class private functions

bool CMsWindowPaint::IsWinDIB(BITMAPINFOHEADER* pBIH)
{
    ASSERT(pBIH);
    if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER)) {
        return FALSE;
    }
    return TRUE;
}

//	NumDIBColorEntries
//
//	This function calculates the number of colors in the DIB's color table
//	by finding the bits per pixel for the DIB (whether Win3.0 or other-style
//	DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
//	if 24, no colors in color table.
//
//	nBitCount == 1
//	The bitmap is monochrome, and the bmiColors member contains two entries.
//	Each bit in the bitmap array represents a pixel. If the bit is clear, the pixel
//	is displayed with the color of the first entry in the bmiColors table; if the bit
//	is set, the pixel has the color of the second entry in the table.
//	
//	nBitCount == 4
//	The bitmap has a maximum of 16 colors, and the bmiColors member contains up
//	to 16 entries. Each pixel in the bitmap is represented by a 4-bit index
//	into the color table. For example, if the first byte in the bitmap is 0x1F,
//	the byte represents two pixels. The first pixel contains the color in the second
//	table entry, and the second pixel contains the color in the sixteenth table entry.
//	
//	nBitCount == 8
//	The bitmap has a maximum of 256 colors, and the bmiColors member contains up to 256
//	entries. In this case, each byte in the array represents a single pixel.
//	
//	nBitCount == 16
//	The bitmap has a maximum of 2^16 colors. If the biCompression member of the
//	BITMAPINFOHEADER is BI_RGB, the bmiColors member is NULL. Each WORD in the bitmap
//	array represents a single pixel. The relative intensities of red, green, and blue
//	are represented with 5 bits for each color component. The value for blue is in the
//	least significant 5 bits, followed by 5 bits each for green and red, respectively.
//	The most significant bit is not used.
//	If the biCompression member of the BITMAPINFOHEADER is BI_BITFIELDS, the bmiColors
//	member contains three DWORD color masks that specify the red, green, and blue
//	components, respectively, of each pixel. Each WORD in the bitmap array represents
//	a single pixel.	
//
//	(Windows NT): When the biCompression member is BI_BITFIELDS, bits set in each DWORD
//	mask must be contiguous and should not overlap the bits of another mask. All the
//	bits in the pixel do not have to be used. 
//	(Windows 95): When the biCompression member is BI_BITFIELDS, Windows 95 supports only
//	the following 16bpp color masks: A 5-5-5 16-bit image, where the blue mask is 0x001F,
//	the green mask is 0x03E0, and the red mask is 0x7C00; and a 5-6-5 16-bit image, where
//	the blue mask is 0x001F, the green mask is 0x07E0, and the red mask is 0xF800.
//	
//	nBitCount == 24
//	The bitmap has a maximum of 2^24 colors, and the bmiColors member is NULL. Each 3-byte
//	triplet in the bitmap array represents the relative intensities of blue, green, and red,
//	respectively, for a pixel.
//	
//	nBitCount == 32
//	The bitmap has a maximum of 2^32 colors. If the biCompression member
//	of the BITMAPINFOHEADER is BI_RGB, the bmiColors member is NULL. Each DWORD in the
//	bitmap array represents the relative intensities of blue, green, and red, respectively,
//	for a pixel. The high byte in each DWORD is not used.
//	
//	If the biCompression member of the BITMAPINFOHEADER is BI_BITFIELDS, the bmiColors member
//	contains three DWORD color masks that specify the red, green, and blue components,
//	respectively, of each pixel. Each DWORD in the bitmap array represents a single pixel.
//	
//	(Windows NT): When the biCompression member is BI_BITFIELDS, bits set in each DWORD mask 
//	must be contiguous and should not overlap the bits of another mask. All the bits in the 
//	pixel do not have to be used.
//	
//	(Windows 95): When the biCompression member is BI_BITFIELDS, Windows 95 supports only the 
//	following 32bpp color mask: The blue mask is 0x000000FF, the green mask is 0x0000FF00, 
//	and the red mask is 0x00FF0000.
//
// Inputs:
//		BITMAPINFO* pBmpInfo:	Pointer to bitmap info header
//
// Returns:
//		Number of colors in the color table
//
int CMsWindowPaint::NumDIBColorEntries(BITMAPINFO* pBmpInfo) 
{
    BITMAPINFOHEADER* pBIH = &(pBmpInfo->bmiHeader);
    BITMAPCOREHEADER* pBCH = (BITMAPCOREHEADER*) pBIH;

    // Start off by assuming the bit count from the bits-per-pixel field
    int iBitCount;
    if (IsWinDIB(pBIH))
    {	iBitCount = (int) pBIH->biBitCount;
    }
    else
    {	iBitCount = (int) pBCH->bcBitCount;
    }

    int iColors;
    switch (iBitCount)
    {
    case 1:
        iColors = 2;
        break;
    case 2:
        iColors = 4;
        break;
    case 4:
        iColors = 16;
        break;
    case 8:
        iColors = 256;
        break;
    case 16:
    case 24:
    case 32:
    default:
        iColors = 0;
        break;
    }

    // If this is a Windows DIB, then the color table length
    // is determined by the biClrUsed field if the value in
    // the field is nonzero.
    if (IsWinDIB(pBIH) && (pBIH->biClrUsed != 0))
    {	iColors = (int) pBIH->biClrUsed;
    }

    return iColors;
}

//
// PaletteSize
//
// This function gets the size required to store the DIB's palette by
// multiplying the number of colors by the size of an RGBQUAD (for a
// Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
// style DIB).
//
// Inputs:
//		BITMAPINFO* pBmpInfo: Pointer to bitmap info header
//
// Returns:
//		Size of the color palette of the DIB
//
WORD CMsWindowPaint::PaletteSize(BITMAPINFO* pBmpInfo)
{
    BITMAPINFOHEADER* pBIH = &(pBmpInfo->bmiHeader);

    // Calculate the size required by the palette
    if (IsWinDIB(pBIH))
    {	
        return (WORD)(NumDIBColorEntries(pBmpInfo) * sizeof(RGBQUAD));
    }
    else
    {	
        return (WORD)(NumDIBColorEntries(pBmpInfo) * sizeof(RGBTRIPLE));
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMsWindowPaint class public functions 


bool CMsWindowPaint::CreateBMI()
{

    // Allocate memory for the header (if necessary)
    BITMAPINFO* pBMI = AllocBMIBuffer(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

    if (!pBMI)
    {
        //		AfxMessageBox("Create failed, ran out of memory for DIB header.", MB_OK|MB_ICONEXCLAMATION);
        Destroy();
        return FALSE;
    }

    // Fill in the header info.

    BITMAPINFOHEADER* pBI = (BITMAPINFOHEADER*) pBMI;
    pBI->biSize = sizeof(BITMAPINFOHEADER);
    pBI->biWidth = GetWidth();
    pBI->biHeight = -GetHeight();
    pBI->biPlanes = 1;
    pBI->biBitCount = (GetType()==TypeBYTE)?8:(GetType() == TypeBGR)? 24:32;
    pBI->biCompression = BI_RGB;
    pBI->biSizeImage = 0;
    pBI->biXPelsPerMeter = 0;
    pBI->biYPelsPerMeter = 0;
    pBI->biClrUsed = 0;
    pBI->biClrImportant = 0;

    RGBQUAD* prgb = GetClrTabAddress();

    if (m_pMonochromeLut)
    {
        for (int i = 0; i < 256; i++)
        {	
            prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = (BYTE) m_pMonochromeLut[i];
            prgb->rgbReserved = 0;
            prgb++;
        }

    } 
    else
        for (int i = 0; i < 256; i++)
        {	
            prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = (BYTE) i;
            prgb->rgbReserved = 0;
            prgb++;
        }

        if (!CreateDIBPalette(TRUE, FALSE))
        {	
            Destroy();
            return FALSE;
        }

        return TRUE;
}

// Destroy DIB (deallocate buffers)
void CMsWindowPaint::Destroy()
{

    FreeBMIBuffer();
    FreePalleteBuffer();

    if (m_hPal)
    {   
        ::DeleteObject(m_hPal);
        m_hPal = NULL;
    }

}


// Creates a palette from a DIB by allocating memory for the
// logical palette, reading and storing the colors from the DIB's color table
// into the logical palette, creating a palette from this logical palette
// This allows the DIB to be displayed using the best possible colors (important
// for DIBs with 256 or  more colors).
//
bool CMsWindowPaint::CreateDIBPalette(bool bWithMsgBox,bool bUsingDirectDraw)
{

    BITMAPINFO* pBMI = GetBMIBuffer();	
    if (!pBMI) { return FALSE; }

    // get the number of colors in the DIB


    int iNumColors = NumDIBColorEntries(pBMI);


    if (m_hPal)
    {   
        ::DeleteObject(m_hPal);
        m_hPal = NULL;
    }

    if (iNumColors)
    {
        // allocate memory block for logical palette
        LPLOGPALETTE lpPal = AllocPalleteBuffer(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * iNumColors);
        if (!lpPal)
        {	
            TCHAR szBuf[256];
            sprintf(szBuf, "Ran out of memory for logical palette.");
            if (bWithMsgBox) 
                ::MessageBox(NULL,szBuf, NULL,MB_OK|MB_ICONEXCLAMATION);
            return FALSE;
        }

        // set version and number of palette entries 
        lpPal->palVersion = 0x300; // PALVERSION
        lpPal->palNumEntries = (WORD)iNumColors;
        for (int i = 0; i < iNumColors; i++)
        {   lpPal->palPalEntry[i].peRed = pBMI->bmiColors[i].rgbRed;
        lpPal->palPalEntry[i].peGreen = pBMI->bmiColors[i].rgbGreen;
        lpPal->palPalEntry[i].peBlue = pBMI->bmiColors[i].rgbBlue;
        lpPal->palPalEntry[i].peFlags = 0;
        }

        // create the palette and get handle to it
        if (!m_hPal)
            m_hPal = ::CreatePalette(lpPal);



    }
    return TRUE;
}



// Draw the DIB to a given DC.
void CMsWindowPaint::Draw(EdtImage *pImage,
                          ImageWindowData *pWindow,
                          HDC hdc,
                          EdtBox *pRect,
                          bool erase)
{


    pWindow->SetupDraw(pImage,pRect);

    EdtImage *pDisplayImage = pWindow->GetDisplayImage();

    HWND hwnd = (HWND) pWindow->GetWindow();

    ASSERT(pDisplayImage);

    AttachImageToDIB(pDisplayImage);

    // Get client rectangle in display coordinates
    CRect rcClientDP;
    GetClientRect(hwnd,&rcClientDP);


    //TRACE("ClientRectDP(%d,%d,%d,%d)\n", rcClientDP.left, rcClientDP.top, rcClientDP.right, rcClientDP.bottom);

    // Get client rectangle in logical coordinates
    CRect rcClientLP = rcClientDP;
    DPtoLP(hdc,(POINT *) &rcClientLP,2);

    //TRACE("ClientRectLP(%d,%d,%d,%d)\n", rcClientLP.left, rcClientLP.top, rcClientLP.right, rcClientLP.bottom);

    // Get clipping region
    CRect rcClipBox;
    GetClipBox(hdc, &rcClipBox);
    //TRACE("ClipBox(%d,%d,%d,%d)\n", rcClipBox.left, rcClipBox.top, rcClipBox.right, rcClipBox.bottom);

    BITMAPINFO* pBMI = GetBMIBuffer();	
    if (!pBMI) { return; }

    bool stretching = FALSE;

    int x, y, w, h;
    // Ignore clipping under certain scenarios

    if (pRect)
    {
        x = pRect->left;
        y = pRect->top;
        w = pRect->Width();
        h = pRect->Height();

    }
    else if (0 && !rcClipBox.IsRectEmpty())
    {


        x = rcClipBox.left;
        y = rcClipBox.top;
        w = rcClipBox.Width() - pWindow->GetScrollWidth();
        h = rcClipBox.Height() - pWindow->GetScrollHeight();


        rcClipBox &= rcClientDP;

    }

    else

    {

        x = rcClientDP.left;
        y = rcClientDP.top;
        w = rcClientDP.Width() -  pWindow->GetScrollWidth();
        h = rcClientDP.Height() - pWindow->GetScrollHeight();

        if (erase)
        {
            static HBRUSH hbrBkgnd = NULL;  // handle of background-color brush 
            if (!hbrBkgnd)
            {
                COLORREF crBkgnd = RGB(128,128,128); 
                hbrBkgnd = CreateSolidBrush(crBkgnd); 
            }
            ::FillRect(hdc,&rcClientDP,hbrBkgnd);
        }


    }

    int ix = pWindow->GetDisplayCornerX();
    int iy = pWindow->GetDisplayCornerY();
    int iw = pWindow->GetDisplayWidth();
    int ih = pWindow->GetDisplayHeight();

    int nWidth;
    int nHeight;

    //	if (x < ix)
    //	{
    //		w += x;
    //		x = ix;
    //	}

    //	if (y < iy)
    //	{
    //		h += y;
    //		y = iy;
    //	}

    if (w < 1 || h < 1)
        return;

    // Make sure the bits being drawn intersect with the dib
    if (x >= iw || y >= ih) 
    { 
        return; 
    }


    // Check for top down bitmaps where scan lines are stored in top down order
    int nSrcx; 
    int nSrcy; 
    // Adjust width and height to not exceed dib width and height

    if (m_bStretchData)
    {
        /* adjust */
        float xscale = (float) GetWidth()/(float ) rcClientDP.Width() ;		
        float yscale = (float) GetHeight()/(float ) rcClientDP.Height();
        nSrcx = (int) (x * xscale);
        nSrcy = (int) (yscale *  y);
        if (nSrcy < 0) 
            nSrcy = 0;
        nWidth = GetWidth();
        nHeight = GetHeight();
        stretching = TRUE;
    }
    else 
    {
        if (x + w > iw)
        {	
            w = iw - x;
        }
        if (y + h > ih)
        {	
            h = ih - y;
        }

        int vpagesize = h ;


        if (pWindow->GetZoom().IsActive())
        {

            vpagesize = (int) (vpagesize / pWindow->GetZoom().GetZoomY());


            nWidth = (int) (w / pWindow->GetZoom().GetZoomX());
            nHeight = (int) (h / pWindow->GetZoom().GetZoomY());
            nSrcx = (int)(x /pWindow->GetZoom().GetZoomX()) + ix;
            nSrcy = GetHeight() - (vpagesize + (int) (y / pWindow->GetZoom().GetZoomY()) +  iy);

            if (pWindow->GetZoom().GetZoomX() != 1.0 || 
                pWindow->GetZoom().GetZoomY() != 1.0)
                stretching = TRUE;
        }
        else
        {
            nWidth = w;
            nHeight = h;
            nSrcx = x + ix;
            nSrcy = GetHeight() - (h + (y) + iy);
        }

    }
    // If Direct Draw available then use it

    //TRACE("CMsWindowPaint::Draw\n");

    HPALETTE hOldPal = NULL;
    HBITMAP hbm = NULL;
    HBITMAP hbmOld = NULL;
    HDC hdcImage = NULL;
    bool bSuccess = TRUE;
    DWORD dwError = 0;

    for (int idx = 0; idx < 1; idx++)	// for error handling break outs to work
    {
        // Get the DIB's palette, then select it into DC
        if (m_hPal)
        {	
            hOldPal = ::SelectPalette(hdc, (HPALETTE)m_hPal, TRUE);
            ::RealizePalette(hdc);
        }

        // Specific setup for using SetDIBits()

        // End specific setup for using SetDIBits()


        int nRepeat = 1;
        DWORD dwStartTime = GetTickCount();



        for (int idy = 0; idy < nRepeat; idy++)
        {
            // Determine whether to call StretchBlt() or BitBlt()
            DWORD dwError =0;

            //TRACE ("Drawing %d %d -> %d %d\n",
            //	nSrcx, nSrcy, nWidth, nHeight);

            if (stretching)
            {

                int oldstretch = ::SetStretchBltMode(hdc, COLORONCOLOR);

                int nResult = ::StretchDIBits(
                    hdc,
                    x,						// Destination x
                    y,						// Destination y
                    w,					// Destination width
                    h,				// Destination height
                    nSrcx,					// Source x
                    nSrcy,					// Source y
                    nWidth,						// nStartScan
                    nHeight,			// nNumScans
                    pDisplayImage->GetFirstPixel(),					// Pointer to bits
                    pBMI,					// BITMAPINFO
                    DIB_RGB_COLORS,
                    SRCCOPY);		// Options

                if (nResult == GDI_ERROR)
                {
                    bSuccess = FALSE;  
                    dwError = GetLastError();
                    break;
                }

                if (oldstretch)
                    SetStretchBltMode(hdc, oldstretch);

            }
            else
            {
                int nWidth = edt_min((WORD)iw, w);

                int nHeight = edt_min((WORD)ih, h);
                //int nResult = ::BitBlt(hdc, x, y, nWidth, nHeight, hdcImage, nSrcx, nSrcy, SRCCOPY);

                int nResult = ::SetDIBitsToDevice(hdc,
                    x,						// Destination x
                    y,						// Destination y
                    nWidth,					// Destination width
                    nHeight,				// Destination height
                    nSrcx,					// Source x
                    nSrcy,					// Source y
                    0,						// nStartScan
                    GetHeight(),			// nNumScans
                    pDisplayImage->GetFirstPixel(),					// Pointer to bits
                    pBMI,					// BITMAPINFO
                    DIB_RGB_COLORS);		// Options

                if (nResult == GDI_ERROR)
                {
                    bSuccess = FALSE;  
                    dwError = GetLastError();
                    break;
                }



            }
        }

    }

    if (hdcImage)
    {
        if (hbmOld)
        {	
            ::SelectObject(hdcImage, hbmOld);
        }
        ::DeleteDC(hdcImage);
    }

    if (hbm)
    {	
        ::DeleteObject(hbm);
    }

    if (hOldPal)
    {   
        ::SelectPalette(hdc, hOldPal, TRUE);
        ::RealizePalette(hdc);
    }

}


// Allocate BMI buffer
BITMAPINFO* CMsWindowPaint::AllocBMIBuffer(int nNewBMISize)
{
    if (m_pBMI && m_lBMI < nNewBMISize)
    {	
        FreeBMIBuffer();
    }

    if (!m_pBMI)
    {	
        m_lBMI = 0;
        m_pBMI = (BITMAPINFO*) malloc(nNewBMISize);
        if (!m_pBMI)
        {	
            return NULL;
        }
    }

    m_lBMI = nNewBMISize;
    return GetBMIBuffer();
}

// Get pointer to BMI buffer
BITMAPINFO* CMsWindowPaint::GetBMIBuffer()
{	
    return m_pBMI;
}


// Free BMI buffer
void CMsWindowPaint::FreeBMIBuffer()
{	if (m_pBMI)
{	free(m_pBMI);
m_pBMI = NULL;
m_lBMI = 0;
}
}


// Allocate pallete buffer
LPLOGPALETTE CMsWindowPaint::AllocPalleteBuffer(int nNewPALSize)
{
    if (m_pPal && m_lPal < nNewPALSize)
    {	FreePalleteBuffer();
    }

    if (!m_pPal)
    {	m_lPal = 0;
    m_pPal = (LPLOGPALETTE) malloc(nNewPALSize);
    if (!m_pPal)
    {	return NULL;
    }
    }

    m_lPal = nNewPALSize;
    return GetPalleteBuffer();
}

// Get pointer to the pallete buffer
LPLOGPALETTE CMsWindowPaint::GetPalleteBuffer()
{	return m_pPal;
}

// Free pallete buffer
void CMsWindowPaint::FreePalleteBuffer()
{	if (m_pPal)
{	
    free(m_pPal);
    m_pPal = NULL;
    m_lPal = 0;
}
}

// Get pointer to color table                       
RGBQUAD* CMsWindowPaint::GetClrTabAddress()
{	
    BITMAPINFO* pBMI = GetBMIBuffer();	
    if (!pBMI) { 
        return NULL; 
    }

    return (LPRGBQUAD)(((BYTE *)(pBMI)) + sizeof(BITMAPINFOHEADER));

}       



EdtImage * CMsWindowPaint::NewDisplayableImage(const int nType)

{

    EdtImage *pImage;

    EdtDataType display_type = TypeBYTE;

    pImage =  new EdtImage;

    if (nType == TypeBGR)
        display_type = TypeBGR;
    else if (nType == TypeBGRA)
        display_type = TypeBGRA;

    pImage->SetType(display_type);

    return pImage;
}

EdtImage * CMsWindowPaint::NewDisplayableImage(const long nWidth, const long nHeight, const int nType)
{
    EdtImage *pImage;

    EdtDataType display_type = TypeBYTE;

    if (nType == TypeBGR)
        display_type = TypeBGR;
    else if (nType == TypeBGRA)
        display_type = TypeBGRA;

    pImage =  new EdtImage;

    pImage->Create(display_type, nWidth, nHeight);

    return pImage;
}



void CMsWindowPaint::SetPaletteLinear()
{
    RGBQUAD* prgb = GetClrTabAddress();

    if (prgb)
    {
        for (int i = 0; i < 256; i++)
        {	
            prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = i;
            prgb->rgbReserved = 0;
            prgb++;
        }
        if (!CreateDIBPalette(TRUE, FALSE))
        {	
            Destroy();

        }

    }
}


void CMsWindowPaint::SetPaletteInfrared()
{
    RGBQUAD* prgb = GetClrTabAddress();

    if (prgb)
    {
        ULONG binsize = (256 / RAINBOW_COLORS) ;
        float rstep, gstep, bstep, rrange, brange, grange;
        ULONG low, high, tmpidx;
        for (int i = 0; i < 256; i++)
        {
            tmpidx = i / binsize ;
            low = colorspread[tmpidx] ;
            high = colorspread[tmpidx+1] ;
            rrange = RED(high) - RED(low) ;
            grange = GREEN(high) - GREEN(low) ;
            brange = BLUE(high) - BLUE(low) ;
            rstep = rrange / (float)binsize ;
            gstep = grange / (float)binsize ;
            bstep = brange / (float)binsize ;
            prgb->rgbRed = (BYTE)(RED(low) + (rstep * (i % binsize))) ;
            prgb->rgbGreen = (BYTE)(GREEN(low) + (gstep * (i % binsize))) ;
            prgb->rgbBlue = (BYTE)(BLUE(low) + (bstep * (i % binsize))) ;
            prgb->rgbReserved = 0 ;
            prgb++ ;
        }


        if (!CreateDIBPalette(TRUE, FALSE))
        {	
            Destroy();	
        }
    }
}


void CMsWindowPaint::SetMonochromeLut(byte * pLut)
{
    m_pMonochromeLut = pLut;

    RGBQUAD* prgb = GetClrTabAddress();

    if (prgb)
    {
        for (int i = 0; i < 256; i++)
        {	
            prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = (BYTE) m_pMonochromeLut[i];
            prgb->rgbReserved = 0;
            prgb++;
        }

        if (!CreateDIBPalette(TRUE, FALSE))
        {	
            Destroy();

        }
    }

}

void CMsWindowPaint::AttachImageToDIB(EdtImage *pImage)
{

    //Make sure DIB matches the image

    if (pImage->GetWidth() != GetWidth() ||
        pImage->GetHeight() != GetHeight() || 
        pImage->GetType() != GetType() ||
        !m_pBMI)
    {

        SetBufferValues(*pImage);

        CreateBMI();

    }

}

EdtImage * CMsWindowPaint::GetDisplayableImage(ImageWindowData *pWindow,
                                               EdtImage *pSrcImage)

{
    EdtImage *pImage = new EdtImage;

    pImage->Create(TypeBYTE, pSrcImage->GetWidth(), pSrcImage->GetHeight());

    return pImage;
}

