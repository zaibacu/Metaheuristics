#pragma once
#include "IServiceLocator.h"
#include "IWorker.h"

namespace Service
{
	/**
	 *	@brief A worker system locator based on a Service pattern.
	 *	@details  Used to provide worker functionality for various log implementations, eg.:
	 *	Thread, remote machine etc
	 */
	class CWorkerLocator : public IServiceLocator<IWorker>
	{
	public:
		virtual ~CWorkerLocator();

		/**
		 * 	@brief Method used to get worker
		 *	@returns Worker implementation
		 */
		static IWorker* GetWorker();
		/**
		 *	@brief Method to provide worker implementation to the system
		 *	@param p_pService Worker implementation
		 */
		static void ProvideWorker(IWorker* p_pWorker);
		/**
		 *	@brief Method used to check if all workers done their job
		 *	@returns Result boolean
		 */
		static bool IsWorkDone();

	protected:
		CWorkerLocator();
		/**
		 *	@brief Method to provide worker implementation to the system
		 *	@param p_pService Worker implementation
		 */
		virtual void Provide(IWorker* p_pService) override;
		/**
		 *	@brief Method used to get worker
		 *	@returns Worker implementation
		 */
		virtual IWorker* Get() override;

		/**
		 *	@brief Method used to check if all workers done their job
		 *	@returns Result boolean
		 */
		bool IsDone() const;

		/**
		 *	@brief Method used to get locator instance
		 *	@returns WorkerLocator
		 */
		static CWorkerLocator* GetInstance();
	private:
		static CWorkerLocator* m_pInstance;
		std::vector<IWorker*> m_vWorkers;
	};
}