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

	// detruire_tube("ComClientServ");		//Destruction tube communication


	//  if  ( creer_tube("ComClientServ") ==-1 )	//Création tube communication
	//   {
 //      	perror("Problème - création du tube\n"); exit(1);
	//   }
	//   printf("[CREATION OK][COMMUNICATION CLIENT]\n");


		Client[0]=fork();
		if(Client[0]==0)
		{
		
		
		//CONSTRUCTION DU NOM
		char *N=NULL;			//Initialisation chaine de caractere qui recevra le PID courant
		char client[]="Client_";
		char pipe[]="_pipe";

	   	N = inttochar(getpid());		//Conversion du PID en chaine
	   	
	   	strcat(client, N);		//Concaténation
	   	strcat(client, pipe);
	   	

	   	//DESTRUCTION
	   	detruire_tube(client);	

	   	//CREATION	
	   	if  ( creer_tube(client) ==-1 )	//Création tube
		  {
	      	perror("Problème - création du tube, client \n"); exit(1);
		  }

		printf("[CREATION OK][CLIENT %d] - ID : %s \n", i, client);


		//REQUETE DU CLIENT
		//Generation nombres aléatoires et opérande
		srand(getpid()<<16);
		n1= rand()%9+1; n2=rand()%9+1;

		nb1=inttochar( n1 );
		nb2=inttochar( n2 );
		operande="+";

		strcat(nb1, operande);	//Mise en forme de la requete sous forme de chaine de caractere
		strcat(nb1, nb2);
		req=nb1;



		
		requete[i].client_pid=getpid();		//PID client
		strcpy(requete[i].expression, req);//Expression a evaluer

		//Ecriture dans le tube
		tube=open("ComClientServ", O_WRONLY);
		write( tube, (char*)&requete[i], sizeof(requete[i]) );

		printf("[CLIENT %d] émission - Requete envoyée : PID = %d, Expression = %s\n", i, requete[i].client_pid , requete[i].expression);

		close(tube);

		//strlen(requete[i].expression)+strlen(requete[i].client_pid)


		//LECTURE DE LA REPONSE PAR LE CLIENT
		tube=open(client, O_RDONLY);
		read(tube, &res, sizeof(res)); 	//Lecture
		
		printf("[CLIENT %d] réception - Expression : %d\n", i , res);
		close(tube);

		exit(0);
	   	
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