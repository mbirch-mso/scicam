
//
// Do a ring buffer of live windows if more than one is defined
//


#include "edtimage/EdtImage.h"

#include "EdtQtLiveDisplay.h"

#include <iostream>




extern "C" {

#include "edtinc.h"

}

#ifdef WIN32

#include <process.h>

#endif

#include "dispatch/ErrorHandler.h"

EdtQtLiveDisplay::EdtQtLiveDisplay(const char *devname, const int nUnit, const int nChannel)
{
    if (((m_pCamera = new PdvInput(devname, nUnit,nChannel)) == NULL)
        || (m_pCamera->GetDevice() == NULL))
        return;

    InitVariables();

    m_bOwnCamera = TRUE;

}

EdtQtLiveDisplay::EdtQtLiveDisplay(PdvInput *pCamera)
{

    m_pCamera = pCamera;

    m_bOwnCamera = FALSE;

    InitVariables();

}

EdtQtLiveDisplay::EdtQtLiveDisplay(PdvDev *pdv_p)
{

    m_pCamera = new PdvInput(pdv_p);

    m_bOwnCamera = TRUE;

    InitVariables();

}
EdtQtLiveDisplay::~EdtQtLiveDisplay()
{
    DeleteCriticalSection(&m_lock);

    if (m_pSingle)
        delete m_pSingle;

    if (m_bOwnCamera)
        delete m_pCamera;
}

THREAD_FUNC_DECLARE
LiveThreadFunc(void *param)

{

    EdtQtLiveDisplay *pObj = (EdtQtLiveDisplay *) param;

    return (THREAD_RETURN) pObj->DoLiveThreadFunc();

}


bool EdtQtLiveDisplay::StopLive()

{

    Lock();
    TRACE("Locked %d: %x\n", m_nChannelID, m_pThread);	

    HANDLE pThread = m_pThread;

    StopPump();

    Unlock();
    TRACE("UnLocked %d: %x\n", m_nChannelID, m_pThread);	

    if (pThread)
    {
        //	WaitForThread(pThread,INFINITE);
    }


    return TRUE;
}

EdtImage * EdtQtLiveDisplay::DoLiveWindow(bool display)
{
    EdtImage *pImage = NULL;

    pImage = GetNextBufferImage(m_dwImageCount);

    // 

    if (pImage)
    {
        // m_Time.SetBufferTime(m_dwImageCount, pImage->GetTimeStamp());

        EdtQtImageItem *pWindow = NULL;

        if (display)
            pWindow = DoDisplay(pImage, m_dwImageCount);

        NotifyEventFuncs(pImage,pWindow,m_dwImageCount);

        m_dwImageCount ++;

    }

    return pImage;

}


void EdtQtLiveDisplay::AddEventFunc(ImageEventFunc f, void *target)
{
    // Make sure not already in list

    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;

    for (iter = m_AcqList.begin(); iter != m_AcqList.end();iter++)
    {
        pTest = *iter;

        if (pTest->target == target && pTest->pFunc == f)
            return;

    }

    m_AcqList.push_back(new EventFuncRec(f,target));
}

void EdtQtLiveDisplay::RemoveEventFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;
    for (iter = m_AcqList.begin(); iter != m_AcqList.end();iter++)
    {
        pTest = *iter;

        if (pTest->target == target && pTest->pFunc == f)
        {
            m_AcqList.remove(pTest);

            return;
        }        
    }
}

void EdtQtLiveDisplay::NotifyEventFuncs(EdtImage * pImage, EdtQtImageItem *pWindow, long nFrameNumber)
{
    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;
    for (iter = m_AcqList.begin(); iter != m_AcqList.end();iter++)
    {
        pTest = *iter;

        if (pTest->pFunc)
        {
            pTest->pFunc(pTest->target, pImage, pWindow, nFrameNumber);
        }        
    }

}

void EdtQtLiveDisplay::AddFinishFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;

    for (iter = m_FinishList.begin(); iter != m_FinishList.end();iter++)
    {
        pTest = *iter;

        if (pTest->target == target && pTest->pFunc == f)
            return;

    }

    m_FinishList.push_back(new EventFuncRec(f,target));


}

void EdtQtLiveDisplay::RemoveFinishFunc(ImageEventFunc f, void *target)
{

    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;
    for (iter = m_FinishList.begin(); iter != m_FinishList.end();iter++)
    {
        pTest = *iter;

        if (pTest->target == target && pTest->pFunc == f)
        {
            m_FinishList.remove(pTest);

            return;
        }        
    }

}

void EdtQtLiveDisplay::NotifyFinishFuncs()
{
    EventFuncRec *pTest;
    list<EventFuncRec *>::iterator iter;
    for (iter = m_FinishList.begin(); iter != m_FinishList.end();iter++)
    {
        pTest = *iter;

        if (pTest->pFunc)
        {
            pTest->pFunc(pTest->target, NULL, NULL, 0);
        }        
    }
}


int EdtQtLiveDisplay::GetFrameCount()
{
    return m_dwImageCount;
}

void EdtQtLiveDisplay::Abort()

{
    if (m_pThread)
    {
        thread_t start_thread = m_pThread;

        m_pThread = (thread_t) 0;

        SetActiveFlag(FALSE);

        m_pCamera->Abort();

    }


}


bool EdtQtLiveDisplay::Start(bool bUseThread)
{

    m_Time.Start();

    TRACE("StartLive\n");

    // Launch the thread
    if (bUseThread && m_pThread)
        return 0;

    SetActiveFlag(TRUE);


    StartPump();

    m_dwImageCount = 0;
    m_nLastTimeStamp = 0;

    // Set Start Time
    m_dStartTime = edt_timestamp();

    if (bUseThread)
    {

        LaunchThread(m_pThread, LiveThreadFunc, this);

    }

    m_Time.StartTimer();

    return TRUE;
}

bool EdtQtLiveDisplay::AllFinished()
{
    return BuffersFinished();
}

//template class EdtListHead<EventFuncRec>;


UINT EdtQtLiveDisplay::DoLiveThreadFunc()
{

    bool bStarted = FALSE;

    while (GetActiveFlag())
    {
        // Debugging stuff


        if (m_bStartPending)
        {
            edt_msleep(1);
        }
        else
        {

            if (!bStarted)
            {
                edt_msleep(10);
                bStarted = TRUE;
            }

            // printf("Going to DoLiveWindow()\n");

            DoLiveWindow();

            if (AllFinished())
                SetActiveFlag(FALSE);

        }
    }


#if 0 
    char s[80];
    sprintf(s,"EdtQtLiveDisplay::DoLiveThreadFunc()Finished... %x\n", m_pThread);

    OutputDebugString(s);
#endif

    if (m_pCamera)
        m_pCamera->StopAcquiring();

    NotifyFinishFuncs();

    m_pThread = (thread_t) 0;


    return 0;
}

/**********************************************************************************************//**
 * @fn  EdtQtImageItem * EdtQtLiveDisplay::GetNextLiveWindow()
 *
 * @brief   Gets the next live window.
 *
 * @return  null if it fails, else the next live window.
 **************************************************************************************************/

EdtQtImageItem *  EdtQtLiveDisplay::GetNextLiveWindow()

{

    if (m_nLiveWindows == 1)

        m_pCurrentWindow = m_wndLiveWindows[0];

    else if (m_nLiveWindows == 0)
    {
        std::cout << "Setting to NULL" << std::endl;
        m_pCurrentWindow = NULL;
    }

    else
    {
        m_pCurrentWindow = m_wndLiveWindows[m_nCurrentLiveWindow];
        m_nCurrentLiveWindow = (m_nCurrentLiveWindow + 1) % m_nLiveWindows;
    }

    return m_pCurrentWindow;
}


bool EdtQtLiveDisplay::SetDisplayWindows(EdtQtImageItem * *LiveWindows, int nWindows)
{

    for (int i = 0; i< nWindows;i++)
    {
        AddDisplayWindow(LiveWindows[i]);
    }

    m_pCurrentWindow = LiveWindows[0];

    return TRUE;
}

void EdtQtLiveDisplay::AddDisplayWindow(EdtQtImageItem *wnd)

{
    m_wndLiveWindows.push_back(wnd);
    m_nLiveWindows = m_wndLiveWindows.size();
}

void EdtQtLiveDisplay::DisplayLiveImage(EdtImage *pImage, 
                                   EdtQtImageItem * pWindow,
                                   int nCount)
{
 
   pWindow->SetDataImage(pImage);

   

}


EdtQtImageItem * EdtQtLiveDisplay ::DoDisplay(EdtImage *pImage, long nCount)
{
    if (!m_nDisplaySkip)
        return NULL;

    if (m_nDisplaySkip > 1)
    {
        if ((nCount % m_nDisplaySkip) != 0)
            return NULL;
    }

    EdtQtImageItem * pWindow = GetNextLiveWindow();

    TRACE("pWindow = %x pImage %x\n",		   pWindow, pImage);

    if (pWindow && pImage)
    {
        DisplayLiveImage(pImage, pWindow, nCount);
    }
    else
    {

    }	

    return pWindow;
}

void EdtQtLiveDisplay::capture_single_image(bool resize)
{

    if (!m_pSingle)
        return;

    if (resize || (m_pSingle->GetBaseData() == NULL))
	{
        m_pSingle->Resize(m_pCamera->GetType(), m_pCamera->GetWidth(), m_pCamera->GetHeight(), true);
		m_pSingle->SetMinMax(0, m_pCamera->GetMaxValue());
	}

    m_pCamera->CaptureImage(m_pSingle);

    EdtQtImageItem * pWindow = GetNextLiveWindow();

    if (pWindow)
        pWindow->SetDataImage(m_pSingle);

}

// PdvImageSource

void EdtQtLiveDisplay::PrepareBuffers()
{

    // printf("PrepareBuffers for Camera %x\n", m_pCamera);


    if (m_pCamera)
    {

        // printf("Calling m_pCamera->PrepareBuffers\n");

        m_pCamera->PrepareBuffers();

        // printf("Back from m_pCamera->PrepareBuffers\n");

        m_nWidth = m_pCamera->GetWidth();
        m_nHeight = m_pCamera->GetHeight();
        m_nType = m_pCamera->GetType();
        m_nPitch = m_pCamera->GetPitch();

        m_bStartPending = TRUE;

    }
    else
    {

    }

}

PdvInput * EdtQtLiveDisplay::GetCamera()
{
    return m_pCamera;
}


EdtImage * EdtQtLiveDisplay::GetNextBufferImage(int nCount)

{
    static double last_time = 0.0;

    if (m_pCamera)
    {
        EdtImage *pImage;


        pImage = m_pCamera->GetNextBufferImage();

        m_nLastCount = m_pCamera->GetDoneCount();

        if (m_bStartPending)
        {
            m_bStartPending = FALSE;

            TRACE("Turning off start pending %x: %d > %d %f\n",
                m_pCamera, m_pCamera->GetStarted(), m_pCamera->GetDone(), edt_timestamp());

        }

        double timestamp = 0;

        if (pImage)
        {

            timestamp = pImage->GetTimeStamp();
        }
        else
            timestamp = edt_timestamp();

        SetBufferTime(m_nLastCount, timestamp);

        TRACE("Last count = %d timestamp = %f diff = %f\n", m_nLastCount, timestamp,
            timestamp - last_time);
        last_time = timestamp;

        //	pImage->SetSequenceNumber(m_nLastCount);

        //printf("Captured image %p\n", pImage);
        m_pLastImageCaptured = pImage;

        if (GetActiveFlag() && m_pPostOp)
        {
            m_pPostOp->Setup(pImage, pImage);

            if (m_pPostOp->Ok())
                m_pPostOp->Go();

        }

        return (GetActiveFlag())?pImage:NULL;

    }

    return NULL;
}

void EdtQtLiveDisplay::StartPump()
{
    TRACE("STarting channel %d Timeouts = %d\n",m_nChannelID, GetTimeouts());

    m_nLastCount = 0;
    if (m_pCamera)
    {
        m_pCamera->PrepareBuffers();
        m_pCamera->StartAcquire(m_nMaxCount);
    }

    m_bStartPending = FALSE;
}

void EdtQtLiveDisplay::StopPump()
{
    if (m_pCamera && m_pCamera->IsCapturingImages())
    {

        m_pCamera->InitiateStop();

    }

}


bool EdtQtLiveDisplay::BuffersFinished()

{
    if (m_pCamera)
    {

        return !(m_bStartPending || m_pCamera->BuffersPending());

    }
    return TRUE;
}


int EdtQtLiveDisplay::GetTimeouts()

{
    if (m_pCamera)
        return m_pCamera->GetTimeouts();
    else
        return 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void EdtQtLiveDisplay::SetNBuffers(const int nBuffers)

{

    if (m_pCamera)
        m_pCamera->SetNBufs(nBuffers);

}


void EdtQtLiveDisplay::InitVariables()

{
    m_dwImageCount = 0;
    m_dStartTime = 0;
    m_dEndTime = 0;

    m_nLocks = 0;
    m_bDoLocks = 1;

    m_nWidth = m_nHeight = 0;
    m_nType = TypeBYTE;

    m_pThread = (thread_t) 0;

    m_nFrameMod = 30;

    m_wndStatus = (HANDLE) 0;

    m_bActiveFlag = FALSE;
    m_bTimedOut = FALSE;

    m_nStatus = 0;

    m_nChannelID = 0;

    InitializeCriticalSection(&m_lock);

    m_nLiveWindows = 0;
    m_nCurrentLiveWindow = 0;

    m_bWaitForScreenUpdate = FALSE;
    m_pLiveLut = NULL;

    m_bDrawLive = TRUE;


    m_pLastImageCaptured = NULL;
    m_pCurrentWindow = NULL;


    m_nAcquireSkip = 1;
    m_nDisplaySkip = 1;


    m_nMaxCount = 0;
    m_nLastCount = 0;


    m_bStartPending = FALSE;

    m_nChannelID = 0;

    m_pPostOp = NULL;

    m_pSingle = NULL;

    m_pSingle = m_pCamera->GetCameraImage(m_pSingle);

    SetBufferValues(*m_pSingle);

}

void EdtQtLiveDisplay::SetCamera(PdvInput *pCamera)
{

}

void EdtQtLiveDisplay::UpdateDisplay()
{
    if (m_pCurrentWindow && m_pLastImageCaptured)
    {
        // pass count of 0 so it's always displayed

        TRACE("DoDisplay %p\n", m_pLastImageCaptured);

        DoDisplay(m_pLastImageCaptured, 0);
    }
}

EdtImage * EdtQtLiveDisplay::GetSingleImage()
{
    return m_pSingle;
}

EdtImage * EdtQtLiveDisplay::GetLastImage()
{
    return m_pLastImageCaptured;
}


void EdtQtLiveDisplay::SetSkip(unsigned int value)
{
    m_nDisplaySkip = value;
}

