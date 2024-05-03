#include "task264.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "constants.h"
#include <direct.h> 
using namespace std;

string readFromConsole() {
	string input;
	do {
		cout << "������� ������ � ��������� (��� �������� ������ � �������): ";
		getline(cin, input);

		regex pattern(R"(\((?:[^()]*)\))");
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

string readFromFile(const string& fileName) {
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

string getPathFromUser(const string& prompt, const string& defaultPath) {
	string path;
	cout << prompt << " [" << defaultPath << "]: ";
	getline(cin, path);
	if (path.empty()) {
		return defaultPath;
	}
	return path;
}

void writeToConsole(const string& output) {
	cout << "���������: " << output << endl;
}

void writeToFile(const string& output, const string& fileName) {
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
			dataBuffer = readFromConsole();
		}
		else {
			string filePath = getPathFromUser("������� ���� ��� ����� �������� ������ ��� ������ ���������", MyConstants::defaultTask264Input);
			ifstream testFile(filePath);
			if (!testFile) {
				cerr << "������: ���� �� ������ ��� �� �������� ��� ������: " << filePath << endl;
				continue;
			}
			dataBuffer = readFromFile(filePath);
			if (dataBuffer == "") {
				cout << "������ ������ ����, ������� ���� ������.\n";
				exit(1);
			}
		}

		string outputData = removeParentheses(dataBuffer);

		if (out_option == '1') {
			writeToConsole(outputData);
		}
		else {
			string filePath = getPathFromUser("������� ���� ��� ������ ����������� ������ ���������", MyConstants::defaultTask264Output);
			ofstream testFile(filePath, ios::out | ios::trunc);
			writeToFile(outputData, filePath);
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