#pragma once

namespace ppx
{
  enum Error 
  {
    Error_OK = 0,

    Error_FileData_EmptyFilename,
    Error_FileData_fopenFailed,
    Error_FileData_InvalidLength,
    Error_FileData_AllocateFailed,
    Error_FileData_PartiallyLoaded,

    Error_SillyImage_LengthMetadata,
    Error_SillyImage_InvalidHeader,
    Error_SillyImage_InvalidVersion,
    Error_SillyImage_InvalidFormat,

    Error_Palette_GenTexturesFailed,
    Error_Palette_BindTexturesFailed,
    Error_Palette_GenColorTableFailed,

    Error_Texture_InvalidFormat,
    Error_Texture_GenTexturesFailed,
    Error_Texture_BindTexturesFailed,
    Error_Texture_TexImage2DFailed,
    Error_Texture_AssignPaletteFailed,

    Error_TextureAtlas_fopenFailed,
    Error_TextureAtlas_InvalidHeader,
    Error_TextureAtlas_InvalidVersion,
    Error_TextureAtlas_TrimRotateDisabled,
    Error_TextureAtlas_InvalidStringType,
    Error_TextureAtlas_AllocateFailed,
  };
}