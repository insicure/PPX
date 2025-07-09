#include "SillyImgTester.hpp"
#include "ppx/Drawing.hpp"
#include "ppx/Memory.hpp"
#include "ppx/Texture.hpp"
#include "nds/arm9/input.h"
#include "nds/arm9/video.h"
#include "nds/input.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>

static const std::array<const char*, 6> tex_name {
  "rgb16", "indexed256", "indexed16", "indexed4", "indexed32a8", "indexed8a32"
};

void SillyImgTester::LoadTexture()
{
  char path[128];
  snprintf(path, sizeof(path), "nitro:/sagiri/%s.sillyimg", tex_name[selector.value]);
  printf("%s\n", tex_name[selector.value]);
  Unload_Texture(tex1);
  tex1 = Load_Texture(path);
}

SillyImgTester::SillyImgTester()
{
  videoSetMode(MODE_0_3D);
  videoSetModeSub(MODE_0_2D);

  vramSetPrimaryBanks(
    VRAM_A_TEXTURE,
    VRAM_B_TEXTURE,
    VRAM_C_SUB_BG_0x06200000,
    VRAM_D_TEXTURE);

  vramSetBankF(VRAM_F_TEX_PALETTE);
  vramSetBankG(VRAM_G_TEX_PALETTE);

  lcdMainOnBottom();
  InitDrawing();

  consoleInit(&console, 1, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
  consoleSelect(&console);

  setBrightness(3, -16);
}

SillyImgTester::~SillyImgTester()
{
  Unload_Texture(tex1);
}

void SillyImgTester::Preload()
{
  selector.max = tex_name.size() - 1;
  LoadTexture();

  setBrightness(3, 0);
}

static void hsvToRgb(float h, float s, float v, float& r, float& g, float& b) {
  int i = static_cast<int>(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch(i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }
}

static uint32_t updateRainbowColor(float& hue) {
  hue += 0.008f; // Adjust speed as needed
  if (hue > 1.0f) hue -= 1.0f;
  
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  
  hsvToRgb(hue, 1.0f, 1.0f, r, g, b);
  return ((uint8_t)(r * 255) << 16) | ((uint8_t)(g * 255) << 8) | (uint8_t)(b * 255) | (uint8_t)(255) << 24;
}

void SillyImgTester::Update()
{
  if (keysUp() & KEY_RIGHT)
  {
    selector.Next();
    LoadTexture();
  }

  if (keysUp() & KEY_LEFT)
  {
    selector.Prev();
    LoadTexture();
  }

  if (keysHeld() & KEY_L)
  {
    LoadTexture();
  }
  
  BeginDrawing();
  static float hue = 0.0f;
  Color c(updateRainbowColor(hue));
  ClearBackground(c);
  tex1->Draw({0,0});
  EndDrawing();
}