#ifndef AnalyseSemantique_H
#define AnalyseSemantique_H

#include <iostream>
#include <string>
#include "AnalyseLexical.h"
#include <vector>

typedef struct variable variable;
using namespace std;

class AnalyseSemantique{
public:
	struct variable {
		string ident; // Va contenir l'identifiant de la variable courante
		string type=""; // Va contenir le type des identifiants
		int scope =0 ; //Variables globales, locales .... par niveau (0 = globale)
		int taille = 0; // s'il s'agit d'un tableau
		int times = 0; // nombre de fois on est passe par cet identifiant
		int ou = 0; //Dans quelle ligne la definition de la variable a ete rencontree
		int function = 0; // la fonction ou se trouve la declaration de la variable.
	};
	
	vector<variable*> idents; //Tableau qui va contenir les identifiants et leurs types (struct variable)
	
	int sc = 0;
	
	AnalyseLexical lex; //Le lexical 
	
	AnalyseSemantique() {
	
	}
		
	AnalyseSemantique(string str) : lex(str) {
		
		go();
	}	
	
	void ajouterIdentifiants();// fonction qui ajoute tous les identifiants dans la table des identifiants avec leurs types
	
	void go() {
		ajouterIdentifiants();
	}
	
	bool verifierRedefinition(AnalyseLexical::unite, int, bool = false, int = 0, int =0); 
	
	bool estDefini(AnalyseLexical::unite, int, int, int);

	bool estTable(AnalyseLexical::unite, int, int, int); 
	
	bool checkType(AnalyseLexical::unite , AnalyseLexical::unite , AnalyseLexical::unite , int , int, int , int);
	
	void afficherTable() {
		cout << "=============== Table des Symboles ==========" << endl;
		for(int i = 0; i < idents.size() ; i++) {
			cout << "(identifiant: " <<idents[i]->ident << "\t type: ";
			cout << idents[i]->type << "\tcontexte: " << idents[i]->scope << "\ttaille: " <<  idents[i]->taille << " Ligne: " << idents[i]->ou <<" Fonction: " <<idents[i]->function <<  ")\n\n" << endl;
		}
	}
};

#endif

