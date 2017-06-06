#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "contador.h"
#include "convertidor.h"

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
    char buffer[80],archivo[50];
    int nread;
    int request1[2],request2[2],reply[2];
    int words=0;
    int c;
    int control=-1;
    int FD;
    int leer=0;
    int leido;
    char *out, texto[80];

    //Verifico Argunemntos
    while ((c = getopt(argc, argv,"i:o:")) != -1) {
    	switch (c) {
    		case 'i':
    			FD = open (optarg,O_RDONLY);
    			leer=1;
    			break;
    		case 'o':
                strcpy(archivo,optarg);
                control=1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-o] file\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (control==-1) {
        fprintf(stderr, "Se esperaban argumentos\n");
        exit(EXIT_FAILURE);
    }
    //Creo pipes
    pipe(request1);
    pipe(request2);
    pipe(reply);

    //Leo los datos de entrada
    do {
        memset(buffer, 0, sizeof buffer);
        if(leer==0){
        	nread = read(STDIN_FILENO, buffer, sizeof buffer);
        }
        else{
        	nread = read(FD, buffer, sizeof buffer);
        }

		write(request2[WRITE],buffer,nread);
		write(reply[WRITE],buffer,nread);

    } while (nread > 0);

    //Creo hijo que cuenta palabras
	if(fork()==0){
		int palabras=0;
		close(request2[WRITE]);
		close(request1[READ]);
		close(reply[READ]);
		close(reply[WRITE]);

		do{
			memset(texto, 0, sizeof texto);
			leido = read(request2[READ],texto,sizeof texto);
			palabras += contador(texto);
		} while (leido > 3);

		write(request1[WRITE],&palabras,sizeof(int));
		close(request1[WRITE]);
		close(request2[READ]);
		return 0;
	}

	//Creo hijo que pasa palabras a mayusculas
	if(fork()==0){
		close(request1[WRITE]);
		close(request1[READ]);
		close(request2[WRITE]);
		close(request2[READ]);
		close(reply[WRITE]);
		char *temp = malloc(80);

		do{
			memset(texto, 0, sizeof texto);
			leido = read(reply[READ],texto,sizeof texto);
			
			const size_t old_len = strlen(temp);
	    	const size_t out_len = old_len + leido + 1;
			out = malloc(out_len);

	    	memcpy(out, temp, old_len);
	    	memcpy(out + old_len, texto, leido);

	    	temp = realloc(temp,out_len);
	    	memcpy(temp, out, out_len);
		} while (leido > 3);

		temp = convertidor(temp);
		FD = open (archivo,O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
		write(FD,temp,strlen(temp));
		printf("Se creo un archivo llamado: %s\n",archivo);
		
		close(FD);
		close(reply[READ]);
		free(temp);
		return 0;
	}

	//Muestro la cantidad de palabras en el proceso padre
	else{
		sleep(1);
		close(request1[WRITE]);
		close(request2[WRITE]);
		read(request1[READ],&words,sizeof(int));
		printf("Las palabras son: %d\n",words);
    	return 0;
	}

return 0;
}
