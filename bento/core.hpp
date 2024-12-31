#pragma once

namespace nb
{
  class Scene {
  public:
    virtual ~Scene() = 0;
    virtual void Preload() = 0;
    virtual void Update() = 0;

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