#include "task417.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <utility>
#include "SFML/Graphics.hpp"
#include <array>
#include <cmath>

using namespace std;
using namespace sf;

void saveDrawingToFile(const array<pair<int, int>, MAX_POINTS>& points, size_t point_count, const array<array<int, MAX_POINTS>, MAX_POINTS>& matrix, const string& filePath) {
	const int width = 800, height = 600;
	RenderTexture renderTexture;
	renderTexture.create(width, height);
	renderTexture.clear(Color::White);

	VertexArray lines(Lines);
	for (size_t i = 0; i < point_count; ++i) {
		for (size_t j = 0; j < point_count; ++j) {
			if (matrix[i][j] == 1) {
				Vertex lineStart(Vector2f(points[i].first, points[i].second), Color::Black);
				Vertex lineEnd(Vector2f(points[j].first, points[j].second), Color::Black);
				lines.append(lineStart);
				lines.append(lineEnd);
			}
		}
	}
	for (size_t i = 0; i < point_count; ++i) {
		CircleShape shape(5);
		shape.setPosition(static_cast<float>(points[i].first) - 5, static_cast<float>(points[i].second) - 5);
		shape.setFillColor(Color::Red);
		renderTexture.draw(shape);
	}


	renderTexture.draw(lines);
	renderTexture.display();

	Texture texture = renderTexture.getTexture();
	Image image = texture.copyToImage();
	image.saveToFile(filePath);
}

void drawWithSFML(const array<pair<int, int>, MAX_POINTS>& points, size_t point_count, const array<array<int, MAX_POINTS>, MAX_POINTS>& matrix) {
	RenderWindow window(VideoMode(800, 600), "Task 417 Visualization");
	VertexArray lines(Lines);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear(Color::White);
		for (size_t i = 0; i < point_count; ++i) {
			for (size_t j = 0; j < point_count; ++j) {
				if (matrix[i][j] == 1) {
					Vertex lineStart(Vector2f(points[i].first, points[i].second), Color::Black);
					Vertex lineEnd(Vector2f(points[j].first, points[j].second), Color::Black);
					lines.append(lineStart);
					lines.append(lineEnd);
				}
			}
		}
		for (size_t i = 0; i < point_count; ++i) {
			CircleShape shape(5);
			shape.setPosition(static_cast<float>(points[i].first) - 5, static_cast<float>(points[i].second) - 5);
			shape.setFillColor(Color::Red);
			window.draw(shape);
		}

		window.draw(lines);
		window.display();
	}
}

void printPointsAndMatrix(const array<pair<int, int>, MAX_POINTS>& points, const array<array<int, MAX_POINTS>, MAX_POINTS>& matrix) {
	cout << "Координаты точек:" << endl;
	for (size_t i = 0; i < points.size(); i++) {
		cout << points[i].first << " " << points[i].second << endl;
	}

	cout << "Матрица смежности:" << endl;
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

string readFromConsole() {
	regex pattern("^([1-9][0-9]* [1-9][0-9]*(, [1-9][0-9]* [1-9][0-9]*)*)$");
	string input;
	array<pair<int, int>, MAX_POINTS> points;
	string line;
	int point_count = 0;

	while (true) {
		cout << "Введите координаты точек в формате 'x y, x y, ...': ";
		getline(cin, input);
		if (regex_match(input, pattern)) {
			break;
		}
		cout << "Некорректный ввод. Попробуйте еще раз.\n";
	}

	stringstream ss(input);
	while (getline(ss, line, ',') && point_count < MAX_POINTS) {
		stringstream coordStream(line);
		int x, y;
		coordStream >> x >> y;
		points[point_count] = { x, y };
		++point_count;
	}

	stringstream result;
	result << point_count << '\n';
	for (int i = 0; i < point_count; i++) {
		result << points[i].first << ' ' << points[i].second << '\n';
	}

	//array<array<int, MAX_POINTS>, MAX_POINTS> matrix;
	//for (int i = 0; i < point_count; ++i) {
	//	for (int j = 0; j < point_count; ++j) {
	//		matrix[i][j] = (i != j) ? (rand() % 2) : 0;
	//	}
	//}

	//for (int i = 0; i < point_count; i++) {
	//	for (int j = 0; j < point_count; j++) {
	//		result << matrix[i][j] << ' ';
	//	}
	//	result << '\n';
	//}

	//cout << "Входные данные: " << endl << result.str() << endl << "Конец входных данных" << endl;

	return result.str();
}

string readFromFile(const string& filePath) {
	ifstream file(filePath);
	if (!file) {
		cerr << "Ошибка: Не удалось открыть файл '" << filePath << "'." << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	int line_count = 0;
	int number_of_points;
	std::array<std::pair<int, int>, MAX_POINTS> points;
	std::array<std::array<int, MAX_POINTS>, MAX_POINTS> matrix;
	regex points_pattern("^\\d+ \\d+$");
	regex matrix_pattern("^([01] )+[01]$");

	while (getline(file, line)) {
		if (line_count == 0) {
			if (!regex_match(line, regex("^\\d+$"))) {
				cerr << "Ошибка: Первая строка должна содержать только одно число." << endl;
				exit(EXIT_FAILURE);
			}
			number_of_points = stoi(line);
			if (number_of_points > MAX_POINTS) {
				cerr << "Ошибка: Количество точек превышает максимально допустимое значение." << endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (line_count <= number_of_points) {
			if (!regex_match(line, points_pattern)) {
				cerr << "Ошибка: Координаты точек должны быть в формате 'x y'." << endl;
				exit(EXIT_FAILURE);
			}
			istringstream iss(line);
			int x, y;
			iss >> x >> y;
			points[line_count - 1] = { x, y };
		}
		else {
			if (!regex_match(line, matrix_pattern) || count(line.begin(), line.end(), ' ') + 1 != number_of_points) {
				cerr << "Ошибка: Строка матрицы смежности должна содержать " << number_of_points << " значений." << endl;
				exit(EXIT_FAILURE);
			}
			istringstream iss(line);
			for (int j = 0; j < number_of_points; j++) {
				iss >> matrix[line_count - number_of_points - 1][j];
			}
		}
		line_count++;
	}

	if (line_count - 1 != number_of_points * 2) {
		cerr << "Ошибка: Количество строк в файле не соответствует ожидаемому." << endl;
		exit(EXIT_FAILURE);
	}

	stringstream buffer;
	buffer << number_of_points << '\n';
	for (int i = 0; i < number_of_points; i++) {
		buffer << points[i].first << ' ' << points[i].second << '\n';
	}
	for (int i = 0; i < number_of_points; i++) {
		for (int j = 0; j < number_of_points; j++) {
			buffer << matrix[i][j] << ' ';
		}
		buffer << '\n';
	}
	return buffer.str();
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
pair<array<pair<int, int>, MAX_POINTS>, array<array<int, MAX_POINTS>, MAX_POINTS>> processData(const string& data, bool generateMatrix) {
	stringstream ss(data);
	int n;
	ss >> n;

	array<pair<int, int>, MAX_POINTS> points;
	array<array<int, MAX_POINTS>, MAX_POINTS> matrix;

	for (int i = 0; i < MAX_POINTS; i++) {
		for (int j = 0; j < MAX_POINTS; j++) {
			matrix[i][j] = 0;
		}
	}

	for (int i = 0; i < n; i++) {
		ss >> points[i].first >> points[i].second;
	}

	if (!generateMatrix) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				ss >> matrix[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				int dx = points[i].first - points[j].first;
				int dy = points[i].second - points[j].second;
				if (sqrt(pow(dx, 2) + pow(dy, 2)) < 100) {
					matrix[i][j] = matrix[j][i] = 1;
				}
			}
		}
	}

	cout << "Точки:\n";
	for (int i = 0; i < n; i++) {
		cout << "(" << points[i].first << ", " << points[i].second << ")\n";
	}

	cout << "Матрица смежности:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}

	return { points, matrix };
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

		bool generateMatrix{ (in_option == '1') };
		string dataBuffer;
		if (in_option == '1') {
			dataBuffer = readFromConsole();
		}
		else {
			string filePath = getPathFromUser("Укажите файл для ввода исходных данных для работы программы", MyConstants::defaultTask417Input);
			dataBuffer = readFromFile(filePath);
			generateMatrix = false;
		}

		auto result = processData(dataBuffer, generateMatrix);
		array<pair<int, int>, MAX_POINTS> points = result.first;
		array<array<int, MAX_POINTS>, MAX_POINTS> matrix = result.second;

		size_t point_count = static_cast<size_t>(count_if(points.begin(), points.end(),
			[](const pair<int, int>& p) { return p.first != 0 || p.second != 0; }));


		if (out_option == '1') {
			drawWithSFML(points, point_count, matrix);
		}
		else {
			string filePath = getPathFromUser("Укажите файл для вывода результатов работы программы", MyConstants::defaultTask417Output);
			saveDrawingToFile(points, point_count, matrix, filePath);
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