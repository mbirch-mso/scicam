#ifndef LimitSingleInstance_H
#define LimitSingleInstance_H

#include <windows.h> 

//this code is from Q243953 in case you lose the article and wonder
//where this code came from...
class CLimitSingleInstance
{
protected:
  DWORD  m_dwLastError;
  HANDLE m_hMutex;

public:
  CLimitSingleInstance(char *uniqueappstr, char *devname, int unit, int channel)

  {
	
    //be sure to use a name that is unique for this application otherwise
    //two apps may think they are the same if they are using same name for
    //3rd parm to CreateMutex

	char strMutexName[128];

	sprintf(strMutexName, "{%s%s%d%d}", uniqueappstr, devname, unit,channel);

    m_hMutex = CreateMutex(NULL, FALSE, TEXT(strMutexName)); //do early
    m_dwLastError = GetLastError(); //save for use later...
  }
#if 0 // in the example that this was taken from, the object is instanced statically
  // before any subroutines are called so the destructor doesn't get called until
  // program exit. But WE're calling it inside a subroutine so we don't want do
  // destruct it since it will then release the mutex upon exit from the calling
  // subroutine -- not what we want. Bottom line is, skip this.
  ~CLimitSingleInstance() 
  {
 	if (m_hMutex)  //don't forget to close handles...
    {
  	   CloseHandle(m_hMutex); //do as late as possible
       m_hMutex = NULL; //good habit to be in
    }
  }
#endif

  bool IsAnotherInstanceRunning() 
  {
    return (ERROR_ALREADY_EXISTS == m_dwLastError);
  }
};
#endif 
