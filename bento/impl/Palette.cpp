#include "../Palette.hpp"
#include "../ErrorEnum.hpp"
#include "../SillyImage.hpp"
#include "../Tracelog.hpp"
#include "nds/arm9/sassert.h"
#include "nds/arm9/videoGL.h"
#include <cstdio>
#include <vector>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0

namespace ppx
{
  std::vector<Palette> PaletteList;
  
  bool Palette::isValid()
  {
    return (pid > 0) && (texid > 0);
  }

  int Palette::Get(Palette *palette, const int paletteId)
  {
    // check if palette already loaded
    for (Palette &i : PaletteList)
      if (i.pid == paletteId)
      {
        palette = &i;
        return Error_OK;
      }

    // palette not loaded yet
    int error = Error_OK;
    SillyImage palette_img;
    Palette pal;

    while(true)
    {
      // TODO: increase string buffer?
      char path[30];

      // TODO: dont hardcode palette directory
     
      sprintf(path, "%s%u_pal.bin", "nitro:/palette/", paletteId);

      error = palette_img.Load(path);
      if (error != Error_OK)
      {
        TraceLog("Palette: load failed pid:%u", paletteId);
        break;
      }

      // generate palette texture
      {
        int res = glGenTextures(1, &pal.texid);
        sassert(res == 1, "Palette: glGenTextures failed");
        if (res != 1)
        {
          error = Error_Palette_GenTexturesFailed;
          break;
        }
      }

      // bind palette texture
      {
        int res = glBindTexture(IGNORED, pal.texid);
        sassert(res == 1, "Palette: glBindTexture failed");
        if (res != 1)
        {
          error = Error_Palette_BindTexturesFailed;
          break;
        }
      }

      // generate ColorTableEXT
      {
        int res = glColorTableEXT(IGNORED, IGNORED, palette_img.width, IGNORED, IGNORED, palette_img.data);
        sassert(res == 1, "Palette: glColorTableEXT failed");
        if (res != 1)
        {
          error = Error_Palette_GenColorTableFailed;
          break;
        }
      }

      TraceLog("Palette: loaded id:%u pid:%u", pal.pid, pal.texid);

      pal.pid = palette_img.paletteId;
      PaletteList.push_back(pal);

      break;
    }

    palette_img.Unload();
    if (error != Error_OK) palette = nullptr;
    else palette = &PaletteList.back();
    return error;
  }
}