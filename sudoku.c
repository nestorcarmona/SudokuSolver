#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>

/* Implanter les fonctions de sudoku.h ici */
T_coordonnees obtenirCoords(int indice){
	T_coordonnees coord;
	coord.ligne = indice/9;
	coord.colonne = indice%9;
	return coord;
}

int obtenirIndice(T_coordonnees coords){
	int indice;
	indice = 9*coords.ligne + coords.colonne;
	return indice;
}

T_coordonnees debutRegion(int indiceRegion){
	T_coordonnees coords;
	coords.ligne = 3*(indiceRegion/3);
	coords.colonne = 3*(indiceRegion%3);
	return coords;
}

int indiceRegion(T_coordonnees coords){
	int indice;
	indice = 3*(coords.ligne/3) + (coords.colonne/3);
	return indice;
}

T_sudoku lireSudoku(char* chemin){
	FILE *file;
	T_sudoku sudoku;
	char ch;
	int i=0;
	file = fopen(chemin, "r");
	int candidats[9] = {1,2,3,4,5,6,7,8,9};
	int candidats2[9] = {0,0,0,0,0,0,0,0,0};
	if(file == NULL){
		printf("Erreur ouverture fichier");
	}

	// On change val, n_candidats et candidats
	while((ch = fgetc(file)) != EOF){
		if(ch != ' ' && ch != '\n'){
			sudoku.grille[i].val = ch - '0';

			if(ch-'0' == 0){
				sudoku.grille[i].n_candidats = 9;
				for(int j=0;j<9;j++){
					sudoku.grille[i].candidats[j] = candidats[j];
				}
				
			}
			else {
				sudoku.grille[i].n_candidats = 0;
				for(int j=0;j<9;j++){
					sudoku.grille[i].candidats[j] = candidats2[j];
				}
			}
			i += 1;
		}
	}


	return sudoku;

}

void initialiserSudoku(T_sudoku* s){
	int z=1;

	for(int i=0;i<81;i++){
		if((s->grille[i]).val == 0){
			(s->grille[i]).n_candidats = 9;
			for(int j=0;j<9;j++){
				(s-> grille[i]).candidats[j] = z;
				z++;
			}
			z = 1;
		}
		else{
			(s->grille[i]).val = (s->grille[i]).val;
			(s->grille[i]).n_candidats = 0;

		}
	}
}

void supprimerValeur(int ival, T_case* pc){
	//On supprime et réduit ncandidat
	for(int z = 0;z < 9; z++){
		if(pc->candidats[z] == ival){
			pc->candidats[z] = 0;
			pc->n_candidats -= 1;
			break;
		}
	}

	//On remet bien la liste
	int count = 0;
    for (int i = 0; i < 9; i++){
        if (pc->candidats[i] != 0) pc->candidats[count++] = pc->candidats[i];
	}
    while (count < 9)  pc->candidats[count++] = 0;
}

int R1_case(T_case* pc){
	int output=0;
	//Un seul candidat dans la case
	if(pc->n_candidats == 1){
		//On enlève candidat de la liste des candidats
		pc->val = pc->candidats[0];
		pc->candidats[0] = 0;
		pc->n_candidats = 0;
		output = 1;
	} 
	return output;
}

int R1_sudoku(T_sudoku* ps){
	int output=0,j;

	for(int i=0;i<81;i++){
		//Si T: on retourne 1; Si F: retourne 0	
		if(ps->grille[i].val == 0 && R1_case(&(ps->grille[i])) == 1){
			output = 1;
			int indiceRegionCase = indiceRegion(obtenirCoords(i));
			int indicePremCase = obtenirIndice(debutRegion(indiceRegion(obtenirCoords(i))));

			//On parcourt les cases de la région
			for(int j=indicePremCase;j<=indicePremCase+20;j++){
				// On contrôle si on est encore dans la bonne région. On retourne au bon endroit
				if(indiceRegion(obtenirCoords(j)) != indiceRegionCase){
					j += 5;
				}
				else{
					//On vérifie qu'on est pas dans la même case
					if(j != i && ps->grille[j].n_candidats != 0) supprimerValeur(ps->grille[i].val,&ps->grille[j]);
				}
				
				
			}

			// On enlève le candidat de la liste des candidats de la ligne de la case
			int indiceLigne = obtenirCoords(i).ligne;
			int indiceColonne = obtenirCoords(i).colonne;

			for(j=i-indiceColonne;j<i-indiceColonne+9;j++){
				if(j != i && ps->grille[j].n_candidats != 0) supprimerValeur(ps->grille[i].val,&ps->grille[j]);
			}

			//On enlève le candidat de la liste des candidats de la colonne de la case
			for(j=indiceColonne;j<=indiceColonne+72; j +=9){
				if(j != i && ps->grille[j].n_candidats != 0) supprimerValeur(ps->grille[i].val,&ps->grille[j]);
			}

		}
	}

	return output;
}

int R2_case(int indCase, T_sudoku* pc){
	//Initialisations
	int i, j, val=0;
	int indiceLigne = obtenirCoords(indCase).ligne;
	int indiceColonne = obtenirCoords(indCase).colonne;

	//On contrôle la région
	int indiceRegionCase = indiceRegion(obtenirCoords(indCase));
	int indicePremCase = obtenirIndice(debutRegion(indiceRegion(obtenirCoords(indCase))));

	for(int i=indicePremCase;i<=indicePremCase+20;i++){
		if(i != indCase){
			// On contrôle si on est encore dans la bonne région.
			if(indiceRegion(obtenirCoords(i)) != indiceRegionCase) i += 5;

			else {
				if((pc->grille[i]).val != 0){
					//On enlève le candidat de la liste de notre case et du nbr de candidats
					supprimerValeur(pc->grille[i].val,&pc->grille[indCase]);
				}
			}
		}
		
	}

	//On contrôle la ligne
	for(i=indCase-indiceColonne;i<indCase-indiceColonne+9;i++){
		if(i != indCase && (pc->grille[i]).val != 0){
			//On enlève le candidat de la liste de notre case et du nbr de candidats
			supprimerValeur(pc->grille[i].val,&pc->grille[indCase]);
		}	
	}




	//On contrôle la colonne
	for(i=indiceColonne;i<=indiceColonne+72; i = i+9){
		if(i != indCase && (pc->grille[i]).val != 0){
			//On enlève le candidat de la liste de notre case et du nbr de candidats
			supprimerValeur(pc->grille[i].val,&pc->grille[indCase]);
		}
	}
	return val;
}

int R2_sudoku(T_sudoku* ps){
	int i, j=0, val=0, count = 1;
	while(count != 0){
		count = 0;
		for(i = 0; i<81; i++){
			if((ps->grille[i]).val == 0){
				if(R2_case(i,ps)==1){
					R1_case(&(ps->grille[i]));
					val = 1;
					count += 1;
				}
			}
		}
	}
	

	return val;
}

int R3_case(int indCase, T_sudoku* ps){
	int i, j,z,counter=0, val=0, test=0;
	int indiceLigne = obtenirCoords(indCase).ligne;
	int indiceColonne = obtenirCoords(indCase).colonne;

	for(i=0;i<9;i++){
		if((ps->grille[indCase]).candidats[i] == 0) break;

		//On contrôle les candidats dans la ligne
		for(j=indCase-indiceColonne;j<indCase-indiceColonne+9;j++){
			if(j!=indCase){
				for(z=0;z<9;z++){
					if((ps->grille[j]).candidats[z] == 0) break; 

					if((ps->grille[indCase]).candidats[i] != (ps->grille[j]).candidats[z] && (ps->grille[j]).candidats[z] != 0) counter += 1; 

					else {
						test = 1;
						counter = 0;
						break;
					}
				}
			}
			if(test == 1) break;
		}

		test = 0;
		if(counter != 0){
			val = 1;
			(ps->grille[indCase]).val = (ps->grille[indCase]).candidats[i];

			for(z=0;z<9;z++) (ps->grille[indCase]).candidats[z] = 0;

			(ps->grille[indCase]).n_candidats = 0;
			break;
		}

		//On contrôle les candidats dans la colonne
		for(j=indiceColonne;j<=indiceColonne+72; j = j+9){
			if(j!=indCase){
				for(z=0;z<9;z++){
					if((ps->grille[j]).candidats[z] == 0) break;
					if((ps->grille[indCase]).candidats[i] != (ps->grille[j]).candidats[z] && (ps->grille[j]).candidats[z] != 0) counter += 1; 

					else{
						test = 1;
						counter = 0;
						break;
					}
				}
			}
			if(test == 1) break;
		}

		test = 0;
		if(counter != 0){
			val = 1;
			(ps->grille[indCase]).val = (ps->grille[indCase]).candidats[i];

			for(z=0;z<9;z++) (ps->grille[indCase]).candidats[z] = 0;
			(ps->grille[indCase]).n_candidats = 0;
			break;
		}

		//On contrôle les candidats dans la région
		int indiceRegionCase = indiceRegion(obtenirCoords(indCase));
		int indicePremCase = obtenirIndice(debutRegion(indiceRegion(obtenirCoords(indCase))));
		for(j=indicePremCase;j<=indicePremCase+20;j++){
			// On contrôle si on est encore dans la bonne région.
			if(indiceRegion(obtenirCoords(j)) != indiceRegionCase) j += 5;

			if(j!=indCase){

				for(z=0;z<9;z++){

					if((ps->grille[j]).candidats[z] == 0) break;

					if((ps->grille[indCase]).candidats[i] != (ps->grille[j]).candidats[z] && (ps->grille[j]).candidats[z] != 0) counter += 1; 

					else{
						test = 1;
						counter = 0;
						break;
					}
					
				}
				if(test == 1) break;
			}
		}

		test = 0;

		if(counter != 0){
			val = 1;
			(ps->grille[indCase]).val = (ps->grille[indCase]).candidats[i];

			for(z=0;z<9;z++) (ps->grille[indCase]).candidats[z] = 0;

			(ps->grille[indCase]).n_candidats = 0;
			break;
		}
	}

	return val;
}

int R3_sudoku(T_sudoku* ps){
	int i, j=0, val=0, count = 1;
	
	while(count != 0){
		count = 0;
		for(i = 0; i<81; i++){
			if((ps->grille[i]).val == 0){
				//On effectue une modification
				if(R3_case(i,ps)==1){
					val = 1;
					count += 1;
					R2_sudoku(ps);
				}
			}
		}
	}
	return val;
}

//Fonction qui controle les repetitions d'une liste 
int repet(int tab[]){ 
	int i, j; 
	for(i = 0; i < 9; i++) for(j = i+1; j < 9; j++) if(tab[i] == tab[j]) return 0;
	
	return 1;
}  

//Fonction qui controle si le sudoku est valide
int valide(T_sudoku s){
	//Initialisations
	int sum=0,tab[10],tab2[10],i,j,z,k,count=0,indRegion=-1,indRegionCour=-1,count2=0,sum2=0;

	for(i=0;i<9;i++){
		//On contrôle la ligne
		for(j=i*9;j<9+i*9;j++){
			
			tab[count] = s.grille[j].val;
			sum += s.grille[j].val;
			count += 1;
					
			//On contrôle la région
			indRegionCour = indiceRegion(obtenirCoords(j));
			if(indRegionCour > indRegion){
				indRegion = indRegionCour;
				for(k=j;k<j+21;k++){
					if(indiceRegion(obtenirCoords(k)) != indRegionCour) k += 6; 

					tab2[count2] = s.grille[k].val;
					sum2 += s.grille[k].val;
					count2+= 1;

				}
				if(sum2 != 45 || repet(tab2) == 0) return 0;

				count2 = 0;
				sum2 = 0;
			}
			
		}
		
		

		if(sum != 45 || repet(tab) == 0) return 0;

		//On réinitialise les variables après les tests
		sum = 0;
		count = 0;

		//On contrôle la colonne
		for(z=i;z<8*9+i+1;z += 9){
			tab[count] = s.grille[z].val;
			sum += s.grille[z].val;
			count += 1;
		}
		if(sum != 45 || repet(tab) == 0) return 0; 

		//On réinitialise les variables après les tests
		sum = 0;
		count = 0;
	}	
	return 1;
}

int verifResultat(T_sudoku initialS,T_sudoku s){
	for(int i=0;i<81;i++){
		if(initialS.grille[i].val != 0){
			if(initialS.grille[i].val != s.grille[i].val && valide(s) == 1) return 0;
		}
	}
	return 1;
}

//Fonction qui regarde les repetitions d'une liste sans tenir compte des 0
int repetSans0(int tab[]){ 
	int i, j; 
	for(i = 0; i < 9; i++) {
		for(j = i+1; j < 9; j++){
			//Pour eviter de faire trop de calculs
			if(tab[i] == 0) break;
			//0: Il y a repetition
			if(tab[i] == tab[j]) return 0;
		} 
	}
	return 1;
} 

//Fonction qui controle si changer la valeur à la position est valide
int posValide(T_sudoku s,int index, int valeur){
	//Toutes les initialisations
	int tab[10],tab2[10],i,j,z,k,count=0,indRegion=-1,indRegionCour=-1,count2=0;

	//On essaye cette valeur
	s.grille[index].val = valeur;

	for(i=0;i<9;i++){
		//On contrôle la ligne
		for(j=i*9;j<9+i*9;j++){
			tab[count] = s.grille[j].val;
			count += 1;

			//On contrôle la région
			if(indiceRegion(obtenirCoords(j)) > indRegion){
				indRegion = indiceRegion(obtenirCoords(j));
				for(k=j;k<j+21;k++){
					if(indiceRegion(obtenirCoords(k)) != indiceRegion(obtenirCoords(j))){
						k += 6;
					}
					tab2[count2] = s.grille[k].val;
					count2+= 1;

				}
				if(repetSans0(tab2) == 0) return 0;

				count2 = 0;
			}
			
		}
		if(repetSans0(tab) == 0) return 0;

		//On réinitialise les variables après les tests
		count = 0;

		//On contrôle la colonne
		for(z=i;z<8*9+i+1;z += 9){
			tab[count] = s.grille[z].val;
			count += 1;
		}
		if(repetSans0(tab) == 0) return 0;

		//On réinitialise les variables après les tests
		count = 0;
	}	

	return 1;
}

int bruteForce(T_sudoku* ps){

	int index,i;
	//Si le sudoku est plein et valide, c'est bon
	if(valide(*ps)) return 1;

	//On cherche une case vide
	for(int i=0;i<81;i++) if(ps->grille[i].val == 0) index = i; 

	//On cherche un candidat parmis les potentiels
	for(i = 0; i<9; i++){
		if(ps->grille[index].candidats[i] != 0){
			//La position de ce candidat est valide
			if(posValide(*ps,index,ps->grille[index].candidats[i]) == 1){
				ps->grille[index].val = ps->grille[index].candidats[i];
				//Etape recursive 
				if(bruteForce(ps) == 1) return 1;

				//La recursion marche pas, on remet la valeur a 0
				ps->grille[index].val = 0;
			}
		}
		//Candidat est 0, on sort de la boucle
		else {
			break;
		}
	}

	return 0;
}