#pragma once

namespace Util
{
	/**
	 *	@brief The most basic, automatic memory manageable object
	 */
	class IObject
	{
	public:
		virtual ~IObject(){};
	};

	/**
	 *	@brief Class which does object memory handling. Follows Factory pattern
	 */
	class CObjectFactory
	{
	private:
		static CObjectFactory* m_pInstance;
		std::list<IObject*> m_lstObjects;
	public:
		/**
		 *	@brief Method to get ObjectFactory
		 *	@return ObjectFactory instance
		 */
		static CObjectFactory* GetInstance();

		/**
		 *	@brief Method for object creation
		 *	@return Constructed object
		 */
		template<typename T>
		T* Create()
		{
			T* Obj = new T();
			m_lstObjects.push_back(dynamic_cast<IObject*>(Obj));
			return Obj;
		}
		virtual ~CObjectFactory();

		/**
		 *	@brief Release control of memory ownership
		 *	@param p_pObject Object which ownership you want to get
		 *	@return Can it be done?
		 */
		bool RequestOwnership(IObject* p_pObj) const;
	private:
		CObjectFactory();
	};
}