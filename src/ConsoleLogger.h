#pragma once
#include "ILogger.h"
namespace Util
{
	/**
	 *	@brief A basic Console logging implementation
	 */
	class CConsoleLogger : public ILogger
	{
	public:
		CConsoleLogger();
		virtual ~CConsoleLogger();

	private:
		/**
		 *	@brief Method to handle Console format
		 *	@param p_crInput
		 */
		virtual void AddToLog(const std::string& p_crInput) override;
	};
}