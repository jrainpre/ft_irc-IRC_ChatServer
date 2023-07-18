#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

bool    errorMsg(std::string msg);
bool    validPort(std::string port);
std::vector<std::string> split(std::string &s, std::string delimit, bool keepLimiter);
std::string getSubstr(std::string &str, std::string del);
std::string addMsgs(int itStart, std::vector<std::string> &cmd);

template<typename T>
std::string toString (const T& val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}