#ifndef WF
#define WF

#include <string>
#include <fstream>
#include <direct.h>
#include <dirent.h>

#include "constantes.h"
#include "record.h"
#include "mfcc_dtw.h"

void writeFile(string nom, char delimiteur, paTestData data, bool Test){    
    ofstream fichier;
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        fichier.open(path + (DIR_ENREGISTREMENTS_TEST + nom));
    else
        fichier.open(path + (DIR_ENREGISTREMENTS + nom));
        
    cout << "Ecriture du fichier" << endl << flush;

    for (int i = 0; i < data.maxFrameIndex; i++)
        fichier << data.recordedSamples[i] << delimiteur;

    fichier.close();
    cout << "Fichier ecrit dans le dossier enregistrements" << endl << flush;
}

void getData(string nom, paTestData &data, bool Test){
    ifstream fichier;
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));
    
    if (Test)
        fichier.open(path + (DIR_VALEURS_UTILES_TEST + nom));
    else
        fichier.open(path + (DIR_VALEURS_UTILES + nom)); 
    
    cout << "Copie du fichier : " << path + (DIR_VALEURS_UTILES_TEST + nom) << endl << flush;
    

    for (int i = 0; i < data.maxFrameIndex; i++)
        fichier >> data.recordedSamples[i];

    fichier.close();
    cout << "Fichier copiay" << endl << flush;
}

void writeMFCCcoefs(string nom, char delimiteur, mfcc_coefficients coefs, bool Test){
    // if (coefs.isEmpty())
    //     return;

    ofstream fichier;
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        fichier.open(path + (DIR_ENREGISTREMENTS_TEST + nom));
    else
        fichier.open(path + (DIR_ENREGISTREMENTS + nom));
        
    cout << "Ecriture du fichier" << endl << flush;
    fichier << '1' << endl;

    for (int i = 0; i < NB_COEF_MFCC; i++)
        fichier << coefs.coefs[i]       << delimiteur 
                << coefs.delta[i]       << delimiteur 
                << coefs.delta_delta[i] << delimiteur << endl;

    fichier.close();
    cout << "Fichier ecrit dans le dossier enregistrements" << endl << flush;
}

void writeMFCCcoefs(string nom, char delimiteur, mfcc_coefficients* coefs, int nbFois, bool Test){
    // for (int i = 0; i < nbFois; i++)
    //     if (coefs[i].isEmpty())
    //         nbFois--;
    // if ( ! nbFois ){
    //     cout << "Tous les enregistrements sont corrompus" << endl << flush;
    //     return;
    // }

    ifstream file;
    ofstream fichier;
    int lastNbFois = 0;
    string content;
    bool fail;
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        file.open(path + (DIR_ENREGISTREMENTS_TEST + nom));
    else
        file.open(path + (DIR_ENREGISTREMENTS + nom));
    
    
    if ( ! (fail = file.fail()) ){
        file >> lastNbFois;
        file.get();
        content.assign( (std::istreambuf_iterator<char>(file) ),
                        (std::istreambuf_iterator<char>()     ) );
        cout << "Fichier dayja existant" << endl << flush;
    }
    
    if (Test)
        fichier.open(path + (DIR_ENREGISTREMENTS_TEST + nom));
    else
        fichier.open(path + (DIR_ENREGISTREMENTS + nom));
    
    cout << "Ecriture de " << nbFois << " enregistrements" << endl << flush;
    fichier << nbFois + lastNbFois << endl;

    if ( ! fail )
        fichier << content;

    for (int j = 0; j < nbFois; j++)
        for (int i = 0; i < NB_COEF_MFCC; i++)
            // if ( ! coefs[i].isEmpty() )
                fichier << coefs[j].coefs[i]       << delimiteur 
                        << coefs[j].delta[i]       << delimiteur 
                        << coefs[j].delta_delta[i] << delimiteur << endl;

    fichier.close();
    cout << "Fichier ecrit dans le dossier enregistrements" << endl << flush;
}

mfcc_coefficients getMFCCcoefs(string nomFichier, bool Test){
    mfcc_coefficients coefs;
    if (nomFichier.find(".coefs") != string::npos){
        ifstream fichier;
        char path[MAX_TAILLE_PATH];
        _getcwd(path, sizeof(path));

        if (Test)
            fichier.open(path + (DIR_ENREGISTREMENTS_TEST + nomFichier));
        else
            fichier.open(path + (DIR_ENREGISTREMENTS + nomFichier)); 
        
        cout << "Copie du fichier : " << path + (DIR_ENREGISTREMENTS_TEST + nomFichier) << endl << flush;
        int tmp;
        fichier >> tmp;
    
        for (int i = 0; i < NB_COEF_MFCC; i++)
            fichier >> coefs.coefs[i] >> coefs.delta[i] >> coefs.delta_delta[i];

        fichier.close();
        cout << "Fichier copiay" << endl << flush;

    } else {
        paTestData data;
        data.maxFrameIndex = NUM_SECONDS * SAMPLE_RATE;
        data.recordedSamples = new float[data.maxFrameIndex * NUM_CHANNELS] ;

        getData(nomFichier, data, Test);
        coefs = getMFCCcoefs(data);

        delete [] data.recordedSamples;
    }
    
    return coefs;
}

mfcc_coefficients* getMFCCcoefs(string nomFichier, int &nbCoefs, bool Test){
    ifstream fichier;
    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        fichier.open(path + (DIR_ENREGISTREMENTS_TEST + nomFichier));
    else
        fichier.open(path + (DIR_ENREGISTREMENTS + nomFichier)); 
    
    cout << "Copie du fichier : " << path + (DIR_ENREGISTREMENTS_TEST + nomFichier) << endl << flush;
    fichier >> nbCoefs;
    mfcc_coefficients* coefs = new mfcc_coefficients[nbCoefs];

    for (int j = 0; j < nbCoefs; j++)
        for (int i = 0; i < NB_COEF_MFCC; i++)
            fichier >> coefs[j].coefs[i] >> coefs[j].delta[i] >> coefs[j].delta_delta[i];

    fichier.close();
    cout << "Fichier copiay" << endl << flush;
    
    return coefs;
}

class mot{
    protected:
        int nbEchant;
        string nom;
        mfcc_coefficients* echant;
    public:
        mot(string nomFichier, string dir){
            size_t pos;
            if ( (pos = nomFichier.find(".coefs")) != string::npos)
                nom = nomFichier.substr(0, pos);
            else
                nom = nomFichier;
                        
            ifstream fichier;
            fichier.open(dir + nomFichier);

            fichier >> nbEchant;
            echant = new mfcc_coefficients[nbEchant];

            for (int i = 0; i < nbEchant; i++)
                for (int j = 0; j < NB_COEF_MFCC; j++)
                    fichier >> echant[i].coefs[j] >> echant[i].delta[j] >> echant[i].delta_delta[j]; 
            fichier.close();
        }

        float minDTW(mfcc_coefficients coefs){
            float min_dist = INF;
            for (int i = 0; i < nbEchant; i++)
                min_dist = min( min_dist, DTWdistance(echant[i], coefs) );
            return min_dist;
        }

        float averageDTW(mfcc_coefficients coefs){
            float average = 0;
            for (int i = 0; i < nbEchant; i++)
                average += DTWdistance(echant[i], coefs);
            return average / nbEchant;
        }

        ~mot(){ delete [] echant; }

        string getNom() { return nom; }
        int getNbEchant() { return nbEchant; }
        mfcc_coefficients operator[](int i){ return echant[i]; }
};

string compareCoefsMin(mfcc_coefficients coefs, bool Test){
    float          min_dist = INF;
    string         min_nom,
                   pathDir,
                   empty = "";
    struct dirent* ent;

    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        pathDir = path + (DIR_ENREGISTREMENTS_TEST + empty); 
    else
        pathDir = path + (DIR_ENREGISTREMENTS + empty);
    DIR* dir = opendir( pathDir.c_str() );

    while ( (ent = readdir (dir)) != NULL )
        if ( ( ent->d_name + empty ).find(".coefs") != string::npos){
            mot m(ent->d_name, pathDir);
            float DTW = m.minDTW(coefs);

            cout << "Diffayrences avec " << m.getNom() << "...  " << DTW << endl << flush;
            if ( DTW < min_dist ){
                min_dist = DTW;
                min_nom  = m.getNom();
            }
        }
    closedir (dir);

    if ( min_dist > DIST_MAX )
        min_nom.clear();
    return min_nom;
}

string compareCoefsAverage(mfcc_coefficients coefs, bool Test){
    float          min_dist = INF;
    string         min_nom,
                   pathDir,
                   empty = "";
    struct dirent* ent;

    char path[MAX_TAILLE_PATH];
    _getcwd(path, sizeof(path));

    if (Test)
        pathDir = path + (DIR_ENREGISTREMENTS_TEST + empty); 
    else
        pathDir = path + (DIR_ENREGISTREMENTS + empty);
    DIR* dir = opendir( pathDir.c_str() );

    while ( (ent = readdir (dir)) != NULL )
        if ( ( ent->d_name + empty ).find(".coefs") != string::npos){
            mot m(ent->d_name, pathDir);
            float DTW = m.averageDTW(coefs);

            cout << "Diffayrences avec " << m.getNom() << "...  " << DTW << endl << flush;
            if ( DTW < min_dist ){
                min_dist = DTW;
                min_nom  = m.getNom();
            }
        }
    closedir (dir);

    if ( min_dist > DIST_MAX )
        min_nom.clear();
    return min_nom;
}

#endif