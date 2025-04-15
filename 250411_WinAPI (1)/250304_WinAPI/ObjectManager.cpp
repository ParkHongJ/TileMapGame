#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager()
	: nextId(0)
{

}

void ObjectManager::Init()
{
}

void ObjectManager::AddObject(GameObject* obj)
{	
	auto iter = m_objects.find(nextId + 1);
	if (m_objects.end() != iter)
	{
		MessageBox(g_hWnd, TEXT("�̹� �ش� �ε��� ������Ʈ�� �����մϴ�."), TEXT("���"), MB_OK);
		return;
	}

	obj->Init(); // Init(pos) �̷� ������ �� ���� ����..
	obj->SetObjectId(++nextId);
	m_objects.emplace(nextId, obj);
}

void ObjectManager::AddObject(unsigned int id, GameObject* obj)
{
	if (nullptr != FindObject(id))
	{
		MessageBox(g_hWnd, TEXT("�̹� �ش� �ε��� ������Ʈ�� �����մϴ�."), TEXT("���"), MB_OK);
		return;
	}

	m_objects.emplace(id, obj);

}

void ObjectManager::ReplaceObject(unsigned int id, GameObject* obj)
{
	if (false == RemoveObject(id))
	{
		MessageBox(g_hWnd, TEXT("�ش� �ε��� ������Ʈ�� �������� �ʽ��ϴ�."), TEXT("���"), MB_OK);
	}

	m_objects.emplace(id, obj);
}

bool ObjectManager::RemoveObject(unsigned int id)
{
	auto iter = m_objects.find(id);
	bool isExist = m_objects.end() != iter;
	
	if (isExist)
	{
		(*iter).second->Release();
		delete (*iter).second;
		m_objects.erase(iter);
	}

	return isExist;
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

GameObject* ObjectManager::FindObject(unsigned int id)
{
	auto iter = m_objects.find(id);
	if (m_objects.end() != iter)
	{
		return (*iter).second;
	}

	return nullptr;
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

ObjectManager::~ObjectManager()
{

}
