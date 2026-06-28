#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> // printf
#include <string.h>// memset

static void p32(uint32_t n)
{
	for (int i = 0; i < 32; i++){
		if (n & 2147483648)//2**31
			printf("1");
		else
			printf("0");
		n <<= 1;
	}
	printf("\n");
}


static void p322(uint32_t n)
{
	for (int i = 0; i < 32; i++){
		if (n & 2147483648)//2**31
			printf("1");
		else
			printf("0");
		n <<= 1;
	}
}


static void pnum(int xc)
{
	if (100 <= xc)
		printf("%d",xc);
	else if (10 <= xc)
		printf(" %d",xc);
	else
		printf("  %d",xc);
}


/*

xxxxxx

yyyyyywwww

xxxxxxzzzz  este debe de ser de la misma
            longitud o mayor que yyyyyywwww
            por construcción
=>

yyyyyywwww <= xxxxxzzzz

-----

Longitud igual

Si difieren y = 1, x = 0
yyy1yy|wwww
xxx0xx|zzzz
	=> xxx0xxzzzz < yyy1yywwww !

Si difieren y = 0, x = 1
yyy0yy|wwww
xxx1xx|zzzz
	=> yyy0yywwww < xxx1xxzzzz ! es un proceso ascendente

Longitud diferente

Argumento de piso de árbol (argumento visual)

*/


/*
  prefix length = 4

  alphabet length = 19

  6|0|0|5|6|5|5|4|2|2|7|0|6|7|0|6|2|6|0|

  0|00000000000000000000000000000000| 8|2
  1|00000000000000000000000000000001|16|2
  2|00000000000000000000000000000010| 9|2
  3|00000000000000000000000000000011| 7|4
  4|00000000000000000000000000000100| 8|2
  5|00000000000000000000000000000101|16|2
  6|00000000000000000000000000000110| 9|2
  7|00000000000000000000000000000111|22|0
  8|00000000000000000000000000001000| 8|2
  9|00000000000000000000000000001001|16|2
 10|00000000000000000000000000001010| 9|2
 11|00000000000000000000000000001011|18|0
 12|00000000000000000000000000001100| 8|2
 13|00000000000000000000000000001101|16|2
 14|00000000000000000000000000001110| 9|2
 15|00000000000000000000000000001111|30|0
 16|00000000000000000000000000010000| 0|0
 17|00000000000000000000000000010001| 0|0
 18|00000000000000000000000000010010| 3|5
 19|00000000000000000000000000010011| 5|5
 20|00000000000000000000000000010100| 0|0
 21|00000000000000000000000000010101| 0|0
 22|00000000000000000000000000010110| 6|5
 23|00000000000000000000000000010111| 0|6
 24|00000000000000000000000000011000| 6|5
 25|00000000000000000000000000011001| 4|6
 26|00000000000000000000000000011010| 0|0
 27|00000000000000000000000000011011| 0|0
 28|00000000000000000000000000011100| 0|0
 29|00000000000000000000000000011101| 0|0
 30|00000000000000000000000000011110|12|6
 31|00000000000000000000000000011111|17|6
 32|00000000000000000000000000100000|15|6
 33|00000000000000000000000000100001|10|7
 34|00000000000000000000000000100010|12|6
 35|00000000000000000000000000100011|17|6
 36|00000000000000000000000000100100|15|6
 37|00000000000000000000000000100101|13|7
*/


/*
int well_formed(uint8_t *bl, uint16_t nbl)
{
	uint16_t bl_count[16];

	uint16_t i;

	for (i = 0; i < 16; i++)
		bl_count[i] = 0;

	for (i = 0; i < nbl; i++)
		if (15 < bl[i]) return 1;
		else bl_count[bl[i]]++;

	int left = 1;
	for (i = 1; i <= 15; i++){
		left <<= 1;
		left -= bl_count[i];
		if (left < 0) return 2;
	}

	if (0 < left) return 3;

	return 0;
}
*/
/*
If 'well_formed' returns 2 the code is over-subscribed.
If 'well_formed' returns 3 the code is incomplete.

These definitions come from function 'construct'
from 'zlib/contrib/puff/puff.c'
*/


/*
First approach

Let 'bl[i]' be the length of the i-th symbol
of the input alphabet. An let 'nbl' be the
number of symbols in the input alphabet.

Following 3.2.2 of RFC 1951:

"1)  Count the number of codes for each code
 length. Let bl_count[N] be the number of
 codes of length N, N >= 1."

	uint16_t i;
	uint16_t bl_count[17], codes[17];

	for (i = 0; i < nbl; i++)
		bl_count[bl[i]]++;

"2)  Find the numerical value of the smallest
code for each code length:"

	uint16_t code = 0;
	for (i = 1; i < 17; i++){
		code = (code + bl_count[i-1]) << 1;
		codes[i] = base;
	}

"3)  Assign numerical values to all codes, using consecutive
 values for all codes of the same length with the base
 values determined at step 2. Codes that are never used
 (which have a bit length of zero) must not be assigned a
 value."

for (i = 0; i < nbl; i++){

	uint8_t len = bl[i];

	//Codes that are never used (which have
	//a bit length of zero) must not be
	//assigned a value."
	if (!bl_count[len])
		continue;

	uint16_t j = codes[len]++;
	//using consecutive values for all
	//codes of the same length with the base
	//values determined at step 2

}

Now at every iteration of the loop
'j' will be the code assigned to 'bl[i]'.

By 3.1.1 'j' is stored inversed. Let 'k'
be the inverted value of 'j'.

We illustrate the search procedure with an
example.

Let 'k' be a huffman code with 6 bits
Let 'BITS' be 9.
Let f be XXX

k :
x x x c c c c c c
8|7|6|5|4|3|2|1|0

k << (BITS - s)
k << (9 - 6)
k << 3:
c c c c c c x x x
8|7|6|5|4|3|2|1|0

f:
0 0 0 0 0 0 0 0 1
8|7|6|5|4|3|2|1|0

f << (BITS - s)
f << 3:
0 0 0 0 0 1 0 0 0
8|7|6|5|4|3|2|1|0

Let g = 0
g  : 0 0 0 0 0 0 0 0 0
g++: 0 0 0 0 0 0 0 0 1
g++: 0 0 0 0 0 0 0 1 0
g++: 0 0 0 0 0 0 0 1 1
g++: 0 0 0 0 0 0 1 0 0
g++: 0 0 0 0 0 0 1 0 1
g++: 0 0 0 0 0 0 1 1 0
g++: 0 0 0 0 0 0 1 1 1

We have now
	g + 1 = f.

Notice that

(k << 3) + g:
c c c c c c 0 0 0
c c c c c c 0 0 1
c c c c c c 0 1 0
c c c c c c 0 1 1
c c c c c c 1 0 0
c c c c c c 1 0 1
c c c c c c 1 1 0
c c c c c c 1 1 1

Let 'fast' be
	uint16_t fast[1 << BITS]
such that for all the 8 previous values
	fast[ccccccxxx] = cccccc
then for any uint16_t 'v' such that
	v & ( (1 << BITS) - 1):
	c c c c c c x x x
we have
	fast[v] = cccccc

Let 'P' and 'Q' be two diferent
codes:

P
p p p p p x x x x
Q
q q q q x x x x x

If
	fast[pppppxxxx] = fast[qqqqxxxxx]
then
	ppppp = qqqqx
for some 'x' meaning that 'Q' is a prefix of 'P' but
huffman codes are prefix free.
In conclusion we can't have collisions.


[XXX tener la tabla completa es demasiado entonces
más bien usamos 'k & BITS']

If 'BITS' is not 16 there can be two different codes,
say 'c1' and 'c2' such that
	c1 & BITS == c2 & BITS.

*/

static int huffi(uint16_t *v,
	  uint8_t  *l,
	  uint8_t  *bl,
	  uint16_t  nbl,
	  uint16_t *hmem,
	  uint16_t  mem,
	  uint8_t   bits,
	  uint8_t   isfixed,
          uint8_t   type)
{
	if (15 < bits) return 1;
	if (286 < nbl) return 1;

	uint16_t used = 1U << bits;
	if (mem < used) return 1;

	uint16_t *count = hmem;
	uint16_t *offset = hmem + 16;
	uint16_t *map    = hmem + 16 + 16;
	uint16_t *maq    = hmem + 16 + 16 + 286;

	uint16_t i;

	memset(v,0,mem*sizeof(uint16_t));
	memset(l,0,mem);

	memset(count ,0,16*sizeof(uint16_t));
	memset(offset,0,16*sizeof(uint16_t));

	for (i = 0; i < nbl; i++)
		if (15 < bl[i]) return 1;
		else count[bl[i]]++;

	int left = 1;
	for (i = 1; i <= 15; i++){
		left <<= 1;
		left -= count[i];
		if (left < 0) return 1; /*over-subscribed*/
	}

	/*Detect if there is only one code*/
	uint32_t sum = 0;
	for (i = 1; i < 16; i++) sum += count[i];

	/*
	  There are only three instances in which we accept
	  incomplete codes.
	  1) A code for distances where there is only one element.
	  2) A code for literals/lengths for an empty block. In
	  this case the only symbol present is the symbol that
	  encodes the end of block.
	  3) Fixed huffman codes. Both for distances and literals/lengths.
	*/
	if (0 < left){ /*incomplete code */
		if      (type == 1 && sum == 1);
		else if (type == 2 && sum == 1);
		else if (isfixed);
		else return 1;
	}

	/*
	The method we use requires that we generate
	all huffman codes for a given length and then
	move to the next length until we generate all
	codes.
	*/
	uint8_t len;
	uint16_t f = 0;
	uint16_t r = 0;
	uint16_t code = 0;
	for (len = 1; len <= 15; len++){
		offset[len] = r;
		if (count[len] != 0){
			if (bits < len && f == 0) f = r;
			for (i = 0; i < count[len]; i++){
				map[r] = code;
				maq[r] = len;

				uint16_t incr = 1U << (len - 1);
				while (code & incr)
					incr >>= 1;

				if (incr != 0) {
				    code &= incr - 1;
				    code += incr;
				}
				else
				    code = 0;
				r++;
			}
		}
	}
	if (code != 0){ /*incomplete code*/
		if      (type == 1 && sum == 1);
		else if (type == 2 && sum == 1);
		else if (isfixed);
		else return 1;
	}

	uint16_t mask = (1U << bits) - 1;

	if (f){
		for (i = f; i < r; i++){
			code = map[i];
			len  = maq[i];

			v[code & mask] = len - bits;
		}

		/*We use this initial value so that during
		  the first iteration of the loop
  			low != code   & mask
			     = map[f] & mass
		*/
		uint16_t low = ~(map[f] & mask);
		for (i = f; i < r; i++){
			code = map[i];
			len  = maq[i];
			if ((code & mask) != low){

				low = code & mask;

				uint8_t curr = v[low];
				used += (1U << curr);
				if (mem < used) return 1;

				uint16_t scurr = (uint16_t)(curr);
				scurr <<= 10;

				/*used  <= 1024    = 1<<10
				  scurr <= 15 < 16 = 1<<4*/
				v[low] = scurr|used;
			}
		}
	}

	uint16_t last = 0;

	uint16_t ceiling;
	uint16_t x;

	for (i = 0; i < nbl; i++){
		len = bl[i];
		if (len){
			last = i;

			uint16_t position = map[offset[len]++];

			if (len <= bits){
				ceiling = 1U << bits;
				x = 1U << len;
			}
			else{
				uint16_t pivot = v[position & mask];
				uint16_t floor = pivot & 1023;

				position >>= bits;
				position += floor;

				ceiling = floor + (1 << (pivot >> 10));

				x = 1U << (len - bits);
			}
			while (position < ceiling){
				v[position] = i;
				l[position] = len;
				position += x;
			}
		}
	}

	/*When we decode lengths and distances
	  we read from arrays. In order to make
	  sure we do not read beyond their bounds
	  we need 'last' to be within the following
	  bounds. This step might be redundant but
	  given how essential this is the following
	  checks are added here for clarity. */

	/* lengths for dynamic code */
	if (type == 0)
		if (18 < last) return 1;
	/* literal or length */
	else if (type == 1)
		if (285 < last) return 1;
	/* distance */
	else if (type == 2)
		if (29 < last) return 1;
	else return 1;

	return 0;
}







/*
The code represented by 'fixed_lengths' is incomplete. The standard
in 3.2.6 asks us to add the values 286 and 287 to the code for it to
be complete. We do not do this and instead we like to keep in mind
that this is an incomplete but valid code.

The same happens also in 3.2.6 for 'fixed_lengths' and values 30 and
31 and we do the same.
*/
static uint8_t fixed_lengths[286] =
{
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,8,8,8,8,8,8
};

static uint8_t fixed_distances[30] =
{
   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};



/*
RFC 1951 page 11

	 Extra               Extra               Extra
    Code Bits Length(s) Code Bits Lengths   Code Bits Length(s)
    ---- ---- ------     ---- ---- -------   ---- ---- -------
     257   0     3       267   1   15,16     277   4   67-82
     258   0     4       268   1   17,18     278   4   83-98
     259   0     5       269   2   19-22     279   4   99-114
     260   0     6       270   2   23-26     280   4  115-130
     261   0     7       271   2   27-30     281   5  131-162
     262   0     8       272   2   31-34     282   5  163-194
     263   0     9       273   3   35-42     283   5  195-226
     264   0    10       274   3   43-50     284   5  227-257
     265   1  11,12      275   3   51-58     285   0    258
     266   1  13,14      276   3   59-66
*/
static const uint16_t LZ77len[29] =
{
	3,4,5,6,7,8,9,10,11,13,
	15,17,19,23,27,31,35,43,51,59,
	67,83,99,115,131,163,195,227,258
};

static const uint16_t LZ77len_extra[29] =
{
	0,0,0,0,0,0,0,0,1,1,
	1,1,2,2,2,2,3,3,3,3,
	4,4,4,4,5,5,5,5,0
};

/*
RFC 1951 page 11

	 Extra           Extra               Extra
     Code Bits Dist  Code Bits   Dist     Code Bits Distance
     ---- ---- ----  ---- ----  ------    ---- ---- --------
       0   0    1     10   4     33-48    20    9   1025-1536
       1   0    2     11   4     49-64    21    9   1537-2048
       2   0    3     12   5     65-96    22   10   2049-3072
       3   0    4     13   5     97-128   23   10   3073-4096
       4   1   5,6    14   6    129-192   24   11   4097-6144
       5   1   7,8    15   6    193-256   25   11   6145-8192
       6   2   9-12   16   7    257-384   26   12  8193-12288
       7   2  13-16   17   7    385-512   27   12 12289-16384
       8   3  17-24   18   8    513-768   28   13 16385-24576
       9   3  25-32   19   8   769-1024   29   13 24577-32768
*/
static const uint16_t LZ77distance[30] =
{
	1,2,3,4,5,7,9,13,17,25,
	33,49,65,97,129,193,257,385,513,769,
	1025,1537,2049,3073,4097,6145,8193,12289,16385,24577
};

static const uint16_t LZ77distance_extra[30] =
{
	0,0,0,0,1,1,2,2,3,3,
	4,4,5,5,6,6,7,7,8,8,
	9,9,10,10,11,11,12,12,13,13
};


#define U(cl) do{                                \
if (f < cl){                                     \
	do{                                      \
		if (p + 1 <= l){                 \
			d32 = (uint32_t)(d[p++]);\
			d32 <<= f;               \
			f += 8;                  \
			n |= d32;                \
		}                                \
		else break;                      \
	} while(f <= 24);                        \
	if (f < cl) goto error;}                 \
u = n & ((1 << cl) - 1);                         \
f -= cl;                                         \
n >>= cl;} while(0)                              \


#define N() do{                                  \
if (f < 15){                                     \
	do{                                      \
		if (p + 1 <= l){                 \
			d32 = (uint32_t)(d[p++]);\
			d32 <<= f;               \
			f += 8;                  \
			n |= d32;                \
		}                                \
		else break;                      \
	} while(f <= 24);                        \
	if (f <= 0) goto error;}} while(0)       \


#define D(vt,lt,bits,mask) do{              \
address = n & mask;                         \
code  = vt[address];                        \
lcode = lt[address];                        \
if (lcode == 0){                            \
	address = n;                        \
	address >>= bits;                   \
	address &= (1 << (code >> 10)) - 1; \
	address += (code & 1023);           \
	code  = vt[address];                \
	lcode = lt[address];                \
}                                           \
f -= lcode;                                 \
n >>= lcode;}while(0)                       \


/*RFC 1951 page 13*/
static const uint8_t dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,
11,4,12,3,13,2,14,1,15 };

/*
Memory layout

 m8 = lencodes  llit  ldist      pbuf
      286 + 30   852   592   1<<16 + 258

          hmem = 604
      --------------------   
m16 = count offset map maq vlit vdist
        16    16   286 286  852  592
*/
/*
Notes:

For length+distance pairs we don't ask for
length <= distance. That is why 'pbuf' has 258 "extra"
bytes.

The first bytes of 'm8 are used as temporal memory
throughout 'inflate'.
*/
int inflate(uint8_t *d, uint64_t l, uint8_t **result, uint64_t *szresult)
{
	uint8_t *m8 = (uint8_t *)malloc((286 + 30 + 852 + 592 + (1<<16) + 258)*sizeof(uint8_t));
	uint16_t *m16 = (uint16_t *)malloc((604 + 852 + 592)*sizeof(uint16_t));
	if (!m8 || !m16) goto error;

	uint8_t *llit  = m8 + 286 + 30;
	uint8_t *ldist = m8 + 286 + 30 + 852;
	uint8_t *pbuf  = m8 + 286 + 30 + 852 + 592;

	uint16_t *vlit  = m16 + 16 + 16 + 286 + 286;
	uint16_t *vdist = m16 + 16 + 16 + 286 + 286 + 852;

	uint64_t p = 0, q = 0, r = 0;
	uint64_t inf = 0;

	uint8_t *b = NULL;
	uint64_t szb = 0;

	uint8_t in_non = 0;

	uint8_t f = 0;
	uint16_t u;
	uint32_t n = 0;
	uint32_t d32;

	uint8_t lcode;
	uint16_t code;
	uint16_t address;

	uint8_t done = 0;
block:
	U(3);
	/*bfinal == 1*/
	done = (uint8_t)(u&1);
	uint8_t btype = (uint8_t)(u >> 1);
	if (btype == 0) goto non;
	else if (btype == 1) goto fixed;
	else if (btype == 2) goto dynamic;
	goto error;
non:
	in_non = 1;

	/*Discard bits until byte boundary*/
	n >>= (7 & f);
	f -=  (7 & f);

	uint8_t k = 0;
	for (;0 < f; f-=8){
		m8[k++] = (uint8_t)(n & 255);
		n >>= 8;
	}

	if (f < 0) goto error;

	for (; k < 4; k){
		if (l < p + 1) goto error;
		m8[k++] = d[p++];
	}

	/*LEN and NLEN*/
	uint16_t sz   = (uint16_t)((m8[1] << 8) + m8[0]);
	uint16_t nlen = (uint16_t)((m8[3] << 8) + m8[2]);
	if (nlen != (sz ^ 0xffff)) goto error;
nonl:
	if (!(q + sz <= 65535)){
		for(; q < 65535;sz--)
			pbuf[q++] = d[p++];
		goto save;
	}

	for( ; sz; sz--)
		pbuf[q++] = d[p++];

	in_non = 0;

	if (done) goto save;

	goto block;
fixed:
	if (huffi(vlit, llit, fixed_lengths, 286, m16, 852, 9, 1, 1)) goto error;
	if (huffi(vdist, ldist, fixed_distances, 30, m16, 592, 6, 1, 2)) goto error;
	goto lz77;
dynamic:
	U(14);
	/*lengths: 5, 5, 4*/
	uint16_t hlit, hdist, hclen;
	/*Get 5 bits*/
	hlit = u & 31; u >>= 5;
	/*RFC 1951 3.2.7 hlit + 257 must be <= 286*/
	if (29 < hlit) goto error;
	hlit += 257;
	hdist = u & 31; u >>= 5;
	if (29 < hdist) goto error;
	hdist += 1;
	hclen = u & 15; u >>= 4;
	hclen += 4;

	uint8_t i;
	for (i = 0; i < 19; i++)
		m8[i] = 0;
	for (i = 0; i < hclen; i++){
		U(3);
		m8[dezigzag[i]] = (uint8_t)(u & 255);
	}

	if (huffi(vlit, llit, m8, 19, m16, 852, 7, 0, 0)) goto error;

	uint16_t total = hlit + hdist;
	uint16_t m = 0;
clens:
	N();
	D(vlit,llit,7,127);

	if (code < 16){
		m8[m++] = (uint8_t)(code & 255);
	}
	else{
		uint8_t fill = 0;
		if (code == 16){
			if (m == 0) goto error;
			U(2);
			u += 3;
			fill = m8[m-1];
		}
		else if (code == 17){
			U(3);
			u += 3;
		}
		else if (code == 18){
			U(7);
			u += 11;
		}
		else goto error;

		if (total < m + u) goto error;
		for (uint16_t k = 0; k < u; k++) m8[m++] = fill;
	}
	if (m < total) goto clens;

	if (huffi(vlit, llit, m8, hlit, m16, 852, 9, 0, 1)) goto error;
	if (huffi(vdist, ldist, m8 + hlit, hdist, m16, 592, 6, 0, 2)) goto error;
lz77:
	uint16_t len, dist, extra;
	N();
	D(vlit,llit,9,511);

	if (code < 256){
		pbuf[q++] = (uint8_t)(code);
		goto store;
	}
	else if (code == 256)
		if (done) goto save;
		else goto block;
	code -= 257;

	len = LZ77len[code];
	extra = LZ77len_extra[code];
	if (extra){
		U(extra);
		len += u;
	}

	N();
	D(vdist,ldist,6,63);

	dist = LZ77distance[code];
	extra = LZ77distance_extra[code];
	if (extra){
		U(extra);
		dist += u;
	}

	if (q < dist) goto error;
	uint64_t base = q - dist;
	for (uint64_t k = 0; k < len; k++)
		pbuf[q++] = pbuf[base + k];
store:
	/* 1 << 16 = 65536 */
	if (q <= 65535) goto lz77;
save:
	szb += (q - inf);
	b = (uint8_t *)realloc(b, szb);
	if (!b) goto error;

	uint64_t s;
	for (s = inf; s < q; s++)
		b[r++] = pbuf[s];

	uint64_t t = 0;
	for (s = 32768; s < q; s++)
		pbuf[t++] = pbuf[s];

	q -= 32768;
	inf = q;

	if (in_non) goto nonl;

	if (!done) goto lz77;

	*szresult = szb;
	*result = b;

	free(m8);
	free(m16);
	return 0;
error:
	free(b);
	free(m8);
	free(m16);
	return 1;
}



/*

dheader = hlit hdist hclen hheader hheader hheader hheader [hheader]

huffman = litlen [ [extra_length] distance [extra_distance] ]

dynamic = dheader (huffman)

static = (huffman)

data = dynamic | static

//TODO puede haber bloques vacíos que entonces sólo tengan "256" encodeado
//	revisar si la gramática describe bien éstos

block = (0 data eob) 1 data eob

deflate_stream = (block)

*/












static const uint8_t ilength[259] = {
	0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12,
	12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 16,
	16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18,
	18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
	21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
	22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
	23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 28,
};

static const uint8_t idistance[512] = {
	0, 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7,
	7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,
	9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 0, 16, 17, 18, 18, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
	25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
};


/*
Let
       'x' |be a bit currently being used
'f' or 'F' |be a bit we will use to store the code
       'y' |a bit that will remain unused after
            storing the new code
'c' or 'C' |be a bit of the code to be stored

1) len <= f

b[q]     n
xxFFyyyy 00000000|000000CC
b[q]     n <<= (f - len)
xxFFyyyy 00000000|00CC0000
b[q]     u = (uint8_t)(n & 255)
xxFFyyyy 00CC0000
b[q]|u
xxCC0000
  len(xx) = 8-f
  len(CC) = len
  len(0000) = 8-((8-f) + len) = 8 - (8-f) - len
                              = 8 - len - (8-f)
                              = 8 - len - 8 + f
                              = f - len

len(xx)+len(CC)+len(0000) = (8 - f) + len + (f - len)
                          = 8

Alternatively:

b[q]     n
xxFFyyyy 00000000|000000CC
b[q]     m = n << 16 - len
xxFFyyyy CC000000|00000000
b[q]     m >>= 8 - f
xxFFyyyy 00CC0000|00000000
b[q]     u = (uint8_t)(m >> 8)
xxFFyyyy 00CC0000
b[q]|u
xxCC0000

2) len <= f + 8

b[q]     b[q+1]   n
xxxxxFFF|FFFFFFFy 000000cc|cCCCCCCC
b[q]     b[q+1]   m = n << 16 - len
xxxxxFFF|FFFFFFFy cccCCCCC|CC000000
b[q]     b[q+1]   m >>= 8 - f
xxxxxFFF|FFFFFFFy 00000ccc|CCCCCCC0
b[q]     b[q+1]   u = (uint8_t)(m >> 8)
xxxxxFFF|FFFFFFFy 00000ccc
b[q]|u   b[q+1]
xxxxxccc|FFFFFFFy
b[q]     b[q+1]   u = (uint8_t)(m & 255)
xxxxxccc|FFFFFFFy CCCCCCC0
b[q]     b[q+1]=u
xxxxxccc|CCCCCCC0
  len(CCCCCCC) = len - f
        len(0) = f + 8 - len
               = (f - len) + 8

len(CCCCCCC)+len(0) = (len - f) + (f - len + 8)
                    = 8

3) f + 8 <= len

b[q]     b[q+1]   b[q+2]   n
xxxFFFFF|FFFFFFFF|Fyyyyyyy 00cccccC|CCCCCCCc
b[q]     b[q+1]   b[q+2]   m = n << 16 - len
xxxFFFFF|FFFFFFFF|Fyyyyyyy cccccCCC|CCCCCc00
b[q]     b[q+1]   b[q+2]   m >>= 8 - f
xxxFFFFF|FFFFFFFF|Fyyyyyyy 000ccccc|CCCCCCCC
b[q]     b[q+1]   b[q+2]   u = (uint8_t)(m >> 8)
xxxFFFFF|FFFFFFFF|Fyyyyyyy 000ccccc
b[q]|u   b[q+1]   b[q+2]
xxxccccc|FFFFFFFF|Fyyyyyyy
b[q]     b[q+1]   b[q+2]   u = (uint8_t)(m & 255)
xxxccccc|FFFFFFFF|Fyyyyyyy CCCCCCCC
b[q]     b[q+1]=u b[q+2]
xxxccccc|CCCCCCCC|Fyyyyyyy
b[q]     b[q+1]   b[q+2]    n <<= 8 - (len - f - 8)
xxxccccc|CCCCCCCC|Fyyyyyyy  CCCCCCCC|c00000000
b[q]     b[q+1]   b[q+2]    u = (uint8_t)(n & 255)
xxxccccc|CCCCCCCC|Fyyyyyyy  c00000000
b[q]     b[q+1]   b[q+2]=u
xxxccccc|CCCCCCCC|c0000000
        len(c) = len - f - 8
  len(0000000) = 8 - (len - f - 8)
               = 8 - len + f + 8
               = 16 - len + f
               = (f - len) + 16

len(c) + len(0000000) = (len - f - 8) + f - len + 16
                      = 8

  0 <= f - len + 16
iff
  len <= f + 16
and 
  len <= 16 <= 16 + f for f in [0,8]
so
  n << (f - len) + 16 is always well defined.
*/

static void send(int8_t *f, uint32_t *q, uint8_t *b, uint8_t len, uint16_t n)
{
	uint8_t nf = (*f - l) + 16;
	uint8_t nq = 0;

	uint16_t m;
	m = n << (16 - l);
	m >>= (8 - *f);

	if (l <= *f){
		nf -= 16;
		goto b0;
	}
	else if (l <= *f + 8){
		nf -= 8;
		goto b1;
	}

	/* *f + 8 <= l */
	nq++;
	uint16_t k = n << nf;
	b[*q+2] = (uint8_t)(k & 255);
b1:
	nq++;
	b[*q+1] = (uint8_t)(m & 255);
b0:
	b[*q]  |= (uint8_t)(m >> 8);

	*f = nf;
	*q += nq;

}

static int huffd(uint16_t *hmem,
		uint16_t *t, uint8_t *l,
		uint8_t *bl, uint16_t nbl)
{
	uint16_t *count  = hmem;
	uint16_t *offset = hmem + 16;
	uint16_t *map    = hmem + 16 + 16;
	uint16_t *maq    = hmem + 16 + 16 + 286;

	uint16_t i;

	memset(count,  0, 16 * sizeof(uint16_t));
	memset(offset, 0, 16 * sizeof(uint16_t));

	for (i = 0; i < nbl; i++)
		count[bl[i]]++;

	uint8_t len;
	uint16_t r = 0;
	uint16_t code = 0;

	for (len = 1; len <= 15; len++){
		offset[len] = r;
		if (count[len] != 0){
			for (i = 0; i < count[len]; i++){
				map[r] = code;
				maq[r] = len;

				uint16_t incr = 1U << (len - 1);

				while (code & incr)
				    incr >>= 1;

				if (incr != 0) {
				    code &= incr - 1;
				    code += incr;
				}
				else
				    code = 0;
				r++;
			}
		}
	}

	for (i = 0; i < nbl; i++){
		len = bl[i];
		if (len){
			t[map[offset[len]++]] = i;
			l[maq[offset[len]++]] = len;
		}
	}

	return 0;
}




/*
From
https://chromium.googlesource.com/chromium/src/third_party/zlib/+/91a295c72eeba2b16e51350c0ca8de8ed160f6fc
--------------------------------
[zlib] Introducing ANZAC++ hash

Since 2015 Chromium's zlib used a hardware accelerated hashing function
(i.e. CRC32) to insert symbols in the dictionary during compression.

As it was suggested by Noel Gordon, turns out that a portable hash can
actually outperform it, yielding an average gain of 30% to 40% faster* compression while keeping similar compression ratios**.

In these days, such gains are equivalent to more than 3 generations
of silicon.

The new hashing function is called ANZAC++ and can be technically
considered a multiplicative hash (i.e. multiply-add-shift-mask).

Aside being faster, it is also portable and doesn't depend on crypto
extensions to perform (i.e. will work on RISC-V and Armv7).

This patch will use ANZAC as default hashing, plus performs a huge cleanup.

* Average gains: m1(31.3%), RPL_e(39.7%), RPL_p(39.4%),
Cascade(39%), ICX(+38%), SPR(41.9%).

** Average compression ratio loss is 0.1% for the snappy data
corpus.
*/

/*
From
https://chromium.googlesource.com/chromium/src/third_party/zlib/+/1938d49d05cb93de60aaf6b6d0e572a9aa764c72
--------------------------------
[Gardener] Revert "[zlib] Introducing ANZAC++ hash"

This reverts commit e1a960f284b6f81eddb6b04eccd4f81dd07d6c95.

Reason for revert: speculative revert because inspector-protocol/overlay/overlay-with-emulation-scale.js stably fails on Linux Tests builder and this CL is most probably the culprit.
*/


/*
On zlib memory usage for buffers.

From 'deflate.c':

#define UPDATE_HASH(s,h,c) (h = (((h) << s->hash_shift) ^ (c)) & s->hash_mask)

and

#define INSERT_STRING(s, str, match_head) \
   (UPDATE_HASH(s, s->ins_h, s->window[(str) + (MIN_MATCH-1)]), \
    match_head = s->prev[(str) & s->w_mask] = s->head[s->ins_h], \
    s->head[s->ins_h] = (Pos)(str))

meaning that each element of 'head' in zlib is an integer
in [0, s->hash_mask] where
	s->hash_bits = (uInt)memLevel + 7;
	s->hash_size = 1 << s->hash_bits;
	s->hash_mask = s->hash_size - 1;
and for default parameters 'memLevel' is 8.
.:. if 'x' is an element of 'head',
    'x' is an intger in [0,32767] = [0,(1<<15)-1]
.:.
    the length of 'head' is 's->hash_size'

also each element of 'prev' in zlib is an integer
in [0, s->w_mask] where
	s->w_bits = (uInt)windowBits;
	s->w_size = 1 << s->w_bits;
	s->w_mask = s->w_size - 1;
the default is
	windowBits = 15.
.:. if 'y' is an element of 'prev',
    'y' is an intger in [0,32767] = [0,(1<<15)-1]
.:.
    the lenght of 'head' is 's->w_size'

Zlib reads the bytes to be compressed into a buffer that
has 'w->size' bytes and after processing them they are moved
into another buffer of the same size. Then zlib uses
2*(s->w_size) bytes at this stage ('s->window').

The amount of memory previosly described is thus:
	2*(s->w_size) + lenght(prev) + length(head)
Given that an element in either 'prev' and 'head' will
fit in a 'uint16_t' we will use this type for them.
Lets consider 'uint16_t' as being 2 bytes long:
	2*(s->w_size) + lenght(prev) + length(head)
      = 2*(s->w_size) + 2*(s->hash_size) + 2*(s->w_size)
      = 2*(1<<windowBits) + 2*(1<<(memLevel+7)) + 2*(1<<windowBits)
      = 4*(1<<windowBits) + 2*(1<<(memLevel+7))
      = (1<<(windowBits+2)) + (1<<(memLevel+8))

With regards to 'symbuf' and 'pending_buf':
	s->lit_bufsize = 1 << (memLevel + 6);
	s->pending_buf = (uchf *) ZALLOC(strm, s->lit_bufsize, 4);
	s->pending_buf_size = (ulg)s->lit_bufsize * 4;
	s->sym_buf = s->pending_buf + s->lit_bufsize;
.:.
	lenght(pending_buf) = 4*(1<<(memLevel + 6))
                            = 1<<(memLevel + 6 + 2)
                            = 1<<(memLevel + 8)

All the memory so far discussed:
	 (1<<(windowBits+2)) + (1<<(memLevel+8)) + (1<<(memLevel + 8))
       = (1<<(windowBits+2)) + 2*(1<<(memLevel+8))
       = (1<<(windowBits+2)) + (1<<(memLevel+8 + 1))
       = (1<<(windowBits+2)) + (1<<(memLevel+9))
So for default values (15 and 8), using 'uint16_t' for 'prev' and 'head'
and cosidering 'uint16_t' as using 2 bytes:
       = (1 << 17) + (1 << 17) = 1 << 18
*/

/*
On 'pbuf' and 'symbuf'

We use the same system zlib uses for gathering
symbols. We use a "pending buffer" and a "symbol buffer"
('pbuf' and 'symbuf').

Using deflate we can either store a literal or a distance+length
pair, we will call these two options a "symbol".

Before deciding how to encode a symbol we first gather
1<<14 - 1 = 16383 symbols in 'symbuf'. We copy this from
zlib where they do this to avoid the "wraparound at 64K on
16 bit machines" (deflate.c). There is no reason why we should
do the same but we do.

Just as zlib we use 3 bytes per symbol so 'symbuf' must
be 3*(1<<14) bytes long. Before encoding them a literal
at position 'r' in 'symbuf' is stored as
	symbuf[r]   = 0;
	symbuf[r+1] = 0;
	symbuf[r+2] = literal;
and a length+distance pair at position 'r' is saved as
	symbuf[r]   = (uint8_t)(255 & distance);
	symbuf[r+1] = (uint8_t)(distance >> 8);
	symbuf[r+2] = (uint8_t)(255 & (length - 3));

After we have 16383 symbols or the whole file has been turned
to symbols (the last block will almost certainly not be 16383
long) we encode them into 'pbuf'.

We define 'pbuf' to be 4*(1<<14) bytes long and
	symbuf = pbuf + 1<<14

Lets assume we are encoding the symbols using codes that are
at most 32 bits in long.
If we encode a symbol we use at most 4 bytes. When encoding a symbol
we are freeing 3 bytes from 'symbuf' because we no longer need to
keep the memory used by the symbol before encoding. Since there is
always 1 byte unused in 'pbuf' for every symbol ('symbuf begins
1<<14 bytes after the first byte from 'pbuf'), counting the 3 bytes
just freed we have 4 bytes per symbol available and we never
write over 'symbuf' before using its contents.

Note that (deflate.c):
	"The longest fixed codes are a length code of 8 bits plus 5
	extra bits, for lengths 131 to 257. The longest fixed distance codes
	are 5 bits plus 13 extra bits, for distances 16385 to 32768. The
	longest possible fixed-codes length/distance pair is then 31 bits
	total."
We conclude that as long as no symbol uses more than 32 bits
'symbuf' and 'pbuf' can be used this way without "overlapping".

For completeness we include the analysis done by zlib in deflate.c:
   * We overlay pending_buf and sym_buf. This works since the average size
   * for length/distance pairs over any compressed block is assured to be 31
   * bits or less.
   *
   * Analysis: The longest fixed codes are a length code of 8 bits plus 5
   * extra bits, for lengths 131 to 257. The longest fixed distance codes are
   * 5 bits plus 13 extra bits, for distances 16385 to 32768. The longest
   * possible fixed-codes length/distance pair is then 31 bits total.
   *
   * sym_buf starts one-fourth of the way into pending_buf. So there are
   * three bytes in sym_buf for every four bytes in pending_buf. Each symbol
   * in sym_buf is three bytes -- two for the distance and one for the
   * literal/length. As each symbol is consumed, the pointer to the next
   * sym_buf value to read moves forward three bytes. From that symbol, up to
   * 31 bits are written to pending_buf. The closest the written pending_buf
   * bits gets to the next sym_buf symbol to read is just before the last
   * code is written. At that time, 31*(n - 2) bits have been written, just
   * after 24*(n - 2) bits have been consumed from sym_buf. sym_buf starts at
   * 8*n bits into pending_buf. (Note that the symbol buffer fills when n - 1
   * symbols are written.) The closest the writing gets to what is unread is
   * then n + 14 bits. Here n is lit_bufsize, which is 16384 by default, and
   * can range from 128 to 32768.
   *
   * Therefore, at a minimum, there are 142 bits of space between what is
   * written and what is read in the overlain buffers, so the symbols cannot
   * be overwritten by the compressed data. That space is actually 139 bits,
   * due to the three-bit fixed-code block header.
   *
   * That covers the case where either Z_FIXED is specified, forcing fixed
   * codes, or when the use of fixed codes is chosen, because that choice
   * results in a smaller compressed block than dynamic codes. That latter
   * condition then assures that the above analysis also covers all dynamic
   * blocks. A dynamic-code block will only be chosen to be emitted if it has
   * fewer bits than a fixed-code block would for the same set of symbols.
   * Therefore its average symbol length is assured to be less than 31. So
   * the compressed data for a dynamic block also cannot overwrite the
   * symbols from which it is being constructed.
*/

/*
On 'prev' and 'head'

Let 's->strstart' be the place in 'window' we are about
to process. Zlib does the following:
	hash_head = NIL;
        if (s->lookahead >= MIN_MATCH) {
            INSERT_STRING(s, s->strstart, hash_head);
        }
This hashes the string 'window[s->strstart .. s->strstart+2]'
into 's->inh' and sets
	hash_head = s->prev[s->strstart & s->w_mask] = s->head[s->ins_h]
	s->head[s->ins_h] = s->strstart
then it calls 'longest_match'
	if (hash_head != NIL && s->strstart - hash_head <= MAX_DIST(s)) {
		s->match_length = longest_match (s, hash_head);
        }
Note that with the condition
	hash_head != NIL 
zlib does not allow matches with the string at window[0].
Every time a new window is filled 'CLEAR_HASH' sets all
the elements of 'head' to 'NIL' so this can also mean
that there are no previous matches for that hash value.
	
By deflate.h:

   #define MAX_DIST(s)  ((s)->w_size-MIN_LOOKAHEAD)

   * In order to simplify the code, particularly on 16 bit machines, match
   * distances are limited to MAX_DIST instead of WSIZE.

Zlib now compares the match at 's->strstart' with
the contents of 'window' at 'cur_match' with
	cur_match = hash_head

Notice that since
	hash_head == s->head[s->ins_h]
'hash_head' is the last position of window were we found
the same hash value.
Notice that said position is now kept in 'prev' at 's->strstart & s->w_mask'.

After processing this match we update 'cur_match'
	cur_match = prev[cur_match & wmask]
This new position will me used to search for a new
match as long as two conditions are met:
	(1) cur_match > limit
and
	(2) --chain_length != 0
where 'limit' si defined as:
	IPos limit = s->strstart > (IPos)MAX_DIST(s) ?
			s->strstart - (IPos)MAX_DIST(s) : NIL;
Let
	MAX_DIST(s) < s->strstart
and let
	y < x - MAX_DIST(s)
then
	x - (x - MAX_DIST(s)) < x - y
          x - x + MAX_DIST(s) < x - y
                  MAX_DIST(s) < x - y
Meaning that condition (1) is meant to avoid
matches longer than 'MAX_DIST(s)'.
If
	s->strstart <= MAX_DIST(s)
then
	limit = 0
meaning that condition (1) avoids matches at
'window' position 0.

Condition two is meant to bound the depth search into 'prev'.
We follow the hash policy used in zlib which can
be found in 'deflate.c'. Current parameters
follow the ones used for "max compression".
There are four parameters that control the search
for patterns in 'prev':

                       name used        configuration_table
                       in the code      parameter
        ----------------------------------------------------
	good_length  | good_match     |good_length
	chain_length | chain_length   |max_chain_length
	max_lazy     | max_lazy_match |max_lazy
	max_length   | nice_length    | nice_match

We comment the code that pertains to 'max_lazy'
because under the current parameters such code
is redundant.

'prev' holds 'uint64_t' values in our implmentation.

We read 'head' and 'prev' like this:
	   last = head[h];
	prev[q] = last;
	head[h] = q;

We just make sure that 'last' is within the max distance
allowed:
	abs(x - last) < 32768
the condition
	(3) x < last
makes no sense since we want matches against past data
then in order to avoid matching with data beyond 'q'
we initilize to zero 'head' before processing any data.
All new values for 'head' will then fullfil (3).
Then
	abs(x - last) < 32768
	q - x < 32768
	q < 32768 + x

We also need to initialize 'prev' to zero in order
to guarantee that for elements in 'prev' not previosly
used we never have a value greater that 'q'.

Note that we do not update 'head' (head[h] = q)
during lazy matching.
*/


/*

Write to symbuf a literal or a length + distance

Create the huffman encoding

Encode 'symbuf':

literal:

         1 << 14
       -----------
       *************************
       ^          ^
     huff(x)

length + distance:

         1 << 14
       -----------
       *************************
       ^          ^
     enc(x)       x

enc(x) = huff(length) [extra_length] huff(distance) [extra_distance]
*/

/*
1 << 15 = 32768 = 32k window size

m8 =          e                          f
     sizeof(uint8_t)*(1<<16)  sizeof(uint8_t)*(1<<16)+316

e = 2 * (1<<15) = 1<<16

     ---- 1<<16 ----   ----- 316 -----
f =  pbuf    symbuf   llitlen  ldists
    1<<14   3(1<<14)    286      30

     pbuf    symbuf
    1<<14  3 * 1<<14 = 4*(1<<14) = 1<<(14+2) = 1<<16

m16 = sizeof(uint16_t)*((1<<15)+(1<<15)+1236)
      sizeof(uint16_t)*((1<<16)+1236)

                    ------------------- 1236 ---------------------
                         hmem = 604
                    ------------------------
m16 =  prev   head  freq  count  offset  map  maq  hlitlen  hdists
      1<<15  1<<15   316   16      16    286  286    286      30       

	            freq:
	                   lengths         |      distances
	            --------------------------------------------
	             257..285 [0 a 5 bits] | 0..29 [0 a 13 bits]
	             257..285 [0..31]      | 0..29 [0,8191]
	                 .:.
	             literals eob lengths distances = 316
	                256    1    29       30
.:.
For
sizeof(uint8_t) = 1
sizeof(uint16_t)= 2

 sizeof(uint8_t)*(1<<16) + sizeof(uint8_t)*(1<<16)+316
= (1<<16) + (1<<16) + 316
= 2 * (1<<16) + 316
= (1<<17) + 316

 sizeof(uint16_t)*((1<<16)+1236)
= 2 * ( (1<<16) + 1236 )
= 2 * (1<<16) + 2 * 1236
= (1<<17) + 2472

=> 

  (1<<17) + 316 + (1<<17) + 2572
= 2 * (1<<17) + 316 + 2572
= (1 << 18) + 2788

========================================0
https://zlib.net/zlib_tech.html

For
windowBits = 15
memLevel = 8

  (1 << (windowBits+2)) + (1 << (memLevel+9)) + 6 KB
= (1 << (15 + 2)) + (1 << (8 + 9)) + 6KB
= (1 << 17) + (1 << 17) + 6KB
= 2 * (1 << 17) + 6KB
= (1 << 18) + 6KB

*/
int deflate(uint8_t *d, uint64_t ld, uint8_t **result, uint64_t *szresult)
{
	uint8_t *m8 = (uint8_t *)malloc(sizeof(uint8_t) *((1<<16) + (1<<16) + 316));
	if (!m8) goto error;

	uint16_t *m16 = (uint16_t *)malloc(sizeof(uint16_t)*((1<<15) + 1236));
	if (!m16) goto error;

	uint8_t *e = m8;

	uint8_t *pbuf    = m8 + (1<<16);
	uint8_t *symbuf  = m8 + (1<<16) + (1<<14);
	uint8_t *llitlen = m8 + (1<<16) + (1<<14) + (1<<16); 
	uint8_t *ldists  = m8 + (1<<16) + (1<<14) + (1<<16) + 286;

	uint16_t *head    = m16;
	uint16_t *prev    = m16 + (1<<16);
	uint16_t *freq    = m16 + (1<<16);
	uint16_t *hmem    = m16 + (1<<16) + 316;
	uint16_t *hlitlen = m16 + (1<<16) + 316 + 604;
	uint16_t *hdists  = m16 + (1<<16) + 316 + 604 + 286;

	uint16_t good_length  = 32;
	uint16_t chain_length = 4096;
	uint16_t max_length = 258;
	/*uint16_t max_lazy = 258;*/

	uint8_t *b = NULL;
	uint64_t szb = 0;

	uint16_t chain;

	/*Runs over 'd'*/
	uint64_t p = 0;
	/*Runs over 'e'*/
	uint32_t q;
	/*Runs over 'symbuf' when storing a symbol
	and when reading the content of 'symbuf' as
	they are being encoded.*/
	uint32_t r;
	/*Stores how many symbols 'symbuf' contains.*/
	uint32_t nsym;
	/*Runs over 'pbuf'*/
	uint32_t z;
	/*After filling 'e' this is how
	many bytes it contains*/
	uint32_t l;

	uint16_t index;
	uint16_t symbol;

	uint16_t length;
	uint16_t distance;

	uint32_t v;
	uint16_t h;

	uint16_t last;
	uint16_t k;

	uint8_t *bl_litlen;
	uint8_t *bl_dists;
	uint16_t nbl_litlen;
	uint16_t nbl_dists;

	uint16_t code;
	uint8_t len;
	uint16_t extra;

	int8_t f;

	uint8_t done = 0;

	/*Runs over the bytes of the memory where
	the symbols are finally stored.*/
	uint64_t t = 0;
fill:
	l = 0;
	q = 32768;

	{/*Read into 'e' ----*/
	uint32_t sup = 32768;
	if (ld <= p + 32768){
		sup = ld - p;
		done = 1;
	}

	//XXX cotas
	for (uint32_t j = 32768; j < 65535; j++)
		e[l++] = e[j];

	while (sup--)
		e[l++] = d[p++];
	}/*---- Read into 'e'*/
block:
	memset(head, 0, (1<<15)*sizeof(uint16_t));
	memset(prev, 0, (1<<15)*sizeof(uint16_t));
	nsym = 0;
	r = 0;
	z = 0;
	f = 0;
match:
	chain = 0;
hash:
	length = 3;
	distance = 0;
	if (l <= q + 4) goto end;

	v = (e[q+3]<<24) | (e[q+2]<<16) | (e[q+1]<<8) | e[q];
	h = ((v * 66521 + 66521) >> 16) & 32767;//32767=(2**15)-1

	   last = head[h];
	prev[q] = last;
	head[h] = q;

	if (q < last + 32768){
		for (k = 0; k < max_length; k++)
			if (l < q + k)
				if (e[last+k] != e[q+k]) break;
			else goto end;
		if (length <= k){
			length = k;
			distance = q - last;
		}
	}

	if (distance == 0) goto store;

	/*Policy*/
	if (good_length <= length) chain_length >>= 2;
	/*if (max_lazy <= length) goto store;*/

	if (chain++ < chain_length) goto hash;

	chain = 0;
lazy:
	if (l <= q + 5) goto end;

	v = (e[q+4]<<24) | (e[q+3]<<16) | (e[q+2]<<8) | e[q+1];
	h = ((v * 66521 + 66521) >> 16) & 32767;

	   last = head[h];
	prev[q] = last;

	if (q < last + 32767){
		for (k = 0; k < max_length; k++)
			if (l < q + 1 + k)
				if (e[last+k] != e[q+1+k]) break;
			else goto end;
		if (length < k){
			/*If the lazy match is better
			discard the match by just storing
			a literal*/
			distance = 0;
			goto store;
		}
	}

	/*Policy*/
	if (good_length <= length) chain_length >>= 2;
	/*if (max_lazy <= length) goto store;*/

	if (chain++ < chain_length) goto lazy;
store:
	if (distance == 0){
		uint8_t literal = e[q];
		freq[literal]++;
		symbuf[r++] = 0;
		symbuf[r++] = 0;
		symbuf[r++] = literal;
	}
	else{
		index = ilength[length];
		symbol = 257 + index;
		freq[symbol]++;

		if (distance <= 256)
			index = idistance[distance];
		else
			index = idistance[256 + ((distance-1) >> 7)]; //XXX de dónde saqué esto

		symbol = 257 + 29 + index;
		freq[symbol]++;

		symbuf[r++] = (uint8_t)(255 & distance);
		symbuf[r++] = (uint8_t)(distance >> 8);
		symbuf[r++] = (uint8_t)(255 & (length - 3));

		/*Insert the bytes of the match into the hash table*/
		last = head[h];
		prev[q] = last;
	}
	q++;

	if (nsym++ < 16383) goto match;
encode:
	{/*Writing block headers ---- */
	/*
	uint8_t header = 255;            //11111111
		header = (header & 127); //11111111 & 01111111 not final
		header = (header & 191); //01111111 & 10111111 fixed
	                                 //00111111 = 63
	-----
	fixed:	10111111 = 191 final and fixed
	*/
	uint16_t header;
	if (!done)
		header = 63;
	else header = 191;

	header <<= 8;

	send(&f,&z,pbuf,3,header);
	}/* ---- Writing block headers*/

	{/*Build huffman codes ----*/

	bl_litlen = fixed_lengths;
	nbl_litlen = 286;
	bl_dists = fixed_distances;
	nbl_dists = 30;

	if (huffd(hmem,hlitlen,llitlen,bl_litlen,nbl_litlen))
		goto error;
	if (huffd(hmem,hdists,ldists,bl_dists,nbl_dists))
		goto error;

	}/*---- Build huffman codes*/

	while(nsym--){
		distance  = symbuf[r++];
		distance += (symbuf[r++] << 8);
		/*Literal*/
		if (distance == 0 ){
			uint8_t lit = symbuf[r++];
			code = hlitlen[lit];
			len  = llitlen[lit];
			send(&f,&z,pbuf,len,code);
		}
		/*Length + distance*/
		else{
			length = symbuf[r++] + 3;

			index = ilength[length];
			symbol = 257 + index;
			code = hlitlen[symbol];
			len  = llitlen[symbol];
			send(&f,&z,pbuf,len,code);

			extra = length - LZ77len[index];
			if (extra){
				len = LZ77len_extra[index];
				send(&f,&z,pbuf,len,extra);
			}

			if (distance <= 256)
				index = idistance[distance];
			else
				index = idistance[256 + (distance-1) >> 7];
			symbol = 257 + 29 + index;
			code = hdists[symbol];
			len  = ldists[symbol];
			send(&f,&z,pbuf,len,code);

			extra = distance - LZ77distance[index];
			if (extra){
				len = LZ77distance_extra[index];
				send(&f,&z,pbuf,len,extra);
			}
		}
	}
	/*End of block*/
	send(&f,&z,pbuf,llitlen[256],hlitlen[256]);

	{/*Writing ----*/
	szb += z;
	b = (uint8_t *)realloc(b, szb);
	if (!b) goto error;

	for (uint32_t inc = 0; inc < z; inc++)
		b[t++] = pbuf[inc];

	if (!done) goto fill;

	*szresult = szb;
	*result = b;
	}/*Writing ----*/

	free(m8);
	free(m16);
	return 0;
error:
	*szresult = 0;
	free(b);
	free(m8);
	free(m16);
	return 1;
end:
	for (;q < l; q++){
		uint8_t literal = e[q];
		freq[literal]++;
		symbuf[r++] = 0;
		symbuf[r++] = 0;
		symbuf[r++] = literal;
	}
	goto encode;
}


/*
encodeamos cuando:

Symbuf lleno

e vacío
 - symbuf sin llenar

*/


/*
https://web.archive.org/web/20190108202303/http://www.hackersdelight.org/hdcodetxt/crc.c.txt
https://web.archive.org/web/20190716204559/http://www.hackersdelight.org/permissions.htm
*/
static uint32_t crc32b(uint8_t *message, uint64_t len) {
	uint32_t crc, mask;

	crc = 0xFFFFFFFF;
	for (uint64_t i = 0; i < len; i++) {
		crc = crc ^ message[i];
		for (uint8_t j = 0; j < 8; j++) {
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}
	return ~crc;
}

static uint32_t crc32(uint8_t *buffer, uint64_t len)
{
	static uint32_t crc_table[256] =
	{
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
	};

	uint32_t crc = ~0u;
	for (uint64_t i = 0; i < len; ++i)
		crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
	return ~crc;
}


/*
'd' is the deflated file

'l' is the length of 'd'

'*result' must be 18 bytes long. Its first 10 bytes
          are the header for the gziped file and the
          last 8 the footer.
*/
void gziphf(uint8_t *d, uint64_t l, uint8_t **result)
{
	uint8_t *b = *result;

	/*
	RFC 1952 2.3.1.2:
	"A compliant compressor must produce files with correct ID1,
        ID2, CM, CRC32, and ISIZE, but may set all the other fields in
        the fixed-length part of the header to default values (255 for
        OS, 0 for all others).  The compressor must set all reserved
        bits to zero."
	*/
	b[0] = 31;  /*ID1*/
	b[1] = 139; /*ID2*/
	b[2] = 8;   /*CM*/
	b[3] = 0;   /*FLG*/
	b[4] = 0;     /*MTIME*/
	b[5] = 0;     /*MTIME*/
	b[6] = 0;     /*MTIME*/
	b[7] = 0;     /*MTIME*/
	b[8] = 0;   /*XFL*/
	b[9] = 255; /*OS*/

	uint32_t crc = crc32(d, l);
	b[10] = (uint8_t)(crc >>  0);
	b[11] = (uint8_t)(crc >>  8);
	b[12] = (uint8_t)(crc >> 16);
	b[13] = (uint8_t)(crc >> 24);

	uint32_t isize = (uint32_t)(4294967296 & l);
	b[14] = (uint8_t)(isize >>  0);
	b[15] = (uint8_t)(isize >>  8);
	b[16] = (uint8_t)(isize >> 16);
	b[17] = (uint8_t)(isize >> 24);
}
























/*
"2.2. File format

A gzip file consists of a series of "members" (compressed data
sets).  The format of each member is specified in the following
section.  The members simply appear one after another in the file,
with no additional information before, between, or after them."
*/
int gzip(uint8_t *a, uint64_t l, uint8_t **b, uint64_t *szb)
{
	uint8_t max_fname = 100; //XXX uint8_t ¿?
	uint8_t max_fcomment = 100;

	uint64_t acc = 0;
	/*
	"Each member has the following structure:


	+---+---+---+---+---+---+---+---+---+---+
	|ID1|ID2|CM |FLG|     MTIME     |XFL|OS | (more-->)
	+---+---+---+---+---+---+---+---+---+---+
	"
	*/
	if (l < 10) return 1;
	/*
	ID1 (IDentification 1)
	ID2 (IDentification 2)
	These have the fixed values ID1 = 31 (0x1f, \037), ID2 = 139
	(0x8b, \213), to identify the file as being in gzip format."
	*/
	if (a[acc++] != 31) return 1;
	if (a[acc++] != 139) return 1;

	/*
	"CM (Compression Method)
            This identifies the compression method used in the file.  CM
            = 0-7 are reserved.  CM = 8 denotes the "deflate"
            compression method, which is the one customarily used by
            gzip and which is documented elsewhere."
	*/
	if (a[acc++] != 8) return 1;
	/*
	"FLG (FLaGs)
	    This flag byte is divided into individual bits as follows:

	       bit 0   FTEXT
	       bit 1   FHCRC
	       bit 2   FEXTRA
	       bit 3   FNAME
	       bit 4   FCOMMENT
	       bit 5   reserved
	       bit 6   reserved
	       bit 7   reserved"
	*/
	uint8_t flag = a[acc++];

	/*
	We skip MTIME, XFL and OS.
	a->d[4] - a->d[9]
	*/
	acc += 6;

	uint32_t xlen = 0;
	/*FEXTRA = (2^2) = 4*/
	if (flag & 4){
		if (l < acc + 12) return 1;
		xlen = (a[acc++] << 8) + a[acc++];
	}

	if (l < acc + xlen) return 1;

	/*We skip the Extra Field*/
	acc += xlen;

	/*FNAME = (2^3) = 8*/
	if (flag & 8)
		/*We skip the file name*/
		for (uint64_t k = 0; acc <= l; k++){
			if (a[acc++] == 0) break;
			if (max_fname < k) return 1;
		}

	/*FCOMMENT = (2^4) = 16*/
	if (flag & 16)
		/*We skip the file comment*/
		for (uint8_t k = 0; acc <= l; k++){
			if (a[acc++] == 0) break;
			if (max_fcomment < k) return 1;
		}

	/*FHCRC = (2^1) = 2*/
	if (flag & 2)
		/*We skip the crc*/
		acc += 2;

	if (l <= acc) return 1;

	if (inflate(a+acc,l-acc,b,szb)) return 1;

	return 0;
}






#define TYPE(a,b,c,d)  (((uint8_t) (a) << 24) + ((uint8_t) (b) << 16) + ((uint8_t) (c) << 8) + (uint8_t) (d))
#define nm16(a)  (((uint8_t) *(a+p+1) << 8) + (uint8_t) *(a+p))
#define nm32(a)  (((uint8_t) *(a+p+3) << 24)+((uint8_t) *(a+p+2) << 16)+((uint8_t) *(a+p+1) << 8)+(uint8_t) *(a+p))

#define MOD256(x) ((uint8_t) ((x) & 255))

/*
"3.1. PNG file signature

The first eight bytes of a PNG file always contain the following
(decimal) values:

137 80 78 71 13 10 26 10"
*/
/*
"The IHDR chunk shall be the first chunk in
 the PNG datastream.
*/
/*
"The IHDR chunk must appear FIRST.  It contains:

	Width:              4 bytes
	Height:             4 bytes
	Bit depth:          1 byte
	Color type:         1 byte
	Compression method: 1 byte
	Filter method:      1 byte
	Interlace method:   1 byte"
*/
/*
"Color    Allowed     Interpretation
 Type    Bit Depths

   0     1,2,4,8,16   Each pixel is a grayscale sample.

   2     8,16         Each pixel is an R,G,B triple.

   3     1,2,4,8      Each pixel is a palette index;
		      a PLTE chunk must appear.

   4     8,16         Each pixel is a grayscale sample,
		      followed by an alpha sample.

   6     8,16         Each pixel is an R,G,B triple,
		      followed by an alpha sample."
*/

/*
        Table 11.1 — Allowed combinations of colour type and bit depth

PNG image type	   Colour type	Allowed bit depths	Interpretation
Greyscale	        0	1, 2, 4, 8, 16	      Each pixel is a greyscale sample
Truecolour	        2	8, 16	              Each pixel is an R,G,B triple
Indexed-colour	        3	1, 2, 4, 8	      Each pixel is a palette index;
                                                      a PLTE chunk shall appear.
Greyscale with alpha    4	8, 16	              Each pixel is a greyscale sample
                                                      followed by an alpha sample.
Truecolour with alpha	6	8, 16	              Each pixel is an R,G,B triple
                                                      followed by an alpha sample.
*/

/*
m = m8

m8 = palette
      255
*/


/*
'abs' recibe 'int' y regresa 'int', además
hay restas entonces dejamos 'int' en todos lados.
*/
static int paeth(int a, int b, int c)
{
	int p = a + b - c;
	int pa = abs(p-a);
	int pb = abs(p-b);
	int pc = abs(p-c);
	if (pa <= pb && pa <= pc) return a;
	if (pb <= pc) return b;
	return c;
}


/*
Palette = 256 * 4
	  R G B A
*/




int pngr(uint8_t *d, uint64_t l, uint8_t **result, uint64_t *szresult)
{
	uint64_t p = 0;
	
	uint64_t i;
	uint64_t j;

	uint8_t *m = (uint8_t *)malloc(1024*sizeof(uint8_t));

	uint8_t *m8 = (uint8_t *)m;
	uint8_t *palette = m8;

	uint8_t *a = NULL;
	uint64_t szidat = 0;

	/*PNG signature ----*/
	if (l < 8) goto error;

	if (d[p++] != 137) goto error;
	if (d[p++] != 80)  goto error;
	if (d[p++] != 78)  goto error;
	if (d[p++] != 71)  goto error;
	if (d[p++] != 13)  goto error;
	if (d[p++] != 10)  goto error;
	if (d[p++] != 26)  goto error;
	if (d[p++] != 10)  goto error;
	/*---- PNG signature*/

	uint16_t pal_len = 0;

	/*trans == 1 if we read a tRNS chunk*/
	uint8_t trans = 0;

	uint8_t grayt;
	uint8_t rgbt[3];

	/*Chunk length*/
	uint32_t clen;
	/*Chunk type*/
	uint32_t ctype;

	/*IHDR ----*/
	/*4 length + 4 type*/
	if (l < p + 8) goto error;

	clen = nm32(d);  p+=4;
	if (clen != 13) goto error;

	ctype = nm32(d); p+=4;
	if (TYPE('I','H','D','R') != ctype) goto error;

	if (l < p + 13) goto error;

	uint32_t x = nm32(d); p+=4;
	uint32_t y = nm32(d); p+=4;
	if (x == 0) goto error;
	if (y == 0) goto error;
	if (16777216 < x) goto error;//1<<24
	if (16777216 < y) goto error;

	/* (1<<24)*(1<<24) */
	uint64_t limit  = 281474976710656U;

	uint8_t depth = d[p++];
	if (depth != 1 && depth != 2 && depth != 4 && depth != 8)
		goto error;

	/*"When pixels have fewer
	than 8 bits and the scanline width is not evenly divisible by the
	number of pixels per byte, the low-order bits in the last byte of
	each scanline are wasted.  The contents of these wasted bits are
	unspecified."*/
	/*The next two variables are only used when 'depth < 8'*/
	/*Used bits in last byte of a scanline*/
	uint8_t ub = 8;
	/*Pixels per byte*/
	uint32_t ppb = 1;
	if (depth < 8){
		if (depth == 1) ppb = 8;
		else if (depth == 2) ppb = 4;
		else if (depth == 4) ppb = 2;
		/*After the loop 'ub' will be how many bits in the last
		byte of each scanline are specified.*/
		uint32_t d = x/ppb;
		if ((d * ppb) != x){
			for(ub = 1; ub < ppb; ub++){
				d = (x + ub)/ppb;
				if ((d * ppb) == (x + ub)) break;
			}
		}
	}

	uint8_t color = d[p++];
	if (color == 3 && depth == 16) goto error;
	if (color == 1 || color == 5 || 6 < color) goto error;

	/*Values from table 11.1*/	
	uint8_t nchannels;
	switch (color){
	case 0: nchannels = 1; break;/* gray */
	case 2: nchannels = 3; break;/* rgb */
	case 3: nchannels = 1; break;/* indexed rgb with PLTE */
	case 4: nchannels = 2; break;/* gray + alpha */
	case 6: nchannels = 4; break;/* rgb+alpha */
	default: goto error;
	}
	/*"Alpha samples are represented with the same bit
	depth used for the image samples"*/
	/*"Bit depth is a single-byte integer giving the number of bits
	per sample or per palette index (not per pixel).  Valid values
	are 1, 2, 4, 8, and 16"*/
	uint8_t lenpix = nchannels * (depth == 16 ? 2 : 1);

	/*"Only compression method 0 (deflate/inflate compression with a
	sliding window of at most 32768 bytes) is defined in this
	International Standard."*/
	if (d[p++] != 0) goto error;

	/*"Only filter method 0 (adaptive filtering with five
	basic filter types) is defined in this International Standard"*/
	if (d[p++] != 0) goto error;

	/*"Two values are defined in this International
	Standard: 0 (no interlace) or 1 (Adam7 interlace)."*/
	/*We won't support interlacing*/
	if (d[p++] != 0) goto error;

	/*Skip crc*/
	if (l < (p+=4)) goto error;

	/*---- IHDR*/

	/*We process four possible chunk types: IDAT, IEND,
	PLTE and tRNS.
	We only accept tRNS if we received before PLTE.
	Following "Figure 5.3 — Lattice diagram: PNG images
	without PLTE in datastream" we only accept chunks
	different from IDAT and IEND before receiving either.
	All other chunk types are ignored.*/
chunks:
	//XXX esto sí parsea adecuadamente?

	/*4 length + 4 type*/
	if (l < p + 8) goto error;
	clen = nm32(d);  p+=4;
	ctype = nm32(d); p+=4;

	if      (TYPE('I','D','A','T') == ctype) goto idat;
	else if (TYPE('I','E','N','D') == ctype) goto iend;
	else if (szidat) goto error;
	else if (TYPE('P','L','T','E') == ctype) goto plte;
	else if (TYPE('t','R','N','S') == ctype) goto trns;
	else if (l < (p+=clen)) goto error;/*skip chunk data*/
crc:
	/*Skip crc*/
	if (l < (p+=4)) goto error;
	goto chunks;

plte:
	/*"This chunk ...shall not appear for colour types 0 and 4"*/
	if (color == 0 || color == 4) goto error;

	/*"For color types 2 and 6 (truecolor and truecolor with alpha),
	the PLTE chunk is optional.  If present, it provides a
	suggested set of from 1 to 256 colors to which the truecolor
	image can be quantized if the viewer cannot display truecolor
	directly.  If PLTE is not present, such a viewer will need to
	select colors on its own, but it is often preferable for this
	to be done once by the encoder."*/
	/*We do not support this option.*/
	if (color == 2 || color == 6) goto error;

	/* By table 11.1 */
	if (depth == 16) goto error;

	/*"The PLTE chunk contains from 1 to 256 palette entries,
	each a three-byte series of the form:
	Red	1 byte
	Green	1 byte
	Blue	1 byte"*/
	/*"Note that the palette uses 8 bits (1 byte) per sample
	regardless of the image bit depth specification.  In
	particular, the palette is 8 bits deep even when it is a
	suggested quantization of a 16-bit truecolor image."*/
	if (clen == 0) goto error;
	if (768 < clen) goto error;/* 256 * 3 */

	/*"The number of palette entries
	must not exceed the range that can be represented in the image
	bit depth (for example, 2^4 = 16 for a bit depth of 4).  It is
	permissible to have fewer entries than the bit depth would
	allow.  In that case, any out-of-range pixel value found in the
	image data is an error."*/
	if ((1 << depth) < clen) goto error;

	/*"The number of entries is determined from
	the chunk length. A chunk length not divisible
	by 3 is an error."*/
	pal_len = clen / 3;
	if (pal_len * 3 != clen) goto error;

	if (l < p + pal_len) goto error;
	for (int k = 0; k < pal_len; k++) {
		palette[(k*4)+0] = d[p++];/*red*/
		palette[(k*4)+1] = d[p++];/*green*/
		palette[(k*4)+2] = d[p++];/*blue*/
		palette[(k*4)+3] = 255;   /*alpha*/
	}

	if (color == 3){
		/*We treat an overflow as error so if
			255 < input * (MAXOUTSAMPLE / MAXINSAMPLE)
		we return error.*/

		if (depth == 4)
			for (int k = 0; k < pal_len; k++){
				// 15 * 17 = 255
				if (15 < palette[k*4+0]) return 1;
				if (15 < palette[k*4+1]) return 1;
				if (15 < palette[k*4+2]) return 1;
				palette[k*4+0] = 17 * palette[k*4+0];
				palette[k*4+1] = 17 * palette[k*4+1];
				palette[k*4+2] = 17 * palette[k*4+2];
			}
		else if (depth == 2)
			for (int k = 0; k < pal_len; k++){
				// 3 * 85 = 255
				if (3 < palette[k*4+0]) return 1;
				if (3 < palette[k*4+1]) return 1;
				if (3 < palette[k*4+2]) return 1;
				palette[k*4+0] = 85 * palette[k*4+0];
				palette[k*4+1] = 85 * palette[k*4+1];
				palette[k*4+2] = 85 * palette[k*4+2];
			}
		else if (depth == 1)
			for (int k = 0; k < pal_len; k++){
				if (1 < palette[k*4+0]) return 1;
				if (1 < palette[k*4+1]) return 1;
				if (1 < palette[k*4+2]) return 1;
				/*
				palette[k*4+0] = 255 * palette[k*4+0]
                                               = 255 * 1 = palette[k*4+0]
                                or
				palette[k*4+0] = 255 * palette[k*4+0]
                                               = 255 * 0 = palette[k*4+0]
				*/
			}
	}
	goto crc;
trns:
	trans = 1;

	/*For colour type 3 (indexed-colour),
	the tRNS chunk contains a series of
	one-byte alpha values, corresponding
	to entries in the PLTE chunk*/
	if (color == 3 && !pal_len) goto error;

	if (clen == 0) goto error;

	/*"A tRNS chunk shall not appear for colour types
	4 and 6, since a full alpha channel is already
	present in those cases"*/
	if (color == 4 || color == 6) goto error;

	if (color == 3){
		/*"For colour type 3 (indexed-colour) ...
		The tRNS chunk shall not contain more alpha
		values than there are palette entries, but a
		tRNS chunk may contain fewer values than there
		are palette entries."*/
		if (pal_len < clen) goto error;
		if (l < p + clen) goto error;

		/*For colour type 3 (indexed-colour), the tRNS
		chunk contains a series of one-byte alpha values,
		corresponding to entries in the PLTE chunk. Each
		entry indicates that pixels of the corresponding
		palette index shall be treated as having the
		specified alpha value."*/
		for (uint16_t k=0; k < clen; k++)
			palette[(k*4)+3] = d[p++];

		/*The tRNS chunk shall not contain more alpha values
		than there are palette entries, but a tRNS chunk may
		contain fewer values than there are palette entries.
		In this case, the alpha value for all remaining palette
		entries is assumed to be 255.*/
		/*We already stored '255' as a deflautl value in all
		other palette entries.*/
	}
	else if (color == 0){
		if (l < p + 2) goto error;
		grayt = MOD256(nm16(d)); p+=2;
	}
	else if (color == 2){
		uint16_t scale;
		if (depth == 1) scale = 255;
		else if (depth == 2) scale = 85;
		else if (depth == 3) scale = 17;
		if (l < p + 6) goto error;
		rgbt[0] = MOD256(MOD256(nm16(d))*scale); p+=2;
		rgbt[1] = MOD256(MOD256(nm16(d))*scale); p+=2;
		rgbt[2] = MOD256(MOD256(nm16(d))*scale); p+=2;
	}
	goto crc;
idat:
	/* "For colour type 3 (indexed-colour),
	the PLTE chunk is required."*/ 
	if (color == 3 && (!pal_len)) goto error;

	/*We do not accept an empty IDAT chunk*/
	if (clen == 0) goto error;

	if (limit < szidat + clen) goto error;

	a = (uint8_t *)realloc(a,szidat + clen);
	if (!a) goto error;

	for (uint64_t j = 0; j < clen; j++)
		d[p++] = a[szidat++];

	goto crc;
iend:
	/*We do not accept no data because we already
	asked for xg != 0 and yg != 0.*/
	if (!szidat) goto error;
	
	/*"The chunk's data field is empty."*/

	/*Skip crc*/
	if (l < (p+=4)) goto error;

	/*Whole image received. Process the data received.*/

	/*De-filtering ---- */
	
	/*Iterate over width of the image*/
	i = 0;
	/*Iterate over depth of the image*/
	j = 0;

	uint64_t q = 0;
	uint64_t r = 0;

	/* scanlen <= 2 * 1<<32 = 1<<33 < 1<<64 */
	uint64_t scanlen = lenpix * x;

	/*
	"an additional "filter type" byte is added to the beginning of
	every scanline (see filtering, section 2.5).  the filter type byte
	is not considered part of the image data, but it is included in
	the datastream sent to the compression step."
	*/
	uint8_t filter;

	/*Unfilter the first line ----*/

	/*line bytes + filter byte*/
	if (szidat < q + scanlen + 1) goto error;

	filter = a[q++];

	if (filter == 0)
		/*Here we remove the 'filter' value of each line*/
		for (i = 0; i < scanlen; i++, r++, q++)
			a[r] = a[q];
	/* Sub(x) = Orig(x) - Orig(x-lenpix) */
	else if (filter == 1){
		/*First pixel in line*/
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = a[q];
		for (     ; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + a[q - lenpix]);
	}		
	/* Up(x) = Orig(x) - Orig(x-scanline) */
	else if (filter == 2)
		for (i = 0; i < scanlen; i++, r++, q++)
			a[r] = a[q];
	/* Average(x) = Orig(x) - floor((Orig(x-lenpix) + Orig(x-scanline)) / 2) */
	else if (filter == 3){
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = a[q];
		for (     ; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + (a[q - lenpix] >> 1));
	}
	/* Paeth = Orig(x) - PaethPredictor(Orig(x-lenpix), Orig(x-scanline), Orig(x-scanline-lenpix))*/
	else if (filter ==4){
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = MOD256(a[q] + paeth(0, 0, 0));
		for (     ; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + paeth(a[q - lenpix], 0, 0));
	}
	else goto error;

	j++;

	/*---- Unfilter the first line*/
filter:
	if (szidat < q + scanlen + 1) goto error;

	filter = a[q++];

	if (filter == 0)
		for (i = 0; i < scanlen; i++, r++, q++)
			a[r] = a[q];
	else if (filter == 1){
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = a[q];
		for (     ; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + a[q - lenpix]);
	}		
	else if (filter == 2)
		for (i = 0; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + a[q - scanlen]);
	else if (filter == 3){
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = MOD256(a[q] + (a[q - scanlen] >> 1));
		for (     ; i < scanlen; i++, r++, q++)
			a[r] = MOD256(a[q] + ((a[q - lenpix] + a[q - scanlen]) >> 1));
	}
	else if (filter ==4){
		for (i = 0; i < lenpix; i++, r++, q++)
			a[r] = MOD256(a[q] + ((a[q - lenpix] + a[q - scanlen]) >> 1));
		for (     ; i < scanlen; i++, r++, q++)
			a[i] = MOD256(a[q] + paeth(a[q - lenpix],
							 a[q - scanlen],
							 a[q - scanlen - lenpix]));
	}
	else goto error;

	if (y < j++) goto filter;

	/* ---- De-filtering*/

	/*Scaling ---- */

	if ((color == 0) && (depth < 8)){

		/*(1<<24) * (1<<24) * 8  = 2251799813685248 */
		uint64_t szt = x * y * ppb;

		uint8_t *b = (uint8_t *)malloc(szt);
		if (!b) goto error;

		uint8_t z;

		//XXX está mal i = 1 deja el primero sin procesar no el útlimo byte de cada scan
		if (depth == 4)
			for (j = 0; j < y; j++){
				for (i = 1; i < x; i++, i++){
					b[j++] = 17 * (a[i] >> 4);
					b[j++] = 17 * (a[i]  & 0x0f);
				}
				for (z = 4; z <= ub; z+=4)
					b[j++] = (a[i] >> (8 - z)) & 0x0f;
				/*
				In the first for loop we advance to the next pixel
				in the scanline by doing i++. Since we can't do the
				same in the second for loop we do it here.
				*/
				i++;
			}
		else if (depth == 2)
			for (j = 0; j < y; j++){
				for (i = 1; i < x; i++, i++){
					b[j++] = 85 * ( a[i] >> 6);
					b[j++] = 85 * ((a[i] >> 4) & 0x03);
					b[j++] = 85 * ((a[i] >> 2) & 0x03);
					b[j++] = 85 * ( a[i] & 0x03);
				}
				for (z = 2; z <= ub; z+=2)
					b[j++] = (a[i] >> (8 - z)) & 0x03;
				i++;
			}
		else if (depth == 1)
			for (j = 0; j < y; j++){
				for (i = 1; i < x; i++, i++){
					b[j++] = 85 * ( a[i] >> 7);
					b[j++] = 85 * ((a[i] >> 6) & 0x01);
					b[j++] = 85 * ((a[i] >> 5) & 0x01);
					b[j++] = 85 * ((a[i] >> 4) & 0x01);
					b[j++] = 85 * ((a[i] >> 3) & 0x01);
					b[j++] = 85 * ((a[i] >> 2) & 0x01);
					b[j++] = 85 * ((a[i] >> 1) & 0x01);
					b[j++] = 85 * ( a[i]  & 0x01);
				}
				for (z = 1; z <= ub; z++)
					b[j++] = (a[i] >> (8 - z)) & 0x01;
				i++;
			}
		free(a);//XXX
		a = b;//XXX
	}

	/* ---- Scaling*/

	/*Transparency and palette substitution ---- */

	if (color == 3){
		


	}
	//XXX revisar que haya espacio en a para x, y, etc
	else if (trans){
		/*For colors 0,2. The transparency for color 3
		was established in 'trns' if trans == 1. For
		color 4 or 6 we already returned error if there
		was a tRNS chunk.*/
		uint8_t *c;
		uint64_t zb = 0;
		uint64_t zc = 0;
		if (color == 0){
			c = (uint8_t *)malloc(2 * x * y);
			if (!c) goto error;
			for (j = 0; j < y; j++)
				for (i = 0; i < x; i++){
					uint8_t gray = a[zb++];
					c[zc++] = gray;
					if (gray == grayt)
						c[zc++] = 0;
					else
						c[zc++] = 255;
				}

		}
		else if (color == 2){
			c = (uint8_t *)malloc(4 * x * y);
			if (!c) goto error;
			for (j = 0; j < y; j++)
				for (i = 0; i < x; i++){
					uint8_t R = a[zb++];
					uint8_t G = a[zb++];
					uint8_t B = a[zb++];
					c[zc++] = R;
					c[zc++] = G;
					c[zc++] = B;
					if (R == rgbt[0] && G == rgbt[1] && B == rgbt[2])
						c[zc++] = 0;
					else
						c[zc++] = 255;
				}
		}
		free(a);//XXX
		a = c;//XXX

	}


	











	/* ---- Transparency and palette substitution*/





end:
	//TODO



error:
	free(a);
	free(m);
	return 1;
}







/*
https://ia600102.us.archive.org/16/items/annualreportfory1961united/annualreportfory1961united.pdf
https://library.si.edu/digital-library/book/annualreportfory1961united
*/



/*
Siento que aquí está el futuro de
las funciones hash

((1<<14)*3)==((1<<14)+(1<<16))

*/











































