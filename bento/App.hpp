#pragma once

#include "Scene.hpp"

namespace ppx
{
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