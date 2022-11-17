#include <iostream>
#include <process.h>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

using namespace std;

int Gold = 0;

CRITICAL_SECTION GoldCS;

unsigned WINAPI Thread(void* Arg) 
{
	//std::cout << "Thread" << std::endl; // -> 오류 
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
		EnterCriticalSection(&GoldCS);	// 다른 스레드가 접근 못하게 막음
		Gold++;	// 1. Read, 2. Add, 3. Save
		LeaveCriticalSection(&GoldCS);	// 상태 품 
	}
	return 0;
}
unsigned WINAPI Decrese(void* Arg)
{
	EnterCriticalSection(&GoldCS);	// 다른 스레드가 접근 못하게 막음
	for (int i = 0; i < 1000000; i++)
	{
		Gold--;	// 1. Read, 2. Add, 3. Save
	}
	LeaveCriticalSection(&GoldCS);

	return 0;
}



int main()	//		main 쓰레드 
{
	//		Rocking 걸기
	InitializeCriticalSection(&GoldCS);
	
	unsigned int ThreadID = 0; 

	//		현재 시간  
	DWORD StartTime = timeGetTime();

	//		핸들 묶음 
	HANDLE ThreadHandles[2];
	

	//		핸들 등록  
	ThreadHandles[0] = (HANDLE)_beginthreadex(NULL, 0, Increse, nullptr,
		0, &ThreadID);
	ThreadHandles[1] = (HANDLE)_beginthreadex(NULL, 0, Decrese, nullptr,
		0, &ThreadID);

	WaitForMultipleObjects(2, ThreadHandles, true, INFINITE);

	//		쓰레드 죽이기
	//TerminateThread(ThreadHandle, -1);	// 강제 삭제용 

	//		기다리기 
	//Sleep(1000);
	
	//		쓰레드 실행 
	//	시간 체크 
	cout << (timeGetTime() - StartTime) << endl;
	
	//		Rocking 풀기
	DeleteCriticalSection(&GoldCS);

	return 0;
}


////		핸들 등록  
//HANDLE ThreadHandle1 = (HANDLE)_beginthreadex(NULL, 0, Increse, nullptr,
//	CREATE_SUSPENDED, &ThreadID);

////			멈췄다 다시 실행하기 
//	//		핸들 재계(실행)
//ResumeThread(ThreadHandle);
//Sleep(10);
////		핸들 멈춤 
//SuspendThread(ThreadHandle);
//Sleep(3000);
//ResumeThread(ThreadHandle);
//
//
////		핸들 실행 기다리기 
//WaitForSingleObject(ThreadHandle, INFINITE); //		계속 기다리는 함수 