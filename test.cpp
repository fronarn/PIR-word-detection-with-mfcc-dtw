#include "writefile.h"
#include "play.h"

int main(){
    mfcc_coefficients *c1, c2;
    int n1, n2;
    c1 = getMFCCcoefs("jaimal.coefs", n1, true);
    float average=0.0;
    for (int i=0; i<n1; i++)
        for (int j=0; j<n1; j++)
            if (i!=j){
                int x = average;
                cout << (average+=DTWdistance(c1[i],c1[j]))-x << "   ";
            }
    cout << endl << average/((n1-1)*(n1-1));

    
    c2 = getMFCCcoefs("cava.coefs", true);

    compareCoefsMin(c2, true);
    return 0;
}