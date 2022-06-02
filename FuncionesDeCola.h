

typedef struct nodo{
    char id_auto[7];
    int pos_puente;
    struct nodo *sig;
}NODO_COLA;

typedef struct cola{
    NODO_COLA *cabeza,*pie;
    int cant;
}COLA;

void inicializar(COLA *);
NODO_COLA* crear(char []);
void encolar(COLA *,char []);
char* desencolar(COLA *);
int estaRepetido(COLA ,COLA,char []);
void incrementar(COLA *);
int vacio(COLA c);
int cantidad(COLA c);

void inicializar(COLA *c){
  c->cabeza = c->pie = NULL;
  c->cant = 0;
}

NODO_COLA* crear(char nombre[]){
    NODO_COLA *temp = (NODO_COLA*)malloc(sizeof(NODO_COLA));
    strcpy(temp->id_auto,nombre);
    temp->pos_puente = 0;
    temp->sig = NULL;
    return temp;
}

void encolar(COLA *cola,char nombre[]){
    NODO_COLA *nodo = crear(nombre);
    if((*cola).cabeza==NULL) (*cola).cabeza = nodo;
    else (*cola).pie->sig = nodo;
    (*cola).pie = nodo;
    cola->cant++;
}

char* desencolar(COLA *cola){
    char *ret = NULL;
    if (cola!=NULL){
        if((cola->cabeza)!=NULL){
            ret = (char*)calloc(7,sizeof(char));
            strcpy(ret,cola->cabeza->id_auto);
            NODO_COLA *aux = cola->cabeza;
            cola->cabeza = cola->cabeza->sig;
            free(aux);
            if( cola->cabeza==NULL ) cola->pie =NULL;
            cola->cant--;
        }
    }
    return ret;
}

int estaRepetido(COLA cola_izq,COLA cola_der,char id[]){
    NODO_COLA *actual_izq = cola_izq.cabeza;
    NODO_COLA *actual_der = cola_der.cabeza;
    while(actual_izq!=NULL){
        if(!strcmp(actual_izq->id_auto,id)) return 1;
        actual_izq = actual_izq->sig;
    }
    while(actual_der!=NULL){
        if(!strcmp(actual_der->id_auto,id)) return 1;
        actual_der = actual_der->sig;
    }
    return 0;
}

void incrementar(COLA *cola){
	NODO_COLA *aux = cola->cabeza;
	while(aux){
		if(aux->pos_puente >= 3 || aux->pos_puente < 0) aux->pos_puente = -1;	
		else aux->pos_puente++;
		aux = aux->sig;
	}
}

int vacio(COLA c){ return c.cabeza? 0:1; }

int cantidad(COLA c){ return c.cant;}
