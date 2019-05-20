#include "detection.h"

void UI_begin(bool &Test){
    char rep;

    cout << "=== Programme d'enregistrement et detection de mots ===" << endl << endl << flush;
    
    cout << "Ceci est-il un test ? [Y/n]" << endl << flush;
    cin >> rep;

    if ( toupper(rep) == 'Y' )
        Test = true;
    else 
        Test = false;
}

int UI_askEnreg(){
    char rep;    

    cout << endl << "Voulez vous enregistrer un mot ? [Y/n]" << endl << flush;
    cin >> rep;

    if ( toupper(rep) == 'Y' )
        return true;
    else 
        return false;
}

void UI_enreg(string &nomFichier, int &nbFois, int &numSecondes){

    cout << endl << "Quel mot enregistrer ? (pas de caractayre spayciaux ni d'espace)" << endl << flush;
    cin >> nomFichier;

    cout << endl << "Combien de fois ?" << endl << flush;
    cin >> nbFois;

    cout << endl << "Combien de secondes doit durer chaque enregistrement ?" << endl << flush;
    cin >> numSecondes;
}

int main(){
    // Permet de mettre le bon répertoire, à voir comment on l'implémente à la fin
    bool Test;
    UI_begin(Test);

    // Enregistre les mots
    while (UI_askEnreg()){
        string nomFichier;
        int    nbFois, 
               numSecondes;
        UI_enreg(nomFichier, nbFois, numSecondes);
        enregistrerMot(Test, nomFichier, nbFois, numSecondes);
    }

    paTestData dataPlay;
    dataPlay.maxFrameIndex = 2 * SAMPLE_RATE;
    dataPlay.frameIndex = 0;
    dataPlay.recordedSamples = new float[dataPlay.maxFrameIndex * NUM_CHANNELS] ; 

    getData("estcequevousallezbien.txt", dataPlay, Test);

    // Faire la requête oM2M ici
    // while (true){
        // attente_oM2M();
        
        playData(dataPlay);
        string mot = devineMot(Test, 4);

        if ( mot.empty() ) {
            // faire un truc
            cout << 1;
        } else if ( mot == "oui" || mot == "cava" || mot == "jevaisbien" || mot == "d'autres mots où il confirme") {
            // faire autre chose
            cout << 2;
        } else {
            // potentiellement faire pareil que quand c'est vide ou autre chose
            cout << 3;
        }
    // }
}