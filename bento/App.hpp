#pragma once

#include "Scene.hpp"

namespace ppx
{
  class App {
  public:
    static App& Get();

    void AppUpdate();
    void SetScene(Scene* scene);
    Scene* GetScene();

  private:
    Scene* _ptr_scene_current = nullptr;
    Scene* _ptr_scene_next = nullptr;

    App() = default;
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void _process_update();
  };
}