// S5FC_Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <memory>
#include<Windows.h>
#include <assert.h>

typedef int (*S5FC_List)(char *serialNo);
typedef int (*S5FC_Open)(char* serialNo, int nBaud, int timeout);
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
int pcount;

extern "C" int Init();

int Init()
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
	return 0;
}