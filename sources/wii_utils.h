#ifndef WII_UTILS_H_
#define WII_UTILS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifdef _WII
#define ROOTDIR_PREFIX "sd:/apps/mazeofgalious-wii/"
#else
#define ROOTDIR_PREFIX ""
#endif

void insert_rootdir_prefix(const char *path_in, char *path_out);
SDL_Surface * SDLCALL RootDirPrefix_IMG_Load(const char *file);
Uint8 *WPAD_SDL_GetKeyState(int *numkeys);

#endif /*WII_UTILS_H_*/
