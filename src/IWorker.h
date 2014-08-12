#pragma once
namespace Service
{
	/**
	 * @brief	A worker interface used to provide different implementations
	 */
	class IWorker
	{
	public:
		/**
		 * @brief Empty virtual destructor. A trick to prevent memory leak (Old C++ issue)
		 */
		virtual ~IWorker(){};

		/**
		 * @brief Method used to get current worker priority. Higher - bigger chance to be used
		 * @return Priority
		 */
		virtual int GetPriority() const = 0;
		/**
		 * @brief Method used to provide work for worker
		 * @param p_fnWork Work lambda function
		 */
		virtual void Do(std::function<void()> p_fnWork) = 0;
		/**
		 *	@brief Method used to check if current worker is busy
		 *	@return State of worker boolean
		 */
		virtual bool IsBusy() const = 0;
	};
}