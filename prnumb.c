#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>//memset

#include <errno.h>
//open
#include <sys/stat.h>
#include <fcntl.h>
//write
#include <unistd.h>


uint32_t pcg32(uint64_t *s)
{
	const uint64_t inc = (1442695040888963407ULL | 1);
	const uint64_t mult = 6364136223846793005ULL;

	uint64_t oldstate = *s;
	*s = (oldstate * mult) + inc;

	uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
	uint32_t rot = oldstate >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

#define h_used(flag, i)       ((flag[i>>6] >> (i&63UL)) & 1U)
#define h_set_used(flag, i)   (flag[i>>6] |=  (1UL<<(i&63UL)))
#define h_set_unused(flag, i) (flag[i>>6] &= ~(1UL<<(i&63UL)))

#define h_size(m) ((m) < 64? 1 : (m)>>6)

int eq(uint8_t *x, uint8_t *xx, uint8_t l)
{
	uint8_t k;
	for (k = 0; k < l; k++)
		if (x[k] != xx[k])
			break;
	return l == k;
}

#define FNV_64_SEED 11
static inline uint64_t fnv(const uint8_t *s, uint64_t ls)
{
	uint64_t h = FNV_64_SEED ^ 14695981039346656037U;
	for (uint64_t i = 0; i < ls; i++)
			h ^= s[i], h *= 1099511628211U;
	return h;
}

static inline uint64_t h_scrambling(uint64_t hash, uint8_t bits)
{return hash * 11400714819323198486U >> (64 - bits);}

#define N 8

int main()
{
	uint64_t iter = 0;

	uint64_t *ti;
	uint64_t pti = 0;
	uint64_t szti = 100;
	uint64_t curriter;
	uint64_t currpval;

	{//tabla iteraciones ----

	ti = (uint64_t *)malloc(2 * szti * sizeof(uint64_t));
	if (!ti) goto error;

	}//---- tabla iteraciones

	/*
	      1   2   3   4   5
	297  14  14  16   7  13
	*/

	/*
	14/64 = 7/32
	16/64 = 8/32 = 4/16 = 2/8 = 1/4
	7/64
	13/64
	*/
	/*
	14	939524096
	16	1073741824
	7	469762048
	13	872415232
	*/
	uint64_t max_iter;

	max_iter = 200000;
	//max_iter = 1000000;
	//max_iter = 1000000;
	//max_iter = 10000000;
	//max_iter = 10000000000;

	uint8_t *t;

	uint8_t bits = 2;
	uint64_t pval = 0;
	uint64_t count = 0;

	uint64_t hsz = 1ULL << bits;
	uint64_t mask = hsz - 1;

	uint64_t *key = (uint64_t *)(malloc(hsz*sizeof(uint64_t)));
	if (!key) goto error;

	uint64_t *flags = (uint64_t *)(malloc(h_size(hsz)*sizeof(uint64_t)));
	if (!flags) goto error;
	memset(flags,0,h_size(hsz)*sizeof(uint64_t));

	uint8_t *val = (uint8_t *)(malloc(N * hsz * sizeof(uint8_t)));
	if (!val) goto error;

	{/*Generación ---- */

	uint64_t *seeds = (uint64_t *)(malloc( 1 * sizeof(uint64_t) ));
	if (!seeds) goto error;
	seeds[0] = 599069558410000471ULL;

	uint32_t *l = (uint32_t *)(malloc( 5 * sizeof(uint32_t)));
	if (!l) goto error;

	l[0] = 939524096U;
	l[1] = 2013265920U;
	l[2] = 2483027968U;
	l[3] = 3355443200U;
	l[4] = 4294967295U;
	//2**32 - l[4] = 1
	
	t = (uint8_t *)(malloc(N * sizeof(uint8_t)));
	if (!t) goto error;

loop:
	for (uint64_t casilla = 0; casilla < N; casilla++){

		uint32_t rand = pcg32(seeds);

		if      (rand <= l[0]) t[casilla] = 1;
		else if (rand <= l[1]) t[casilla] = 2;
		else if (rand <= l[2]) t[casilla] = 3;
		else if (rand <= l[3]) t[casilla] = 4;
		else                   t[casilla] = 5;
	}

	curriter = iter; //XXX

	if (iter++ < max_iter) goto hash;
	else goto search;

	}/* ---- Generación*/

	{/* Hash table ---- */
hash:
	// Resize ----
	if ((hsz >> 1) + (hsz >> 2) <= count){
		uint8_t bits0  = bits + 1;
		uint64_t hsz0  = hsz << 1;
		uint64_t mask0 = hsz0 - 1;

		key = (uint64_t *)realloc(key,hsz0*sizeof(uint64_t));
		if (!key) goto error;

		uint64_t *flags0 = (uint64_t *)malloc(h_size(hsz0)*sizeof(uint64_t));
		if (!flags0) goto error;
		memset(flags0,0,h_size(hsz0)*sizeof(uint64_t));

		val = (uint8_t *)realloc(val,N * hsz0 * sizeof(uint8_t));
		if (!val) goto error;

		for (uint64_t i = 0; i < hsz; i++){
			if (!h_used(flags,i)) continue;
			h_set_unused(flags,i);
			uint64_t k = key[i];
			while (1){
				uint64_t v = h_scrambling(fnv(val+k,N),bits0);
				while (h_used(flags0,v)) v = (v + 1) & mask0;
				h_set_used(flags0,v);
				if (v < hsz && h_used(flags,v)){
					{
					uint64_t tmp = key[v];
					key[v] = k;
					k = tmp;
					}
					h_set_unused(flags,v);
				}
				else{
					key[v] = k;
					break;
				}
			}
		}

		free(flags);

		flags = flags0;

		bits = bits0;
		hsz  = hsz0;
		mask = mask0;
	}
	// ---- Resize

	uint64_t h = h_scrambling(fnv(t,N),bits);
	uint64_t last = h;
	while (h_used(flags,h) && !eq(t,val+key[h],N)){
		h = (h + 1U) & mask;
		if (h == last) goto notfound;
	}
	if (!h_used(flags,h)) goto notfound;
	else goto found;

notfound:
	currpval = pval; //XXX

	// Guardar ----
	count++;
	key[h] = pval;
	h_set_used(flags,h);
	for (uint64_t i = 0; i < N; i++) val[pval++] = t[i];
	// ---- Guardar

	if (szti < count){
		szti+=100;
		ti = (uint64_t *)realloc(ti, 2* szti * sizeof(uint64_t));
		if (!ti) goto error;
	}

	ti[pti++] = curriter;
	ti[pti++] = currpval;
	

found:
	goto loop;

	}/* ---- Hash table */

search:
	printf("count: %ld\n", count);

	uint8_t *target = (uint8_t *)malloc(N * sizeof(uint8_t));
	if (!target) goto error;

	/*
	20,2979,3,3,2,2,1,3,2,1,300000,05/03/2026
	*/
	target[0]  = 3;
	target[1]  = 3;
	target[2]  = 2;
	target[3]  = 2;
	target[4]  = 1;
	target[5]  = 3;
	target[6]  = 2;
	target[7]  = 1;




	uint64_t h = h_scrambling(fnv(target,N),bits);
	uint64_t last = h;
	while (h_used(flags,h) && !eq(target,val+key[h],N)){
		h = (h + 1U) & mask;
		if (h == last) goto notfound2;
	}
	if (!h_used(flags,h)) goto notfound2;
	else goto found2;

notfound2:
	printf("No encontrado\n");
	goto end;
found2:
	printf("Encontrada\n");


	{//Imprimir a archivo ----

	int fp;
	fp = open("../data/z.csv", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	if (fp == -1){
		printf("Error al abrir archivo de escritura\n");
		goto error;
	}

	/*'i' is a 64 bit number */

	/* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 */
	/*       i         , 5  ,  5  ,  5  ,  5  ,  5  ,  5  ,  5  ,  5 \n */

	int len = 25;

	uint8_t *mem_imp = (uint8_t *)(malloc(len * sizeof(uint8_t)));
	if (!mem_imp) goto error;

	for (uint64_t pimp = 0; pimp < pti; ){

		curriter = ti[pimp++];
		currpval = ti[pimp++];

		uint8_t k = 7;

		{//u64 -> visible ----
		while (10 <= curriter){
			mem_imp[k--] = 48 + (curriter % 10);
			curriter /= 10;
		}
		mem_imp[k] = 48 + curriter;
		}//---- u64 -> visible

		uint8_t j = 8;

		mem_imp[j++] = ',';

		for ( ; j < len; ){ //9 = len(u64) + ','
			mem_imp[j++] = val[currpval++]+ 48;
			mem_imp[j++] = ',';
		}

		mem_imp[--j] = '\n';

		/*
		'j' has been used so far as "position". With
		this it becomes "length" for printing.
		*/
		j++;
		
		int r = 0;
		int s = 0;

		while(1){
			r = write(fp, mem_imp + k + s, j - s - k );
			s+=r;

			if (r < 0){
				printf("Writing to file failed with error: %s.\n", strerror(errno));
				goto error;
			}

			if (s + k == j) break;
		}

	}
        close(fp);

	}//---- Imprimir a archivo









end:
	return 0;

















error:
	printf("Error\n");
	return 1;
}






































