#include "BMFontTest.hpp"
#include "ppx/Color.hpp"
#include "ppx/Drawing.hpp"
#include "ppx/Memory.hpp"
#include "ppx/Touch.hpp"
#include "nds/system.h"
#include "nds/touch.h"

BMFontTest::BMFontTest()
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

BMFontTest::~BMFontTest()
{
  Unload_BMFont(font);
}

void BMFontTest::Preload()
{
  font = Load_BMFont("nitro:/font/nds12.fnt");
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
  hue += 0.01f; // Adjust speed as needed
  if (hue > 1.0f) hue -= 1.0f;
  
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  
  hsvToRgb(hue, 1.0f, 1.0f, r, g, b);
  return ((uint8_t)(r * 255) << 16) | ((uint8_t)(g * 255) << 8) | (uint8_t)(b * 255) | (uint8_t)(255) << 24;
}

void BMFontTest::Update()
{
  static int max_width = 40;
  static int max_height = 40;
  touchPosition tp;
  if (GetTouch(ppx::TouchPhase_HELD, tp))
  {
    max_width = tp.px;
    max_height = tp.py;
  }

  BeginDrawing();
  ClearBackground(Color::Gray());
  // max_width++;
  if (max_width > 256) max_width = 15;
  
  static float hue = 0.0f;
  Color color(updateRainbowColor(hue));

  font->DrawStringEx("The quick brown fox jumps over the lazy dog!\n\nLörem ïpsum dôlor âmet, cönséctetür ©dipïscïng €lit — æœ ß ñ ¿¡ µ ¶ †‡ § ¢¥ →←↑↓ ↔↕ ⇒⇐⇑⇓ ➔➜➤ ➠➢ ◊◆◇■□▲△▼▽●○◐◑◒◓◔◕ ▣▤▥▦▧▨▩ ☀☁☂☃★☆✦✧♠♣♥♦ ♂♀ ✓✔✘✖ ∆∇∞∩∪∈∉∅ ⊂⊃⊆⊇ ⊕⊗⊥∴∵ あいうえお かきくけこ さしすせそ なにぬねの はひふへほ まみむめも やゆよ らりるれろ わをん アイウエオ カキクケコ サシスセソ ナニヌネノ ハヒフヘホ マミムメモ ヤユヨ ラリルレロ ワヲン", {5, 10}, max_width, max_height, color);
  
  SetColor(Color::RayWhite());
  DrawLine({max_width+5, 10}, {max_width+5, 150}, 2);
  DrawLine({5, max_height+10}, {150, max_height+10}, 2);
  EndDrawing();
}
