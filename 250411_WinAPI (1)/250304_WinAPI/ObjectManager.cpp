#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager()
	: nextId(0)
{

}

void ObjectManager::Init()
{
	nextId = 0;
}

void ObjectManager::AddObject(int id, GameObject* obj)
{
	obj->SetObjectId(nextId++);
	m_objects.emplace(id, obj);
}

void ObjectManager::RemoveObject(int id)
{
}

void ObjectManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	for (auto& obj : m_objects)
	{
		if (obj.second->IsActive() == false	||
			obj.second->IsHidden())
			continue;
		obj.second->Render(renderTarget);
	}
}

void ObjectManager::Update(float TimeDelta)
{
	for (auto& obj : m_objects)
	{
		if (obj.second->IsActive() == false)
			continue;
		obj.second->Update(TimeDelta);
	}
}

std::shared_ptr<GameObject> ObjectManager::FindObject(int id)
{
	return std::shared_ptr<GameObject>();
}

void ObjectManager::Release()
{
	for (auto& iter : m_objects)
	{
		delete iter.second;
		iter.second = nullptr;
	}
	m_objects.clear();
}
