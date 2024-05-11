#pragma once
#include <iostream>
#include <fstream>
#include <array>
#include <utility>
#include <string>
#include "constants.h"

void saveDrawingToFile(const std::array<std::pair<int, int>, MAX_POINTS>& points, size_t point_count, const std::array<std::array<int, MAX_POINTS>, MAX_POINTS>& matrix, std::string& filePath);
void drawWithSFML(const std::array<std::pair<int, int>, MAX_POINTS>& points, size_t point_count, const std::array<std::array<int, MAX_POINTS>, MAX_POINTS>& matrix);
void printPointsAndMatrix(const std::array<std::pair<int, int>, MAX_POINTS>& points, const std::array<std::array<int, MAX_POINTS>, MAX_POINTS>& matrix);
std::string readFromConsole(int& pointCount);
std::string readFromFile(const std::string& filePath);
std::string getPathFromUser(const std::string& prompt, const std::string& defaultPath);
std::pair<std::array<std::pair<int, int>, MAX_POINTS>, std::array<std::array<int, MAX_POINTS>, MAX_POINTS>> processData(const std::string& data, bool generateMatrix);
bool inputMatrixBody(double**& matrix, int& rows, int& cols);
bool inputAdjacencyMatrix(std::string& dataBuffer, int& pointCount);
void init417();
