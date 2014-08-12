#pragma once
#include "ILogger.h"
#include "LogLocator.h"
namespace Algorithm
{
	//Defines inner data structure
	template<typename T>
	struct DataSet
	{
		typedef std::vector<T> Type;
	};

	//Defines Fitness function structure
	template<typename T, typename U>
	struct Fitness
	{
		typedef std::function<double(const T&, const U&)> Function; 
	};

	//Defines Stop function structure
	struct Stop
	{
		typedef std::function<bool(const int, const double, const double, const double)> Function;
	};

	/**
	 *	@brief Algorithm interface class. Template parameters _Input and _Output defines input and output datatypes
	 */
	template<typename _Input, typename _Output>
	class IAlgorithm
	{
		typedef typename DataSet<_Input>::Type InputDataSet;
		typedef typename DataSet<_Output>::Type OutputDataSet;
		typedef typename Fitness<InputDataSet, OutputDataSet>::Function FittnessFn;
		typedef typename Stop::Function StopFn;
		
	public:
		/**
		 *	@brief Empty virtual destructor. A trick to prevent memory leak (Old C++ issue)
		 */
		virtual ~IAlgorithm(){};

		/**
		 *	@brief Init function
		 *	@details Used to fill required parameters for algorithm to start
		 */
		virtual void Init() = 0;
		/**
		 *	@brief Computation method. 
		 *	@param p_crInput gives input variables
		 *	@param p_fnFitness is our defined Fitness function
		 *	@param p_fnStop is our defined Stop function
		 *	@return computation result
		 */
		virtual OutputDataSet Compute(const InputDataSet& p_crInput, const FittnessFn p_fnFitness, const StopFn p_fnStop) = 0;
	};
}