#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "contarP.h"

int contarP(char *texto){

	int palabras=0;

	char *pos,*pch;

	pch = strtok_r(texto," \n",&pos);
	while(pch){

			palabras++;
		pch = strtok_r(NULL," \n",&pos);
	}
	return palabras;
}

