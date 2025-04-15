#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager()
	: nextId(0)
{

}

void ObjectManager::Init()
{
}

void ObjectManager::AddObject(RENDERORDER renderId, GameObject* obj)
{
	auto iter = objects.find(nextId + 1);
	if (objects.end() != iter)
	{
		MessageBox(g_hWnd, TEXT("�̹� �ش� �ε��� ������Ʈ�� �����մϴ�."), TEXT("���"), MB_OK);
		return;
	}

	obj->Init(); // Init(pos) �̷� ������ �� ���� ����..
	obj->SetObjectRenderId(renderId);
	objects.emplace(nextId, obj);
}

void ObjectManager::AddObject(unsigned int id, RENDERORDER renderId, GameObject* obj)
{
	if (nullptr != FindObject(id))
	{
		MessageBox(g_hWnd, TEXT("�̹� �ش� �ε��� ������Ʈ�� �����մϴ�."), TEXT("���"), MB_OK);
		return;
	}

	obj->Init(); // Init(pos) �̷� ������ �� ���� ����..
	obj->SetObjectRenderId(renderId);
	objects.emplace(id, obj);
}

void ObjectManager::ReplaceObject(unsigned int id, GameObject* obj)
{
	if (false == RemoveObject(id))
	{
		MessageBox(g_hWnd, TEXT("�ش� �ε��� ������Ʈ�� �������� �ʽ��ϴ�."), TEXT("���"), MB_OK);
	}

	objects.emplace(id, obj);
}

bool ObjectManager::RemoveObject(unsigned int id)
{
	auto iter = objects.find(id);
	bool isExist = objects.end() != iter;
	
	if (isExist)
	{
		(*iter).second->Release();
		delete (*iter).second;
		objects.erase(iter);
	}

	return isExist;
}

GameObject* ObjectManager::FindObject(unsigned int id)
{
	auto iter = objects.find(id);
	if (objects.end() != iter)
	{
		return (*iter).second;
	}

	return nullptr;
}


void ObjectManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	for (auto& iter : renders)
	{
		for (auto& iter2 : iter)
		{
			iter2->Render(renderTarget);
		}
	}

	renders->clear();
}

void ObjectManager::Update(float TimeDelta)
{
	for (auto& obj : objects)
	{
		if (obj.second->IsActive() == false)
		{
			continue;
		}

		obj.second->Update(TimeDelta);
	}
}

void ObjectManager::LateUpdate(float TimeDelta)
{
	for (auto obj = objects.begin(); obj != objects.end();)
	{
		if (true == obj->second->IsDestroyed())
		{
			obj = objects.erase(obj);
		}

		else
		{
			if (true == obj->second->IsActive())
			{
				obj->second->LateUpdate(TimeDelta);
			}
			
			if (true == obj->second->IsActive() && false == obj->second->IsHidden())
			{
				renders[obj->second->GetObjectRenderId()].emplace_back(obj->second);
			}

			++obj;
		}
	}
}



void ObjectManager::Release()
{
	for (auto& iter : objects)
	{
		iter.second->Release();
		delete iter.second;
		iter.second = nullptr;
	}

	objects.clear();
}

ObjectManager::~ObjectManager()
{

}
