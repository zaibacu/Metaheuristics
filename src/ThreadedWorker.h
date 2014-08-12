#pragma once
#include "IWorker.h"

#include <thread>
#include <iostream>

namespace Service
{
	class CThreadedWorker : public IWorker
	{
	private:
		std::unique_ptr<std::thread> m_upThread;
		bool m_bBusy;
	public:
		CThreadedWorker()
			: m_bBusy(false)
		{

		}
		virtual ~CThreadedWorker(){ };


		/**
		 *  @brief	Method used to get current worker priority. Higher - bigger chance to be used
		 *	@return Priority
		 */
		virtual int GetPriority() const override
		{
			return 2;
		}

		/**
		 *  @brief Method used to provide work for worker
		 *	@param p_fnWork Work lambda function
		 */
		virtual void Do(std::function<void()> p_fnWork) override
		{
			m_upThread.reset(new std::thread([&, this](){
				m_bBusy = true;
				p_fnWork();
				m_bBusy = false;
			}));
			m_upThread->join();
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