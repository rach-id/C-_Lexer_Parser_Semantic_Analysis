#include "AnalyseLexical.h"
#include "MotCle.h"
#include <ctype.h>

using namespace std;

string stringToLower(string str) { // prend un string et le convertit en miniscule
    for(auto it=str.begin(); it!= str.end(); ++it){
        *it = tolower(*it);
    }
    return str;
} // Fin stringToLower




AnalyseLexical::AnalyseLexical(){
    assert(ajouterMotCle()); //Si les mots cles du langage ne sont pas ajoutes correctement
        // on sort du programme
} // Fin AnalyseLExical()

AnalyseLexical::AnalyseLexical(string i){
    assert(ajouterMotCle()); //Si les mots cles du langage ne sont pas ajoutes correctement
        // on sort du programme
    inputFile = i;
    lireFichier(i); // lire le fichier pour avoir une string a traduire
    c = ' ';
} // Fin AnalyseLexical(string)

AnalyseLexical::AnalyseLexical(string i, string o){
    assert(ajouterMotCle()); //Si les mots cles du langage ne sont pas ajoutes correctement
        // on sort du programme
    inputFile = i;
    outputFile = o;
    lireFichier(i);
    c = ' ';
} // Fin AnalyseLexical (string, string)

AnalyseLexical::~AnalyseLexical(){
} // Fin ~AnalyseLexical



bool AnalyseLexical::ajouterMotCle(){
    /* On ajoute pair par pair les mots reserves du langage de type (int, string) */
    motCles.insert(std::make_pair("if", IF));
    motCles.insert(std::make_pair("else", ELSE));
    motCles.insert(std::make_pair("while", WHILE));
    motCles.insert(std::make_pair("for", FOR));
    motCles.insert(std::make_pair("extern", EXTERN));
    motCles.insert(std::make_pair("int", INT));
    motCles.insert(std::make_pair("void", VOID));
    motCles.insert(std::make_pair("chaine", CHAINE));
    motCles.insert(std::make_pair("return", RETURN));
    motCles.insert(std::make_pair("lire", LIRE));
    motCles.insert(std::make_pair("ecrire", ECRIRE));
    return true;
} // Fin ajouterMotCle()


char AnalyseLexical::lireCar() {
    /* fonction qui verifie si on a termine le fichier
     * si oui, on enregistre ce qu'on a traduit dans le fichier 'o'
     * et puis on sort avec un SUCCESS
     * sinon, on lit un caractere et on le stocke dans 'c'
    */
    char c ;
    if(indexCourant >= toBeParsed.length() && !termine() ) {

        /* Enregistrement de la traduction dans un fichier */
        fstream of;
        of.open(outputFile.c_str(), fstream::out);
        if(of.is_open()){
            of << output;
            of.close();
        }
        endOfFile = true;
    }
    c = toBeParsed[indexCourant];
    indexCourant++;
    return c;
} // Fin lireCar()




int AnalyseLexical::hash(string str) {
    /* fonction de hashage :
     *  1- transforme le mot en miniscule
     *  2- parcours tous ces caracters et multiplie par un nonce
     *  3- retourne un nombre < 100 (taille du tableau des identifiants)
    */ 
    int nonce = 1;
    int hash = 0;
    str = stringToLower(str);
    for(std::string::iterator it=str.begin(); it!=str.end(); ++it){
        hash += ((int)*it ) * nonce; 
        ++nonce;
    }
    return hash % 100;
} // Fin hash(string)



pair<int,int> AnalyseLexical::ajouterIdentifiant(std::string id){
    /* methode qui ajoute un identifiant dans la table des identifiants
     * calcule son hash premierement
     * verifie s'il existe deja un identifiant avec le meme hash
     * -Si oui, on cree une liste chainee et on stocke a la fin d'elle
     * -Sinon, on remplit la case d'indice hash(identifiant)
    */ 
    int hsh = hash(id);
    int indice = 0;
    if(tableauIdentif[hsh] == nullptr) {
        identif *temp = new identif();
        temp->identifiant= stringToLower(id);
        temp->suivant = nullptr;
        tableauIdentif[hsh] = temp;
    } 
    else { 
        identif *ptr = tableauIdentif[hsh];
        identif *temp = new identif() ;
        temp->identifiant = stringToLower(id);
        temp->suivant = nullptr;
        indice++;
        while(ptr->suivant != nullptr) { // parcours pour trouiver le dernier element
            indice++;
            ptr = ptr->suivant;
        }
        ptr->suivant = temp;
    }
    return make_pair(hsh, indice);
} // fin ajouterIDentifiant(string)



pair<int, int> AnalyseLexical::ajouterChaine(std::string ch){
    /* methode qui ajoute une chaine de caracteres dans la table des chaines
     * calcule son hash premierement
     * verifie s'il existe deja une chaine avec le meme hash
     * -Si oui, on cree une liste chainee et on stocke a la fin d'elle
     * -Sinon, on remplit la case d'indice hash(chaine)
    */ 
    int hsh = hash(ch);
    int indice = 0;
    if(Chaines[hsh] == nullptr) {
        chaine temp;
        temp.valeur= stringToLower(ch);
        temp.suivant = nullptr;
        Chaines[hsh] = &temp;
    } 

    else {
        chaine *ptr = Chaines[hsh];
        chaine temp ;
        temp.valeur = stringToLower(ch);
        temp.suivant = nullptr;
		indice++;
        while(ptr->suivant != nullptr) { // parcours pour trouiver le dernier element
            indice++;
            ptr = ptr->suivant;
        }
        ptr->suivant = &temp;
    }
    return make_pair(hsh, indice);
} // fin ajouterChaine(string)



AnalyseLexical::identif* AnalyseLexical::estIdentifiant(string id) { // Verifie si c'est un identifiant
        /* calcule le hash de l'id
         * parcours la liste chainee, s'elle existe, pour le trouver
         * si on termine sans le trouver, c'est pas un identifiant, et on retourne nullptr
         * si on le trouve, on retourne un pointeur vers lui
        */
        int hsh = hash(id);
        identif* ptr = tableauIdentif[hsh];
        while(ptr != nullptr){ // parcours de la chaine
            if(ptr->identifiant.compare(stringToLower(id)) == 0) return ptr;
            ptr = ptr->suivant;
        }
        return nullptr;
} // fin estIDentifiant



string AnalyseLexical::getIdentifiant(int a , int b) {
	if(a == -1 || b == -1) {
		return "Id non existant";
	}
    AnalyseLexical::identif* ptr = tableauIdentif[a];
    for (int i = 0; i < b ; i++) {
        ptr = ptr->suivant;
    }
    if(ptr == nullptr) return "n'existe pas";
    else return ptr->identifiant;
}



string AnalyseLexical::getChaine(int a , int b) {
	if(a == -1 || b == -1) {
		return "Chaine non existante";
	}

    AnalyseLexical::chaine* ptr = Chaines[a];
    for (int i = 0; i < b ; i++) {
        ptr = ptr->suivant;
    }
    if(ptr == nullptr) return "n'existe pas";
    else return ptr->valeur;
}



AnalyseLexical::chaine* AnalyseLexical::estChaine(string ch) { // Verifie si c'est une chaine
        /* calcule le hash de l'ch
         * parcours la liste chainee, s'elle existe, pour le trouver
         * si on termine sans le trouver, c'est pas une chaine, et on retourne nullptr
         * si on le trouve, on retourne un pointeur vers lui
        */
        int hsh = hash(ch);
        chaine* ptr = Chaines[hsh];
        while(ptr != nullptr){ // parcours de la chaine
            if(ptr->valeur.compare(stringToLower(ch)) == 0) return ptr;
            ptr = ptr->suivant;
        }
        return nullptr;
} // fin estIDentifiant



bool AnalyseLexical::estBlanc(char c)
{
    return c == ' ' || c == '\t' || (c == '\n' && ++ligneNombre) || (c == '\r' && ++ligneNombre);
}

bool AnalyseLexical::estLettre(char c)
{
    return ('A' <= c && 'Z' >= c )|| ('a' <= c && 'z' >= c);
}

bool AnalyseLexical::estChiffre(char c)
{
    return '0' <= c && '9' >= c;
}
bool AnalyseLexical::estChaine(char c) {
    return estChiffre(c) || estLettre(c) || c == '!' || c == '<' || c == '>' || c == '='
            || c == '&' || c == '|' || c == '+' || c == '-' || c == '*' || c == '/' || c == ' ' ;
}


void AnalyseLexical::ajouterCar(char c) { // fonction qui ajoute un caractere a une string lors de la lecture sequentielle
    lxNonComplet.push_back(c);
}


bool AnalyseLexical::lireFichier(string f) {
    /* fonction qui lit le fichier input introduit
     * stocke son contenu dans 'toBeParser'
     * le ferme 
     * si le fichier ne veut pas s'ouvrir, on sort du programme
     */ 
    fstream file;
    file.open(f.c_str(), std::fstream::in);
    if(file.is_open()) 
    {  
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            // std::istreambuf_iterator<char>(f) : iterateur qui lit les char depuis le fichier f
            //      lorsqu'il arrivera a la fin du fichier il sera equivalent au constructeur par defaut 
            // std::istreambuf_iterator<char>() : constructeur par defaut qui reference la fin du fichier
        toBeParsed += str; // concatenation possible puisqu'il s'agit de deux strings
        file.close();
        return true;
    }
    cout<< "Fichier " << f << "  n'a pas pu etre lu" << endl;
    exit(1);
}
AnalyseLexical::unite AnalyseLexical::uniteSuivante1() {
	AnalyseLexical::unite un = uniteSuivante();
	cout << un.toString(this) << endl;
	return un;
}


AnalyseLexical::unite AnalyseLexical::uniteSuivante() {
    // fonction qui renvoie l'unite lexical correspondante a une entree selon l'automate 
    while(estBlanc(c)) c = lireCar();
    AnalyseLexical::unite unt;
    unt.ul = ZERO;
    unt.attribut.first = -1; 
    unt.attribut.second= -1; 
    switch(c){
        case '+':{
            unt.ul = PLUS;
            c = lireCar();
            return unt;
        }
        case '-': {
            unt.ul = MOINS;
            c = lireCar();
            return unt;
        }
        case ';': {
            unt.ul = PTVIRG;
            c = lireCar();
            return unt;
        }
        case ',': {
            unt.ul = VIRG;
            c = lireCar();
            return unt;
        }
        case '(':{
            unt.ul = PARENTHOUV;
            c = lireCar();
            return unt;
        }
        case ')': {
            unt.ul = PARENTHFERM;
            c = lireCar();
            return unt;
        }
        case '[':{
            unt.ul = CROCH_OT;
            c = lireCar();
            return unt;
        }
        case ']': {
            unt.ul = CROCH_FM;
            c = lireCar();
            return unt;
        }
        case '{': {
            unt.ul = ACCOLADE_OT;
            c = lireCar();
            return unt;
        }
        case '}': {
            unt.ul = ACCOLADE_FM;
            c = lireCar();
            return unt;
        }
        case '<' :{
            c = lireCar();
            if (c == '=') {
                unt.ul = INFEG;
                c = lireCar();
                return unt;
            } 
            else if (c == '<') {
                unt.ul = BININF;
                c = lireCar();
                return unt;
            }
            unt.ul = INF;
            return unt;
        }
        case '>' : {
            c =lireCar();
            if(c == '=') {
                unt.ul = SUPEG;
                c = lireCar();
                return unt;
            }
            else if(c == '>') {
                unt.ul = BINSUP;
                c = lireCar();
                return unt;
            }
            unt.ul = SUP;
            return unt;
        }
        case '=' : {
            c = lireCar();
            if(c == '=') {
                unt.ul = EGAL;
                c = lireCar();
                return unt;
            }
            unt.ul = AFFECT;
            return unt;
        }
        case '&' : {
            c = lireCar();
            if(c == '&') {
                unt.ul = ET;
                c = lireCar();
                return unt;
            }
            unt.ul = AND;
            return unt;
        }
        case '|' : {
            c = lireCar();
            if(c == '|') {
                unt.ul = OU;
                c = lireCar();
                return unt ;
            }
            unt.ul = BARRE;
            return unt;
        }
        case '!' : {
            c = lireCar();
            if(c == '=') {
                unt.ul = DIFF;
                c = lireCar();
                return unt;
            }
            unt.ul = NON;
            return unt;
        }
        case '/' :{
            c = lireCar();
            if(c == '*') {
                unt.ul = DEBCOMM;
                c = lireCar();
                debutCommentaire = true;
                goto commentaire; // Pour traiter le commentaire
            }
            unt.ul = DIV;
            return unt;
        }
        case '*' : {
            c = lireCar();
            if (c == '/') {
                comm:
                debutCommentaire = false;
                unt.ul = FINCOMM;
                c = lireCar();
                return unt;
            }
            unt.ul = MULTIP;
            return unt;
        }
        case '"' : {
            c = lireCar();
            while(estChaine(c)){
                ajouterCar(c);
                c = lireCar();
            }
            if(c == '"') {
                c = lireCar();
                unt.ul = CH;
                unt.attribut = ajouterChaine(stringToLower(lxNonComplet));
            }
            else unt.ul = ERRCH;
            lxNonComplet = "";
            return unt;
        }
        default : {

            /* On verifie s'il s'agit d'un commentaire */
            if (debutCommentaire) {
            	commentaire:
                debutCommentaire = false;
                if(estChaine(c) || estBlanc(c)|| estChiffre(c)) {
                    // Traitement commentaire
                    ajouterCar(c);
                    char temp ;
                    ligneNombre++;
                    while(true) {
                        c = lireCar();
                        temp = lireCar();
                        if(c == '*' && temp == '/') {
                            indexCourant += 1;
                            break;
                        }
                        else if(temp == '*' && lireCar() == '/'){
                            ajouterCar(c);
                            indexCourant +=1 ;
                            c = temp;
                            break;
                        }
                        else {
                            ajouterCar(c);
                            ajouterCar(temp);
                        }
                    }
                    unt.ul = COMMENT;
                    cout << "Commentaire : " << lxNonComplet<< endl << endl;
                    lxNonComplet = "";
                    uniteSuivante1();
                }
            }
            
            /* On verifie s'il s'agit d'un entier */
            if(estChiffre(c)) {
                ajouterCar(c);
                while(true) {
                    c = lireCar();
                    if(estChiffre(c)) ajouterCar(c);
                    else break;
                }
                unt.ul = CONSTANTE;
                unt.attribut.first = std::stoi(lxNonComplet);
                lxNonComplet ="";
                return unt;
            }

            /* On verifie s'il s'agit d'un identifiant */
            if (estLettre(c)) {
                ajouterCar(c);
                while(true) {
                    c = lireCar();
                    if(estChiffre(c) || estLettre(c) || c=='_' ) ajouterCar(c);
                    else break;
                }

                /* Verification qu'il s'agit d'un mot cle */
                if(motCles[lxNonComplet] == 0 ) { // Donc, identifiant
                    unt.ul = ID;
                    unt.attribut = ajouterIdentifiant(stringToLower(lxNonComplet));
                }
                else { unt.ul = motCles[stringToLower(lxNonComplet)];}
                lxNonComplet ="";
                return unt;
            }
            unt.ul = ERR;
            unt.attribut.first = -1;
            c = lireCar();
            return unt;
        }
    }
} // fin uniteSuivante



std::string AnalyseLexical::enumToString(int a) {
    // utilise pour savoir si l'analyseur lexical fonctionne bien
    // retourne le modele correspondant a chaque lexeme
    switch(a) {
        case 0 : return "rien";
        case 1 : return "int";
        case 2 : return "chaine";
        case 3 : return "Id";
        case 4 : return "void";
        case 5 : return "extern";
        case 6 : return "return";
        case 7 : return "for";
        case 8 : return "while";
        case 9 : return "if";
        case 10 : return "else";
        case 11 : return "constante";
        case 12 : return "{";
        case 13 : return "}";
        case 14 : return "[";
        case 15 : return "]";
        case 16 : return "<=";
        case 17 : return "!=";
        case 18 : return "<";
        case 19 : return "==";
        case 20 : return "+";
        case 21 : return "-";
        case 22 : return "*";
        case 23 : return "/";
        case 24 : return "||";
        case 25 : return "|";
        case 26 : return "&&";
        case 27 : return "&";
        case 28 : return "!";
        case 29 : return ">=";
        case 30 : return ">";
        case 31 : return ",";
        case 32 : return ";";
        case 33 : return "(";
        case 34 : return ")";
        case 35 : return "/*";
        case 36 : return "*/";
        case 37 : return "<<";
        case 38 : return ">>";
        case 39 : return "=";
        case 40 : return "Chaine de Caracteres ";
        case 41 : return "Commentaire";
        case 42 : return "Lire";
        case 43 : return "Ecrire";
        case 44 : return "erreur d'alphabet";
        case 45 : return "erreur chaine non complete";
        default : return "Lexem faux";
    }
} // fin enumToString
