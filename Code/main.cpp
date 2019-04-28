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
   
		 AnalyseSyntaxique s (argv[1]);
		 cout << "\n\n" << endl;
     	if(s.programme()) {
     		if(s.lexical.termine()) cout << "\n\n==> La syntaxe/semantique du code est correcte." << endl;
     		else cout << "\n\n==> Probleme dans la partie lexicale." << endl;
     	}
     	else cout << "\n\n==> Le code est syntaxiquement faux." << endl;
		
    } 
    


    return 0;
}
