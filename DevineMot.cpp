#include "writefile.h"
#include "play.h"

void UI_guess(bool &Test, int &numSecondes){
    char rep;

    cout << "=== Programme pour deviner un mot ===" << endl << endl << flush;
    
    cout << "Ceci est-il un test ? [Y/n]" << endl << flush;
    cin >> rep;

    if ( toupper(rep) == 'Y' )
        Test = true;
    else 
        Test = false;

    cout << endl << "Combien de secondes doit durer l'enregistrement ?" << endl << flush;
    cin >> numSecondes;
}

paTestData recordOneWord(int numSecondes){
    Pa_Initialize();

    player play( SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff );
    play.setInputParameters( Pa_GetDefaultInputDevice(), 1, PA_SAMPLE_TYPE );

    paTestData data;
    data.maxFrameIndex   = numSecondes * SAMPLE_RATE;
    data.frameIndex      = 0; 
    data.recordedSamples = new float[data.maxFrameIndex] ;

    play.openStreamInput( recordCallback, &data );
    play.startStream();

    cout << "Enregistrement..." << flush;
    while( Pa_IsStreamActive( play.stream ) == 1 );

    cout << "   Enregistray." << endl << flush;
    play.closeStream(); 
    
    Pa_Terminate();

    return data;
}

int main(){

    // UI
    bool Test;
    int  numSecondes;

    UI_guess(Test, numSecondes);
    
    // Enregistrement du son
    paTestData data = recordOneWord(numSecondes);

    // MFTT-DTW
    cout << "Calcul des coefficients..." << flush;
    mfcc_coefficients coefs = getMFCCcoefs(data);
    cout << "   Calculay." << endl << flush;

    // DTW
    cout << "Comparaison des coefficients..." << endl << flush;
    string min = compareCoefsMin(coefs, Test);
    if ( min.empty() )
        cout << "Aucun mot n'a aytay reconnu" << endl << flush;
    else
        cout << "Mot reconnu : " << min << endl << flush;

    // Clean
    delete [] data.recordedSamples;

    if (!Test){
        char rep;
        cin >> rep;
    }
    return 0;
}