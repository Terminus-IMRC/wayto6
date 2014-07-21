#define FDI_FILENAME "baseof9.db"
#define FDI_SIZE (9*26360118400)
#define FDO_FILENAME "baseof9.index"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#define OBUFSIZE BUFSIZ

typedef uint64_t index_t;

int main()
{
	int fdi, fdo;
	uint8_t *p, *p_orig;
	long int c=0;
	char *obuf;
	long long int i;

	fdi=open(FDI_FILENAME, O_RDONLY);
	if(fdi==-1){
		perror("open(fdi)");
		exit(EXIT_FAILURE);
	}

	fdo=open(FDO_FILENAME, O_WRONLY|O_CREAT|O_TRUNC);
	if(fdo==-1){
		perror("open(fdo)");
		exit(EXIT_FAILURE);
	}

	p=mmap(NULL, FDI_SIZE, PROT_READ, MAP_SHARED, fdi, 0);
	if(p==MAP_FAILED){
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	p_orig=p;

	obuf=(char*)malloc(OBUFSIZE*sizeof(char));
	if(obuf==NULL){
		fprintf(stderr, "error: failed to malloc obuf\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<FDI_SIZE/9; i++){
		index_t u=(index_t)0;
		uint8_t prev=0;
		int j;

		for(j=0; j<9; j++){
			u<<=*p-prev;
			u|=(index_t)1;
			prev=*p++;
		}
		u<<=sizeof(index_t)*8-prev;

		for(j=0; j<(signed int)sizeof(index_t); j++){
			obuf[c++]=u&(index_t)0xff;
			if(c==OBUFSIZE){
				if(write(fdo, obuf, OBUFSIZE)==-1){
					perror("write");
					exit(EXIT_FAILURE);
				}
				c=0;
			}
			u>>=8;
		}
	}

	if(c!=0){
		if(write(fdo, obuf, c)==-1){
			perror("write");
			exit(EXIT_FAILURE);
		}
		c=0;
	}

	if(munmap(p_orig, FDI_SIZE)==-1){
		perror("munmap");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
