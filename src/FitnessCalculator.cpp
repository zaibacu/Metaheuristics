#include "FitnessCalculator.h"
#include "Util.h"

namespace Util
{
	CFitnessCalculator::CFitnessCalculator()
		: m_dMin(Util::GetMax<double>())
		, m_dMax(Util::GetMin<double>())
	{

	}

	CFitnessCalculator::~CFitnessCalculator()
	{

	}

	void CFitnessCalculator::AddFitness(const double p_cdResult)
	{
		m_dMax = p_cdResult > m_dMax ? p_cdResult : m_dMax;
		m_dMin = p_cdResult < m_dMin ? p_cdResult : m_dMin;
		m_lstResult.push_back(p_cdResult);
	}

	double CFitnessCalculator::GetMax() const
	{
		return m_dMax;
	}

	double CFitnessCalculator::GetMin() const
	{
		return m_dMin;
	}

	double CFitnessCalculator::GetAvg() const
	{
		double dSum = 0;

		for(const double& crdValue : m_lstResult)
		{
			dSum += crdValue;
		}

		return dSum / m_lstResult.size();
	}
}