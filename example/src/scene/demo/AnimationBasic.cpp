#include "AnimationBasic.hpp"
#include "ppx/CircularCounter.hpp"
#include "ppx/Drawing.hpp"
#include "ppx/Memory.hpp"
#include "ppx/Texture.hpp"
#include "ppx/TextureAtlas.hpp"
#include "ppx/TextureMap.hpp"
#include "ppx/Tracelog.hpp"
#include "nds/arm9/background.h"
#include "nds/arm9/input.h"
#include "nds/arm9/video.h"
#include <cstdio>
#include <vector>

using namespace ppx;

AnimationBasic::AnimationBasic()
{
  videoSetMode(MODE_0_3D);
  videoSetModeSub(MODE_0_2D);

  vramSetPrimaryBanks(
    VRAM_A_TEXTURE,
    VRAM_B_TEXTURE,
    VRAM_C_SUB_BG_0x06200000,
    VRAM_D_MAIN_BG_0x06000000);

  vramSetBankF(VRAM_F_TEX_PALETTE);

  lcdMainOnBottom();
  InitDrawing();

  consoleInit(&console, 1, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
  consoleSelect(&console);

  setBrightness(3, -16);
}

AnimationBasic::~AnimationBasic()
{
  Unload_TextureAtlas(atlas);
}

static std::vector<TextureMap*> anim = std::vector<TextureMap*>();

void AnimationBasic::Preload()
{
  anim.clear();
  anim.reserve(10);
  setBrightness(3, 0);
}


void AnimationBasic::Update()
{
  if (keysUp() & KEY_L)
  {
    if (atlas) return;
    atlas = Load_TextureAtlas("nitro:/atlas/cat.txt");
    anim.push_back(atlas->find("Run1"));
    anim.push_back(atlas->find("Run2"));
    anim.push_back(atlas->find("Run3"));
    anim.push_back(atlas->find("Run4"));
    anim.push_back(atlas->find("Run5"));
    anim.push_back(atlas->find("Run6"));
    anim.push_back(atlas->find("Run7"));
    anim.push_back(atlas->find("Run8"));
    anim.push_back(atlas->find("Run9"));
    anim.push_back(atlas->find("Run10"));

    TraceLog("%u", atlas->find("Run1"));
  }

  if (keysUp() & KEY_R)
  {
    Unload_TextureAtlas(atlas);
    TraceLog("%u", atlas);
    anim.clear();
    anim.reserve(10);
    return;
  }

  
  BeginDrawing();
  ClearBackground(Color::Gray());

  if (atlas)
  {
    static int frame = 0;
    static CircularCounter framecounter(anim.size()-1);

    TextureMap *tex = anim[framecounter.value];

    frame++;
    if (frame % 4 == 0) framecounter.Next();

    if (tex)
    {
      tex->Draw({0, 0});
    }
  }
  EndDrawing();
}

