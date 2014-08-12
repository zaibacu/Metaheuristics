#include "ConsoleLogger.h"

#include <iostream>

namespace Util
{
	CConsoleLogger::CConsoleLogger()
	{

	}

	CConsoleLogger::~CConsoleLogger()
	{

	}

	void CConsoleLogger::AddToLog(const std::string& p_crInput)
	{
		std::cout << p_crInput << std::endl;
	}
}