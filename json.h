/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/*********** Componentes lexicos y constantes********/
#define L_CORCHETE 256
#define R_CORCHETE 257
#define L_LLAVE 258
#define R_LLAVE 259
#define COMA 260
#define DOS_PUNTOS 261
#define LITERAL_CADENA 262
#define LITERAL_NUM 263
#define PR_TRUE 264
#define PR_FALSE 265
#define PR_NULL 266
#define EPSILON 267
#define TAMLEX 60
#define TAMBUFF 30

/************* Estructuras ********************/
typedef struct {
	int complexico;
	char lexema[TAMLEX];
    int numlinea;
}token;

typedef struct nodo{
    token t;
    struct nodo* sgte;
}nodo;

typedef struct nodo* n;

/************* Prototipos ********************/
void insertar(token);
token extraer();
void listar();
int esta_vacia();
void lexer(FILE*);
void error_lexico(int,char*);