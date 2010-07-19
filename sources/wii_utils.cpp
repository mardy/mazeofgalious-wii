#include "wii_utils.h"
#include <string.h>
#include <wiiuse/wpad.h>

extern SDLKey UP_KEY,DOWN_KEY,LEFT_KEY,RIGHT_KEY;
extern SDLKey SWORD_KEY,WEAPON_KEY,ITEM_KEY,PAUSE_KEY;

Uint8 wpad_keyboard_mapping[SDLK_LAST];

bool changing_skin_or_sounds = false;

void insert_rootdir_prefix(const char *path_in, char *path_out) 
{
    // 100 below is a magic number - The platform-dependent ROOTDIR_PREFIX
    // is never expected to exceed that many characters in length
    strncpy(path_out, ROOTDIR_PREFIX, 100); 
    strncat(path_out, path_in, strlen(path_in)); 
}

SDL_Surface * SDLCALL RootDirPrefix_IMG_Load(const char *file)
{
    char absFile[256];
    insert_rootdir_prefix(file, absFile);
    return IMG_Load(absFile);
}

Uint8 *WPAD_SDL_GetKeyState(int *numkeys)
{
    WPAD_ScanPads();
    u32 held = WPAD_ButtonsHeld(0);
    
    wpad_keyboard_mapping[SWORD_KEY] = ((held & WIIMOTE_BUTTON_TWO)) ? 1 : 0;
    wpad_keyboard_mapping[WEAPON_KEY] = ((held & WIIMOTE_BUTTON_ONE)) ? 1 : 0;
    // Wiimote is placed sideways to play - therefore, the positions have to be changed
    wpad_keyboard_mapping[LEFT_KEY] = (held & WIIMOTE_BUTTON_UP) ? 1 : 0;
    wpad_keyboard_mapping[RIGHT_KEY] = (held & WIIMOTE_BUTTON_DOWN) ? 1 : 0;
    wpad_keyboard_mapping[DOWN_KEY] = (held & WIIMOTE_BUTTON_LEFT) ? 1 : 0;
    wpad_keyboard_mapping[UP_KEY] = (held & WIIMOTE_BUTTON_RIGHT) ? 1 : 0;       
    wpad_keyboard_mapping[ITEM_KEY] = (held & WIIMOTE_BUTTON_PLUS) ? 1 : 0;    
    wpad_keyboard_mapping[PAUSE_KEY] = (held & WIIMOTE_BUTTON_MINUS) ? 1 : 0;
    // ESC - options menu
    wpad_keyboard_mapping[SDLK_ESCAPE] = (held & WIIMOTE_BUTTON_HOME) ? 1 : 0;
    // change graphic set
    // TODO MUDAR A LÓGICA PARA QUE USE UMA VARIÁVEL DE CONTROLE QUE NÃO 
    // PERMITA QUE UM SEGUNDO SDL_Event SEJA COLOCADO NA FILA
    // ( bool changingSkinOrSounds (global) ) 
    if (held & WIIMOTE_BUTTON_A) {
        if (!changing_skin_or_sounds) {
            SDL_Event F10;
            F10.type = SDL_KEYDOWN;
            F10.key.keysym.sym = SDLK_F10;
            SDL_PushEvent(&F10);
            changing_skin_or_sounds = true;
        }
    }
    // change sound set
    if (held & WIIMOTE_BUTTON_B) {
        if (!changing_skin_or_sounds) {
            SDL_Event F11;
            F11.type = SDL_KEYDOWN;
            F11.key.keysym.sym = SDLK_F11;
            SDL_PushEvent(&F11);
            changing_skin_or_sounds = true;
        }
    }
    
    return wpad_keyboard_mapping;
}
   