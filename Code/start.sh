g++ AnalyseSyntaxique.cpp AnalyseSyntaxique.h AnalyseLexical.h AnalyseLexical.cpp MotCle.h AnalyseSemantique.h AnalyseSemantique.cpp main2.cpp -std=c++11

./a.out $1 > lexical.txt

g++ AnalyseSyntaxique.cpp AnalyseSyntaxique.h AnalyseLexical.h AnalyseLexical.cpp MotCle.h AnalyseSemantique.h AnalyseSemantique.cpp main1.cpp -std=c++11

./a.out $1 > syn_sem.txt


