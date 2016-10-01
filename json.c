#include "json.h"

FILE *salida;//Archivo de salida del proceso del lexer de json

char msj[5*TAMLEX];
char id[TAMLEX];
int numlinea = 1;

void error_lexico(int numlinea,char* mensaje)//Funcion para imprimir detalle de errores
{
	printf("Lin: %d. Error Lexico: %s\n",numlinea,mensaje);
}

void detalle(int numlinea,char* compl,char* id)//Funcion para mprimir detalle lexico
{
    printf("linea: %d - componente lexico: %s - lexema: %s \n",numlinea,compl,id);
}

void lexer(FILE* archivo){
    
    token t;
	int i = 0;
	int estado,aceptado;
	char c = 0;
	salida = fopen("output.txt","w");
    
    
    //==============================================================================
    //Empieza el ciclo while principal de la funcion lexer
    
	while((c=fgetc(archivo)) != EOF){
		//printf("linea actual: %d\n",numlinea);
		if ( c == '\t' || c==' '){
			fputc(' ',salida);
		}
        else if(c == '\n'){
			fputc('\n',salida);
            numlinea ++;
        }
        //==============================================================================
        //comprobar si es uno de los caracteres [ ] { } : ; ,
		else if (c == '['){
            id[0] = c;
            id[1] = '\0';
			fputs("L_CORCHETE ",salida);
            strcpy(t.lexema,id);
            t.complexico = L_CORCHETE;
            t.numlinea = numlinea;
            detalle(numlinea,"L_CORCHETE",id);
            insertar(t);
		}
		else if(c == ']'){
            id[0] = c;
            id[1] = '\0';
			fputs("R_CORCHETE ",salida);
            strcpy(t.lexema,id);
            t.complexico = R_CORCHETE;
            t.numlinea = numlinea;
            detalle(numlinea,"R_CORCHETE",id);
            insertar(t);
		}
		else if(c == '{'){
            id[0] = c;
            id[1] = '\0';
			fputs("L_LLAVE ",salida);
			strcpy(t.lexema,id);
            t.complexico = L_LLAVE;
            t.numlinea = numlinea;
            detalle(numlinea,"L_LLAVE",id);
            insertar(t);
		}
		else if(c == '}'){
            
            id[0] = c;
            id[1] = '\0';
			fputs("R_LLAVE ",salida);
			strcpy(t.lexema,id);
            t.complexico = R_LLAVE;
            t.numlinea = numlinea;
            detalle(numlinea,"R_LLAVE",id);
            insertar(t);
		}
		else if(c == ','){
            
            id[0] = c;
            id[1] = '\0';
			fputs("COMA ",salida);
			strcpy(t.lexema,id);
            t.complexico = COMA;
            t.numlinea = numlinea;
            detalle(numlinea,"COMA",id);
            insertar(t);
		}
		else if(c == ':'){
            
            id[0] = c;
            id[1] = '\0';
			fputs("DOS_PUNTOS ",salida);
			strcpy(t.lexema,id);
            t.complexico = DOS_PUNTOS;
            t.numlinea = numlinea;
            detalle(numlinea,"DOS_PUNTOS",id);
            insertar(t);
		}
        //==============================================================================
        //comprobar si es un literal
		else if(c == '"'){
			ungetc(c,archivo);
			estado = 1;
			aceptado = 0;
            i = 0;
			while(!aceptado){
				switch(estado){
					case 1:
						c = fgetc(archivo);
						if (c == '"'){
                            id[i++] = c;
							estado = 2;
						}
						break;
					case 2:
						c = fgetc(archivo);
						if (c == '"'){
                            id[i++] = c;
							estado = 3;
							//printf("cambio de estado 2 a 3\n");
						}
						else{
                            id[i++] = c;
							estado = 2;
						}
						break;
					case 3:
						aceptado = 1;
                        id[i] = '\0';
						break;
				}
			}
			if (aceptado){
                fputs("LITERAL_CADENA ",salida);
                strcpy(t.lexema,id);
                t.complexico = LITERAL_CADENA;
                t.numlinea = numlinea;
                detalle(numlinea,"LITERAL_CADENA",id);
                insertar(t);
			}
			
		}
        //==================================================
        //comprobar si es un true,TRUE,false,FALSE,null o NULL
        else if (isalpha(c))
		{
			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
				if (i>=TAMLEX)
					error_lexico(numlinea,"Longitud de Identificador excede tamaño de buffer");
			}while(isalpha(c) || isdigit(c));
			id[i]='\0';
			if (c!=EOF)
				ungetc(c,archivo);
			else
				c=0;
			if (strcmp(id,"true")==0 || strcmp(id,"TRUE")==0){
                fputs("PR_TRUE ",salida);
                strcpy(t.lexema,id);
                t.complexico = PR_TRUE;
                t.numlinea = numlinea;
                detalle(numlinea,"PR_TRUE",id);
                insertar(t);
            }
            else if (strcmp(id,"false")==0 || strcmp(id,"FALSE")==0){
                fputs("PR_FALSE ",salida);
                strcpy(t.lexema,id);
                t.complexico = PR_FALSE;
                t.numlinea = numlinea;
                detalle(numlinea,"PR_FALSE",id);
                insertar(t);
            }
            else if (strcmp(id,"null")==0 || strcmp(id,"NULL")==0){
                fputs("PR_NULL ",salida);
                strcpy(t.lexema,id);
                t.complexico = PR_NULL;
                t.numlinea = numlinea;
                detalle(numlinea,"PR_NULL",id);
                insertar(t);
            }
		}
        //==============================================================================
        //comprobar si es un numero
        else if (isdigit(c))
		{
				//es un numero
				i=0;
				estado=0;
				aceptado=0;
				id[i]=c;
				
				while(!aceptado)
				{
					switch(estado){
					case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					
					case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(c=='.')
						{
							i--;
							fseek(archivo,-1,SEEK_CUR);
							estado=6;
						}
						else{
							sprintf("","No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf("","No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf("","No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;
						id[++i]='\0';
                        aceptado=1;
						fputs("LITERAL_NUM ",salida);
                        strcpy(t.lexema,id);
                        t.complexico = LITERAL_NUM;
                        t.numlinea = numlinea;
                        detalle(numlinea,"LITERAL_NUM",id);
                        insertar(t);
						break;
					case -1:
						if (c==EOF)
							error_lexico(numlinea,"No se esperaba el fin de archivo");
						else
							printf("");
						exit(1);
					}
				}
		}
	}
    t.complexico = EOF;
    strcpy(t.lexema,"fin de archivo");
    t.numlinea = numlinea;
    insertar(t);
}

//==============================================================================
//Funcion main principal
FILE *archivo;

int main(int argc, char* args[]){
    
	if ((archivo=fopen(args[1],"rt"))){
        lexer(archivo);
	}
	else{
		printf("Error no se pudo abrir el archivo\n");
	}
	fclose(archivo);
    return 0;
}