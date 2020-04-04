// WindowPaint.h: interface for the WindowPaint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_EDT_WINDOWPAINT_H_)
#define  _EDT_WINDOWPAINT_H_

#include "edtimage.h"

#include "EdtScaledLut.h"


class ImageWindowData;

class WindowPaint  : public EdtImageData
{
public:


    WindowPaint();
    virtual ~WindowPaint();


    virtual void Draw(
	EdtImage *pImage,
	ImageWindowData * pWindow,
	EdtDC hDC,
	EdtBox *pRect,
	bool clear = false);



    virtual bool IsDirectlyDisplayable(EdtImage *pImage);

    virtual EdtImage *GetDisplayableImage(ImageWindowData * pWindow,
	EdtImage *pSrcImage);

    virtual void SetPaletteInfrared() {}
    virtual void SetPaletteLinear() {}


};

#endif // !defined(_EDT_WINDOWPAINT_H_)
