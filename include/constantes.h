#ifndef CONSTANTES
#define CONSTANTES

#include <iostream>
using namespace std;
#include "portaudio.h"


/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)

/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (1)

/* Sample format. */
#define PA_SAMPLE_TYPE  paFloat32

#define NUM_CHANNELS    (1)
#define SAMPLE_SILENCE  (0.0f)

// Emplacement
#define MAX_TAILLE_PATH 100

#define DIR_ENREGISTREMENTS "\\..\\enregistrements\\"
#define DIR_ENREGISTREMENTS_TEST "\\PIR\\enregistrements\\"

#define DIR_VALEURS_UTILES "\\..\\valeurs_utiles\\"
#define DIR_VALEURS_UTILES_TEST "\\PIR\\valeurs_utiles\\"

// MFCC
#define NB_COEF_MFCC 13
#define NB_FILTERS_MFCC 48

// DTW
#define INF 0x7FFFFFFF
#define DIST_MAX INF

#endif