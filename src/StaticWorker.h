#pragma once
#include "IWorker.h"

namespace Service
{
	class CStaticWorker : public IWorker
	{
	private:
		bool m_bBusy;
	public:
		CStaticWorker()
			: m_bBusy(false)
		{

		}
		virtual ~CStaticWorker(){ };


		/**
		 *  @brief	Method used to get current worker priority. Higher - bigger chance to be used
		 *	@return Priority
		 */
		virtual int GetPriority() const override
		{
			return 1;
		}

		/**
		 *  @brief Method used to provide work for worker
		 *	@param p_fnWork Work lambda function
		 */
		virtual void Do(std::function<void()> p_fnWork) override
		{
			m_bBusy = true;
			p_fnWork();
			m_bBusy = false;
		}

		/**
		 *  @brief	Method used to check if current worker is busy
		 *	@return State of worker boolean
		 */
		virtual bool IsBusy() const
		{
			return m_bBusy;
		}
	};
}