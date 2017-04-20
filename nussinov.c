#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define EOF (-1)

//code avec verification des appariements

typedef struct app {
	char base; 
	int position;
	int eng; 
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
		 
	if (a == 'a' && b == 'u')
		return 1;
	if (a == 'u' && b == 'a')
		return 1; 
	if (a == 'g' && b == 'c')
		return 1; 
	if (a == 'c' && b == 'g')
		return 1; 
		
	return 0;	
}

APP** remplir_matrice (APP** tab, int size, char* seq){	// remplir la matrice
	int i, j, k, energie, position = 0;
	int cont = 1;
	
	while (cont < size){	
		j = cont;
		for (i = 0; i < size; ++i){	
			if (j > i) 
				tab[i][j].eng = tab[i][j-1].eng; 
			for (k = i; k<j ; ++k){								
				if (liaison_possible(seq[j], seq[k]) ) {
					if (k > 0){	
						energie = tab[i][k-1].eng+tab[k+1][j-1].eng+1;   
						if (energie > tab[i][j].eng){
							tab[i][j].eng = energie;
						}
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
	char* C = malloc(size*sizeof(char));
	
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

void verif (APP** tab, int size, char* seq){
	int i, j; 
	
	printf("   ");
	for(i = 0; i<size; i++)
		printf("  %c", seq[i]);
	printf("\n");	
	printf("\n");
	
	for(i = 0; i<size; i++){
		printf(" %c  ", seq[i]);
		for (j = 0; j<size; j++){
			if (tab[i][j].eng >= 0 && tab[i][j].eng <= 10)
				printf(" %d ", tab[i][j].eng);
			else 
				printf("%d ", tab[i][j].eng);
		}
		printf ("\n");	
	}
}
	
	
char * recupere_seq(char * seq, char * nom_fichier)
{
	int i=0,fd,e=1; char c='a'; 
	fd=open(nom_fichier, O_RDONLY);
	while(e && c!='\0')
	{
		e=read(fd,&c,1);
		if (c!='\0' && c!='\n' && c!=' ' && c!='\t')
		{
			seq[i]=c;
			i++;
		}
	}
	printf("seq= %s \n de taille %d", seq, strlen(seq));
	return seq;
}		
	
int verifie_exec(int argc, char ** argv)
{
	if (argc!=2) 
	{
		printf("Le programme n'a pas été appelé correctement. Veuillez réessayer en donnant en argument un fichier contenant une séquence d'ARN à tester.");
		return 0;
	}
	
	return 1;
}	
	
int verifie_fichier(char * nom_fichier)
{
	int fd; char c='a';
	fd=open(nom_fichier, O_RDONLY);
	while (c!='\0')
	{
		read(fd,&c,1);
		if (c=='A' || c=='C' || c=='G' || c=='U' || c=='a' || c=='c' || c=='g' || c=='u')
			{
				return 1;
			}
	}
	printf("Le caractere %c a ete lu dans le fichier %s, veuillez ne mettre que des caractères venant de l'alphabet {a,u,g,c,A,U,G,C}", c, nom_fichier);	
	return 0;
}	
	
int main(int argc, char** argv){
	long clk_tck = CLOCKS_PER_SEC;
	clock_t t1, t2;
	t1=clock();
	APP **tab; 
	char* seq = malloc(10000*sizeof(char*));
	char* str = malloc(sizeof(char*));
	int size; 
	//~ struct timeval tv; 
	//~ struct timezone tz;
	
	
	if(verifie_exec(argc, argv))
	{
		if (verifie_fichier(argv[1]))
		{
			seq = recupere_seq(seq,argv[1]);   				//argv[1];
			size = strlen(seq); 
			
			tab = alloc_matrice(tab, size);						
			tab = init_matrice(tab, size, seq); 
			tab = remplir_matrice(tab, size, seq);			
			
			printf("\n");
			verif(tab, size, seq);
			str = rebroussement(tab, size);
			
			printf("\n");
			printf(" taille de la séquence : %d \n", size);
			printf(" séquence      :  %s \n", seq);
			printf(" appariemments :  %s \n", str);

			free_matrice(tab, size);
		}
	}	
	t2=clock();
	printf("Temps consomme (s) : %lf \n",
                ((double)(t2-t1)/(double)clk_tck));
	return 0; 
}	
	
/*
int main(int argc, char** argv){
	APP **tab; 
	char* seq = malloc(sizeof(char*));
	char* str = malloc(sizeof(char*));
	int size; 

	seq = recupere_seq(seq,argv[1]);   				//argv[1];
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
	
*/
