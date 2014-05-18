cmake_minimum_required(VERSION 2.8)
project(stb)

set(stb_sources
    stb_image.c stb_image.h
	stb_truetype.c stb_truetype.h
)

add_library(stb ${stb_sources})