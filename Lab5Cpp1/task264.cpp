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
		cout << "������� ������ � ��������� (��� �������� ������ � �������): ";
		getline(cin, input);

		regex pattern{ R"(\((?:[^()]*)\))" };
		if (input.empty() || !regex_search(input, pattern)) {
			cerr << "������: ������� ������ � ���� �� ����� �������� � � ������� ��������, ���������� ������� ��� ������.\n";
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
		cout << "�� ������� ������� ����." << endl;
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
	cout << "���������: " << output << endl;
}

extern void writeFile(const string& output, const string& fileName) {
	ofstream file(fileName);

	if (file.is_open()) {
		file << output;
		file.close();
		cout << "���� ������� �������: " << fileName << endl;
	}
	else {
		cerr << "�� ������� �������� � ����: " << fileName << endl;
		cerr << "����������, ��������� ������������� �������� � ������� ���� �� ������." << endl;
		cerr << "���� � �����: " << _getcwd(NULL, 0) << "\\" << fileName << endl;
	}
}

void init264() {
	regex valid_input("^[12]$");
	string input;
	char in_option, out_option;
	char repeat_option = '1';

	do {
		do {
			cout << "������� '1' ��� ����� � �������, '2' ��� ����� �� �����: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		in_option = input[0];

		do {
			cout << "������� '1' ��� ������ �� �������, '2' ��� ������ � ����: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		out_option = input[0];

		string dataBuffer;
		if (in_option == '1') {
			dataBuffer = readConsole();
		}
		else {
			string filePath = getUserPath("������� ���� ��� ����� �������� ������ ��� ������ ���������", MyConstants::defaultTask264Input);
			ifstream testFile(filePath);
			if (!testFile) {
				cerr << "������: ���� �� ������ ��� �� �������� ��� ������: " << filePath << endl;
				continue;
			}
			dataBuffer = readFile(filePath);
			if (dataBuffer == "") {
				cout << "������ ������ ����, ������� ���� ������.\n";
				exit(1);
			}
		}

		string outputData = removeParentheses(dataBuffer);

		if (out_option == '1') {
			writeConsole(outputData);
		}
		else {
			string filePath = getUserPath("������� ���� ��� ������ ����������� ������ ���������", MyConstants::defaultTask264Output);
			ofstream testFile(filePath, ios::out | ios::trunc);
			writeFile(outputData, filePath);
		}

		if (in_option == '1') {
			do {
				cout << "������ ������ �������? 1 - ��, 2 - ���: ";
				getline(cin, input);
			} while (!regex_match(input, valid_input));
			repeat_option = input[0];
		}
		else {
			repeat_option = '2';
		}
	} while (repeat_option == '1');
}