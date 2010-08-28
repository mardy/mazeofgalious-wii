#include "wii_utils.h"
#include <string.h>
#include <wiiuse/wpad.h>

extern SDLKey UP_KEY,DOWN_KEY,LEFT_KEY,RIGHT_KEY;
#ifdef GAMEPAD_ONLY
extern SDLKey JUMP_KEY;
extern SDLKey CHANGE_WEAPON_L_KEY;
extern SDLKey CHANGE_WEAPON_R_KEY;
#endif
extern SDLKey SWORD_KEY,WEAPON_KEY,ITEM_KEY,PAUSE_KEY;

Uint8 wpad_keyboard_mapping[SDLK_LAST];

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
    
    // Wiimote control scheme has changed in release 0.63.3: Use up+2 for sub-weapon, and 1 for jumping
    wpad_keyboard_mapping[SWORD_KEY] = (((held & WPAD_BUTTON_2) && !(held & WPAD_BUTTON_RIGHT))
                                              || (held & WPAD_CLASSIC_BUTTON_A)) ? 1 : 0;    
    wpad_keyboard_mapping[WEAPON_KEY] = (((held & WPAD_BUTTON_2) && (held & WPAD_BUTTON_RIGHT))
                                              || (held & WPAD_CLASSIC_BUTTON_Y)) ? 1 : 0;
    wpad_keyboard_mapping[LEFT_KEY] =   (held & (WPAD_BUTTON_UP      
                                              |  WPAD_CLASSIC_BUTTON_LEFT    )) ? 1 : 0;
    wpad_keyboard_mapping[RIGHT_KEY] =  (held & (WPAD_BUTTON_DOWN    
                                              |  WPAD_CLASSIC_BUTTON_RIGHT   )) ? 1 : 0;
    wpad_keyboard_mapping[DOWN_KEY] =   (held & (WPAD_BUTTON_LEFT    
                                              |  WPAD_CLASSIC_BUTTON_DOWN    )) ? 1 : 0;
    wpad_keyboard_mapping[UP_KEY] =     (held & (WPAD_BUTTON_RIGHT   
                                              |  WPAD_CLASSIC_BUTTON_UP      )) ? 1 : 0;       
    wpad_keyboard_mapping[ITEM_KEY] =   (held & (WPAD_BUTTON_PLUS    
                                              |  WPAD_CLASSIC_BUTTON_PLUS    )) ? 1 : 0;    
    wpad_keyboard_mapping[PAUSE_KEY] =  (held & (WPAD_BUTTON_MINUS   
                                              |  WPAD_CLASSIC_BUTTON_MINUS   )) ? 1 : 0;
    wpad_keyboard_mapping[JUMP_KEY] =   (held & (WPAD_BUTTON_1
                                              |  WPAD_CLASSIC_BUTTON_B       )) ? 1 : 0;
    // ESC - options menu
    wpad_keyboard_mapping[SDLK_ESCAPE] =(held & (WPAD_BUTTON_HOME
                                              |  WPAD_CLASSIC_BUTTON_HOME    )) ? 1 : 0;                                        
    wpad_keyboard_mapping[CHANGE_WEAPON_L_KEY] = (held & WPAD_CLASSIC_BUTTON_FULL_L) ? 1 : 0;
    wpad_keyboard_mapping[CHANGE_WEAPON_R_KEY] = (held & WPAD_CLASSIC_BUTTON_FULL_R) ? 1 : 0;          
    return wpad_keyboard_mapping;
}
