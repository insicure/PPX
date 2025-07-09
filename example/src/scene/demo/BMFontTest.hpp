#include "ppx/Scene.hpp"
#include "ppx/BMFont.hpp"
#include "nds/arm9/console.h"

using namespace ppx;

class BMFontTest : public Scene {
public:
  PrintConsole console;

  BMFont *font = nullptr;

  BMFontTest();
  ~BMFontTest() override;
  void Preload() override;
  void Update() override;
};