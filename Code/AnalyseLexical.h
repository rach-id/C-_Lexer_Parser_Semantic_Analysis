
#ifndef AnalyseLexical_h
#define AnalyseLexical_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <assert.h>
#include "MotCle.h"

typedef struct unite unite;
typedef struct identif identif;
typedef struct chaine chaine;

class AnalyseLexical {

/* initialises les attributs non statiques dans la definiton de la classe est possible pour c++11 */

public:
    /* Structs utilises */
    struct unite {
        Mots ul;
        std::pair<int,int> attribut;

        std::string toString(AnalyseLexical* an) {
                std::string st ;
                if (ul == ID) st  =  "(Lexeme : "  + an->getIdentifiant(attribut.first,attribut.second) 
                    + " \t\tAttribut : " + std::to_string(attribut.first)
                    + "\t\t unite lexicale : " + std::to_string(ul) +  ")\n\n";
                else if (ul == CH) st  =   "(Lexeme : "  + an->getChaine(attribut.first,attribut.second) 
                    + "\t\tAttribut : " + std::to_string(attribut.first)
                    + "\t\t unite lexicale : " + std::to_string(ul) +  ")\n\n";
               else if (ul == CONSTANTE) st  =   "(Lexeme : "  + std::to_string(attribut.first)
                    + "\t\tAttribut : " + std::to_string(-1)
                    + "\t\t unite lexicale : " + std::to_string(ul) +  ")\n\n";
                else if (ul == ERR) {
                	if(an->termine()) st  =   "(Fin Du Fichier)\n\n";
                	else st  =   "(Lexeme : "  + enumToString(ERR)
                    + "\t\tAttribut : " + std::to_string(-1)
                    + "\t\t unite lexicale : " + std::to_string(ul) +  ")\n\n";
                }
                else st  = "(Lexeme : "  + enumToString(ul) + "\t\tAttribut : " + std::to_string(attribut.first) + " \t\t unite lexicale : " + std::to_string(ul) +  ")\n\n";
                return st;
        }
    }; // Struct qui va contenir les unites retournees par la mathode unite suivante
 // Pour l'utiliser dans est chaine (string)
    struct chaine {
        std::string valeur;
        chaine* suivant;
    };
    struct identif {
        std::string identifiant;
        int type; 
        identif* suivant;
    }; //Struct qui va contenir les identifiants
        // On utilise une liste chainee pour gerer les collisions probables de la fonction de hashage.
        // On utilise la technique de seperate chaining pour resoudre ce probleme
    

private:   

    /* Tableaux utilises */
    std::vector<identif*> tableauIdentif = std::vector<identif*> (100,nullptr); 
        // Vecteur des identifiants
    std::vector<chaine*> Chaines = std::vector<chaine*> (100, nullptr);

    std::map<std::string, Mots> motCles; // Map des mots cles


    /* Attributs utilises */
    char c ; // caractere courant
    std::string toBeParsed = ""; //Le string copie depuis le fichier pour etre compile
    std::string output = ""; //Le resultat qu'on va ecrire sur le fichier de sortie 
    int indexCourant = 0; //L'index courant dans le string toBeParsed
    std::string inputFile = "" ; //Path du fichier input
    std::string outputFile =""; //Path du fichier output
    std::string lxNonComplet = ""; //String sur laquelle va agir 'ajouterCar()'
    bool debutCommentaire = false; // flag qui montre si on vient d'un commentaire
    bool endOfFile = false; // flag qui montre que le fichier qu'on parse a fini
    int ligneNombre = 0; // Le nombre de lignes qu'on a traduit (/n ou /r rencontre)


public:
    AnalyseLexical(); //constructeur par defaut
    AnalyseLexical(std::string); // constructeur qui prend un fichier et le lit pour le traduire
    AnalyseLexical(std::string, std::string); // constructeur qui prend un fichier, le lit, et
        // et enregistre la traduction dans le deuxieme fichier
    ~AnalyseLexical();


    int hash(std::string); //fonction hash une entree string et qui retourne 
            //le hash correspondant


    std::pair<int,int> ajouterIdentifiant(std::string);// Fonction qui ajoute un identifiant
            // a la table des identifiants.
            // prend un argument, l'identifiant et retourne son indice si ajout est 
            // fait sans probleme.
    std::pair<int,int> ajouterChaine(std::string);// Fonction qui ajoute un identifiant
    bool ajouterMotCle(); // ajoute tous le mots cles du langage automatiquement


    bool estBlanc(char); //s'il s'agit d'un blanc, on retourne true, false sinon
    bool estLettre(char); //s'il s'agit d'une lettre , on retourne true, false sinon
    bool estChiffre(char); //s'il s'agit d'un chiffre, on retourne true, false sinon
    bool estChaine(char); // verifie que le caractere peut exister dans une chaine de caracteres
    int estMotCle(std::string); //prend en argument un lexeme et verifie s'il s'agit 
            // d'un mot cle, si c'est le cas, il retourne son indice dans l'enumeration
            // -1 sinon

    identif* estIdentifiant(std::string); //prend en argument un lexeme et verifie s'il s'agit 
            // d'un identifiant, si oui, elle retourne son addresse. nullptr sinon
    chaine* estChaine(std::string); //prend en argument un lexem et verifie s'il s'agit d'une
                // chaine de caracteres, si oui, elle retourne son address, nullptr sinon


    std::string getIdentifiant(int,int); //prend ses deux emplacements dans la table des identifiants
                // et le retourne
    std::string getChaine(int,int); //prend ses deux emplacements dans la table des chaines
                // et la retourne


    char lireCar(); //lit un caractere et retourne sa valeur    
    void ajouterCar(char);
    bool lireFichier(std::string); // Lit le fichier et stocke ce qu'il contient dans le 
            // toBeParsed string
            // retourne false si le fichier n'a pas pu etre ouvert
    bool termine(){ // retourne si on a termine le fichier a traduire
        return endOfFile;
    }


    unite uniteSuivante(); //lit le prochain lexeme et le retourne dans une struct
            // cette struct contient le lexem et sa valeur (si disponible)
    unite uniteSuivante1();

    static std::string enumToString(int); // renvoie le lexeme correspondant a l'unite lexicale introduite
        // static pour pouvoir l'utilise dans la struct unite;
    int getLigneNombre(){ // Le nombre de ligne qu'on a traduit
        return ligneNombre;
    }
    std::pair<int,int> getEmplacement() {
        std::pair<int,int> p;
        p.first = getLigneNombre() + 1;
        // if (getLigneNombre() != 0 ) p.second = getCaractere() % (getLigneNombre()+1) ;
        // else p.second = getCaractere() ;
        p.second = -1;
        return p;
    }
   
   	void printCode() {
   		std::cout << toBeParsed << std::endl;
   	}
   	void go() {
   		unite u = uniteSuivante();
   		while(!this->termine() && u.ul != ERR) {
   			u = uniteSuivante1();
   			if(u.ul == 46) {
   				std::cout << "Erreur Chaine non complete, \" non trouve. Ligne : " << getEmplacement().first << std::endl;
   				exit(0);
   			}
   		}
   	}
};


#endif /* analyse_lexical_h */
