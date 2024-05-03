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

string readFromConsole() {
	cout << "Enter your data: ";
	string data;
	getline(cin, data);
	return data;
}

string readFromFile(const string& filePath) {
	ifstream file(filePath);
	string data, buffer;
	while (getline(file, buffer)) {
		data += buffer + "\n";
	}
	file.close();
	return data;
}

void writeToConsole(const string& outputData) {
	cout << "Output: " << outputData << endl;
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

string getPathFromUser(const string& message, const string& defaultPath) {
	cout << message << " [" << defaultPath << "]: ";
	string path;
	getline(cin, path);
	return (path.empty() ? defaultPath : path);
}

pair<vector<pair<int, int>>, vector<vector<int>>> processData(const string& data) {
	stringstream ss(data);
	int n;
	ss >> n;  // Read the number of points

	vector<pair<int, int>> points(n);
	vector<vector<int>> matrix(n, vector<int>(n));

	// Reading points coordinates
	for (int i = 0; i < n; i++) {
		ss >> points[i].first >> points[i].second;
	}

	// Reading adjacency matrix
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ss >> matrix[i][j];
		}
	}

	return make_pair(points, matrix);
}

void saveDrawingToFile(const vector<pair<int, int>>& points, const vector<vector<int>>& matrix, const string& filePath) {
	const int width = 800, height = 600;
	sf::RenderTexture renderTexture;
	renderTexture.create(width, height);
	renderTexture.clear(sf::Color::White);

	// Prepare lines and points
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

	// Draw each point as a circle
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
			dataBuffer = readFromFile(getPathFromUser("Укажите файл для ввода исходных данных для работы программы", MyConstants::defaultTask417Input));
		}

		pair<vector<pair<int, int>>, vector<vector<int>>> result = processData(dataBuffer);
		vector<pair<int, int>> points = result.first;
		vector<vector<int>> matrix = result.second;

		if (out_option == '1') {
			drawWithSFML(points, matrix);  // Прямо нарисуйте с помощью SFML
		}
		else {
			string filePath = getPathFromUser("Укажите файл для вывода результатов работы программы", MyConstants::defaultTask417Output);
			filePath.append("output.png"); // Может потребоваться более надежно обрабатывать имя файла или расширения
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