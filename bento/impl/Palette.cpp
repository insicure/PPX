#include "../Palette.hpp"
#include "../SillyImage.hpp"
#include "../Tracelog.hpp"
#include "../Assert.hpp"
#include "nds/arm9/videoGL.h"
#include <cstdio>
#include <vector>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0

namespace ppx
{
  std::vector<Palette> PaletteList(0);
  
  bool Palette::isValid()
  {
    return (pid > 0) && (texid > 0);
  }

  void Palette::Reset()
  {
    for (auto &i : PaletteList)
    {
      int temp_texid = i.texid;
      int res = glDeleteTextures(1, &i.texid);
      if (res != 0) TraceLog("Palette: unloaded id:%u pid:%u", temp_texid, i.pid);
      else TraceLog("Palette: unload failed id:%u pid:%u", temp_texid, i.pid);
    }

    PaletteList.clear();
    PaletteList.reserve(10);
  }

  Palette *Palette::Get(const int paletteid)
  {
    bool res = false;
    res_sassert(res, paletteid != 0, "Palette: pid:0 is invalid!");
    if (!res) return nullptr;
    
    // check if palette already loaded
    for (Palette &i : PaletteList)
      if (i.pid == paletteid)
      {
        TraceLog("Palette: already exist, id:%u pid:%u", i.texid, i.pid);
        return &i;
      }

    // palette not exist
    SillyImage img;
    Palette pal;

    do
    {
      char path[128];
      snprintf(path, sizeof(path), "nitro:/palette/%u.sillypal", paletteid);

      res = img.Load(path);
      if (!res) break;

      pal.pid = paletteid;

      // generate palette texture
      {
        int r = glGenTextures(1, &pal.texid);
        res_sassert(res, r == 1, "Palette: glGenTextures failed, pid:%u", pal.pid);
        if (!res) break;
      }

      // bind palette texture
      {
        int r = glBindTexture(IGNORED, pal.texid);
        res_sassert(res, r == 1, "Palette: glBindTexture failed, id:%u pid:%u", pal.texid, pal.pid);
        if (!res) break;
      }

      // generate ColorTableEXT
      {
        int r = glColorTableEXT(IGNORED, IGNORED, img.width, IGNORED, IGNORED, img.data);
        res_sassert(res, r == 1, "Palette: glColorTableEXT failed, id:%u pid:%u", pal.texid, pal.pid);
        if (!res) break;
      }

    } while(0);

    img.Unload();

    if (!res)
    {
      TraceLog("Palette: load failed, pid:%u", paletteid);
      return nullptr;
    }
    else
    {
      TraceLog("Palette: load success, id:%u pid:%u", pal.texid, pal.pid);
      PaletteList.push_back(pal);
      return &PaletteList.back();
    }
  }
}