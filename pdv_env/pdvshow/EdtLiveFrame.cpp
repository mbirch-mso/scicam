// EdtLiveFrame.cpp: implementation of the EdtLiveFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EdtLiveFrame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(EdtLiveFrame, CEdtImageFrame)

EdtLiveFrame::EdtLiveFrame(LiveDisplay *pGrabber)
{
    m_pGrabber = pGrabber;
    m_pImage = NULL;
    m_bOwnGrabber = FALSE;
    m_bOwnImage = FALSE;

}

EdtLiveFrame::EdtLiveFrame(const char *devname, 
			   const int unit,
			   const int channel)
{

    char s[100];

    strncpy(s, devname, 99);

    m_pGrabber = new LiveDisplay(s, unit, channel);

    m_pImage = NULL;
    m_bOwnGrabber = TRUE;
    m_bOwnImage = FALSE;
}

EdtLiveFrame::~EdtLiveFrame()
{
    if (m_pImage && m_bOwnImage)
	delete m_pImage;

    if (m_pGrabber && m_bOwnGrabber)
	delete m_pGrabber;
}


BEGIN_MESSAGE_MAP(EdtLiveFrame, CEdtImageFrame)
    //{{AFX_MSG_MAP(EdtLiveFrame)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


EdtLiveFrame *EdtLiveFrame::NewLiveViewer(LiveDisplay *pGrabber)
{
    EdtLiveFrame *pFrame = new EdtLiveFrame;

    pFrame->CreateLiveViewer(pGrabber);

    return pFrame;


}

void EdtLiveFrame::SetGrabber(LiveDisplay *pGrabber)

{
    if (m_pGrabber)
    {
	if (m_bOwnGrabber)
	    delete m_pGrabber;

    }

    m_pGrabber = pGrabber;

}


void EdtLiveFrame::CreateLiveViewer(LiveDisplay *pGrabber)

{

    if (pGrabber)
	SetGrabber(pGrabber);

    if (!m_pGrabber)
	return;

    m_pImage = m_pGrabber->GetCamera()->GetCameraImage(NULL);

    m_bOwnImage = TRUE;

    CreateImageViewer(m_pGrabber->GetCamera()->GetCameraType(), m_pImage, NULL);



}

void EdtLiveFrame::StartLive()
{
    if (m_pGrabber)
    {
	m_pGrabber->StartLive(&m_pImageWindow->m_wData,TRUE);
    }
}

void EdtLiveFrame::StopLive()

{
    if (m_pGrabber)
    {
	m_pGrabber->StopLive();
    }

}


void EdtLiveFrame::Snap()
{
    if (m_pGrabber)
    {
	if (m_pGrabber->IsLive())
	{	
	    StopLive();
	    m_pImage->Copy(m_pGrabber->GetCamera()->GetLastImage());
	}
	else
	{
	    m_pGrabber->GetCamera()->CaptureImage(m_pImage);
	    m_pImageWindow->UpdateDisplay();
	}

    }
}
