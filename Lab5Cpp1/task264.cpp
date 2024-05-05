#include "task264.h"
#include "constants.h"
#include <direct.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

extern string readConsole() {
	string input{};
	do {
		cout << "Введите строку с символами (для удаления текста в скобках): ";
		getline(cin, input);

		regex pattern{ R"(\((?:[^()]*)\))" };
		if (input.empty() || !regex_search(input, pattern)) {
			cerr << "Ошибка: Введите строку с хотя бы одним символом и с парными скобками, содержащую символы вне скобок.\n";
		}
		else {
			return input;
		}
	} while (true);
}

string removeParentheses(const string& input) {
	string result;
	int openParentheses = 0;

	for (char ch : input) {
		switch (ch) {
		case '(':
			openParentheses++;
			break;
		case ')':
			if (openParentheses > 0) {
				openParentheses--;
			}
			else {
				result += ch;
			}
			break;
		default:
			if (openParentheses == 0) {
				result += ch;
			}
			break;
		}
	}

	return result;
}

extern string readFile(const string& fileName) {
	ifstream file(fileName);
	string content, line;

	if (file.is_open()) {
		while (getline(file, line)) {
			content += line + "\n";
		}
		file.close();
	}
	else {
		cout << "Не удалось открыть файл." << endl;
	}
	regex pattern(R"(\((?:[^()]*)\))");
	if (regex_search(content, pattern))	return content;
	else	return "";
}

extern string getUserPath(const string& prompt, const string& defaultPath) {
	string path{};
	cout << prompt << " [" << defaultPath << "]: ";
	getline(cin, path);
	if (path.empty()) {
		return defaultPath;
	}
	return path;
}

extern void writeConsole(const string& output) {
	cout << "Результат: " << output << endl;
}

extern void writeFile(const string& output, const string& fileName) {
	ofstream file(fileName);

	if (file.is_open()) {
		file << output;
		file.close();
		cout << "Файл успешно записан: " << fileName << endl;
	}
	else {
		cerr << "Не удалось записать в файл: " << fileName << endl;
		cerr << "Пожалуйста, проверьте существование каталога и наличие прав на запись." << endl;
		cerr << "Путь к файлу: " << _getcwd(NULL, 0) << "\\" << fileName << endl;
	}
}

void init264() {
	regex valid_input("^[12]$");
	string input;
	char in_option, out_option;
	char repeat_option = '1';

	do {
		do {
			cout << "Введите '1' для ввода с консоли, '2' для ввода из файла: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		in_option = input[0];

		do {
			cout << "Введите '1' для вывода на консоль, '2' для вывода в файл: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		out_option = input[0];

		string dataBuffer;
		if (in_option == '1') {
			dataBuffer = readConsole();
		}
		else {
			string filePath = getUserPath("Укажите файл для ввода исходных данных для работы программы", MyConstants::defaultTask264Input);
			ifstream testFile(filePath);
			if (!testFile) {
				cerr << "Ошибка: файл не найден или не доступен для чтения: " << filePath << endl;
				continue;
			}
			dataBuffer = readFile(filePath);
			if (dataBuffer == "") {
				cout << "Ошибка пустой файл, начните ввод заново.\n";
				exit(1);
			}
		}

		string outputData = removeParentheses(dataBuffer);

		if (out_option == '1') {
			writeConsole(outputData);
		}
		else {
			string filePath = getUserPath("Укажите файл для вывода результатов работы программы", MyConstants::defaultTask264Output);
			ofstream testFile(filePath, ios::out | ios::trunc);
			writeFile(outputData, filePath);
		}

		if (in_option == '1') {
			do {
				cout << "Хотите начать сначала? 1 - Да, 2 - Нет: ";
				getline(cin, input);
			} while (!regex_match(input, valid_input));
			repeat_option = input[0];
		}
		else {
			repeat_option = '2';
		}
	} while (repeat_option == '1');
}