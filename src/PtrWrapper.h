#pragma once

#include "ObjectFactory.h"
namespace Util
{
	/**
	 *	@brief A special SmartPtr class. 
	 *	@details Very similar to std::unique_ptr
	 *  but designed to collaborate with inner ObjectFactory
	 */
	template<typename T>
	class CPtrWrapper
	{
	private:
		T* m_pPtr;
	public:
		CPtrWrapper()
		{
			m_pPtr = nullptr;
		}

		CPtrWrapper(T* p_pPtr)
			: m_pPtr(p_pPtr)
		{

		}

		/**
		 * @brief Avoid this constructor as much as possible
		 * 
		 */
		CPtrWrapper(const T* p_cpPtr)
			: m_pPtr(const_cast<T*>(p_cpPtr))
		{

		}

		virtual ~CPtrWrapper()
		{
			const auto* cpFactory = CObjectFactory::GetInstance();
			/*
			  We get ownership either if member is not IObject type (dynamic cast would result nullptr)
			  or it was created not by ObjectFactory
			 */
			if(cpFactory->RequestOwnership(dynamic_cast<IObject*>(m_pPtr)))
			{
				//delete m_pPtr;
			}
		};

		T* operator->()
		{
			return m_pPtr;
		}

		const T* operator->() const
		{
			return m_pPtr;
		}

		/** 
		 *	@brief Basic Mutator: Setter
		 *	@param p_pPtr Object pointer
		 */
		void Set(T* p_pPtr)
		{
			m_pPtr = p_pPtr;
		}

		/**
		 *	@brief Basic Mutator: Getter
		 *	@return Object pointer
		 */
		T* Get()
		{
			return m_pPtr;
		}

		/**
		 *	@brief Basic Mutator: Const Getter
		 *	@return constant Object pointer
		 */
		const T* Get() const
		{
			return m_pPtr;
		}
	};
}