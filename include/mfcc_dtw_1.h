#ifndef MFCC_DTW
#define MFCC_DTW

#include "fonctionsTest.h"

#include "libmfcc-f.h"
#include "fftw3-f.h"

#include <cmath>

class mfcc_coefficients{
    public:
        float coefs[NB_COEF_MFCC];
        float delta[NB_COEF_MFCC];
        float delta_delta[NB_COEF_MFCC];

        void empty(){
            for (int i = 0; i < NB_COEF_MFCC; i++){
                coefs[i] = NAN;
                delta[i] = NAN;
                delta[i] = NAN;
            }
        }

        bool isEmpty(){
            if ( isnan(coefs[0]) )
                return true;
            else
                return false;
        }
};


mfcc_coefficients getMFCCcoefs(const paTestData data){
    double average;
    float max;
    calculMaxMoyenne(data, max, average);

    float *dataFFT = new float[data.maxFrameIndex] ;

    for (int i = 1000; i < data.maxFrameIndex - 1000; i++)
        if ( abs(data.recordedSamples[i]) > 4 * average)
            for (int j = i - 1000; j < i + 1000; j++)
                dataFFT[j] = data.recordedSamples[j];

    fftwf_complex *out = (fftwf_complex*)fftwf_malloc ( sizeof ( fftwf_complex ) * (( data.maxFrameIndex / 2 ) + 1) );
    fftwf_plan plan_forward = fftwf_plan_dft_r2c_1d ( data.maxFrameIndex, dataFFT, out, FFTW_ESTIMATE );
    fftwf_execute ( plan_forward );
    
    delete [] dataFFT; 
    dataFFT = new float[ data.maxFrameIndex / 2 ] ;

    for (int i = 0; i < data.maxFrameIndex / 2; i++)
        dataFFT[i] = sqrtf( powf(out[i][0], 2) + powf(out[i][1], 2)); 

    fftwf_free( out );

    mfcc_coefficients coefs;
    coefs.delta[0] = 0;
    coefs.delta_delta[0] = 0;

    for (int i = 0; i < NB_COEF_MFCC; i++){
        coefs.coefs[i] = GetCoefficient(dataFFT, SAMPLE_RATE, NB_COEF_MFCC, data.maxFrameIndex / 2, i);
        if ( i ){
            coefs.delta[i] = coefs.coefs[i] - coefs.coefs[i-1];
            coefs.delta_delta[i] = coefs.delta[i] - coefs.delta[i-1];
        }
        if ( (coefs.coefs[i]) || (coefs.coefs[i] > 100) || (coefs.coefs[i] > 100) ){
            coefs.empty();
            break;
        }
    }

    delete [] dataFFT;

    return coefs;
}

mfcc_coefficients* getMFCCcoefs(const paTestData* data, int nbFois){
    mfcc_coefficients* coefs = new mfcc_coefficients[nbFois];

    for (int i = 0; i < nbFois; i++)
        coefs[i] = getMFCCcoefs(data[i]);
    
    return coefs;
}

float DTWdistance(const float* s, const float* t){
    float DTW[NB_COEF_MFCC][NB_COEF_MFCC];

    for (int i = 0; i < NB_COEF_MFCC; i++)
        for (int j = 0; j < NB_COEF_MFCC; j++)
                DTW[i][j] = INF;
    DTW[0][0] = 0;

    for (int i = 0; i < NB_COEF_MFCC; i++)
        for (int j = 0; j < NB_COEF_MFCC; j++)
            if ( i && j)
                DTW[i][j] = abs( s[i] - t[j] ) + min(DTW[i-1][j  ], 
                                                 min(DTW[i  ][j-1], 
                                                     DTW[i-1][j-1]));
            else if ( i )
                DTW[i][j] = abs( s[i] - t[j] ) +     DTW[i-1][j  ];
            else
                DTW[i][j] = abs( s[i] - t[j] ) +     DTW[i  ][j-1];
            
    return DTW[NB_COEF_MFCC-1][NB_COEF_MFCC-1];
}

float DTWdistance(const mfcc_coefficients c1, const mfcc_coefficients c2){
    return  DTWdistance(c1.coefs,       c2.coefs) + 
            DTWdistance(c1.delta,       c2.delta) + 
            DTWdistance(c1.delta_delta, c2.delta_delta);
}

#endif