#include "pch.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Character.h"
#include <queue>

ObjectManager::ObjectManager()
	: nextId(0), player(nullptr)
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
		MessageBox(g_hWnd, TEXT("이미 해당 인덱스 오브젝트가 존재합니다."), TEXT("경고"), MB_OK);
		return;
	}

	obj->SetObjectRenderId(renderId);
	obj->Init(); // Init(pos) 이런 식으로 할 때는 주의..
	
	objects.emplace(++nextId, obj);

	if (renderId == RENDERORDER::RENDER_PLAYER)
	{
		player = (Character*)obj;
	}
}

void ObjectManager::AddObject(unsigned int id, RENDERORDER renderId, GameObject* obj)
{
	if (nullptr != FindObject(id))
	{
		MessageBox(g_hWnd, TEXT("이미 해당 인덱스 오브젝트가 존재합니다."), TEXT("경고"), MB_OK);
		return;
	}

	obj->SetObjectRenderId(renderId);
	obj->Init(); // Init(pos) 이런 식으로 할 때는 주의..
	
	objects.emplace(id, obj);

	if (renderId == RENDERORDER::RENDER_PLAYER)
	{
		player = (Character*)obj;
	}
}

void ObjectManager::ReplaceObject(unsigned int id, GameObject* obj)
{
	// 플레이어는 이 함수로 대체하지 말자.

	if (false == RemoveObject(id))
	{
		MessageBox(g_hWnd, TEXT("해당 인덱스 오브젝트가 존재하지 않습니다."), TEXT("경고"), MB_OK);
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

GameObject* ObjectManager::FindAbleInteractObject(GameObject* obj)
{
	// Don't use this function

	FPOINT pos = obj->GetPos();
	
	priority_queue<pair<float, GameObject*>> interactObjects;

	for (auto& obj : objects)
	{
		if (false == obj.second->IsActive() || false == obj.second->IsHidden())
		{
			continue;
		}

		FPOINT temp = pos - obj.second->GetPos();
		float dis = sqrtf((temp.x * temp.x) + (temp.y * temp.y));
		interactObjects.push({ dis, obj.second });
	}

	if (interactObjects.empty())
	{
		return nullptr;
	}

	return interactObjects.top().second;
}


void ObjectManager::Render(ID2D1HwndRenderTarget* renderTarget)
{
	for (auto& iter : renderObjects)
	{
		for (auto& iter2 : iter)
		{
			iter2->Render(renderTarget);
		}

		iter.clear();
	}
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
				renderObjects[obj->second->GetObjectRenderId()].emplace_back(obj->second);
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

	//ReleaseInstance();
}

void ObjectManager::ClearObjects()
{
	for (auto& iter : objects)
	{
		if (iter.second)
		{
			iter.second->Release();
			delete iter.second;
			iter.second = nullptr;
		}
	}

	objects.clear();
}

ObjectManager::~ObjectManager()
{

}
