#include <iostream>
#include <process.h>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

using namespace std;

int Gold = 0;

CRITICAL_SECTION GoldCS;

unsigned WINAPI Thread(void* Arg) 
{
	//std::cout << "Thread" << std::endl; // -> ���� 
	printf("Hello World\n");
	
	for (int i = 1; i <= 100; ++i)
	{
		cout << i << endl;
	}
	
	return 0;
}

unsigned WINAPI Increse(void* Arg)
{
	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&GoldCS);	// �ٸ� �����尡 ���� ���ϰ� ����
		Gold++;	// 1. Read, 2. Add, 3. Save
		LeaveCriticalSection(&GoldCS);	// ���� ǰ 
	}
	return 0;
}
unsigned WINAPI Decrese(void* Arg)
{
	EnterCriticalSection(&GoldCS);	// �ٸ� �����尡 ���� ���ϰ� ����
	for (int i = 0; i < 1000000; i++)
	{
		Gold--;	// 1. Read, 2. Add, 3. Save
	}
	LeaveCriticalSection(&GoldCS);

	return 0;
}



int main()	//		main ������ 
{
	//		Rocking �ɱ�
	InitializeCriticalSection(&GoldCS);
	
	unsigned int ThreadID = 0; 

	//		���� �ð�  
	DWORD StartTime = timeGetTime();

	//		�ڵ� ���� 
	HANDLE ThreadHandles[2];
	

	//		�ڵ� ���  
	ThreadHandles[0] = (HANDLE)_beginthreadex(NULL, 0, Increse, nullptr,
		0, &ThreadID);
	ThreadHandles[1] = (HANDLE)_beginthreadex(NULL, 0, Decrese, nullptr,
		0, &ThreadID);

	WaitForMultipleObjects(2, ThreadHandles, true, INFINITE);

	//		������ ���̱�
	//TerminateThread(ThreadHandle, -1);	// ���� ������ 

	//		��ٸ��� 
	//Sleep(1000);
	
	//		������ ���� 
	//	�ð� üũ 
	cout << (timeGetTime() - StartTime) << endl;
	
	//		Rocking Ǯ��
	DeleteCriticalSection(&GoldCS);

	return 0;
}


////		�ڵ� ���  
//HANDLE ThreadHandle1 = (HANDLE)_beginthreadex(NULL, 0, Increse, nullptr,
//	CREATE_SUSPENDED, &ThreadID);

////			����� �ٽ� �����ϱ� 
//	//		�ڵ� ���(����)
//ResumeThread(ThreadHandle);
//Sleep(10);
////		�ڵ� ���� 
//SuspendThread(ThreadHandle);
//Sleep(3000);
//ResumeThread(ThreadHandle);
//
//
////		�ڵ� ���� ��ٸ��� 
//WaitForSingleObject(ThreadHandle, INFINITE); //		��� ��ٸ��� �Լ� 