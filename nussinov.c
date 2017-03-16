#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "dirent.h"
#include "stdio.h"
#include "string.h"

#define EOF (-1)

//code avec verification des appariements

typedef struct app {
	char base; 
	int position;
	int eng; 
	//int position_lie; 	
}APP;

APP** alloc_matrice (APP** tab, int size){	//allocation mémoire
	int i; 
	
	tab = malloc (size*sizeof(APP*)); 
	for (i = 0; i<size; ++i)
		tab[i] = malloc(size*sizeof(APP));	
	
	return tab; 
}

void free_matrice (APP** tab, int size){
	int i; 
	
	for (i = size-1; i >= size; --i)
		free(tab[i]);
		
	free(tab);
}

APP** init_matrice (APP** tab, int size, char* seq){    //initialisation de la matrice
	int i, j; 
		
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j){
			//tab[i][j].position_lie = -1;
			tab[i][j].position = j;
			tab[i][j].base = seq[j];
			if (j > i)
				tab[i][j].eng = tab[i][j-1].eng;
			else if (j == i || j == i-1)
					tab[i][j].eng = 0;	
				else 
					tab[i][j].eng = -1; 
		}
	}
	
	return tab;
}

int liaison_possible(char a, char b){					//fonction appariement des bases possibles ?
	if (a == 'A' && b == 'U')
		return 1;
	if (a == 'U' && b == 'A')
		return 1; 
	if (a == 'G' && b == 'C')
		return 1; 
	if (a == 'C' && b == 'G')
		return 1; 
	
	return 0;	
}

APP** remplir_matrice (APP** tab, int size, char* seq){	// remplir la matrice
	int i, j, k, energie, position = 0;
	int cont = 1;
	
	while (cont < size){	
		j = cont;
		for (i = 0; i < size; ++i){	
			if (j > i) 	tab[i][j].eng = tab[i][j-1].eng;
				for (k = i; k<j; ++k){								
					if (liaison_possible(seq[j], seq[k]) ) {
						energie = tab[i][k-1].eng+tab[k+1][j-1].eng+1;   
						if (energie > tab[i][j].eng){
							tab[i][j].eng = energie;
							//tab[i][j].position_lie = k;
						}
					}
				}
				++j;
			}
			++cont;
		}
		
	return tab;		
}

char* rebroussement ( APP** tab, int size){
	int i, j, k, c; 
	c = 0;
	char C[size];
	
	for(i = 0; i < size; ++i)
		C[i] = '-';
	
	i = 0; 
	j = size-1;		
	while(i != j){
		while(tab[i][j].eng > tab[i+1][j-1].eng && tab[i+1][j-1].eng >= 0){
			C[j] = ')';
			C[i] = '(';
			++i;
			--j;					
		}
		if (i != j) --j;			
	}
			
	return C; 		
}

void verif (APP** tab, int size){
	int i, j; 
	
	printf ("matrice .eng \n");
	for(i = 0; i<size; i++){
		for (j = 0; j<size; j++)
			printf("%d ", tab[i][j].eng);
		printf ("\n");	
	}
}
	
int main(/*int argc, char** argv*/){
	APP **tab; 
	char* seq = malloc(sizeof(char*));
	char* str = malloc(sizeof(char*));
	int size; 

	seq = "GCUAGUACGUACUCUGCUAGU";   				//argv[1];
	size = strlen(seq); 
	
	tab = alloc_matrice(tab, size);						
	tab = init_matrice(tab, size, seq); 				
	tab = remplir_matrice(tab, size, seq);			
	
	verif(tab, size);
	str = rebroussement(tab, size);
	
	printf("%d \n", size);
	printf("%s \n", seq);
	printf("%s \n", str);

	//free_matrice(tab, size);
	
	return 0; 
}	
	