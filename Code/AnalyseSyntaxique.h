#ifndef AnalyseSyntaxique_H
#define AnalyseSyntaxique_H

#include "AnalyseLexical.h"
 #include <iostream>
#include "AnalyseSemantique.h"

using namespace std;
class AnalyseSyntaxique {

private:
    AnalyseLexical::unite motCourant;
    
	bool insideFalse = false; //Flag leve s'il s'agit d'une syntaxe non toleree a l'interieur d'une instruction
	
	/* attributs pour le semantique */
	int scope = 0; // Le scope ou on se trouve
	
	int numfunction = 0 ; // le numero de la fonction
	
	bool comingFromLireEcrire = false;
	
	AnalyseLexical::unite temp; // Contiendra le dernier identifiant rencontre
	
	AnalyseLexical::unite a; // a = b+ c;
	AnalyseLexical::unite b; // a = b+ c;
	AnalyseLexical::unite c; // a = b+ c;
	bool comingFromOperation = false;
	bool comingFromAffect = false;
	int cmpt = 0;
	int croch = 0;
	int first = 0;
public:
	AnalyseLexical lexical;

	AnalyseSemantique sm;
	
    AnalyseSyntaxique(string str) : lexical(str), sm(str){
   		motCourant = lexical.uniteSuivante();
   		c = motCourant;
   		c.ul = ZERO;
   		
    }
    AnalyseSyntaxique(AnalyseLexical an): lexical(an) {
   		motCourant = lexical.uniteSuivante();
    }	
    ~AnalyseSyntaxique(){
    }

    bool programme(); // Le debut du programme

    bool prog_prime();

    bool prog_second();


    bool liste_declarations();

    bool declaration();

    bool liste_declarateurs();
    
    bool liste_declarateurs_prime();

    bool declarateur();

    bool declarateur_prime();


    bool type();

    bool liste_parms();

	bool liste_parms_prime();
	
    bool parm();

    bool liste_instructions();

    bool iteration();

    bool selection();

    bool selection_prime();

    bool saut();

    bool saut_prime();

    bool aff_app();
    
    bool affectation();

    bool bloc();

    bool bloc_prime();

    bool variable();

    bool variable_prime();

    bool expression();

    bool expression_prime();

	bool expression_deux_prime();
	
	bool expression_trois_prime();
	
    bool liste_expressions();
    
    bool liste_expressions_prime();

    bool condition();

    bool condition_prime();

	bool condition_deux_prime();
	
    bool binary_op();
    
    bool binary_op_prime();

    bool binary_rel();

    bool binary_comp();

	int getFN() {
		if(scope == 0) return 0;
		else  return numfunction;
	}
};
#endif


