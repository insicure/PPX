#pragma once

#include "ppx/Scene.hpp"
#include "ppx/TextureAtlas.hpp"
#include "nds/arm9/console.h"

using namespace ppx;

class MovieclipTest : public Scene
{
public:
  PrintConsole console;
  TextureAtlas *atlas = nullptr;

  MovieclipTest();
  ~MovieclipTest() override;
  void Preload() override;
  void Update() override;
};