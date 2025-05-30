#include "../App.hpp"
#include <cassert>

namespace ppx {

  App& App::Get() {
    static App instance;
    return instance;
  }

  void App::AppUpdate()
  {
    _process_update();

    if (_ptr_scene_current)
      _ptr_scene_current->Update();
  }
  
  void App::SetScene(Scene* scene)
  {
    assert(scene != nullptr);
    _ptr_scene_next = scene;
  }

  Scene* App::GetScene()
  {
    return _ptr_scene_current;
  }

  void App::_process_update()
  {
    if (!_ptr_scene_next) return;

    if (_ptr_scene_current) {
      delete _ptr_scene_current;
      _ptr_scene_current = nullptr;
    }

    _ptr_scene_current = _ptr_scene_next;
    _ptr_scene_next = nullptr;

    _ptr_scene_current->Preload();

    // handle recursive SetScene() during preload
    if (_ptr_scene_next) _process_update();
  }
  
}