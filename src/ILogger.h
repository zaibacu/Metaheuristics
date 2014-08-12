#pragma once

#include "Util.h"
namespace Util
{
	template<typename T>
	struct identity { typedef T type; };

	/**
	 *	@brief Basic logging interface
	 */
	class ILogger
	{
	public:
		virtual ~ILogger(){};

		/**
		 *	@brief Log any type of value
		 *	@param p_crInput
		 */
		template<typename T>
		void Log(const T& p_crInput)
		{
			Log(p_crInput, identity<T>());
		}
	protected:
		/**
		 *	@brief Log any type of value
		 *	@param p_crInput
		 */
		template<typename T>
		void Log(const T& p_crInput, identity<T>)
		{
			AddToLog(ToString(p_crInput));
		}

		/**
		 *	@brief Special logging case for iterational data
		 *	@param p_crInput
		 */
		void Log(const IterationData& p_crInput, identity<IterationData>)
		{
			HandleData(p_crInput);
		}

		/**
		 *	@brief Virtual method for log implementation
		 *	@param p_crInput
		 */
		virtual void AddToLog(const std::string& p_crInput) = 0;
		/**
		 *	@brief Iterational data handler
		 *	@param p_crData
		 */
		virtual void HandleData(const IterationData& p_crData)
		{

		}
	};
}