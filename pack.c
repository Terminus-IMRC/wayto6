#define NFIELDS 9

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#define IBUFSIZ (((3+1)*NFIELDS>BUFSIZ)?(3+1)*NFIELDS:BUFSIZ)

typedef uint8_t output_t;

int main()
{
	int i, j;
	int c=0;
	char *ec;
	char *ibuf, *obuf;

	ibuf=(char*)malloc(IBUFSIZ*sizeof(char));
	if(ibuf==NULL){
		fprintf(stderr, "error: failed to malloc ibuf\n");
		exit(EXIT_FAILURE);
	}

	obuf=(char*)malloc(BUFSIZ*sizeof(char));
	if(obuf==NULL){
		fprintf(stderr, "error: failed to malloc obuf\n");
		exit(EXIT_FAILURE);
	}

	for(;;){
		char *ibuf_orig=ibuf;

		ec=fgets(ibuf, IBUFSIZ, stdin);
		if(!ec)
			break;

		for(i=0; i<NFIELDS; i++){
			char *s;
			output_t u, b;
			s=strtok(ibuf, " ");
			if(s==NULL){
				fprintf(stderr, "error: invalid the number of the field\n");
				exit(EXIT_FAILURE);
			}
			ibuf=NULL;
			u=(output_t)atoi(s);
			b=(output_t)0xff;
			for(j=0; j<sizeof(output_t); j++){
				obuf[c++]=u&b;
				if(c==BUFSIZ){
					write(STDOUT_FILENO, obuf, c);
					c=0;
				}
				u>>=8;
			}
		}
		ibuf=ibuf_orig;
	}

	if(c!=0){
		write(STDOUT_FILENO, obuf, c);
		c=0;
	}

	return 0;
}
