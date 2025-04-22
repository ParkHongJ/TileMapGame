#pragma once
#include "Singleton.h"
#include "config.h"

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, Scene*> mapScenes;
	map<string, Scene*> mapLoadingScenes;
	ComPtr<ID2D1RenderTarget> renderTarget;
public:
	static Scene* currentScene;
	static Scene* loadingScene;
	static Scene* nextScene;

	void Init(ID2D1RenderTarget* renderTarget);
	void Release();
	void Update(float TimeDelta);
	void LateUpdate(float TimeDelta);
	void Render(ID2D1RenderTarget* renderTarget);

	HRESULT ChangeScene(string akey);
	//HRESULT ChangeScene(string key, string loadingKey);

	Scene* AddScene(string key, Scene* scene);
	Scene* AddLoadingScene(string key, Scene* scene);
};

