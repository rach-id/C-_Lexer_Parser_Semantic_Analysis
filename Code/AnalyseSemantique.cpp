#include "AnalyseSemantique.h"
#include "MotCle.h"


void AnalyseSemantique::ajouterIdentifiants() {
	// lex.printCode();
	variable *var = new variable();
	bool comingFromExtern = false;
	bool comingfromfunction = false;
	AnalyseLexical::unite un;
	AnalyseLexical::unite u;
	int numfunction = 0 ; // le numero de la fonction
	int sc = 0; // Scope de la variable
	
	while(! lex.termine()) {
	
		whilestart1:
		var = new variable();
		un = lex.uniteSuivante();
		
		whilestart2:
		if(un.ul == INT) {
			var->type = "int";
			u = lex.uniteSuivante();
			var->ident = lex.getIdentifiant(u.attribut.first, u.attribut.second);
			var->ou = lex.getEmplacement().first;
			var->scope = sc;
			if(sc==0) var->function = 0;
			else var->function = numfunction;
			AnalyseLexical::unite temp = lex.uniteSuivante();
			if(temp.ul == PARENTHOUV) {
				var->type = "fint"; // fonction de type de retour int
				numfunction++;
				var->function = numfunction;
				sc++; //scope augmente
				comingfromfunction = true;
			} else if (temp.ul == CROCH_OT) {
				var->type = "tint"; // tableau d'int
				var->taille = lex.uniteSuivante().attribut.first;
				if(var->taille <=0) {
					cout << "Tableau de taille negative, Ligne : "<< lex.getEmplacement().first << endl;
					exit(0);
				}
				temp = lex.uniteSuivante();
				temp = lex.uniteSuivante();
				if(temp.ul == VIRG){
					idents.push_back(var);
					goto virgintstart;
				}
			} else if (temp.ul == VIRG) {
				idents.push_back(var);
				
				virgintstart:
				
				var = new variable();
				var->type = "int";
				var->function = numfunction;
				var->ou = lex.getEmplacement().first;
				u = lex.uniteSuivante();
				if(u.ul == INT || u.ul == CHAINE) {
					un=u;
					goto whilestart2;
				}
				var->ident = lex.getIdentifiant(u.attribut.first,u.attribut.second);
				var->scope = sc;
				temp = lex.uniteSuivante();
				if (temp.ul == CROCH_OT) {
					var->type = "tint"; // tableau d'int
					var->taille = lex.uniteSuivante().attribut.first;
					if(var->taille <=0) {
					cout << "Tableau de taille negative, Ligne : "<< lex.getEmplacement().first << endl;
					exit(0);
					}
				}
				idents.push_back(var);
				if(temp.ul == INT) goto virgintstart;
				else goto whilestart1;
			}
			idents.push_back(var);
			
			
		} else if(un.ul ==VOID) {
			numfunction++;
			var->type = "fvoid";
			var->function = numfunction;
			u = lex.uniteSuivante();
			var->ou = lex.getEmplacement().first;
			var->ident = lex.getIdentifiant(u.attribut.first,u.attribut.second);
			var->scope = sc;
			sc++; //scope augmente
			comingfromfunction = true;
			idents.push_back(var);
			
			
		} else if(un.ul == CHAINE) {
			var->type = "chaine";
			u = lex.uniteSuivante();
			var->ou = lex.getEmplacement().first;
			var->ident = lex.getIdentifiant(u.attribut.first,u.attribut.second);
			var->scope = sc;
			if(sc==0) var->function = 0;
			else var->function = numfunction;
			AnalyseLexical::unite temp = lex.uniteSuivante();
			if(temp.ul == PARENTHOUV) {
				var->type = "fchaine"; // fonction de type de retour int
				numfunction++;
				var->function = numfunction;
				sc++; //scope augmente
				comingfromfunction = true;
			}else if (temp.ul == CROCH_OT) {
				var->type = "tchaine"; // tableau d'int
				var->taille = lex.uniteSuivante().attribut.first;
				if(var->taille <=0) {
					cout << "Tableau de taille negative, Ligne : "<< lex.getEmplacement().first << endl;
					exit(0);
				}
				temp = lex.uniteSuivante();
				temp = lex.uniteSuivante();
				if(temp.ul == VIRG){
					idents.push_back(var);
					cout << "In here" << endl;
					goto virgchainestart;
				}
			} else if (temp.ul == VIRG) {
				idents.push_back(var);
			
				virgchainestart:
				
				
				var = new variable();
				if(sc==0) var->function = 0;
				else var->function = numfunction;
				var->type = "chaine";
				u = lex.uniteSuivante();
				if(u.ul == INT || u.ul == CHAINE) {
					un=u;
					goto whilestart2;
				}
				var->ident = lex.getIdentifiant(u.attribut.first,u.attribut.second);
				cout << var->ident << endl;
				var->scope = sc;
				var->ou = lex.getEmplacement().first;
				
				temp = lex.uniteSuivante();
				if (temp.ul == CROCH_OT) {
					var->type = "tchaine"; // tableau d'int
					var->taille = lex.uniteSuivante().attribut.first;
				}
				idents.push_back(var);
				if(temp.ul == VIRG) {
					goto virgchainestart;
				}
				else goto whilestart1;
			}
			idents.push_back(var);
		}
		
		
		else if(un.ul == ACCOLADE_OT) {
				if (comingfromfunction) {
					comingfromfunction = false;
				} else sc++;
		} else if(un.ul == ACCOLADE_FM) {
			 sc--;
		}
		else if(un.ul == EXTERN) {
			while(un.ul != PTVIRG){
				un = lex.uniteSuivante();
			}
		}
	}
	/*
	for(int i = 0; i < idents.size() ; i++) {
		cout << idents[i]->ident << " ";
		cout << idents[i]->type << " " << idents[i]->scope << " " <<  idents[i]->taille << " " <<idents[i]->times << " " << idents[i]->ou <<"  " <<idents[i]->function <<  "\n\n" << endl;
	}*/
	
}

//verifierOperation(unite)

bool AnalyseSemantique::verifierRedefinition(AnalyseLexical::unite u, int em, bool b, int sc, int f) {
	if(b) return true;
	
	string temp = lex.getIdentifiant(u.attribut.first,u.attribut.second);

	for(int i = 0; i < idents.size() ; i++) {
		if(temp == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function ==f ) )) {
			if(idents[i]->times == 0) {
				idents[i]->times++;
				break;
			}
			else {
				cout << "Redefinition de : " << temp << " , ligne : " << em<< endl;
				cout << "===> Le code est semantiquement faux ..." << endl;
				exit(0); 
			}
		}
	}
	return true;
}

bool AnalyseSemantique::estDefini(AnalyseLexical::unite u, int sc, int em, int f) {
	string temp = lex.getIdentifiant(u.attribut.first,u.attribut.second);

	for(int i = 0; i < idents.size() ; i++) {
		if((temp == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function == f)) && idents[i]->ou < em )) {
			return true;
		}
	}
	cout << temp << " doit etre defini avant d'etre utilisee... Ligne " << em << endl;
	exit(0);
}


bool AnalyseSemantique::estTable(AnalyseLexical::unite u, int em, int sc, int f) {
	string temp = lex.getIdentifiant(u.attribut.first,u.attribut.second);
	for(int i = 0; i < idents.size() ; i++) {
		if(temp == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function == f) )) {
			if(idents[i]->type== "tint" ||  idents[i]->type == "tchaine") 				return true;
		}
	}
	cout << endl<< temp << " n'est pas un tableau, Ligne " << em << endl; 
	exit(0);
}


bool AnalyseSemantique::checkType(AnalyseLexical::unite a, AnalyseLexical::unite b, AnalyseLexical::unite c, int nbCrochets, int em , int sc, int f) {
	
	string tempA = lex.getIdentifiant(a.attribut.first,a.attribut.second);
	
	if (c.ul == ZERO) {
		if(b.ul == CONSTANTE) {	
			for(int i = 0; i < idents.size() ; i++) {
				if(tempA == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function == f) )) {
					if(idents[i]->type== "tint") {
						if (nbCrochets == 0) {
							cout << "Veillez specifier l'index ou vous voulez stocker... Ligne : " << em << endl;
							exit(0);
						} else return true;
					}
					else if (idents[i]->type == "fint" ){
						cout << "On ne peut pas affecter a une fonction ... Ligne : "<< em << endl;
						exit(0);
					}
					else if(idents[i]->type == "int" ) 							return true;
				}
			}
			cout << "Pas de conversion connue entre le type chaine et une constante ... Ligne : " << em << endl;
			exit(0); 
			
			
			
			
		} else if(b.ul == CH) {	

			for(int i = 0; i < idents.size() ; i++) {
				if(tempA == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function == f) )) {
					if(idents[i]->type== "tchaine") {
						if (nbCrochets == 0) {
							cout << "Veillez specifier l'index ou vous voulez stocker... Ligne : " << em << endl;
							exit(0);
						} else return true;
					}
					else if (idents[i]->type == "fchaine" ){
						cout << "On ne peut pas affecter a une fonction ... Ligne : "<< em << endl;
						exit(0);
					}
					else if(idents[i]->type == "chaine" ) 							return true;
				}
			}
			cout << "Pas de conversion connue entre le type entier et les chaines ... Ligne : " << em << endl;
			exit(0); 
			
			
			
		} else {
			string tempB = lex.getIdentifiant(b.attribut.first,b.attribut.second);
			for(int i = 0; i < idents.size() ; i++) {
				if(tempA == idents[i]->ident && (idents[i]->scope < sc || (idents[i]->scope == sc && idents[i]->function == f) )) {
					for(int j= 0; j < idents.size() ; j++) {
						if(tempB == idents[j]->ident && (idents[j]->scope < sc || (idents[j]->scope == sc && idents[j]->function == f) )) {
							if(idents[j]->type == idents[i]->type) return true;
							else if (idents[j]->type == "int" && idents[i]->type == "tint" && nbCrochets == 1) return true;
							else if (idents[i]->type == "int" && idents[j]->type == "tint" && nbCrochets == 1) return true;
							else if (idents[j]->type == "chaine" && idents[i]->type == "tchaine" && nbCrochets == 1) return true;
							else if (idents[i]->type == "chaine" && idents[j]->type == "tchaine" && nbCrochets == 1) return true;
						}
					}
				}
			}
			cout << "Incoherence des types manipules... Ligne : " << em << endl;
			exit(0); 
		}
		
	} else {
		
		return true;
	
	}
}
