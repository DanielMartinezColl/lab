#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "service.h"

int service(int sdc) {
	char buff[1024];
	char buffer[1024];
	int leido,fd;
	char *token;
	char *resultado = NULL;
	char archivo[256];
	char metodo[8];
	char ext[8];
	char temp[256];
	char *http_header = "HTTP/1.0 200 Ok\r\nContent-Type: text/html\r\n\r\n";
	char *jpeg_header = "HTTP/1.0 200 Ok\r\nContent-Type: image/jpeg\r\n\r\n";
	char *gif_header = "HTTP/1.0 200 Ok\r\nContent-Type: image/gif\r\n\r\n";
	char *pdf_header = "HTTP/1.0 200 Ok\r\nContent-Type: application/pdf\r\n\r\n";

	leido = read(sdc, buff, sizeof buff);
   	strcpy(buffer,buff);
   	/* Separo la informacion que recibo */
   	token = strtok(buffer, " ");
	strcpy(metodo,token);
   	token = strtok(NULL, " ");
	strcpy(archivo,token+1);

	printf("El cliente me escribio %s \n", buff);

	if ((strcmp(buff,"") == 0)){
		resultado = "500 INTERNAL SERVER ERROR\n";
		write(sdc,resultado,strlen(resultado));
		}

	else if (!(strncmp(metodo,"GET",3) == 0)){
		resultado = "405 METHOD NOT ALLOWED\n";
		write(sdc,resultado,strlen(resultado));
		}

	else if ((fd = open(archivo, O_RDONLY)) < 0){ 

		if(errno == 2){
			resultado = "404 NOT FOUND\n";
		}
		else if(errno == 13){
			resultado = "403 FORBIDDEN\n";
		}
		write(sdc,resultado,strlen(resultado));
	}
	
	else{

		strcpy(temp,archivo);
		token = strtok(temp, ".");
		token = strtok(NULL, ".");
		strcpy(ext,token);
		printf("\n\nel tipo es: %s\n\n",ext);

		if(strcmp(ext,"html")==0){
			write(sdc, http_header, strlen(http_header));
		}
		if(strcmp(ext,"jpeg")==0){
			write(sdc, jpeg_header, strlen(jpeg_header));
		}
		if(strcmp(ext,"gif")==0){
			write(sdc, gif_header, strlen(gif_header));
		}
		if(strcmp(ext,"pdf")==0){
			write(sdc, pdf_header, strlen(pdf_header));
		}
		while((leido = read(fd, buffer, sizeof buffer)) != 0){
				write(sdc, buffer, leido);
			}
		exit(0);
	}

	close(sdc);
	return 0;
}
