#define FDI_FILENAME "baseof9.db"
#define FDI_SIZE (9*26360118400)
#define FDO_FILENAME "baseof9.sums"

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

void add_to_obuf(uint8_t m, char *obuf, long int *c, int fdo)
{
	obuf[(*c)++]=m;
	if(*c==OBUFSIZE){
		if(write(fdo, obuf, OBUFSIZE)==-1){
			perror("write");
			exit(EXIT_FAILURE);
		}
		(*c)=0;
	}
}

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

	fdo=open(FDO_FILENAME, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
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
		int j;
		uint8_t *pp=p;

		for(j=0; j<3; j++)
			add_to_obuf(pp[0+j]+pp[3+j]+pp[6+j], obuf, &c, fdo);
		for(j=0; j<3; j++)
			add_to_obuf(pp[0+j*3]+pp[1+j*3]+pp[2+j*3], obuf, &c, fdo);
		add_to_obuf(pp[0]+pp[4]+pp[8], obuf, &c, fdo);
		add_to_obuf(pp[2]+pp[4]+pp[6], obuf, &c, fdo);

		p+=9;
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
