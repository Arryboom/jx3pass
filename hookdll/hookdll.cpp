// hookdll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdlib.h>
//BYTE Write[5]={0};//���� 5���ֽ�
DWORD address,*temp;//����ĸ��ֽ��ǵ�ַ
BYTE Write[5]={0xE9,0,0,0,0};//��һ���ֽڷ�jmp������

void Ro_hookBlood()
{
 //MessageBoxA(0,"HOOK test","11",MB_YESNO);
 //AfxMessageBox(_T("HOOK test"));
 __asm
 {
 pop edi
 pop esi
 pop ebx
 add esp,0x40
 mov esp,ebp
 pop ebp
 mov ecx,0x401065
 mov dword ptr ss:[ebp-0x4],0x6
 jmp ecx
 }
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		MessageBoxA(0,"���̿�ʼ","11",MB_YESNO);
		__asm
		{
			mov ecx,offset Ro_hookBlood	//�Ѻ����ĵ�ַ����exc
				mov address,ecx             //�Ѻ�����ַ���ݸ�address
				sub address,0x401063		//����ƫ�ƣ���Ϊ��jmp���ƫ�ƣ�����address��������ַ��-��ʼ���ĵ�ַ-��תָ���С һ��Ϊ5
				//add esp,4				//����ѹվ��ֹ��ջ��ƽ��
				
				
		}
		


		temp=(DWORD*)&Write[1];//��write[1]λ�ÿ�ʼд��
	
		*temp=address; //��address д��Write[1]
		::WriteProcessMemory(
			::GetCurrentProcess(),
			(LPVOID)0x40105E,
			Write,
			sizeof(Write),//���ض����������ռ���ڴ��ֽ���
					 NULL);
		break;
	case DLL_PROCESS_DETACH:
		MessageBoxA(0,"�رս���","22",MB_YESNO);
		break;
	}

    return TRUE;
}

