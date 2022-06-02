
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "FuncionesDeCola.h"  



COLA puente,fila_izquierda,fila_derecha;
int c=0;
pthread_mutex_t lock;

int entrada_comando(char [],char []);
void status(int);
void posxy(int, int);
void car(char [], int );
void *car_runtime();
void *starto(void *);
char getch();

int main(int argc, char const *argv[]) {
	system("clear");

	pthread_t p1,p2,p3;

	pthread_mutex_init(&lock,NULL);//inicializa el mutex referenciado por lock, con attr igual NULL ,entonces se usa los atributos mutex predeterminados Tras una inicialización exitosa, el estado del mutex se inicializa y desbloquea

	inicializar(&fila_izquierda);
	inicializar(&fila_derecha);
	inicializar(&puente);

	char comando[15];
	char id_auto[10];
	char end='\0';

	do{
		puts("Ingrese un comando: ");
		comando[0] = end;
		id_auto[0] = end;
		scanf("%15[^\n]",comando);
		getchar();
		strcat(comando,&end);//concatena comando con \0 
		int accion = entrada_comando(comando,id_auto);
		switch (accion){
			case 0:
				car(id_auto,0); // derecha
				break;
			case 1:
				car(id_auto,1); //izquierda
				break;
			case 3:
				status(-1); 
				break;
			case 4:		
				break;
			default:
				printf("ERROR: Comando no valido\n");
		}
		if(accion!=4)while(getchar() != '\n');
		system("clear");
	}while(strcmp(comando,"start"));

	status(-1);
	sleep(1);//para que duerma 1 segundo  
	int izq=1,der=0;
	pthread_create(&p1,NULL,&car_runtime,NULL);
	pthread_create(&p2,NULL,&starto,(void*)&izq);	//izq
	pthread_create(&p3,NULL,&starto,(void*)&der);	//der
	pthread_join(p3,NULL);
	pthread_join(p2,NULL);
	pthread_join(p1,NULL);

	pthread_mutex_destroy(&lock);

	return 0;
}

void car(char id_auto[], int sentido){
	COLA *cola = sentido? &fila_izquierda:&fila_derecha;
	if(!estaRepetido(fila_izquierda,fila_derecha,id_auto)){
        encolar(cola,id_auto);
        printf("Auto agregado a la cola %s", sentido? "izquierda":"derecha");
    }else puts("ERROR: no se pueden agregar autos repetidos");
}

// en esta parte se analisan los comandos ingresados
int entrada_comando(char comando[],char id_auto[]){
	char aux[4]="", direccion[4];
	strncpy(aux,comando,3);//copia 3 caracteres de comando a aux , "car"
	aux[3] = '\0';
	if(!strcmp(aux,"car") && strlen(comando) > 3 ){
		strncpy(id_auto, &comando[4],6);// copia en id_auto "auto01" es lo que recibe como identificador
		id_auto[6] = '\0';
		strncpy(direccion, &comando[11],3);// agrega en la direccion correspondiente "izq o der"
		direccion[3] = '\0';
		if(!strcmp(direccion,"izq")) return 1;
		if(!strcmp(direccion,"der")) return 0;
	}else if(!strcmp(comando,"status")) return 3;
	else if(!strcmp(comando,"start")) return 4;
	return -1;
}

void status(int sentido){
  	char bridge[] = "=======================================================";
	system("clear");
	int i;
	for( i = 1 ; i <= 5 ; i++ ){ //inanicion 
		posxy(i,15);//  aqui se cambia el sentido
		printf("%s",bridge);
	}
	NODO_COLA *auxd = fila_derecha.cabeza , *auxi = fila_izquierda.cabeza ;
	i++;
	posxy(i,1);
	printf("**Autos en espera a la izquierda**");
	posxy(i,50);
	printf("**Autos en espera a la derecha**\n");
	i++;
	while( auxd || auxi ){
		if(auxi){
	  		posxy(i,1);
	  		printf("=> %s",auxi->id_auto);
	  		auxi = auxi->sig;
		}
		if(auxd){
			posxy(i,50);
	  		printf("<= %s",auxd->id_auto);
	  		auxd = auxd->sig;
		}
		i++;
	}
	i++;

	if(~sentido){
		char raya_auto[2][11] = {{"<<<<<<<<<<"},{">>>>>>>>>>"}};
		int j;
		int pos1[3] = { 14 , 32 , 50 }, pos2[3] = { 50 , 32 , 14 };//posiciones en donde salta el auto de izq a der
		incrementar(&puente);
		NODO_COLA *aux = puente.cabeza;
		while(aux){
			if(aux->pos_puente > 0){
				for(j=2 ; j<=4; j++){
					posxy(j, sentido ? pos1[aux->pos_puente-1] : pos2[aux->pos_puente-1]);
					printf("%s",raya_auto[sentido]);	
				}
				posxy(3,sentido ? pos1[aux->pos_puente-1] + 2 : pos2[aux->pos_puente-1] + 2 );
				printf("%s",aux->id_auto);
			}
			aux = aux->sig;
		}
	}
	fflush(stdout);
	posxy(i,1);

	printf("Ingrese cualquier tecla para add autos y al terminar la simulacion,\nhaga lo mismo para finalizar el programa...\n\n");
}


void *starto(void *data){
	int lado = *((int* )data),i;

	COLA *cola = lado? &fila_izquierda:&fila_derecha; //1 = izq    0 = der
	COLA *cola_opuesto = lado ? &fila_derecha : &fila_izquierda; //mod

	if( !lado ){
		pthread_mutex_lock(&lock);
		usleep(100000);
		pthread_mutex_unlock(&lock);
	}

	while( !vacio(*cola) || !vacio(*cola_opuesto) || !vacio(puente) ){
		if( !vacio(*cola) ){
	    	pthread_mutex_lock(&lock);
	    	for( i = 0 ; ( i < 4 || vacio(*cola_opuesto) ) && !vacio(*cola); i++ ){
	    		while(c);
	        	encolar(&puente,desencolar(cola));
	        	cantidad(puente) > 3 ? desencolar(&puente) : "";
	        	status(lado);
	        	sleep(1);
	    	}
	    	while(!vacio(puente)){
	    		while(c);
	    		if(puente.cabeza->pos_puente==3) desencolar(&puente);
	    		status(lado);
	    		sleep(1);
	    	}
	    	pthread_mutex_unlock(&lock);
	    }
	    sleep(1);
  	}
  	
}


void *car_runtime(){
	char parametro[15] , entrada[15] , id_auto[7] ,end='\0';
	while( !vacio(puente) || !vacio(fila_izquierda) || !vacio(fila_derecha) ){
		for(int i=0;i<15;i++)parametro[i]=0;
		sprintf(parametro,"car ");
		fflush(stdin);
		getch();
		if( vacio(puente) && vacio(fila_izquierda) && vacio(fila_derecha) ) break;
		c=1;
		printf("==> car ");
		scanf("%15[^\n]",entrada);
		getchar();
		strcat(entrada,&end);
		strcat(parametro,entrada);
		int accion = entrada_comando(parametro,id_auto);
		car(id_auto,accion);
		c=0;
	}
}

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    //printf("%c\n",buf);
    return buf;
}

void posxy(int x, int y){
	printf("\033[%d;%df",x,y);
  	fflush(stdout);
}

