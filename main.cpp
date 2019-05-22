#include "detection.h"
#include "Sensor.h" 
#include "playmp3.h"

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
    
            playmp3(Test, "est_ce_que_vous_allez_bien");

    // Enregistre les mots
    while (UI_askEnreg()){
        string nomFichier;
        int    nbFois, 
               numSecondes;
        UI_enreg(nomFichier, nbFois, numSecondes);
        enregistrerMot(Test, nomFichier, nbFois, numSecondes);
    }

    // paTestData EstCeQueVousAllezBien;
    // EstCeQueVousAllezBien.maxFrameIndex = 2 * SAMPLE_RATE; // l'audio dure 2 sec
    // EstCeQueVousAllezBien.frameIndex = 0;
    // EstCeQueVousAllezBien.recordedSamples = new float[EstCeQueVousAllezBien.maxFrameIndex * NUM_CHANNELS] ;

    // getData("estcequevousallezbien.txt", EstCeQueVousAllezBien, Test);
    // boostLeSon(EstCeQueVousAllezBien, 4);
    
    Sensor tombe("VOICECHECK");
    Sensor mort("APP");
    mort.getContent();
    // mort.setLocation("la salle de bain");
    tombe.getContent();

    // Faire la requête oM2M ici
    while (true){
        // attente_oM2M();
        tombe.waitEvent();
        mort.setLocation(tombe.getLocation());
        mort.setName(tombe.getName());

        if(tombe.getData()) {
            tombe.updateValue("0");
            // playData(EstCeQueVousAllezBien);
            playmp3(Test, "est_ce_que_vous_allez_bien");
            string mot = devineMot(Test, 4);

            if ( mot.empty() ) {
                // faire un truc
                cout << 1;
            } else if ( mot == "oui" || mot == "cava" || mot == "jevaisbien" || mot == "d'autres mots où il confirme") {
                // faire autre chose
                // tombe.updateValue("0");
                cout << 2;
            } else {
                // potentiellement faire pareil que quand c'est vide ou autre chose
                mort.updateValue("1");
                // playData(NousAllonsAppelerUneAssistance);
                playmp3(Test, "nous_allons_appeler_une_assistance");
                cout << 3;
            }
        }
    }
}