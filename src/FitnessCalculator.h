#pragma once

namespace Util
{
	/**
	 *	@brief Class designed to do standart statistical calculation of provided data
	 */
	class CFitnessCalculator
	{
	private:
		double m_dMin;
		double m_dMax;
		std::list<double> m_lstResult;
	public:
		CFitnessCalculator();
		virtual ~CFitnessCalculator();
		/**
		 *	@brief Adds new value
		 *	@param p_cdResult
		 */
		void AddFitness(const double p_cdResult);
		/**
		 *	@brief Get max value
		 *	@return max value
		 */
		double GetMax() const;
		/**
		 *	@brief Get min value
		 *	@return min value
		 */
		double GetMin() const;
		/**
		 *	@brief Get average value
		 *	@return average value
		 */
		double GetAvg() const;
	};
}
