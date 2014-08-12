#pragma once
#include <sstream>


namespace Util
{
	/**
	 * @brief Helper class to store coordinates
	 */
	template<typename T>
	struct Coord{
		T m_X;
		T m_Y;
		Coord(const T p_X, const T p_Y)
			: m_X(p_X)
			, m_Y(p_Y)
		{

		}

		static T Distance(const Coord<T>& p_crLeft, const Coord<T>& p_crRight)
		{
			return sqrt(pow(p_crLeft.m_X - p_crRight.m_X, 2.0) + pow(p_crLeft.m_Y - p_crRight.m_Y, 2.0));
		}
	};


	/**
	 *  @brief Converts any type of variable to string
	 *	@param p_crInput variable
	 *	@return string of that variable
	 */
	template<typename T>
	std::string ToString(const T& p_crInput)
	{
		std::ostringstream Stream;

		Stream << p_crInput;
		return Stream.str();
	}

	/**
	 *  @brief Gets random number of any variable type	
	 *	@return random variable
	 */
	template<typename T>
	T GetRandom(const T p_cLower, const T p_cUpper)
	{
		if(p_cUpper == 0)
			return 0;
			//throw "Invalid upper limit";

		return rand() % p_cUpper + p_cLower;
	}

	/**
	 *  @brief Gets max value of any variable type
	 *	@return max value
	 */
	template<typename T>
	T GetMax()
	{
		#ifdef MAX_VALUE
			return MAX_VALUE;
		#else
			return std::numeric_limits<T>::max();
		#endif
	}

	/**
	 *  @brief Gets max value of any variable type
	 *	@return min value
	 */
	template<typename T>
	T GetMin()
	{
		#ifdef MIN_VALUE
			return MIN_VALUE;
		#else
			return std::numeric_limits<T>::min();
		#endif
	}

	/**
	 *  @brief Gets default value of any variable type
	 *	@return default value
	 */
	template<typename T>
	T Default()
	{
		return T();
	}

	/**
	 *	@brief Iteration definition struct
	 *
	 */
	struct IterationData
	{
		unsigned long m_ulIteration;
		double m_dMin;
		double m_dMax;
		double m_dAvg;

		IterationData(const unsigned long p_culIteration, const double p_cdMin, const double p_cdMax, const double p_cdAvg)
			: m_ulIteration(p_culIteration)
			, m_dMin(p_cdMin)
			, m_dMax(p_cdMax)
			, m_dAvg(p_cdAvg)
		{

		}
	};

	/**
	 * @brief Function which makes main thread wait
	 * 
	 * @param p_fnWait - Predicate to check if we still need to wait
	 */
	void WaitFor(std::function<bool()> p_fnWait);

	/**
	 * @brief A method to apply thread sleep
	 * 
	 */
	void Sleep(const unsigned long p_culMilisec);
}