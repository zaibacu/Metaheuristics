#include "ObjectFactory.h"

namespace Util
{

	//Static stuff
	CObjectFactory* CObjectFactory::m_pInstance = nullptr;

	CObjectFactory* CObjectFactory::GetInstance()
	{
		if(m_pInstance == nullptr)
			m_pInstance = new CObjectFactory();

		return m_pInstance;
	}

	CObjectFactory::CObjectFactory()
	{
		
	}

	CObjectFactory::~CObjectFactory()
	{
		for(IObject* pItem : m_lstObjects)
		{
			delete pItem;
		}
	}

	bool CObjectFactory::RequestOwnership(IObject* p_pObj) const
	{
		if(!p_pObj) //Nullptr
			return true;

		for(IObject* pItem : m_lstObjects)
		{
			if(pItem == p_pObj) //This object belongs to factory
				return false;
		}

		return true;
	}
}