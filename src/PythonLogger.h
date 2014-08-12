#pragma once
#include "ILogger.h"
namespace Util
{
	/**
	 * @brief	A basic Python graph logging implementation
	 */
	class CPythonLogger : public ILogger
	{
		typedef std::function<void(int, double, double, double)> FnGraph;
	public:
		CPythonLogger(FnGraph p_fnGraph);
		virtual ~CPythonLogger();
	private:
		FnGraph m_fnGraph;
		/**
		 *  @brief	Method to handle Python graph format
		 *	@param p_crInput
		 */
		virtual void AddToLog(const std::string& p_crInput) override;
		/**
		 *	@brief Iterational data handler
		 *	@param p_crData
		 */
		virtual void HandleData(const IterationData& p_crData) override;
	};
}