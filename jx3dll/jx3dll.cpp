// jx3dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

//����ö���������ݽṹ

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
	ThreadHideFormDebugger,//��������������̶߳Ե���������
	ThreadBreakOnTermination,
	ThreadSwitchLegacyState,
	ThreadIsTerminated,
	MaxThreadInfoClass
}THREADINFOCLASS;

//����
typedef DWORD(WINAPI *ZWSETINFORMATIONTHREAD)(HANDLE,THREADINFOCLASS,PVOID,DWORD);

ZWSETINFORMATIONTHREAD ZwsetInformationThread =NULL;
BYTE ZwSetInformationThread_Begin[5]={0};
BYTE ZwSetInformationThread_Hook[5]={0xE9,0,0,0,0};
//hookʱ���Լ���Ҫ��ʲô����
DWORD WINAPI MyZwSetInformationThread(
									  HANDLE ThreadHandle,
									  THREADINFOCLASS ThreadInoformationClass,
									  PVOID ThreadInformation,
									  DWORD ThreadInformationLength
									  )
{
	//�Ȼָ�����ͷ
	::WriteProcessMemory(
						HANDLE(-1),
						ZwsetInformationThread,
						ZwSetInformationThread_Begin,
						5,
						0);
	DWORD Ret= TRUE;
	//�ж��Ƿ�ΪThreadHideFormDebugger Ȼ�� ����ԭʼ�Ľ��
	if(ThreadInoformationClass != ThreadHideFormDebugger)
	{
		Ret=ZwsetInformationThread(
			ThreadHandle,
			ThreadInoformationClass,
			ThreadInformation,
			ThreadInformationLength
			);

	}
	//�����������hookס ������һ��
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
		//��ȡapi��ַ��Ĭ�ϳ�ʼ�ڴ��ֵַ
		::ReadProcessMemory(
			HANDLE(-1),
			ZwsetInformationThread,
			ZwSetInformationThread_Begin,
			5,
			0
			);
		//д��ǰ�����Ĳ�ֵ����ת�� �Լ��ĺ���MyZwSetInformationThread

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

