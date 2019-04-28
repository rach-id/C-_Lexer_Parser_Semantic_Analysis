#include "AnalyseSyntaxique.h"

// g++ *.cpp *.h -std=c++11 -o sortie

bool AnalyseSyntaxique::programme() {
    if(motCourant.ul == CHAINE) {
        motCourant = lexical.uniteSuivante();
        if(prog_prime()) {
            return true;
        }
        else return false;
        
        
    } else if(motCourant.ul == INT) {
		motCourant = lexical.uniteSuivante();
		if(prog_prime()) {
			return true;
		} else {
			return false;
		}
		
		
	} else if(motCourant.ul == VOID ) {
	    motCourant = lexical.uniteSuivante();
	    scope++;
	    numfunction++;
		if(motCourant.ul == ID && sm.verifierRedefinition(motCourant,lexical.getEmplacement().first, false, scope, getFN())) {
            motCourant = lexical.uniteSuivante();
            if(motCourant.ul == PARENTHOUV) {
                motCourant = lexical.uniteSuivante();
                if(liste_parms()) {
                    if(motCourant.ul == PARENTHFERM){
                        motCourant = lexical.uniteSuivante();
                        if(motCourant.ul == ACCOLADE_OT) {
                            motCourant = lexical.uniteSuivante();
                            if(liste_declarations()) {
                                if(liste_instructions()) {
                                    if(motCourant.ul == ACCOLADE_FM) {
                                    	scope--;
                                    	motCourant = lexical.uniteSuivante();
										if(programme()){
											return true;
										} else {
											return false;
										}
                                        
                                    } else {
                                        return false;
                                    }                
                                } else {
                                    return false;
                                }
                            } else {
                                return false;
                            }
                        } else{
							cout << " Erreur : '{' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
							return false;
							} 
                    } else{
						cout << " Erreur : ')' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
						return false;
					} 
                } else {
                    return false;
                }
            } else{
				cout << " Erreur : '(' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
        } else{
			cout << " Erreur : Identifiant attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
			return false;
		} 
    } else if (motCourant.ul == EXTERN) {
        motCourant = lexical.uniteSuivante();
        if(type()){
            if(motCourant.ul == ID && sm.verifierRedefinition(motCourant,lexical.getEmplacement().first, true , scope, getFN())) {
                motCourant = lexical.uniteSuivante();
                if(motCourant.ul == PARENTHOUV) {
                    motCourant = lexical.uniteSuivante();
                    if(liste_parms()) {
                        if(motCourant.ul == PARENTHFERM){
                            motCourant = lexical.uniteSuivante();   
                            if(motCourant.ul == PTVIRG) {
                                motCourant = lexical.uniteSuivante();
								if(programme()) {
									return true;
								} else {
									return false;
								}
                            } else {
								cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
                                return false;
                            }                 
                        } else {
                        	cout << " Erreur : ')' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                	cout << " Erreur : '(' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
                    return false;
                }
            } else {
            	cout << " Erreur : ID attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
                return false;
            }
        } else {
        	cout << " Erreur : int, chaine, void attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
            return false;
        }   
	} else if (motCourant.ul == ERR) {
        return true;
    } else{
		cout << " Erreur : int, chaine, void, extern attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
		return false;
	} 
}

bool AnalyseSyntaxique::prog_prime() {
	if(motCourant.ul == ID && sm.verifierRedefinition(motCourant,lexical.getEmplacement().first,false ,  scope, getFN())) {
		temp = motCourant;
		motCourant = lexical.uniteSuivante();
		if(prog_second()) {
			return true;
		} else {
			return false;
		}
	} else {
		cout << "Id attendu, Ligne:" << lexical.getEmplacement().first <<  endl;
		return false;
	}
}


bool AnalyseSyntaxique::prog_second() {
	if (motCourant.ul == PARENTHOUV) {
	    scope++;
	    numfunction++;
		motCourant = lexical.uniteSuivante();
		if(liste_parms()) {
				if(motCourant.ul == PARENTHFERM) {
				motCourant = lexical.uniteSuivante();
					if(motCourant.ul == ACCOLADE_OT) {
						motCourant = lexical.uniteSuivante();
						if(liste_declarations()) {
							if(liste_instructions()) {
								if(motCourant.ul == ACCOLADE_FM) {
									scope--;
									motCourant = lexical.uniteSuivante();
									if( programme()) {
										return true;
									} else return false;
								} else{
								cout << " Erreur : '}' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
								return false;
								} 
							} else return false;
						} else return false; 
					} else{
						cout << " Erreur : '{' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
						return false;
					} 
			} else{
				cout << " Erreur : ')' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
		} else return false;
	} else if(declarateur_prime()) {
		if(liste_declarateurs_prime() && motCourant.ul == PTVIRG) {
			motCourant = lexical.uniteSuivante();
			if(programme()){
				return true;
			} else {
				return false;
			} 
		} else {
			cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
			return false;
		}
	} 
	
}

bool AnalyseSyntaxique::liste_declarations() {  
	if(motCourant.ul == INT) {
		motCourant = lexical.uniteSuivante();
		if(liste_declarateurs()) {
			if(motCourant.ul == PTVIRG) {
				motCourant = lexical.uniteSuivante();
				if(liste_declarations()){
					return true;
				} else return false;
			} else{
				cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
		} else{
			cout << " Erreur : Identifiant attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
			return false;
		} 
	} else if (motCourant.ul == CHAINE) {
		motCourant = lexical.uniteSuivante();
		if(liste_declarateurs()) {
			if(motCourant.ul == PTVIRG) {
				motCourant = lexical.uniteSuivante();
				if(liste_declarations()){
					return true;
				} else return false;
			} else{
				cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
		} else return false;
	} else return true;
}

bool AnalyseSyntaxique::declaration(){
    if(motCourant.ul == INT) {
        motCourant = lexical.uniteSuivante();
        if(liste_declarateurs()){
            if(motCourant.ul == PTVIRG) {
                motCourant = lexical.uniteSuivante();
                if(declaration()) {
                    return true;
                } else {
                    return false;
                }
            } else{
				cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
        } else {
            return false;
        }
    } else if(motCourant.ul == CHAINE) {
        motCourant = lexical.uniteSuivante();
        if(liste_declarateurs()){
            if(motCourant.ul == PTVIRG) {
                motCourant = lexical.uniteSuivante();
                if(declaration()) {
                    return true;
                } else {

                    return false;
                }
            } else{
				cout << " Erreur : ';' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
				return false;
			} 
        } else {
            return false;
        }
    } else {
        cout << "Erreur : 'int ou chaine' attendu ! Ligne : " << lexical.getEmplacement().first  <<  endl;
        return false;
    }
}




bool AnalyseSyntaxique::liste_declarateurs() {
    if(declarateur()) {
        if(liste_declarateurs_prime()) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool AnalyseSyntaxique::liste_declarateurs_prime() {
    if(motCourant.ul == VIRG) {
        motCourant = lexical.uniteSuivante();
        if(declarateur()){
            if(liste_declarateurs_prime()) {
                return true;
            } else {

                return false;
            }
        } else {
            return false;
        }
    } else {
        return true;
    }
}

bool AnalyseSyntaxique::declarateur() {
    if(motCourant.ul == ID && (sm.verifierRedefinition(motCourant,lexical.getEmplacement().first, comingFromLireEcrire, scope , getFN()) )) {
    	temp = motCourant;
        motCourant = lexical.uniteSuivante();
        if(declarateur_prime()) {
            return true;
        } else {
            return false;
        }
    } else{
		cout << " Erreur : Identifiant attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
		return false;
	} 
}

bool AnalyseSyntaxique::declarateur_prime(){
    if(motCourant.ul == CROCH_OT) {
    	croch++;
        motCourant = lexical.uniteSuivante();
        if(motCourant.ul == CONSTANTE) {
            motCourant = lexical.uniteSuivante();
            if(motCourant.ul == CROCH_FM) {
                motCourant = lexical.uniteSuivante();
                return true;
            } else {
            	cout << " Erreur : ']' attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
                return false;
            }
        } else {
        	cout << " Erreur : Taille du tableau attendu ! Ligne : " << lexical.getEmplacement().first <<  endl;
            return false;
        }
    } else {
        return true;
    }
}


bool AnalyseSyntaxique::type() {
    if(motCourant.ul == VOID) {
        motCourant = lexical.uniteSuivante();
        return true;
    } else if (motCourant.ul == INT){
        motCourant = lexical.uniteSuivante();
        return true;
    } else if(motCourant.ul == CHAINE) {
        motCourant = lexical.uniteSuivante();
        return true;
    } else {
        return false;
    }
}

bool AnalyseSyntaxique::liste_parms(){
	if (parm()) {
		if(liste_parms_prime()) {
			return true;
		} return false;
	} return true;
}//liste_parms

bool AnalyseSyntaxique::liste_parms_prime(){
	if (motCourant.ul==VIRG) {
		motCourant = lexical.uniteSuivante();
		if(parm()) {
			if(liste_parms_prime()){
				return true;
			} else return false;
		} else return false;
	}else return true;
}//liste_parms

bool AnalyseSyntaxique::parm(){
	if(motCourant.ul == INT){
	
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == ID && sm.verifierRedefinition(motCourant,lexical.getEmplacement().first, false,scope, getFN() )){
			temp = motCourant;
			motCourant = lexical.uniteSuivante();
			return true;
		}
		else{
			cout << "Erreur : Identifiant attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            return false;
		}
	}
	else if(motCourant.ul == CHAINE){
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == ID && sm.verifierRedefinition(motCourant,lexical.getEmplacement().first, false, scope, getFN())){
			temp = motCourant;
			motCourant = lexical.uniteSuivante();
			return true;
		}
		else{
			cout << "Erreur : Identifiant attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            return false;
		}
	}
	else{
        return false;
	}
}//parm


bool AnalyseSyntaxique::liste_instructions(){
	croch = 0;
	if(iteration()){
		if (liste_instructions()){
			return true;
		} 
		else return false;
	}
	else if(selection()){
		if (liste_instructions()){
			return true;
		} 
		else return false;
	}
	else if(saut()){
		if (liste_instructions()){
			return true;
		} 
		else return false;
	}
	else if (motCourant.ul == ID && sm.estDefini(motCourant, scope, lexical.getEmplacement().first, getFN()))
	{
		temp = motCourant;
		a = motCourant;
		motCourant = lexical.uniteSuivante();
		if(aff_app()){
			if(motCourant.ul == PTVIRG  && sm.checkType(a,b,c, croch, lexical.getEmplacement().first, scope, getFN())){
				a.ul = ZERO;
				b.ul = ZERO;
				c.ul = ZERO;
				cmpt = 0;
				croch = 0;
				motCourant = lexical.uniteSuivante();
				if (liste_instructions()){
					return true;
				} 
				else return false;
			}
			else{
				cout << "Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
				insideFalse = true;
		        return false;
			}
		} else {
			insideFalse = true;
			return false;
		}
	}	
	else if (bloc())
	{
		if (liste_instructions()){
			return true;
		} 
		else return false;
	}
	else if(motCourant.ul == LIRE){
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == PARENTHOUV) {
			comingFromLireEcrire = true;
			motCourant = lexical.uniteSuivante();
			if (liste_declarateurs()){
				comingFromLireEcrire = false;
				if(motCourant.ul == PARENTHFERM) {
					motCourant = lexical.uniteSuivante();
					if(motCourant.ul == PTVIRG) {
						motCourant = lexical.uniteSuivante();
						if (liste_instructions()){
						return true;
						} 
						else return false;
					}
					else {
						cout << "Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
						insideFalse = true;
						return false;
					}
				} else {
					cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
					insideFalse = true;
					return false;
				}
			} 
			else return false;
		} else {
			cout << "Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
			insideFalse = true;
		}
		
	}
	else if(motCourant.ul == ECRIRE){
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == PARENTHOUV) {
			comingFromLireEcrire = true;
			motCourant = lexical.uniteSuivante();
			if (liste_declarateurs()){ 
				if(motCourant.ul == PARENTHFERM) {
					comingFromLireEcrire = false;
					motCourant = lexical.uniteSuivante();
					if(motCourant.ul == PTVIRG) {
						motCourant = lexical.uniteSuivante();
						return true;
					}
					else {
						cout << "Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
						insideFalse = true;
						return false;
					}
				} else {
					cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
					insideFalse = true;
					return false;
				}
			} 
			else return false;
		} else {
			cout << "Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
			insideFalse = true;
		}
	} 
	else if(insideFalse == true){
           return false;
	} else { return true;}
}//instruction

bool AnalyseSyntaxique::iteration(){
	if(motCourant.ul == FOR){
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == PARENTHOUV){
			motCourant = lexical.uniteSuivante();
			if(affectation()){
				if(motCourant.ul == PTVIRG){
					motCourant = lexical.uniteSuivante();
					if(condition()){
						if(motCourant.ul == PTVIRG){
							motCourant = lexical.uniteSuivante();
							if(affectation()){
								if(motCourant.ul == PARENTHFERM){
									motCourant = lexical.uniteSuivante();
									if(liste_instructions()){
										return true;
									}
									else{
							            insideFalse = true;
							            return false;
									}
								}
								else{
									cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            						insideFalse = true;
            						return false;
								}
							}
							else{	
								cout << "50 Erreur : affectation attendue ! Ligne : " << lexical.getEmplacement().first << endl;
            					insideFalse = true;
            					return false;
							}
						}
						else{
							cout << " Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            				insideFalse = true;
            				return false;
						}
					}	
					else{
						cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            			insideFalse = true;
            			return false;
					}
				}
				else{
					cout << "Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            		insideFalse = true;
            		return false;
				}
			}
			else{
				cout << "50 Erreur : affectation attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            	insideFalse = true;
            	return false;
			}
		}
		else{
			cout << "51 Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first<<  endl;
            insideFalse = true;
            return false;
		}
	}
	else if (motCourant.ul == WHILE)
	{
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == PARENTHOUV){
			motCourant = lexical.uniteSuivante();
			if(condition()){
				if(motCourant.ul == PARENTHFERM){
					motCourant = lexical.uniteSuivante();
					if(liste_instructions()){
						return true;
					}
					else{
            			return false;
					}
				}
				else{
					cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            		insideFalse = true;
            		return false;
				}
			}
			else{
				cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            	insideFalse = true;
            	return false;
			}
		}
		else{
			cout << "Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            insideFalse = true;
            return false;
		}
	}
	else{
            return false;
	}
}//iteration

bool AnalyseSyntaxique::selection(){
	if(motCourant.ul == IF){
		motCourant = lexical.uniteSuivante();
		if(motCourant.ul == PARENTHOUV){
			motCourant = lexical.uniteSuivante();
			if(condition()){
				if(motCourant.ul == PARENTHFERM){
					motCourant = lexical.uniteSuivante();
					if(liste_instructions()){
						if(selection_prime()){
							return true;
						}
						else{
            				return false;
						}
					}
					else{
            			insideFalse = true;
            			return false;
					}
				}
				else{
					cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            		insideFalse = true;
            		return false;
				}
			}
			else{
				cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            	insideFalse = true;
            	return false;
			}
		}
		else{
			cout << "61 Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            insideFalse = true;
            return false;
		}
	}
	else{
        return false;
	}
}//selection

bool AnalyseSyntaxique::selection_prime(){
	if(motCourant.ul == ELSE){
		motCourant = lexical.uniteSuivante();
		if(liste_instructions()){
		
			return true;
		}
		else{
            insideFalse = true;
            return false;
		}
	}
	else{
		return true;
	}
}//selection_prime

bool AnalyseSyntaxique::saut(){
	if(motCourant.ul == RETURN){
		motCourant = lexical.uniteSuivante();
		if(saut_prime()){
			return true;
		}
		else{
			//saut_prime attendue
			cout << "64 Erreur : ';' ou expression attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            insideFalse = true;
            return false;
		}
	}
	else{
        return false;
	}
}//saut
bool AnalyseSyntaxique::saut_prime(){
	if(motCourant.ul == PTVIRG){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if(expression()){
		if(motCourant.ul == PTVIRG){
			motCourant = lexical.uniteSuivante();
			return true;
		}
		else{
			cout << "66 Erreur : ';' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            insideFalse = true;
            return false;
		}
	}	
	else{

        return false;
	}
}//saut_prime

bool AnalyseSyntaxique::aff_app() {
 	if (motCourant.ul == PARENTHOUV){
		motCourant = lexical.uniteSuivante();
		if(liste_expressions()){
			if(motCourant.ul == PARENTHFERM){
				motCourant = lexical.uniteSuivante();
			}
			else{
				cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
        		insideFalse = true;
        		return false;
			}
		}
		else{
        	return false;
		}

	}else if(variable_prime()){
		comingFromAffect = true;
		if(motCourant.ul == AFFECT){
			motCourant = lexical.uniteSuivante();
			b = motCourant;
			if(expression()){
				return true;
			} 
			else {
				cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            	insideFalse = true;
            	return false;
			}
		}
		else{	
			insideFalse = true;
            return false;
		}
	}else{
		cout <<"Erreur : '[' ou '(' ou '='attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
		insideFalse = true;
        return false;
	}
}

bool AnalyseSyntaxique::affectation(){

	if(variable()){
		if(motCourant.ul == AFFECT){
			motCourant = lexical.uniteSuivante();
			if(expression()){
				return true;
			}
			else {
				cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            	insideFalse = true;
            	return false;
			}
		}
		else{	
            return false;
		}
	}
	else{
        return false;
	}
}//affectation

bool AnalyseSyntaxique::bloc(){
	if(motCourant.ul == ACCOLADE_OT){
		scope++;
		motCourant = lexical.uniteSuivante();
		if(bloc_prime()){
			return true;
		}
		else{
			cout << "Erreur :'}' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            insideFalse = true;
            return false;
		}
	}
	else{
        return false;
	}
}//bloc

bool AnalyseSyntaxique::bloc_prime(){
	if(liste_instructions()){
		if(motCourant.ul == ACCOLADE_FM){
			scope--;
			motCourant = lexical.uniteSuivante();
			return true;
		}
		else{
			cout << "Erreur : '}' attendue ! Ligne : " << lexical.getEmplacement().first 
            << "," << lexical.getEmplacement().second <<  endl;
            insideFalse = true;
            return false;
		}
	}
	else if(motCourant.ul == ACCOLADE_FM){
		scope--;
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else{
		cout << "Erreur : '}' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
        return false;
	}
}//bloc_prime


bool AnalyseSyntaxique::variable(){
	if(motCourant.ul == ID  && sm.estDefini(motCourant, scope, lexical.getEmplacement().first, getFN())){
		temp = motCourant;
		b = motCourant;
		motCourant = lexical.uniteSuivante();
		if(variable_prime()){
			return true;
		}
		else{
       		insideFalse = true;
        	return false;
		}
	}
	else{
        return false;
	}
}//variable

bool AnalyseSyntaxique::variable_prime(){
	// cout <<"Variable prime :" << lexical.getEmplacement().first << " " << temp.toString(&lexical) << endl;
	if(motCourant.ul == CROCH_OT && sm.estTable(temp, lexical.getEmplacement().first, scope , getFN())){
		croch++;
		motCourant = lexical.uniteSuivante();
		if(expression()){
			if(motCourant.ul == CROCH_FM){
				motCourant = lexical.uniteSuivante();
				return true;
			}
			else{
				cout << "82 Erreur : ']' attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
        		insideFalse = true;
        		return false;
			}
		}
		else{
			cout << "83Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
        	insideFalse = true;
        	return false;
		}
	}
	else{
		return true;
	}
}//variable_prime

bool AnalyseSyntaxique::expression(){
	first++;
    if(motCourant.ul == PARENTHOUV){
        motCourant = lexical.uniteSuivante();
        if(expression()){
            if(motCourant.ul == PARENTHFERM){
                motCourant = lexical.uniteSuivante();
                if(expression_prime()){
                    return true;
                }
                else{
                    cout << "Erreur : expression_prime attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
                    return false;
                }
            }
            else{
                cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
                return false;
            }
        }
        else{
            cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
            return false;
        }
    }
    else if(motCourant.ul == MOINS){
        motCourant = lexical.uniteSuivante();
        if(expression()){
            if(expression_prime()){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else if(motCourant.ul == ID  && sm.estDefini(motCourant, scope,lexical.getEmplacement().first, getFN())){
    	temp = motCourant;
    	c = motCourant;
        motCourant = lexical.uniteSuivante();
        if(expression_deux_prime()){
            return true;
        }
        else{
                return false;
        }
    } else if (motCourant.ul == CONSTANTE) {
    	if (cmpt == 0) {
    		b = motCourant;
    		cmpt++;
    	} else c = motCourant;
    	motCourant = lexical.uniteSuivante();
    	if(expression_deux_prime()) {
    		return true;
    	} else return false;
    
    } else if (motCourant.ul == CH) {
    	if (cmpt == 0) {
    		b = motCourant;
    		cmpt++;
    	} else c = motCourant;
    	motCourant = lexical.uniteSuivante();
    	if(expression_trois_prime()) {
    		return true;
    	} else return false;
    } else{	
        return false;
    }
}// expression


bool AnalyseSyntaxique::expression_prime(){
    if(binary_op()){
        if(expression()){
            if(expression_prime()) {
            	return true;
            }else return false;
        }
        else{
            cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            return false;
        }
    }
    else{
    	if(first == 1) {
    		c.ul = ZERO;
    		first = 0;
    	}
        return true;
    }
}//expression_prime





bool AnalyseSyntaxique::expression_deux_prime(){
	if(motCourant.ul == PARENTHOUV){
        motCourant = lexical.uniteSuivante();
        if(liste_expressions()){
            if(motCourant.ul == PARENTHFERM){
                motCourant = lexical.uniteSuivante();
                if(expression_prime()) return true; 
                else return false;          
          } else{
                cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first<<   endl;
                return false;
            }
        }
        else{
            cout << "Erreur : liste_expressions attendue ! Ligne : " << lexical.getEmplacement().first <<  endl;
                return false;
        }
    }
    else if(variable_prime()){
        if(expression_prime()){
            return true;
        }else{
            return false;
        }
    }
}//expression_deux_prime



bool AnalyseSyntaxique::expression_trois_prime(){
    if(binary_op_prime()){
        if(expression()){
            if(expression_prime()) {
            	return true;
            }else return false;
        }
        else{
            cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first  <<  endl;
            return false;
        }
    }
    else{
        return true;
    }
}//expression_prime


bool AnalyseSyntaxique::liste_expressions(){
	if(expression()){
		if(liste_expressions_prime()){
				return true;
			} else return false;
		}
	else{
		return true;
	}
}//liste_expressions

bool AnalyseSyntaxique::liste_expressions_prime(){
	if(motCourant.ul == VIRG) {
		motCourant = lexical.uniteSuivante();
		if(expression()){
			if(liste_expressions_prime()){
				return true;
			}
			else return false;
		}
		else{
      		return false;
		}
	}
	else{
		return true;
	}
}//liste_expressions_prime

bool AnalyseSyntaxique::condition(){
    if(motCourant.ul == NON){
        motCourant = lexical.uniteSuivante();
        if(motCourant.ul == PARENTHOUV){
            motCourant = lexical.uniteSuivante();
            if(condition()){
                if(motCourant.ul == PARENTHFERM){
                    motCourant = lexical.uniteSuivante();
                    if(condition_prime()){
                        return true;
                    }
                    else{
                        cout << "Erreur : condition_prime attendue ! Ligne : " << lexical.getEmplacement().first 
                            << "," << lexical.getEmplacement().second <<  endl;
                        return false;
                    }
                }
                else{
                    cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first 
                        << "," << lexical.getEmplacement().second <<  endl;
                    return false;
                }
            }
            else{
                cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
            }
        }
        else{
            cout << "Erreur : '(' attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
        }
    }
    else if(expression()){
        if(condition_deux_prime()){
            return true;
        }
        else{
            cout << "Erreur : condition_deux_prime attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
        }
    }
    else if(motCourant.ul == PARENTHOUV){
        motCourant = lexical.uniteSuivante();
        if(condition()){
            if(motCourant.ul == PARENTHFERM){
                motCourant = lexical.uniteSuivante();
                if(condition_prime()){
                    return true;
                }
                else{
                    cout << "Erreur : condition_prime attendue ! Ligne : " << lexical.getEmplacement().first 
                        << "," << lexical.getEmplacement().second <<  endl;
                    return false;
                }
            }
            else{
                cout << "Erreur : ')' attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
            }
        }
        else{
            cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
        }
    }

    else{
        cout << "Erreur : ')', '!' ou expression attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
    }
}//condition


bool AnalyseSyntaxique::condition_prime(){
    if(binary_rel()){
        if(condition()){
            return true;
        }   
        else{
            cout << "Erreur : condition attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
        }
    }
    else{
        return true;
    }
}//condition_prime


bool AnalyseSyntaxique::condition_deux_prime(){
    if(binary_comp()){
        if(expression()){
            if(condition_prime()){
                return true;
            }
            else{
                cout << "Erreur : condition' attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
            }
        }
        else{
            cout << "Erreur : expression attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
        }
    }   
    else{
        cout << "Erreur : comparaison binaire attendue ! Ligne : " << lexical.getEmplacement().first 
                    << "," << lexical.getEmplacement().second <<  endl;
                return false;
    }
}//condition_deux_prime


bool AnalyseSyntaxique::binary_op(){
	if(motCourant.ul == PLUS){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == MOINS)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == MULTIP)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == DIV)  // /
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == BININF) //<<
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == BINSUP) //>>
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == AND) // '&'
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == BARRE) // '|'
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else{
       	return false;
	}
}//binary_op



bool AnalyseSyntaxique::binary_op_prime(){
	if(motCourant.ul == PLUS){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == MOINS)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else{
       	return false;
	}
}//binary_prime_op



bool AnalyseSyntaxique::binary_rel(){
	if(motCourant.ul == ET){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if(motCourant.ul == OU){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else{
       	return false;
	}
}//binary_rel

bool AnalyseSyntaxique::binary_comp(){
	if(motCourant.ul == INF){
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == SUP)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == SUPEG)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == INFEG)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == EGAL)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else if (motCourant.ul == DIFF)
	{
		motCourant = lexical.uniteSuivante();
		return true;
	}
	else{
		cout << "114 Erreur : '<','>','>=','<=','=='ou '!=' attendue ! Ligne : " << lexical.getEmplacement().first 
        << "," << lexical.getEmplacement().second <<  endl;
       	return false;
	}
}//binary_comp


