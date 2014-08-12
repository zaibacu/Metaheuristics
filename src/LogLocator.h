#pragma once
#include "IServiceLocator.h"
#include "ILogger.h"
namespace Service
{
	/**
	 *	@brief A log system locator based on a Service pattern.
	 *	@details Used to provide log functionality for various log implementations, eg.:
	 *	Console, CSV, GraphOutput, TCP/IP Socket pipe etc
	 */
	class CLogLocator : public IServiceLocator<Util::ILogger>
	{
	public:
		virtual ~CLogLocator();
		
		/**
		 *	@brief Method used to get logger
		 *	@returns Log implementation
		 */
		static Util::ILogger* GetLog();
		/**
		 *	@brief Method to provide log implementation to the system
		 *	@param p_pService Log implementation
		 */
		static void ProvideLog(Util::ILogger* p_pService);

	protected:
		CLogLocator();

		/**
		 *	@brief Method to provide log implementation to the system
		 *	@param p_pService Log implementation
		 */
		virtual void Provide(Util::ILogger* p_pService) override;
		/**
		 *	@brief Method used to get logger
		 *	@returns Log implementation
		 */
		virtual Util::ILogger* Get() override;

		/**
		 *	@brief Method used to get locator instance
		 *	@returns LogLocator
		 */
		static CLogLocator* GetInstance();

	private:
		static CLogLocator* m_pInstance;
		Util::ILogger* m_pService;
	};
}