#include "core.hpp"

namespace nb {
  Scene::Scene() {}
  Scene::~Scene() {}
  void Scene::Preload() {}
  void Scene::Update() {}

  // --

  void App::CreateWindow(const int w, const int h, const char *str)
  {
    
  }

  void App::AppUpdate()
  {
    if (_currentScene != nullptr)
    {
      while (!_currentScene->_preloaded)
      {
        _currentScene->_preloaded = true;
        _currentScene->Preload();
      }

      _currentScene->Update();
    }
  }
  
  void App::SetScene(Scene* scene)
  {
    if (_currentScene != nullptr)
    {
      delete _currentScene;
      _currentScene = nullptr;
    }

    _currentScene = scene;
  }

  Scene* App::GetScene()
  {
    return _currentScene;
  }
}