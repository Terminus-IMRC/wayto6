#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

int baseof3_count;

struct baseof3_chain_t{
	int b1, b2, b3;
	struct baseof3_chain_t *next;
};
struct baseof3_chain_t *baseof3_chain, *baseof3_chain_orig;

struct baseof3_t{
	int b1, b2, b3;
};
struct baseof3_t *baseof3;

typedef uint64_t index_t;
index_t *index_of_baseof3;

FILE *baseof9_out_fp;

void print_64bit(uint64_t n)
{
	uint64_t b=(uint64_t)1<<63;

	while(b){
		putchar(n&b?'1':'0');
		b>>=1;
	}
	putchar('\n');

	return;
}

void read_baseof3_chain()
{
	int e;

	baseof3_count=0;

	baseof3_chain=(struct baseof3_chain_t*)malloc(sizeof(struct baseof3_chain_t));
	if(!baseof3_chain){
		fprintf(stderr, "error: failed to malloc baseof3_chain(orig)\n");
		exit(EXIT_FAILURE);
	}
	baseof3_chain_orig=baseof3_chain;

	e=scanf("%d %d %d\n", &(baseof3_chain->b1), &(baseof3_chain->b2), &(baseof3_chain->b3));
	if(e==EOF){
		free(baseof3_chain);
		baseof3_chain=NULL;
		return;
	}

	do{
		baseof3_count++;
		baseof3_chain->next=(struct baseof3_chain_t*)malloc(sizeof(struct baseof3_chain_t));
		if(!baseof3_chain->next){
			fprintf(stderr, "error: baseof3_count=%d: failed to malloc baseof3_chain(next)\n", baseof3_count);
			exit(EXIT_FAILURE);
		}
		baseof3_chain=baseof3_chain->next;
		e=scanf("%d %d %d\n", &(baseof3_chain->b1), &(baseof3_chain->b2), &(baseof3_chain->b3));
	}while(e!=EOF);

	baseof3_chain->next=NULL;

	printf("baseof3_count: %d\n", baseof3_count);

	return;
}

void pack_baseof3()
{
	int i;

	baseof3=(struct baseof3_t*)malloc(baseof3_count*sizeof(struct baseof3_t));
	if(!baseof3){
		fprintf(stderr, "error: failed to malloc baseof3\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<baseof3_count; i++){
		struct baseof3_chain_t *p;
		baseof3[i].b1=baseof3_chain_orig->b1;
		baseof3[i].b2=baseof3_chain_orig->b2;
		baseof3[i].b3=baseof3_chain_orig->b3;

	 	p=baseof3_chain_orig;
	 	baseof3_chain_orig=baseof3_chain_orig->next;
		free(p);
	}

	return;
}

void make_index_of_baseof3()
{
	int i;

	index_of_baseof3=(index_t*)malloc(baseof3_count*sizeof(index_t));
	if(!index_of_baseof3){
		fprintf(stderr, "error: failed to malloc index_of_baseof3\n");
		exit(EXIT_FAILURE);
	}

	for(i=0; i<baseof3_count; i++){	
		index_of_baseof3[i]=(index_t)0;
		index_of_baseof3[i]|=(index_t)1;
		index_of_baseof3[i]<<=(baseof3[i].b2-baseof3[i].b1);
		index_of_baseof3[i]|=(index_t)1;
		index_of_baseof3[i]<<=(baseof3[i].b3-baseof3[i].b2);
		index_of_baseof3[i]|=(index_t)1;
		//index_of_baseof3[i]<<=(sizeof(index_t)-baseof3[i].b3);
		index_of_baseof3[i]<<=(64-baseof3[i].b3);
	}

	return;
}

void output_baseof9_init()
{
	baseof9_out_fp=fopen("baseof9.txt", "w");
	if(!baseof9_out_fp){
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	return;
}

void output_baseof9_finalize()
{
	int e;

	e=fclose(baseof9_out_fp);
	if(e==EOF){
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	return;
}

void output_baseof9(struct baseof3_t t1, struct baseof3_t t2, struct baseof3_t t3)
{
	fprintf(baseof9_out_fp, "%d %d %d %d %d %d %d %d %d\n", t1.b1, t1.b2, t1.b3, t2.b1, t2.b2, t2.b3, t3.b1, t3.b2, t3.b3);

	return;
}

int main()
{
	int i1, i2, i3;
	index_t index;
	unsigned long long int count=0;

	read_baseof3_chain();
	pack_baseof3();
	make_index_of_baseof3();
	output_baseof9_init();

	index=(index_t)0;
	for(i1=0; i1<baseof3_count-2; i1++){
		index|=index_of_baseof3[i1];
		for(i2=i1+1; i2<baseof3_count-1; i2++){
			if(index&index_of_baseof3[i2])
				continue;
			index|=index_of_baseof3[i2];
			for(i3=i2+1; i3<baseof3_count; i3++){
				if(index&index_of_baseof3[i3])
					continue;
				index|=index_of_baseof3[i3];

				output_baseof9(baseof3[i1], baseof3[i2], baseof3[i3]);
				count++;

				index^=index_of_baseof3[i3];
			}
			index^=index_of_baseof3[i2];
		}
		index=(index_t)0;
	}

	printf("count: %llu\n", count);

	output_baseof9_finalize();

	return 0;
}
