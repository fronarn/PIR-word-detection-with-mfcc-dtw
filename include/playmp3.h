#ifndef PLAYMP3
#define PLAYMP3

#include <windows.h>
#include <string>
#include <direct.h>
#include "constantes.h"

void playmp3(bool Test, string nom){
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));
    
    string a = "open \"" + (string)path;// + (string)name + ".mp3\" type mpegvideo alias mp3";

    if (Test)
        a += DIR_ENREGISTREMENTS_TEST + nom;
    else
        a += DIR_ENREGISTREMENTS + nom;
    
    a += ".mp3\" type mpegvideo alias mp3";
    mciSendString( a.c_str(),                         NULL, 0, NULL);
    mciSendString( "play mp3 wait", NULL, 0, NULL);
    mciSendString( "close mp3",    NULL, 0, NULL);
}

#endif