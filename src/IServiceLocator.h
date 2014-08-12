#pragma once

namespace Service
{
	template<typename T>
	class IServiceLocator
	{
	public:
		virtual ~IServiceLocator(){};

	protected:
		/**
		 * @brief Provides a service to the locator
		 * 
		 * @param p_pService Service implementation
		 */
		virtual void Provide(T* p_pService) = 0;
		/**
		 * @brief Service getter
		 * @return Service implementation
		 */
		virtual T* Get() = 0;
	};
}