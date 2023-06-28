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
