// WindowPaint.cpp: implementation of the WindowPaint class.
//
//////////////////////////////////////////////////////////////////////

#include "edtdefines.h"

#include "WindowPaint.h"

#include <stdio.h>

#ifdef __WXWINDOWS__

#include "wxImageWindowData.h"

#else

#include "imagewindowdata.h"

#endif

#define no_printf

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WindowPaint::WindowPaint()
{

}

WindowPaint::~WindowPaint()
{

}

void WindowPaint::Draw(EdtImage *pImage,
					 ImageWindowData * pWindow,
					EdtDC pDC,
					EdtBox *pRect,
					bool clear_border)

{
	
	pWindow->SetupDraw(pImage,pRect);

	// Do nothing... This function is meant to be overriden


	
}




EdtImage * WindowPaint::GetDisplayableImage(ImageWindowData *pWindow,
											  EdtImage *pSrcImage)

{
	return NULL;
}

bool WindowPaint::IsDirectlyDisplayable(EdtImage *pImage)
{

  printf("Shouldn't get here...\n");

  exit(1);

  return TRUE;

}

