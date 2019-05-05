#include "writefile.h"
#include "play.h"
#include "fonctionsTest.h"

#include "libmfcc-f.h"
#include "fftw3-f.h"

#include <cstdio>
#include <cmath>

int main(){
    char rep;
    string reponse;
    bool Test = false;
    
    cout << "Ceci est-il un test ? [Y/n]" << endl << flush;
    cin >> rep;
    if ( toupper(rep) == 'Y' )
        Test = true;

    cout << endl << "Quel fichier utiliser ?" << endl << flush;
    cin >> reponse;

    mfcc_coefficients coefs = getMFCCcoefs(reponse, Test);
    // writeMFCCcoefs(reponse + ".coefs", ' ', getMFCCcoefs(reponse + ".txt", Test), Test);    

    for (int i = 0; i < NB_COEF_MFCC; i++)
        cout << coefs.coefs[i] << " " << coefs.delta[i] << " " << coefs.delta_delta[i] << endl << flush;

    cout << "Distance : " << DTWdistance(coefs, coefs) << endl << flush;

    return 0;
}