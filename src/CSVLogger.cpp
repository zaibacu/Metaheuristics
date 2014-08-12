#include "CSVLogger.h"

#include <iostream>


namespace Util
{
	CCSVLogger::CCSVLogger()
	{
		m_File.open("log.txt");
	}

	CCSVLogger::~CCSVLogger()
	{
		m_File.close();
	}

	void CCSVLogger::AddToLog(const std::string& p_crInput)
	{
		m_File << p_crInput << std::endl;
	}
}