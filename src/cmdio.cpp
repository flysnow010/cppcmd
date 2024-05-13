#include "cmdio.h"
#include <cstdio>
#include <cstring>

#include <iostream>

namespace Cmd
{

std::string CmdIO::readLine(const char* prompt)
{
	std::string line;
	std::cout << prompt;
	std::getline(std::cin, line);
	return line;
}

void CmdIO::readAnyKey() { fgetc(stdin); }

}