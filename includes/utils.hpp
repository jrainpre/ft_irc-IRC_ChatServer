#pragma once
#include <iostream>
#include <string>
#include <vector>

bool    errorMsg(std::string msg);
bool    validPort(std::string port);
std::vector<std::string> split(std::string &s, std::string delimit, bool keepLimiter);
std::string getSubstr(std::string &str, std::string del);