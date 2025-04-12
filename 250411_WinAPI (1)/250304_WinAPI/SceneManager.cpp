#include "SceneManager.h"
#include "Scene.h"

Scene* SceneManager::currentScene = nullptr;
Scene* SceneManager::loadingScene = nullptr;
Scene* SceneManager::nextScene = nullptr;

DWORD CALLBACK LoadingThread(LPVOID pvParam)
{
	if (SUCCEEDED(SceneManager::nextScene->Init()))
	{
		SceneManager::currentScene = SceneManager::nextScene;
		SceneManager::loadingScene->Release();
		SceneManager::loadingScene = nullptr;
		SceneManager::nextScene = nullptr;
	}

	return 0;
}

void SceneManager::Init()
{



}

// 충돌

void SceneManager::Release()
{
	map<string, Scene*>::iterator iter;
	for (iter = mapScenes.begin(); iter != mapScenes.end(); iter++)
	{
		if (iter->second)
		{
			iter->second->Release();
			delete iter->second;
			iter->second = nullptr;
		}
	}
	mapScenes.clear();
	ReleaseInstance();
}

void SceneManager::Update(float TimeDelta)
{
	if (currentScene)
	{
		currentScene->Update(TimeDelta);
	}
}

void SceneManager::Render(HDC hdc)
{
	if (currentScene)
	{
		currentScene->Render(hdc);
	}
}

HRESULT SceneManager::ChangeScene(string key)
{
	auto iter = mapScenes.find(key);	// nextScene
	if (iter == mapScenes.end())
	{
		return E_FAIL;
	}

	if (iter->second == currentScene)
	{
		return S_OK;
	}

	if (SUCCEEDED(iter->second->Init()))
	{
		if (currentScene)
		{
			currentScene->Release();
		}
		currentScene = iter->second;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string key, string loadingKey)
{
	auto iter = mapScenes.find(key);	// nextScene
	if (iter == mapScenes.end())
	{
		return E_FAIL;
	}

	if (iter->second == currentScene)
	{
		return S_OK;
	}

	// 로딩 씬 찾기
	map<string, Scene*>::iterator iterLoading;
	iterLoading = mapLoadingScenes.find(loadingKey);
	if (iterLoading == mapLoadingScenes.end())
	{
		return ChangeScene(key);
	}


	if (SUCCEEDED(iterLoading->second->Init()))
	{
		if (currentScene)
		{
			currentScene->Release();
		}
		currentScene = iterLoading->second;
		nextScene = iter->second;
		loadingScene = iterLoading->second;

		// 다음 씬을 초기화할 쓰레드를 생성
		DWORD loadingThreadId;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0,
			LoadingThread, NULL, 0, &loadingThreadId);

		if (hThread != NULL)
		{
			CloseHandle(hThread);
		}

		return S_OK;
	}
	return E_FAIL;
}

Scene* SceneManager::AddScene(string key, Scene* scene)
{
	if (scene == nullptr)
	{
		return nullptr;
	}

	auto iter = mapScenes.find(key);
	if (iter != mapScenes.begin())
	{
		return iter->second;
	}

	mapScenes.insert(make_pair(key, scene));

    return scene;
}

Scene* SceneManager::AddLoadingScene(string key, Scene* scene)
{
	if (scene == nullptr)
	{
		return nullptr;
	}

	auto iter = mapLoadingScenes.find(key);
	if (iter != mapLoadingScenes.end())
	{
		return iter->second;
	}

	mapLoadingScenes.insert(make_pair(key, scene));

	return scene;
}
