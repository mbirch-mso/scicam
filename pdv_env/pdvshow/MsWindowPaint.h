// CMsWindowPaint.h : header file

#ifndef __CMsWindowPaint_H__
#define __CMsWindowPaint_H__

#if 0

#define	PAGE_SIZE			4096

#define	DIB_CURRENT			0
#define	DIB_NORMAL			1
#define	DIB_ZOOMED			2
#define	DIB_RAW				3

#define	MAX_WIDTH			SHRT_MAX
#define	MIN_WIDTH			0
#define MAX_HEIGHT			SHRT_MAX
#define	MIN_HEIGHT			0
#define MAX_SIZE			4096*4096

#define	INCHESPERMETER		39.37

#endif

#include "edtimage.h"

#include "ZoomData.h"

#include "WindowPaint.h"


// CMsWindowPaint class
class CMsWindowPaint : public WindowPaint
{

public:
    void AttachImageToDIB(EdtImage *pImage);
    void SetPaletteInfrared();
    void SetPaletteLinear();
    bool m_bStretchData;
    bool m_bWaitForVSync;


    // "Factory" function which will return a displayable image if the specified type is
    // directly displayable (EDT_TYPE_BYTE or EDT_TYPE_BGR)
    // Otherwise it creates a EdtImage which must be remapped to the display
    //

    static EdtImage *NewDisplayableImage(const long nWidth, const long nHeight, const int nType);
    static EdtImage *NewDisplayableImage(const int nType);

    byte * m_pMonochromeLut;
    void SetMonochromeLut(byte *pLut);
    CMsWindowPaint();
    virtual ~CMsWindowPaint();


    virtual void Destroy();
    virtual void Draw(EdtImage *pSrc, 
	ImageWindowData * pWindow,
	HDC pDC,
	EdtBox *pRect,
	bool erase = false);	// Render DIB



    virtual BITMAPINFO* GetBMIBuffer();
    //    virtual BYTE* GetBitsBuffer();


protected:

    virtual bool CreateBMI();

    static bool IsWinDIB(BITMAPINFOHEADER*);			// Determine if bitmap in windows format
    static int NumDIBColorEntries(BITMAPINFO*);			// Determine if bitmap in windows format
    static WORD PaletteSize(BITMAPINFO*);				// Calculate size of the pallete (colormap)

    virtual bool CreateDIBPalette(bool, bool);
    // Create a new DIB pallete
    virtual BITMAPINFO* AllocBMIBuffer(int);
    virtual void FreeBMIBuffer();

    virtual LPLOGPALETTE AllocPalleteBuffer(int);
    virtual LPLOGPALETTE GetPalleteBuffer();
    virtual void FreePalleteBuffer();
    virtual RGBQUAD* GetClrTabAddress();

    bool IsDirectlyDisplayable(EdtImage *pImage)
    {
	if ( pImage && 
	    (pImage->GetType() == EDT_TYPE_BYTE) || 
	    (pImage->GetType() == EDT_TYPE_BGR) ||
	    (pImage->GetType() == EDT_TYPE_BGRA)  ||
	    (pImage->GetType() == EDT_TYPE_RGB) ||
	    (pImage->GetType() == EDT_TYPE_RGBA)  )

	    return TRUE;

	else
	    return FALSE;


    }

    EdtImage *GetDisplayableImage(ImageWindowData *pWindow,
	EdtImage *pSrcImage);

    // Render DIB using Direct Draw
protected:

    BITMAPINFO* m_pBMI;			// Pointer to BMI buffer where BITMAPINFO is stored

    long m_lBMI;				// Length of the BMI buffer allocated

    LPLOGPALETTE m_pPal;		// Pointer to the pallete buffer
    long m_lPal;				// Length of the pallete allocated
    HANDLE m_hPal;				// Handle to the pallete for this DIB


};

#endif // __DIB__
