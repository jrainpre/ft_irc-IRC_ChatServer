#include "../includes/utils.hpp"
#include <cstdlib>


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

std::string getSubstr(std::string &str, std::string del)
{
    size_t pos_end;
    size_t len_del = del.size();
    std::string ret;
    if(str.empty())
        return ret;
    if(pos_end == str.find(del, 0) == std::string::npos)
    {
        ret = str;
        str.clear();
        return ret;
    }
    ret = str.substr(0, pos_end);
    str.erase(0, pos_end + len_del);
    return ret;
}

std::vector<std::string> split(std::string &s, std::string delimit, bool keepLimiter)
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
        if(keepLimiter)
        {
            substr = s.substr(pos_end, len_delimit);
            ret[ret.size() - 1] += " ";
            ret[ret.size() - 1] += substr;
        }
        s.erase(pos_start, pos_end + len_delimit);
    }
    if(!s.empty())
    {
        ret.push_back(s);
        s.clear();
    }
    return ret;
}

std::string addMsgs(int itStart, std::vector<std::string> &cmd)
{
    std::string ret;
    for(int i = itStart; i < cmd.size(); i++)
    {
        ret += cmd[i];
        if(i + 1 < cmd.size())
            ret += " ";
    }
    if(ret[0] == ':')
        ret.erase(ret.begin());
    return ret;
}

