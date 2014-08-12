#include "PythonLogger.h"
#include <iostream>

namespace Util
{
	CPythonLogger::CPythonLogger(FnGraph p_fnGraph)
		: m_fnGraph(p_fnGraph)
	{

	}

	CPythonLogger::~CPythonLogger()
	{

	}

	void CPythonLogger::AddToLog(const std::string& p_crInput)
	{
		std::cout << p_crInput << std::endl;
	}

	void CPythonLogger::HandleData(const IterationData& p_crData)
	{
		//std::cout << "Min Fitness: " << p_crData.m_dMin << std::endl;
		m_fnGraph(p_crData.m_ulIteration, p_crData.m_dMin, p_crData.m_dMax, p_crData.m_dAvg);
	}
}