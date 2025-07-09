#include "ppx/Scene.hpp"
#include "ppx/TextureAtlas.hpp"
#include "nds/arm9/console.h"

class AnimationBasic : public ppx::Scene
{
public:
  PrintConsole console;
  ppx::TextureAtlas *atlas = nullptr;
  
  AnimationBasic();
  ~AnimationBasic() override;
  void Preload() override;
  void Update() override;
};