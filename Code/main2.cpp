#include <iostream>
#include <string>
#include "AnalyseLexical.h"
#include "MotCle.h"
#include "AnalyseSyntaxique.h"
#include "AnalyseSemantique.h"


using namespace std;

int main(int argc, char*  argv[]) {
    
    // Lecture des fichier //
    if(argc == 1)  {
        cout << "veuillez introduire le fichier d'input" << endl; 
        exit(EXIT_FAILURE);
    }
    else if(argc == 2){
    
    	// Partie Lexicale
    	AnalyseLexical *an = new AnalyseLexical(argv[1]);
    	an->printCode();
    	
    	cout << "\n\n===> Analyse Lexicale\n\n" << endl;
		an->go();
	
	}
    


    return 0;
}
