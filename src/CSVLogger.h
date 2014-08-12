#pragma once
#include "ILogger.h"

#include <fstream>

namespace Util
{
	/**
	 *	@brief A basic CSV logging implementation
	 */
	class CCSVLogger : public ILogger
	{
	private:
		std::ofstream m_File;
	public:
		CCSVLogger();
		virtual ~CCSVLogger();

	private:
		/**
		 *	@brief Method to handle CSV format
		 *	@param p_crInput
		 */
		virtual void AddToLog(const std::string& p_crInput) override;
	};
}