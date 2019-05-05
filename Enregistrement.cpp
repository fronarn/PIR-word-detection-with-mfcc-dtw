#include "constantes.h"
#include "record.h"
#include "play.h"
#include "writefile.h"
#include "fonctionsTest.h"


int main(void)
{
    player      play(SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff);
    PaError     err = paNoError;
    paTestData  data;
    char        reponse;
    int         num_secondes;
    bool        Test = false;

    Pa_Initialize();

    play.setInputParameters( Pa_GetDefaultInputDevice(),  1, PA_SAMPLE_TYPE);
    play.setOutputParameters(Pa_GetDefaultOutputDevice(), 1, PA_SAMPLE_TYPE);
    
    cout << "=== Programme d'enregistrement du microphone ===" << endl << endl << flush;
    
    do {
        cout << "Pendant combien de secondes l'enregistrement doit-il durer ?" << endl << flush;
        cin >> num_secondes;

        data.maxFrameIndex = num_secondes * SAMPLE_RATE; // Record for a few seconds. 
        data.frameIndex = 0;
        data.recordedSamples = new float[data.maxFrameIndex * NUM_CHANNELS] ; // From now on, recordedSamples is initialised. 

        play.openStreamInput(recordCallback, &data);
        play.startStream();
        cout << endl << "=== Enregistrement de " << num_secondes << " secondes lancé !! ===" << endl << flush;

        while( ( err = Pa_IsStreamActive( play.stream ) ) == 1 )
        {
            Pa_Sleep(1000);
            cout << "index = " << data.frameIndex << " sur " << data.maxFrameIndex << endl << flush;
        }

        play.closeStream();   

        // Mesure le maximum et la moyenne. 
        afficheMaxMoyenne(data);  
        
        // Ecrit dans un fichier
        cout << endl << "Voulez-vous ecrire dans un fichier ? [Y,n]" << endl << flush;
        cin >> reponse;

        if (toupper(reponse) == 'Y'){
            string rep;
            cout << "Comment voulez-vous appeler votre fichier ?" << endl << flush;
            cin >> rep;
        
            writeFile(rep + ".txt", ' ', data, Test);
        }
        //TEST pour booster le son
        boostLeSon(data, 5);

        /* Playback recorded data.  -------------------------------------------- */
        

        do {
            data.frameIndex = 0;

            cout << endl << "=== Re-ecoute ===" << endl << flush;
            play.openStreamOutput(playCallback, &data);
            play.startStream();
            cout << "Attente de la fin de l'audio." << endl << flush;

            while( ( err = Pa_IsStreamActive( play.stream ) ) == 1 ) 
                Pa_Sleep(100);
            
            play.closeStream();
            cout << "Fini." << endl << flush;
            cout << "Voulez-vous re-ecouter l'audio ? [Y,n]" << endl << flush;
            cin >> reponse;
        } while (toupper(reponse) == 'Y');

        cout << endl << "Voulez-vous faire un nouvel enregistrement ? [Y,n]" << endl << flush;
        cin >> reponse;
    } while (toupper(reponse) == 'Y');
    
    // Ferme l'appli
    Pa_Terminate();
    delete [] data.recordedSamples;
    
    return 0;
}

