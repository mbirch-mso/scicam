
/*
Includes C header with framegrabber code
Includes C++ header with SLD SDK 

Begins image capture
On first FVAL the SLD is switched on
On second FVAL the SLD is switched off
All remaining FVAL are saved as fits file.

 */

#include <string.h>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include "edtinc.h"

// #include "persist.h"


// using namespace std;


typedef int (*S5FC_List)(char *serialNo);
typedef int (*S5FC_Open)(char* serialNo, int nBaud, int timeout);
typedef int (*S5FC_IsOpen)(char* serialNo);
typedef int (*S5FC_Close)(int hdl);
typedef int (*S5FC_GetEnable)(int hdl, bool& isEnable);
typedef int (*S5FC_GetCurrent)(int hdl, double& current);
typedef int (*S5FC_GetPower)(int hdl, double& power);
typedef int (*S5FC_GetActualTemperature)(int hdl, double& temp);
typedef int (*S5FC_GetTargetTemperature)(int hdl, double& temp);
typedef int (*S5FC_SetTargetTemperature)(int hdl, double temp);
typedef int (*S5FC_SetCurrent)(int hdl, double current);
typedef int (*S5FC_SetEnable)(int hdl, bool isEnable);
S5FC_List List; 
S5FC_Open Open;
S5FC_IsOpen IsOpen;
S5FC_Close Close;
S5FC_GetEnable GetEnable;
S5FC_GetCurrent GetCurrent;
S5FC_GetPower GetPower;
S5FC_GetActualTemperature GetActualTemperature;
S5FC_GetTargetTemperature GetTargetTemperature;
S5FC_SetTargetTemperature SetTargetTemperature;
S5FC_SetCurrent SetCurrent;
S5FC_SetEnable SetEnable;

HINSTANCE hdll;

int sld_init(char *sn)
{	

	hdll = LoadLibrary(TEXT("S5FC_COMMAND_LIB_win32.dll"));
	if(hdll== NULL)
	{
		printf_s("Load S5FC_COMMAND_LIB_win32.dll failed,please check\n");	
		return -1;
	}
	List = (S5FC_List)GetProcAddress(hdll, "List");
	Open = (S5FC_Open)GetProcAddress(hdll, "Open");
	Close = (S5FC_Close)GetProcAddress(hdll, "Close");
	GetEnable = (S5FC_GetEnable)GetProcAddress(hdll, "GetEnable");
	IsOpen = (S5FC_IsOpen)GetProcAddress(hdll, "IsOpen");
	GetCurrent = (S5FC_GetCurrent)GetProcAddress(hdll, "GetCurrent");
	GetPower = (S5FC_GetPower)GetProcAddress(hdll, "GetPower");
	GetActualTemperature = (S5FC_GetActualTemperature)GetProcAddress(hdll, "GetActualTemperature");
	GetTargetTemperature = (S5FC_GetTargetTemperature)GetProcAddress(hdll, "GetTargetTemperature");
	SetTargetTemperature = (S5FC_SetTargetTemperature)GetProcAddress(hdll, "SetTargetTemperature");
	SetCurrent = (S5FC_SetCurrent)GetProcAddress(hdll, "SetCurrent");
	SetEnable = (S5FC_SetEnable)GetProcAddress(hdll, "SetEnable");

	if(List == NULL || Open == NULL || Close == NULL ||GetEnable == NULL || GetCurrent == NULL||GetPower== NULL||GetActualTemperature == NULL||GetTargetTemperature == NULL||SetTargetTemperature == NULL||SetCurrent == NULL||SetEnable == NULL)
	{
		printf_s("Load S5FC_COMMAND_LIB_win32.dll failed,please check\n");	
		return -1;
	}

	int hdl=Open(sn,115200,10);
	if(hdl < 0 )
	{ 
		return -1;
	}
	return hdl;
}

void ReadTemp(int hdl)
{
	int ret = -1;
	double d=0;
	ret = GetActualTemperature(hdl,d);
	printf("SLD Temperature: %.2f\n",d);
}

void ReadPower(int hdl)
{
	int ret = -1;
	double d=0;
	ret = GetCurrent(hdl,d);
	printf("Current: %.2f\n",d);
	ret = GetPower(hdl,d);
	printf("Power: %.2f\n",d);
}

int IsEnabled(int hdl)
{
	bool b = false;
	GetEnable(hdl,b);
	printf("Device Enabled: %d\n",b);
	return b;
}

void switch_sld(int hdl,char *sn)
{
	int ret = -1;
	if (IsEnabled(hdl)==0)
	{
		ret = SetEnable(hdl,true);
		assert(ret == 0);
	}
	else
	{
		ret = SetEnable(hdl,false);
		assert(ret == 0);
	}
}


void fval(PdvDev *pdv_p, u_char *image_p){
    pdv_flush_fifo(pdv_p);
    pdv_multibuf(pdv_p, 1);
    edt_start_buffers(pdv_p, 1); 
    fflush(stdout);
    image_p = pdv_wait_image(pdv_p);
}

int main()
{
	int hdl;
	char sn[256];
	strcpy(sn, "COM3");
    u_char *image_p; /*Image data pointer*/
    PdvDev *pdv_p;  /*Device pointer */
	int ret = -1;
	double off_current = 0;

	// Initialise SCICAM & SLD
	pdv_p = pdv_open_channel(EDT_INTERFACE, 0, 0);
	hdl = sld_init(sn);
	if(hdl < 0)
	{
		printf("SLD Error\n");
		return 0;
	}
	
	
	for (int i = 0; i < 10; i++) {
	fval(pdv_p, image_p); 
	printf("FVAL Seen\n");
	}

	// Set current to 0 (SOAK OFF)
	ret = SetCurrent(hdl,off_current);  

}

