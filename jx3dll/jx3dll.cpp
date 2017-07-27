// jx3dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
typedef enum _THREADINFOCLASS{
	ThreadBasicInformateion,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnaleAlignmentFaultFixup,
	ThreadEventPair_Reusable,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmlLastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadiSloPending,
	ThreadHideFormDebugger,
	ThreadBreakOnTermination,
	ThreadSwitchLegacyState,
	ThreadIsTerminated,
	MaxThreadInfoClass
}THREADINFOCLASS;

typedef DWORD(WINAPI *ZWSETINFORMATIONTHREAD)(HANDLE,THREADINFOCLASS,PVOID,DWORD);

ZWSETINFORMATIONTHREAD ZwsetInformationThread =NULL;
BYTE ZwSetInformationThread_Begin[5]={0};
BYTE ZwSetInformationThread_Hook[5]={0xE9,0,0,0,0};

DWORD WINAPI MyZwSetInformationThread(
									  HANDLE ThreadHandle,
									  THREADINFOCLASS ThreadInoformationClass,
									  PVOID ThreadInformation,
									  DWORD ThreadInformationLength
									  )
{
	::WriteProcessMemory(
						HANDLE(-1),
						ZwsetInformationThread,
						ZwSetInformationThread_Begin,
						5,
						0);
	DWORD Ret= TRUE;
	if(ThreadInoformationClass != ThreadHideFormDebugger)
	{
		Ret=ZwsetInformationThread(
			ThreadHandle,
			ThreadInoformationClass,
			ThreadInformation,
			ThreadInformationLength
			);

	}
	::WriteProcessMemory(
		HANDLE(-1),
		ZwsetInformationThread,
		ZwSetInformationThread_Hook,
		5,
		0);
	return Ret;

}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(
			0,
			"ע�����HOOK",
			"11",
			MB_YESNO);
		//��ȡapi�ĵ�ַ
		ZwsetInformationThread = (ZWSETINFORMATIONTHREAD)::GetProcAddress(
			::GetModuleHandle("NTDLL.DLL"),
			"ZwSetInformationThread"
			);
		//��ǰ�Լ�������ַ��ȥapi��ַ�ڼ�ȥ5 ����HOOK�Ĳ�ֵ
		*(DWORD*)(ZwSetInformationThread_Hook+1)=
			(DWORD)MyZwSetInformationThread-(DWORD)ZwsetInformationThread-5;

		::ReadProcessMemory(
			HANDLE(-1),
			ZwsetInformationThread,
			ZwSetInformationThread_Begin,
			5,
			0
			);
		::WriteProcessMemory(
			HANDLE(-1),
			ZwsetInformationThread,
			ZwSetInformationThread_Hook,
			5,
			0);
		MessageBoxA(
			0,
			"ע�����HOOK",
			"11",
			MB_YESNO);
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}

