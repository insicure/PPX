# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2023

NAME		:= ppx-example

GAME_TITLE	:= PPX Example
GAME_SUBTITLE	:= sillysagiri
GAME_AUTHOR	:= nasirebus corp
NITROFSDIR	:= nitrofs
# GAME_ICON := logo.png

DEFINES		:= -DARM9

SOURCEDIRS	:= src \
							 ../src \
							 ../external/FastLZ/fastlz.c \
							 ../external/murmurhash/murmurhash.c

INCLUDEDIRS	:= src \
							 ../src \
							 ../external/FastLZ \
							 ../external/murmurhash/

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
