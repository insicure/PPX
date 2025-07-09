#include "ppx/CircularCounter.hpp"
#include "ppx/Scene.hpp"
#include "ppx/Texture.hpp"
#include "nds/arm9/console.h"

using namespace ppx;

class SillyImgTester : public Scene {
public:
  PrintConsole console;
  CircularCounter selector;
  Texture *tex1 = nullptr;
  Texture *tex2 = nullptr;

  SillyImgTester();
  ~SillyImgTester() override;
  void Preload() override;
  void Update() override;

  void LoadTexture();
};