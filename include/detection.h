#ifndef DETECT
#define DETECT

#include "writefile.h"
#include "play.h"

void UI_record(bool &Test, string &nomFichier, int &nbFois, int &numSecondes){
    char rep;

    cout << "=== Programme d'enregistrement d'un mot ===" << endl << endl << flush;
    
    cout << "Ceci est-il un test ? [Y/n]" << endl << flush;
    cin >> rep;

    if ( toupper(rep) == 'Y' )
        Test = true;
    else 
        Test = false;

    cout << endl << "Quel mot enregistrer ? (pas de caractayre spayciaux ni d'espace)" << endl << flush;
    cin >> nomFichier;

    cout << endl << "Combien de fois ?" << endl << flush;
    cin >> nbFois;

    cout << endl << "Combien de secondes doit durer chaque enregistrement ?" << endl << flush;
    cin >> numSecondes;
}  

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

paTestData* recordOneWord(int nbFois, int numSecondes){
    Pa_Initialize();

    paTestData* data = new paTestData[nbFois];

    player play( SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff );
    play.setInputParameters( Pa_GetDefaultInputDevice(), 1, PA_SAMPLE_TYPE );

    for (int i = 0; i < nbFois; i++){
        data[i].maxFrameIndex   = numSecondes * SAMPLE_RATE;
        data[i].frameIndex      = 0; 
        data[i].recordedSamples = new float[data[i].maxFrameIndex] ;

        play.openStreamInput( recordCallback, data + i );
        play.startStream();

        cout << i + 1 << "eme enregistrement..." << flush;
        while( Pa_IsStreamActive( play.stream ) == 1 );

        cout << "   Enregistray." << endl << flush;
        play.closeStream(); 
    }    

    cout << "Enregistrements terminays." << endl << flush;
    Pa_Terminate();

    return data;
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

int enregistrerMot(){

    // UI
    bool   Test;
    string nomFichier;
    int    nbFois, 
           numSecondes;

    UI_record(Test, nomFichier, nbFois, numSecondes);
    
    // Enregistrement du son
    paTestData* data = recordOneWord(nbFois, numSecondes);

    // MFTT-DTW
    cout << "Calcul des coefficients..." << endl << flush;

    mfcc_coefficients* coefs = getMFCCcoefs(data, nbFois);
    writeMFCCcoefs(nomFichier + ".coefs", ' ', coefs, nbFois, Test);

    // Clean
    for (int i = 0; i < nbFois; i++)
        delete [] data[i].recordedSamples;

    delete [] data; delete [] coefs;

    return 0;
}

int devineMot(){

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

int enregistrerMot(bool Test, string nomFichier, int nbFois, int numSecondes){
    
    // Enregistrement du son
    paTestData* data = recordOneWord(nbFois, numSecondes);

    // MFTT-DTW
    cout << "Calcul des coefficients..." << endl << flush;

    mfcc_coefficients* coefs = getMFCCcoefs(data, nbFois);
    writeMFCCcoefs(nomFichier + ".coefs", ' ', coefs, nbFois, Test);

    // Clean
    for (int i = 0; i < nbFois; i++)
        delete [] data[i].recordedSamples;

    delete [] data; delete [] coefs;

    return 0;
}

string devineMot(bool test, int numSecondes){
    
    // Enregistrement du son
    paTestData data = recordOneWord(numSecondes);

    // MFTT-DTW
    cout << "Calcul des coefficients..." << flush;
    mfcc_coefficients coefs = getMFCCcoefs(data);
    cout << "   Calculay." << endl << flush;

    // DTW
    cout << "Comparaison des coefficients..." << endl << flush;
    string min = compareCoefsMin(coefs, test);
    if ( min.empty() ){
        // Clean
        delete [] data.recordedSamples;

        cout << "Aucun mot n'a aytay reconnu" << endl << flush;
        return "";
    }
    else {
        // Clean
        delete [] data.recordedSamples;

        cout << "Mot reconnu : " << min << endl << flush;
        return min;
    }
}


#endif