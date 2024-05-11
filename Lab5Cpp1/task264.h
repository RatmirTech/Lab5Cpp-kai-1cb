#pragma once

#include <string>

std::string readConsole();
std::string removeParentheses(const std::string& input);
std::string readFile(const std::string& fileName);
std::string getUserPath(const std::string& prompt, const std::string& defaultPath);
void writeConsole(const std::string& output);
void writeFile(const std::string& output, const std::string& fileName);
void init264();