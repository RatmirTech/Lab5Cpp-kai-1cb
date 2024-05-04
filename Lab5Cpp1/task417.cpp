#include "task417.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "SFML/Graphics.hpp"

using namespace std;

void drawWithSFML(const vector<pair<int, int>>& points, const vector<vector<int>>& matrix) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Task 417 Visualization");
	sf::VertexArray lines(sf::Lines);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		for (size_t i = 0; i < points.size(); ++i) {
			for (size_t j = 0; j < points.size(); ++j) {
				if (matrix[i][j] == 1) {
					sf::Vertex lineStart(sf::Vector2f(points[i].first, points[i].second), sf::Color::Black);
					sf::Vertex lineEnd(sf::Vector2f(points[j].first, points[j].second), sf::Color::Black);
					lines.append(lineStart);
					lines.append(lineEnd);
				}
			}
		}

		for (const auto& point : points) {
			sf::CircleShape shape(5);
			shape.setPosition(static_cast<float>(point.first) - 5, static_cast<float>(point.second) - 5);
			shape.setFillColor(sf::Color::Red);
			window.draw(shape);
		}

		window.draw(lines);
		window.display();
	}
}

void printPointsAndMatrix(const vector<pair<int, int>>& points, const vector<vector<int>>& matrix) {
	cout << "Координаты точек:" << endl;
	for (const auto& point : points) {
		cout << point.first << " " << point.second << endl;
	}

	cout << "Матрица смежности:" << endl;
	for (const auto& row : matrix) {
		for (int val : row) {
			cout << val << " ";
		}
		cout << endl;
	}
}


string readFromConsole() {
	regex pattern("^([1-9][0-9]* [1-9][0-9]*(, [1-9][0-9]* [1-9][0-9]*)*)$");
	string input;
	vector<pair<int, int>> points;
	string line;

	while (true) {
		cout << "Введите координаты точек в формате 'x y, x y, ...': ";
		getline(cin, input);
		if (regex_match(input, pattern)) {
			break;
		}
		cout << "Некорректный ввод. Попробуйте еще раз.\n";
	}

	stringstream ss(input);
	while (getline(ss, line, ',')) {
		stringstream coordStream(line);
		int x, y;
		coordStream >> x >> y;
		points.push_back({ x, y });
	}

	stringstream result;
	result << points.size() << '\n';
	for (const auto& p : points) {
		result << p.first << ' ' << p.second << '\n';
	}

	vector<vector<int>> matrix(points.size(), vector<int>(points.size(), 0));
	for (size_t i = 0; i < points.size(); ++i) {
		for (size_t j = i + 1; j < points.size(); ++j) {
			matrix[i][j] = matrix[j][i] = (i + 1) % 2;
		}
	}

	for (const auto& row : matrix) {
		for (const auto& val : row) {
			result << val << ' ';
		}
		result << '\n';
	}

	return result.str();
}

string readFromFile(const string& filePath) {
	ifstream file(filePath);
	stringstream buffer;
	if (file) {
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else {
		cerr << "Ошибка: Не удалось открыть файл '" << filePath << "'." << endl;
		exit(EXIT_FAILURE);
	}
}

void writeToConsole(const string& outputData) {
	cout << "Результат: " << outputData << endl;
}

void writeToFile(const string& outputData, const string& filePath) {
	ofstream file(filePath, ios::out | ios::trunc);
	if (file.is_open()) {
		file << outputData;
		file.close();
	}
	else {
		cerr << "Не удалось открыть файл для записи: " << filePath << endl;
	}
}

string getPathFromUser(const string& prompt, const string& defaultPath) {
	string path;
	cout << prompt << " [" << defaultPath << "]: ";
	getline(cin, path);
	return (path.empty() ? defaultPath : path);
}

pair<vector<pair<int, int>>, vector<vector<int>>> processData(const string& data) {
	stringstream ss(data);
	int n;
	ss >> n;

	vector<pair<int, int>> points(n);
	vector<vector<int>> matrix(n, vector<int>(n));

	for (int i = 0; i < n; i++) {
		ss >> points[i].first >> points[i].second;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ss >> matrix[i][j];
		}
	}

	printPointsAndMatrix(points, matrix);
	return make_pair(points, matrix);
}

void saveDrawingToFile(const vector<pair<int, int>>& points, const vector<vector<int>>& matrix, const string& filePath) {
	const int width = 800, height = 600;
	sf::RenderTexture renderTexture;
	renderTexture.create(width, height);
	renderTexture.clear(sf::Color::White);

	sf::VertexArray lines(sf::Lines);
	for (size_t i = 0; i < points.size(); ++i) {
		for (size_t j = i + 1; j < points.size(); ++j) {
			if (matrix[i][j] == 1) {
				sf::Vertex lineStart(sf::Vector2f(points[i].first, points[i].second), sf::Color::Black);
				sf::Vertex lineEnd(sf::Vector2f(points[j].first, points[j].second), sf::Color::Black);
				lines.append(lineStart);
				lines.append(lineEnd);
			}
		}
	}

	for (const auto& point : points) {
		sf::CircleShape shape(5);
		shape.setPosition(static_cast<float>(point.first) - 5, static_cast<float>(point.second) - 5);
		shape.setFillColor(sf::Color::Red);
		renderTexture.draw(shape);
	}

	renderTexture.draw(lines);
	renderTexture.display();

	sf::Texture texture = renderTexture.getTexture();
	sf::Image image = texture.copyToImage();
	image.saveToFile(filePath);
}

void init417() {
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
			dataBuffer = readFromConsole();
		}
		else {
			string filePath = getPathFromUser("Укажите файл для ввода исходных данных для работы программы", MyConstants::defaultTask417Input);
			dataBuffer = readFromFile(filePath);
		}

		auto result = processData(dataBuffer);
		vector<pair<int, int>> points = result.first;
		vector<vector<int>> matrix = result.second;

		if (out_option == '1') {
			drawWithSFML(points, matrix);
		}
		else {
			string filePath = getPathFromUser("Укажите файл для вывода результатов работы программы", MyConstants::defaultTask417Output);
			//filePath.append("output.png");
			saveDrawingToFile(points, matrix, filePath);
		}

		if (in_option == '1') {
			do {
				cout << "Хотите начать сначала? Да - 1, Нет - 2: ";
				getline(cin, input);
			} while (!regex_match(input, valid_input));
			repeat_option = input[0];
		}
		else {
			repeat_option = '2';
		}

	} while (repeat_option == '1');
}