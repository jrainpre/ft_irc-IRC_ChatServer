#include "../includes/utils.hpp"


bool    errorMsg(std::string msg)
{
    std::cout << msg << std::endl;
    return 1;
}

//1 through 65535
bool    validPort(std::string port)
{
    int len = 1;
    int iPort = std::atoi(port.c_str());
    int tmp = iPort;
    while(tmp /= 10)
        len++;

    if(port.size() > 5 || port.size() != len || iPort < 1 || iPort > 65535)
        return 0;
    return 1;
}

std::vector<std::string> split(std::string &s, std::string delimit)
{
    size_t pos_start = 0;
    size_t pos_end;
    size_t len_delimit = delimit.size();
    std::string substr;
    std::vector<std::string> ret;

    while((pos_end = s.find(delimit, pos_start)) != std::string::npos)
    {
        substr = s.substr(pos_start, pos_end);
        ret.push_back(substr);
        s.erase(pos_start, pos_end + len_delimit);
    }
    if(!s.empty())
    {
        ret.push_back(s);
        s.clear();
    }
    return ret;
}
