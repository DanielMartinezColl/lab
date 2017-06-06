#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "contador.h"

int contador(char *texto){
	int palabras=0;
	char *pos,*pch;

	pch = strtok_r(texto," ,",&pos);
	while(pch){
		palabras++;
		pch = strtok_r(NULL," ,",&pos);
	}
	return palabras;
}


