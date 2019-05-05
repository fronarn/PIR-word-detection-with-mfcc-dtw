#ifndef FTEST
#define FTEST

#include "constantes.h"
#include "record.h"

void calculMaxMoyenne(paTestData data, float &max, double &average){
    float val;
    max = 0.0;
    average = 0.0;

    for( int i = 0; i < data.maxFrameIndex; i++ )
    {
        val = data.recordedSamples[i];
        if( val < 0 ) val = -val; /* ABS */
            if( val > max )
                max = val;
        average += val;
    }

    average /= data.maxFrameIndex;
}

void afficheMaxMoyenne(paTestData data){
    float max;
    double average;

    calculMaxMoyenne(data, max, average);
    
    cout << "Amplitude maximale = " << max << endl << flush;
    cout << "Amplitude moyenne = " << average << endl << flush;
}

void boostLeSon(paTestData &data, int boost){
    int totalFrames = data.maxFrameIndex;
    cout << "Boostage du son " << boost << " fois" << endl << flush;
    for (int i = 0; i < totalFrames; i++)
       data.recordedSamples[i] *= boost;
    cout << "Son boostay" << endl << flush;
}

#endif