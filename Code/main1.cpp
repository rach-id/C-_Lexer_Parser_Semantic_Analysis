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
    	// AnalyseLexical *an = new AnalyseLexical(argv[1]);
    	// an->printCode();
    	
    	// cout << "\n\n===> Analyse Lexicale\n\n" << endl;
		// an->go();

    	// Partie Syntaxique
    	 
    	 AnalyseSemantique *sm = new AnalyseSemantique(argv[1]);
		 sm->go();
		 sm->afficherTable();
		 
		 AnalyseSyntaxique s (argv[1]);
		 cout << "\n\n\n========= Resultat =========\n"<< endl;
     	if(s.programme()) {
     		if(s.lexical.termine()) cout << "\n\n==> La syntaxe/semantique du code est correcte." << endl;
     		else cout << "\n\n==> Probleme dans la partie lexicale." << endl;
     	}
     	else cout << "\n\n==> Le code est syntaxiquement faux." << endl;
		
		delete sm;
    } 
    


    return 0;
}
