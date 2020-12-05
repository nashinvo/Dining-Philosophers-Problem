#include <windows.h>
#include <iostream>

using namespace std;

#define PhilosophersCount 5 // Количество философов

HANDLE philosophers[PhilosophersCount]; // Потоки, или в нашем случае обедающие философы
CRITICAL_SECTION forks[PhilosophersCount]; // Критические секции или же вилки

int LeftFork(int philosoph) // Определения номера левой вилки
{
	if (philosoph == 0) // Если философ под номером 1, то в его левой руке вилка №2
		return 1;
	else // Иначе для i-го философа в левой руке будет вилка i+1
		return (philosoph + 1) % PhilosophersCount;
}

int RightFork(int philosoph) // Определение номера правой вилки
{
	return philosoph; // Для i-го философа в правой руке будет вилка i
}

void takeForks(int philosoph) // Философ входит в критические секции и пытается взять вилки
{
	EnterCriticalSection(&forks[LeftFork(philosoph)]);
	EnterCriticalSection(&forks[RightFork(philosoph)]);
}

void putForks(int philosoph) // Философ освобождает занятые ранее вилки
{
	LeaveCriticalSection(&forks[RightFork(philosoph)]); 
	LeaveCriticalSection(&forks[LeftFork(philosoph)]);
}

DWORD WINAPI EatAndThink(CONST LPVOID philosopher) // Процесс приёма пищи и размышления
{
	Sleep(rand() % 10001);
	while (1)
	{
		printf("Философ %d собирается есть.\n", int(philosopher) + 1);
		takeForks(int(philosopher)); // Философ берёт вилки или ждёт, пока они не освободятся
		printf("Философ %d ест.\n", int(philosopher) + 1);
		Sleep(rand() % 5000);
		putForks(int(philosopher)); // Философ кладёт вилки на стол
		printf("Философ %d начинает размышлять.\n", int(philosopher) + 1);
		Sleep(rand() % 5000);
	}
}

int main()
{
	setlocale(LC_ALL, "Ru");
	for (int i = 0; i < PhilosophersCount; i++)
		InitializeCriticalSection(&forks[i]); // Инициализируем критические секции для вилок

	for (int i = 0; i < PhilosophersCount; i++)
		philosophers[i] = CreateThread(NULL, 0, &EatAndThink, (LPVOID)i, 0, NULL); // Инициализируем философов

	getchar();
	return 0;
}
