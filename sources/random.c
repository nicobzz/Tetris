#include <fcntl.h>
#include <sys/types.h> //pour open et read pour le fichier /dev/random
#include <unistd.h>
#include <stdlib.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

double tetris_random (){
	char buffer [2];
	ssize_t n;
	int erreur = 0,i;
	unsigned long randomint = 0;
	double ret = 0;
	if (mySystem.random != -1){
		n = read (mySystem.random, buffer, sizeof(buffer) );
		if (n != 2){
			erreur = 1;
		}else{
			for (i = 0; i<2 ; i ++){
				randomint = randomint | (((unsigned char) (buffer[i])) << (8 * i));
			}
			ret = ( (double) randomint) /( (double)65536);
		}
	}else {
		erreur = 1;
	}
	if (erreur){
		ret= (((float)rand())/ ((float) RAND_MAX));
	}
	return ret;
}
