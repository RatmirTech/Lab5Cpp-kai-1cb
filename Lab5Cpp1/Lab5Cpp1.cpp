#include <iostream>
#include "task264.h"
#include "task417.h"
#include <Windows.h>
#include <direct.h> 
int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::cout << "Лабораторная работа 5!\n";
	//std::cout << "Текущая директория: " << _getcwd(NULL, 0) << std::endl;
	//init264();
	init417();
}
