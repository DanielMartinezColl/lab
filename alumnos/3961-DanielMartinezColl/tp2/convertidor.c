#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "convertidor.h"

char *convertidor(char *texto){
	int i=0;
	for (i = 0; i < strlen(texto); i++){
		if(texto[i] == 'l' && texto[i+1] == 'a' && texto[i+2] == ' '){
			texto[i] = 'L';
			texto[i+1] = 'A';
		}
		if(texto[i] == 'y' && texto[i+1] == ' '){
			texto[i] = 'Y';
		}
	}
	return texto;
}
