#pragma once

namespace nb
{
  class Scene {
  public:
    Scene();
    virtual ~Scene();
    virtual void Preload();
    virtual void Update();

    friend class App;
  
  private:
    bool _preloaded = false;
  };

  class App {
  public:
    void CreateWindow(const int w, const int h, const char *str);

    void AppUpdate();
    void SetScene(Scene* scene);
    Scene* GetScene();

  private:
    Scene* _currentScene = nullptr;
  };
}