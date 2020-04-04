
//
// Do a ring buffer of live windows if more than one is defined
//

#include "stdafx.h"

#include "edtimage.h"

#include "LiveDisplay.h"




extern "C" {

#include "edtinc.h"

}

#ifdef WIN32

#include <process.h>

#endif

#define no_printf

LiveDisplay::LiveDisplay(const char *devname, const int nUnit, const int nChannel)
{
    m_pCamera = new PdvInput(devname, nUnit,nChannel);

    InitVariables();

    m_bOwnCamera = TRUE;

}

LiveDisplay::LiveDisplay(PdvInput *pCamera)
{

    m_pCamera = pCamera;

    m_bOwnCamera = FALSE;

    InitVariables();

}

LiveDisplay::LiveDisplay(PdvDev *pdv_p)
{

    m_pCamera = new PdvInput(pdv_p);

    m_bOwnCamera = TRUE;

    InitVariables();

}
LiveDisplay::~LiveDisplay()
{
    DeleteCriticalSection(&m_lock);

    if (m_bOwnCamera)
        delete m_pCamera;
}

#ifdef _WIN32
UINT __stdcall
#else
void *
#endif
LiveThreadFunc(void *param)

{

    LiveDisplay *pObj = (LiveDisplay *) param;

    return (THREAD_RETURN) pObj->DoLiveThreadFunc();

}


bool LiveDisplay::StopLive()

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

EdtImage * LiveDisplay::DoLiveWindow(bool display)
{
    EdtImage *pImage = NULL;

    pImage = GetNextBufferImage(m_dwImageCount);

    // 

    if (pImage)
    {

#if 0
        if (display)
            printf("x"); 
        else
            printf("X");

        fflush(stdout);
#endif

        // m_Time.SetBufferTime(m_dwImageCount, pImage->GetTimeStamp());

        ImageWindowData *pWindow = NULL;

        if (display)
            pWindow = DoDisplay(pImage, m_dwImageCount);

        NotifyEventFuncs(pImage,pWindow,m_dwImageCount);

        m_dwImageCount ++;

    }

    return pImage;

}


void LiveDisplay::AddEventFunc(ImageEventFunc f, void *target)
{
    // Make sure not already in list

    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
            return;
        pTest = m_AcqList.GetNext();
    }

    m_AcqList.Append(new EventFuncRec(f,target));
}

void LiveDisplay::RemoveEventFunc(ImageEventFunc f, void *target)
{
    // Make sure not already in list
    // Make sure not already in list

    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
        {
            m_AcqList.DeleteCurrent();
            return;
        }
        pTest = m_AcqList.GetNext();
    }

    ASSERT(FALSE);

}

void LiveDisplay::NotifyEventFuncs(EdtImage * pImage, ImageWindowData *pWindow, long nFrameNumber)
{
    EventFuncRec *pTest;

    pTest = m_AcqList.GetFirst();

    while (!m_AcqList.EndOfList())
    {
        if (pTest->pFunc)
            pTest->pFunc(pTest->target, pImage, pWindow, nFrameNumber);

        pTest = m_AcqList.GetNext();
    }

}

void LiveDisplay::AddFinishFunc(ImageEventFunc f, void *target)
{
    // Make sure not already in list

    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
            return;
        pTest = m_FinishList.GetNext();
    }

    m_FinishList.Append(new EventFuncRec(f,target));
}

void LiveDisplay::RemoveFinishFunc(ImageEventFunc f, void *target)
{
    // Make sure not already in list
    // Make sure not already in list

    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->target == target && pTest->pFunc == f)
        {
            m_FinishList.DeleteCurrent();
            return;
        }
        pTest = m_FinishList.GetNext();
    }

    ASSERT(FALSE);

}

void LiveDisplay::NotifyFinishFuncs()
{
    EventFuncRec *pTest;

    pTest = m_FinishList.GetFirst();

    while (!m_FinishList.EndOfList())
    {
        if (pTest->pFunc)
            pTest->pFunc(pTest->target, NULL, NULL, m_nChannelID);

        pTest = m_FinishList.GetNext();
    }

}

int LiveDisplay::GetFrameCount()
{
    return m_dwImageCount;
}

void LiveDisplay::Abort()

{
    if (m_pThread)
    {
        thread_t start_thread = m_pThread;

        m_pThread = NULL;

        SetActiveFlag(FALSE);

        m_pCamera->Abort();

    }


}


bool LiveDisplay::Start(bool bUseThread)
{

    m_Time.Start();

    no_printf("StartLive\n");

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

bool LiveDisplay::AllFinished()
{
    return BuffersFinished();
}

//template class EdtListHead<EventFuncRec>;


UINT LiveDisplay::DoLiveThreadFunc()
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
    sprintf(s,"LiveDisplay::DoLiveThreadFunc()Finished... %x\n", m_pThread);

    OutputDebugString(s);
#endif

    if (m_pCamera)
        m_pCamera->StopAcquiring();

    NotifyFinishFuncs();

    m_pThread = (thread_t) 0;


    return 0;
}

ImageWindowData *  LiveDisplay::GetNextLiveWindow()

{

    if (m_nLiveWindows == 1)

        m_pCurrentWindow = m_wndLiveWindows[0];

    else if (m_nLiveWindows == 0)

        m_pCurrentWindow = NULL;

    else
    {
        m_pCurrentWindow = m_wndLiveWindows[m_nCurrentLiveWindow];
        m_nCurrentLiveWindow = (m_nCurrentLiveWindow + 1) % m_nLiveWindows;
    }

    return m_pCurrentWindow;
}


bool LiveDisplay::SetDisplayWindows(ImageWindowData * *LiveWindows, int nWindows)
{

    if (!LiveWindows)
        return FALSE;

    if (nWindows > MAX_LIVE_WINDOWS)
        nWindows = MAX_LIVE_WINDOWS;

    for (int i = 0; i< nWindows;i++)
    {
        m_wndLiveWindows[i] = LiveWindows[i];
    }

    m_nLiveWindows = nWindows;

    m_pCurrentWindow = LiveWindows[0];

    return TRUE;
}

void LiveDisplay::DisplayLiveImage(EdtImage *pImage, 
                                   ImageWindowData * pWindow,
                                   int nCount,
                                   CRect *pRect)
{

    EdtWindow wnd =	pWindow->GetWindow();

    if (pWindow->redraw_cb)
    {

        pWindow->AddDelayValue(edt_timestamp() - pImage->GetTimeStamp());

        pWindow->SetImageHandle(pImage,NULL);

        pWindow->redraw_cb(pWindow->redraw_obj, pImage);

    }
    else if (m_bWndDirect)
    {


        //printf("Calling edt_get_dc(%x)\n", wnd);

        EdtDC pdc = edt_get_dc(wnd);

        //printf("edt_get_dc returned %x\n", pdc);

        //printf("Window is %x\n", wnd);

        //EdtDC pdc = NULL;

        pWindow->AddDelayValue(edt_timestamp() - pImage->GetTimeStamp());

        pWindow->SetImageHandle(pImage,NULL);

        pWindow->Draw(pImage,pdc,pRect,true);

        edt_release_dc(wnd,pdc);
    }

#ifdef _WIN32

    else
    {
        pWindow->SetImageHandle(pImage,NULL);

        if (m_bDrawLive)
        {
            if (m_bWaitForScreenUpdate)
            {
                TRACE("Redrawing Image\n");
                ::RedrawWindow(wnd,NULL,NULL,
                    RDW_NOERASE|RDW_UPDATENOW|RDW_INVALIDATE);
            }
            else
            {
                TRACE("Display:   %d %10f delay = %f\n",
                    pImage->GetSequenceNumber(), edt_elapsed(FALSE), (edt_timestamp() - pImage->GetTimeStamp()) * 1000.0);
                if (pWindow->m_nPumpFinished == pImage->GetSequenceNumber())
                {
                    TRACE("Started:   %d skipping\n", pWindow->m_nPumpStarted);
                }
                else
                {
                    pWindow->m_nPumpStarted = pImage->GetSequenceNumber();
                    ::InvalidateRect(wnd,pRect,FALSE);
                }
            }
        }
    }
#endif

}


ImageWindowData * LiveDisplay ::DoDisplay(EdtImage *pImage, long nCount)
{
    if (!m_nDisplaySkip)
        return NULL;

    if (m_nDisplaySkip > 1)
    {
        if ((nCount % m_nDisplaySkip) != 0)
            return NULL;
    }


    ImageWindowData * pWindow = GetNextLiveWindow();

    no_printf("pWindow = %x pImage %x\n",		   pWindow, pImage);

    if (pWindow && pImage)
    {
        DisplayLiveImage(pImage, pWindow, nCount);
    }
    else
    {

    }	

    return pWindow;
}


// PdvImageSource

void LiveDisplay::PrepareBuffers()
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

PdvInput * LiveDisplay::GetCamera()
{
    return m_pCamera;
}


EdtImage * LiveDisplay::GetNextBufferImage(int nCount)

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

        double timestamp = pImage->GetTimeStamp();

        SetBufferTime(m_nLastCount, timestamp);

        no_printf("Last count = %d timestamp = %f diff = %f\n", m_nLastCount, timestamp,
            timestamp - last_time);
        last_time = timestamp;

        //	pImage->SetSequenceNumber(m_nLastCount);

#if 0
        char debugs[128];

        sprintf(debugs,"Acquiring: %d %10f delay = %8.5f ms\n", pImage->GetSequenceNumber(),edt_elapsed(FALSE),
            1000.0 * (edt_timestamp()-timestamp));
        OutputDebugString(debugs);

#endif


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

void LiveDisplay::StartPump()
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

void LiveDisplay::StopPump()
{
    if (m_pCamera && m_pCamera->IsCapturingImages())
    {

        m_pCamera->InitiateStop();

    }

}



bool LiveDisplay::BuffersFinished()

{
    if (m_pCamera)
    {

        return !(m_bStartPending || m_pCamera->BuffersPending());

    }
    return TRUE;
}


int LiveDisplay::GetTimeouts()

{
    if (m_pCamera)
        return m_pCamera->GetTimeouts();
    else
        return 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void LiveDisplay::SetNBuffers(const int nBuffers)

{

    if (m_pCamera)
        m_pCamera->SetNBufs(nBuffers);

}


void LiveDisplay::InitVariables()

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

#ifdef WIN32
    m_bWndDirect = TRUE;
#else
    m_bWndDirect = TRUE;

#endif

    m_pLastImageCaptured = NULL;
    m_pCurrentWindow = NULL;


    m_nAcquireSkip = 1;
    m_nDisplaySkip = 1;

    for (int i = 0; i< MAX_LIVE_WINDOWS; i++)
        m_wndLiveWindows[i]  = NULL;

    m_nMaxCount = 0;
    m_nLastCount = 0;


    m_bStartPending = FALSE;

    m_nChannelID = 0;

    m_pPostOp = NULL;

}



void LiveDisplay::SetCamera(PdvInput *pCamera)
{

}

void LiveDisplay::UpdateDisplay()
{
    if (m_pCurrentWindow && m_pLastImageCaptured)
    {
        // pass count of 0 so it's always displayed

        no_printf("DoDisplay %p\n", m_pLastImageCaptured);

        DoDisplay(m_pLastImageCaptured, 0);
    }
}

EdtImage * LiveDisplay::GetLastImage()
{
    return m_pLastImageCaptured;
}
