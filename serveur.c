//EXERCICE 6


#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define NBCLI 7 			//Nombre de clients

int creer_tube(char NomTube[]);
void detruire_tube(char NomTube[]);
char* inttochar(unsigned n);

struct requete_client_serveur
{
	int client_pid;		//PID du client
	char expression[20];		//Expression a evaluer

	
};

int main(void)
{

	int i, Client[NBCLI], longueur;
	char *nb1="0", *operande="+", *nb2="0", *req="";
	

	

	struct requete_client_serveur requete[NBCLI];

	struct requete_client_serveur lectreq; 
	

	int tube=0;

	int n1, n2, res;

	//srand(time(NULL));		//Init tirage aléatoire
	//srand(getpid()<<16);

	detruire_tube("ComClientServ");		//Destruction tube communication


	 if  ( creer_tube("ComClientServ") ==-1 )	//Création tube communication
	  {
      	perror("Problème - création du tube\n"); exit(1);
	  }
	  printf("[CREATION OK][COMMUNICATION CLIENT]\n");

		
		//LECTURE DE LA REQUETE PAR LE SERVEUR
		while(1)
		{
		tube=open("ComClientServ", O_RDONLY);
		read(tube, &lectreq.client_pid, sizeof(lectreq.client_pid)); 	//Lecture
		longueur=read(tube, &lectreq.expression, sizeof(lectreq.expression)); 	
		lectreq.expression[longueur]='\0';

		printf("[SERVEUR] réception : Client %d - Expression : %s\n", lectreq.client_pid, lectreq.expression);
		close(tube);


		//Traitement - ENVOIE DE LA REPONSE DU SERVEUR
		static int opd1, opd2; 
		static char opr;

		fflush(stdin);
		sscanf( lectreq.expression, "%d %1s %d", &opd1, &opr, &opd2);		//Identification des termes
		res=opd1+opd2;	//Calcul resultat

		//printf("le resultat de %d et %d est %d", opd1, opd2, res);

		char *N=NULL;			//Initialisation chaine de caractere qui recevra le PID courant
		char client[]="Client_";
		char pipe[]="_pipe";

		N = inttochar(lectreq.client_pid);		//Conversion du PID en chaine
	   	strcat(client, N);		//Concaténation
	   	strcat(client, pipe);
	   	 //printf("******%s\n", client);

	   	
		tube=open(client, O_WRONLY);
		
		//write(tube, requete[i], strlen(requete[i].expression)+strlen( inttochar(requete[i].client_pid))); 	//On stocke les données d'expression dans le tube (ecriture)
		write( tube, &res, sizeof(res) );
		printf("[SERVEUR] émission : %s - Expression : %d\n", client, res);

		close(tube);




		}
	

	return 0;
}

int creer_tube(char NomTube[])
{
	return mkfifo(NomTube, S_IRWXU|S_IRWXG|S_IRWXO);		//Création tube analogue
}

void detruire_tube(char NomTube[])
{
	unlink(NomTube);		//Destruction tube
}

char* inttochar(unsigned n)
{
    char *str = NULL;
    int s = 0 ,i = n;
    while(i)
        i /= 10, s++;
    str = malloc((s+1)*sizeof(char));
    if (str == NULL)
        return NULL;
    for (i = s-1 ; i >= 0 ; i--, n /= 10)
        str[i] = n % 10 + '0';
    str[s] = '\0';
    return str;
}