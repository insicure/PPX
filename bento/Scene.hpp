#pragma once

namespace ppx
{
  class Scene {
  public:
    virtual ~Scene() {}
    virtual void Preload() = 0;
    virtual void Update() = 0;

    friend class App;

  private:
    bool _preloaded = false;
  };
}