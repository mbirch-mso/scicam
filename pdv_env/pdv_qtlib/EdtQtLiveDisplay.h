
#ifndef _QT_LIVE_DISPLAY_H_
#define _QT_LIVE_DISPLAY_H_

#include "edtimage/EdtImage.h"

extern "C" {
#include "edtinc.h"
} 

#include "pdvplus/ImageEventFunc.h"

#include "edtimage/BufferTimerData.h"

#include "EdtQtImageItem.h"

#include "pdvplus/PdvInput.h"

#include "edtimage/EdtImgOp.h"

#include <list>

using namespace std;


#define MAX_LIVE_WINDOWS 32

class EdtQtLiveDisplay : public EdtImageData

{
protected:

    int m_nStatus; // What's happening

    bool m_bDoLocks;
    bool m_bActiveFlag;
    bool m_bTimedOut;


    mutable CRITICAL_SECTION m_lock;

    thread_t m_pThread;

    HANDLE m_wndStatus;

    long m_dwImageCount;
    double m_dStartTime;
    double m_dEndTime;

    BufferTimerData m_Time;

    int m_nMaxCount;

    mutable int m_nLocks;

    int m_nLastTimeStamp;
    int m_nFrameMod;


    int m_nChannelID;


    PdvInput * m_pCamera;

    int m_nLastCount;

    bool m_bStartPending;

    int m_nAcquireSkip;

    int m_nDisplaySkip;


    int  m_nLiveWindows;
    int  m_nCurrentLiveWindow;

    vector<EdtQtImageItem *> m_wndLiveWindows;
    byte *m_pLiveLut;


    EdtImage *m_pLastImageCaptured;
    EdtImage *m_pSingle;
    EdtQtImageItem * m_pCurrentWindow;

    bool m_bWaitForScreenUpdate;


    EdtImgOp * m_pPostOp;


public:

    bool m_bWndDirect;
    bool m_bDrawLive;

    unsigned int DoLiveThreadFunc();

    virtual bool AllFinished();

    bool Start(bool bUseThread = TRUE);

    virtual int GetTimeouts();

    int GetFrameCount();

    EdtQtLiveDisplay(const char *devname = NULL, 
        const int nUnit = 0, const int nChannel = 0);
    EdtQtLiveDisplay(PdvInput  *pCamera);
    EdtQtLiveDisplay(PdvDev *pdv_p);

    virtual ~EdtQtLiveDisplay();
    void capture_single_image(bool resize = false);


    void Lock() 
    {
        if (m_bDoLocks) {

            EnterCriticalSection(&m_lock);

            m_nLocks++;
        }
    }

    void Unlock() 
    {
        if (m_nLocks > 0) {
            LeaveCriticalSection(&m_lock);

            m_nLocks--;
        }
    }

    void SetActiveFlag(bool nActiveFlag) 
    {
        Lock();
        m_bActiveFlag = nActiveFlag;
        Unlock();
    }

    bool GetActiveFlag() 
    {
        Lock();
        bool rc = m_bActiveFlag;
        Unlock();
        return rc;
    }

    void SetPostOp(EdtImgOp *pOp)
    {
        m_pPostOp = pOp;
    }

    EdtImgOp *GetPostOp() { return m_pPostOp; }

    void SetTimedOut(bool nTimedOut) 
    {
        Lock();
        m_bTimedOut = nTimedOut;
        Unlock();
    }

    bool GetTimedOut() 
    {
        Lock();
        bool rc = m_bTimedOut;
        Unlock();
        return rc;
    }


    void NotifyEventFuncs(EdtImage *pImage, EdtQtImageItem *pWindow, long nFrameNumber);
    void AddEventFunc(ImageEventFunc f, void *target);
    void RemoveEventFunc(ImageEventFunc f, void *target);
    void RemoveAllEventFuncs();

    void NotifyFinishFuncs();
    void AddFinishFunc(ImageEventFunc f, void *target);
    void RemoveFinishFunc(ImageEventFunc f, void *target);

    //CList<EventFuncRec, EventFuncRec &> m_AcqList;

    list <EventFuncRec *> m_AcqList;
    list <EventFuncRec *> m_FinishList;

    EdtImage * DoLiveWindow(bool display = TRUE);

    bool IsLive() {
        Lock();
        bool rc = (m_pThread != (thread_t) 0);
        Unlock();
        return rc;
    }

    bool StopLive();

    // Display Stuff

    EdtQtImageItem * DoDisplay(EdtImage *pImage, long nCount);

    void UpdateDisplay(); // paint the last image 

    bool SetDisplayWindows(EdtQtImageItem **LiveWindows, int nWindows = 1);

    void AddDisplayWindow(EdtQtImageItem *wnd);

    static int DisplayEventFunc(void *target, void *pNewImage, EdtQtImageItem *pWindow, long nCount);

    EdtQtImageItem *  GetNextLiveWindow();

    virtual void DisplayLiveImage(EdtImage *pImage, 
        EdtQtImageItem * pWindow, int nCount);

    virtual void PrepareBuffers();

    // from VCRChannel


    int Stop()

    {
        StopLive();

        return 0;
    }

    void SetDisplaySkip(int nSkip)

    {
        m_nDisplaySkip = nSkip;
    }

    int GetDisplaySkip()
    {
        return m_nDisplaySkip;
    }

    void SetChannelID(int nSkip)

    {
        m_nChannelID = nSkip;
    }

    int GetChannelID()
    {
        return m_nChannelID;
    }

    virtual void SetupLive(EdtQtImageItem *pWindow)

    {
        SetDisplayWindows(&pWindow, 1);
    }	

    virtual void StartLive(EdtQtImageItem *pWindow, bool bUseThread)

    {
        SetupLive(pWindow);

        Start(bUseThread);
    }

    virtual	void StartLive(EdtQtImageItem **pWindows, int nwindows, bool bUseThread)

    {
        SetDisplayWindows(pWindows, nwindows);

        Start(bUseThread);
    }

    void SetNBuffers(int n = 1);

    void SetMaxCount(const int nMaxCount)
    {
        m_nMaxCount = nMaxCount;
    }

    int GetMaxCount() const 
    {
        return m_nMaxCount;
    }

    int GetNFrames()
    {
        return GetMaxCount();
    }

    void SetBufferTime(int nBuffer, const double dtime)
    {
        m_Time.SetBufferTime(nBuffer, dtime);
    }

    double GetBufferTime(int nBuffer)
    {
        return m_Time.GetBufferTime(nBuffer);
    }


    int GetLastBuffer()
    {
        return m_Time.m_nLastBuffer;
    }

    double GetFrameRate()
    {
        return m_Time.GetFrameRate();
    }

    void SetNFrames(int nMaxCount)
    {
        SetMaxCount(nMaxCount);
    }


    double * GetTimeVector(int &nTimes)
    {
        nTimes = m_Time.m_nLastBuffer;
        return m_Time.m_pTimeVec;
    }


public:
    bool m_bOwnCamera;
    void SetCamera(PdvInput *pCamera);


    bool ChannelReady(double dTime);

    void InitVariables();

    void InitValues(const char *PdvDev, const int nPdvUnit);

    PdvInput * GetCamera();

    void StartTimer()
    {
        m_Time.StartTimer();
    }



    virtual bool BuffersFinished();
    virtual EdtImage *GetNextBufferImage(int nCount);

    // return the last image captured
    virtual EdtImage *GetLastImage();
    EdtImage * GetSingleImage();

    // skip values
    unsigned int GetSkip() { return m_nDisplaySkip; }
    void SetSkip(unsigned int value);

    EdtQtImageItem *GetCurrentWindow() 
    {
        return m_pCurrentWindow;
    }

    virtual void StopPump();

    void StartPump();

    // stop acquiring immediately

    virtual void Abort();



};

#endif
