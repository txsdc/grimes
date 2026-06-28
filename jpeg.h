#include <stdint.h>
#include <stdlib.h>//malloc

/*
We take the value of this constants from
'jpeglib.h' in libjpeg-turbo. They are originally
'MAX_COMPS_IN_SCAN' and 'MAX_SAMP_FACTOR'.

'MC' is the limit of number of components defined
in the frame header.
Meanwhile libjpeg-turbo describes 'MAX_COMPS_IN_SCAN'
as "JPEG limit on # of components in one scan".
See "Source image size" for more information
on this constant.

'SP' is the upper bound for both the horizontal and
vertical sampling factors.
*/
#define MC 4
#define SP 4


/*
ROSS, R. (1908). A Method of Solving Algebraic Equations. Nature, 78(2035), 663–665. doi:10.1038/078663b0 
*/

/*
TODO
	A la hora de comprimir poner un comentario
	aclarando que no se puede tener códigos con
	puros 1. En mí método de compresión no se dan
	esta clase de códigos entonces no tengo que
	preocuparme por ello pero sí hay que escribirlo
	y recordarlo.
*/




#define u16(a)  (((uint8_t) a[p+1] << 8) + ((uint8_t) a[p]))
#define f16(a) ((uint16_t) (a & 65535))

static const uint8_t dezigzag[64+15] =
{
    0,  1,  8, 16,  9,  2,  3, 10,
   17, 24, 32, 25, 18, 11,  4,  5,
   12, 19, 26, 33, 40, 48, 41, 34,
   27, 20, 13,  6,  7, 14, 21, 28,
   35, 42, 49, 56, 57, 50, 43, 36,
   29, 22, 15, 23, 30, 37, 44, 51,
   58, 59, 52, 45, 38, 31, 39, 46,
   53, 60, 61, 54, 47, 55, 62, 63,
   // let corrupt input sample past end
   63, 63, 63, 63, 63, 63, 63, 63,
   63, 63, 63, 63, 63, 63, 63
};

/*
Per MCU
per DCU
A un scan lo modifica el restart interval

We should check band consistency in progressive mode
but we don't

Explicar point transform es un desmadre
entonces me lo salto y justifico con
libjpeg-turbo

En progressive
Se tratan diferente primeros scans que posteriores
*/
	/*
	We initialize the block to zero so
	that when we encounter RRRRSSSS = 0
	or RRRRSSSS = F0 we don't have to set
	the block values to zero then.
	*/
	/*G.1.2.1 "The first scan for a given component
	shall encode the DC coefficient values"*/


/*G.1.2.3
"The four most significant bits, RRRR, give
the number of zero coefficients that are between
the current coefficient and the previously coded
coefficient (or the start of band)."*/

/*El comportamiento de cómo contar ceros para la
corrección en progresivo está mal. Arriba dice que
desde el principio de la banda lo que creo que implicaría
que se cuenta para atrás pero turbo cuenta para delante.
Entonces más que justificarlo con el standard copio
lo que hace turbo y esa es mi justificación.*/



int32_t pcategory[16] = {0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
int32_t ncategory[16] = {0,-1,-3,-7,-15,-31,-63,-127,-255,-511,-1023,-2047,-4095,-8191,-16383,-32767};

int16_t extend(uint16_t buf, uint8_t T)
{
	uint16_t sign = buf >> 15;
	int16_t sbuf = (int16_t)(buf >> 15-T);

	return sign ? (int16_t)(sbuf + pcategory[T]): (int16_t)(sbuf + ncategory[T]);
}

/*
S = 0|1
R = 0|1
B = 0|1

SSSS = S S S S
RRRR = R R R R

DC = SSSS {B}
	SSSS an integer in [0,11]
	{B} must produce at most 11 bits
AC = RRRR SSSS {B}
	RRRR an integer in [0,15]
	SSSS an integer in [0,10]
	{B} must produce at most 10 bits
PBLOCK = DC AC {AC}
	{AC} must produce at most 62 ACs


At the beginning of the scan
	zz0 = 0
if there is a restart interval
	zz0 = 0
again.

To read 'DC' for component 'c':

Read from the data a huffman code,
this decoded value is SSSS.

if (SSSS == 0)
	DIFF = 0

	Sice DIFF = 0 we do not nedd to
	dequantize nor update zz0.

	Store a zero.

	Begin the process to read an 'AC'
	for component 'c'.
else
	Read SSSS bits into udiff.

	Extend udiff into diff

	zz0 = zz0 + diff

	dc = zz0 * qt[c][0]

	Where 'qt[c]' is the quantization table for
	component 'c'.

	Store dc.

	Begin the process to read an 'AC'
	for component 'c'.


To read an 'AC' for component 'c':

Read from the data a huffman code,
this decoded value is RRRR SSSS.

if (SSSS == 0)
	if (RRRR == 15)
		Store zero fifteen times.

		Store zero one time.

		Restart the process to read
		an 'AC' for component 'c'.
	else if (RRRR == 0)
		All the following ACs
		are zero.

		End.
	else
		In theory this is forbidden
		but libjpeg-turbo treats this
		as if RRRR == 0.
else if (SSSS != 0)
	Store RRRR zeroes.

	Read SSSS bits into udiff.

	Extend udiff into diff.

	If the next AC value to be stored
	is the k-th value.

	zig = dezigzag[k]

	ac = diff * qt[c][zig]

	Store ac at position zig.

	if (k <= 63)
		Restart the process to
		read an 'AC' for component 'c'.
	else 
		End.
*/

/*
Table F.1 – Difference magnitude categories for DC coding

SSSS	          DIFF Value
  0                   0
  1                 -1,1
  2            [-3,-2],[2,3]
  3            [-7,-4],[4,7]
  4           [-15,-8],[8,15]
  5          [-31,-16],[16,31]
  6          [-63,-32],[32,63]
  7         [-127,-64],[64,127]
  8        [-255,-128],[128,255]
  9        [-511,-256],[256,511]
 10       [-1023,-512],[512,1023]
 11      [-2047,-1024],[1024,2047]
*/

/*Figure F.1 - Two dimensional value array for Huffman coding

	                   SSSS	
	           0     1   2  . . . 9  10
	      0  EOB0 | 
	      1  N/A  |
	      2  N/A  |  
	RRRR  .       |  Composite values
	      .       |
	      .       |
	      14 N/A  |
	      15 ZRL  |
*/

/*
Table F.2 – Difference magnitude categories for DC coding

SSSS	      AC coefficients
  1                 -1,1
  2            [-3,-2],[2,3]
  3            [-7,-4],[4,7]
  4           [-15,-8],[8,15]
  5          [-31,-16],[16,31]
  6          [-63,-32],[32,63]
  7         [-127,-64],[64,127]
  8        [-255,-128],[128,255]
  9        [-511,-256],[256,511]
 10       [-1023,-512],[512,1023]
*/

/*Decode one data unit in sequential mode*/
int DU(int16_t du[64], uint8_t qt[64],
	int16_t *zz0, int8_t *f, uint32_t *buf,
	uint8_t *d, uint64_t *p, uint64_t l)
{
	/*For iterating over ACs*/
	uint8_t k = 1;

	/*3.2
	"DIFF difference between quantized
	DC and prediction"*/
	int16_t diff;
	/*This becomes 'diff' after 'extend'*/
	uint16_t udiff;

	/*We read huffman decoded values
	into here.*/
	uint16_t code;

	/*We initialize the block to zero so that
	when we encounter RRRRSSSS = 0 or 
	RRRRSSSS = X'F0' we don't have to set the
	block values to zero then.*/
	for (uint8_t i = 0; i < 64; i++) du[i] = 0;

	/*Decode DC coefficient ----*/

	/*Huffman decoding ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);//XXX *p++, *d
			*f += 8;
		}
		else break;
	/*The smallest huffman code is 1 bit long*/
	if (*f <= 0) return 1;
	// TODO decode into 'code'
	/*---- Huffman decoding*/


	uint8_t T = (uint8_t)(code & 255);
	/*F.1.2.1.1
	"The two’s complement difference magnitudes
	are grouped into 12 categories, SSSS, and a
	Huffman code is created for each of the 12
	difference magnitude categories (see Table F.1)."*/
	if (11 < T) return 1;

	/*F.2.2.1
	"If T is zero, DIFF is set to zero"*/
	/* Sice DIFF = 0 we do not nedd to
	dequantize nor update zz0 */
	if (T==0) goto ac;
	else{
		/*Receive ----*/
		while (*f <= 24)
			if (*p + 1 <= l){
				*buf |= d[*p++] << (24 - *f);
				*f += 8;
			}
			else break;
		/*We need at least 'T' bits*/
		if (*f < T) return 1;

		udiff = f16(*buf & ((1 << T)-1));

		*buf = *buf >> T;
		*f -= T;
		/*---- Receive*/

		diff = extend(udiff,T);
	}
	/*F.2.1.3.1: ZZ(0) = PRED + DIFF*/
	*zz0 += diff;
	/*A.3.4 Dequantize DC coefficient*/
	du[0] = (int16_t)(*zz0 * qt[0]);
	/*---- Decode DC coefficient*/

	/*Figure F.13 decoding AC coefficients ----*/
ac:
	/*Huffman decoding ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);//XXX *p++, *d
			*f += 8;
		}
		else break;
	/*The smallest huffman code is 1 but long*/
	if (*f <= 0) return 1;
	// TODO decode into 'code'
	/*---- Huffman decoding*/
	
	uint8_t r = (uint8_t)(code >> 4);  /*RRRR Figure F.1*/
	uint8_t s = (uint8_t)(code & 0x0f);/*SSSS Figure F.2*/

	if (15 < r) return 1;
	if (10 < s) return 1;

	if (s == 0){
		if (r == 15){/* ZRL */
			/*F.1.2.2.1
			"the value 'RRRRSSSS' = X'F0' is defined to
			represent a run length of 15 zero coefficients
			followed by a coefficient of zero amplitude"
			*/
			/*16 =  15 zero coefficients run
			       + 1 zero amplitude coefficient*/
			/*Since we already zeroed this block we only
			addvance.*/
			k += 16;
			goto ac;
		}
		/*Following Figure F.1 we should
		throw an error in case r != 0 here.
		Since there is little to gain from this
		we don't. libjpeg-turbo doesn't either.*/

		/*F.1.2.2.1
		"a special value 'RRRRSSSS' = '00000000'
		is used to code the end-of-block (EOB),
		when all remaining coefficients in the block
		are zero"*/
		return 0;/* EOB */
	}
	/*F1.2.2.1
	"’RRRR’, give the position of the coefficient in
	ZZ relative to the previous non-zero coefficient"*/
	k += r;

	/*Figure F.14 Decode_ZZ(k) ----*/

	/*Receive ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);
			*f += 8;
		}
		else break;
	/*We need at least 's' bits*/
	if (*f < s) return 1;

	udiff = f16(*buf & ((1 << s)-1));

	*buf = *buf >> s;
	*f -= s;
	/*---- Receive*/

	diff = extend(udiff,s);

	/*---- Figure F.14 Decode_ZZ(k)*/

	/*A.3.6 and Figure A.6*/
	uint8_t zig = dezigzag[k];
	/*A.3.4 Dequantize AC coefficient*/
	du[zig] = (int16_t)(diff * qt[zig]);

	k++;
	/*63 = 64 values in a block
	      - 1 the DC coefficient*/
	if (k < 63) goto ac;
	/*---- Figure F.13 decoding AC coefficients*/

	return 0;
}

/*
At the beginning of the scan
	zz0 = 0
if there is a restart interval
	zz0 = 0
again.

To read 'DC' for component 'c':

if (SSSS == 0)
	DIFF = 0

	Sice DIFF = 0 we do not nedd to
	dequantize nor update zz0.

	Store a zero.

	End.
else
	Read SSSS bits into udiff.

	Extend udiff into diff

	zz0 = zz0 + diff

	dc = zz0 << Al

	Store dc.

	End.
*/

int initialize_DC(int16_t du[64], uint8_t Al,
	int16_t *zz0, int8_t *f, uint32_t *buf,
	uint8_t *d, uint64_t *p, uint64_t l)
{
	uint16_t code;

	/*3.2
	"DIFF difference between quantized
	DC and prediction"*/
	int16_t diff;
	/*This becomes 'diff' after 'extend'*/
	uint16_t udiff;

	/*Huffman decoding ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);//XXX *p++, *d
			*f += 8;
		}
		else break;
	/*The smallest huffman code is 1 but long*/
	if (*f <= 0) return 1;
	// TODO decode into 'code'
	/*---- Huffman decoding*/

	uint8_t T = (uint8_t)(code & 255);
	if (11 < T) return 1;

	/*F.2.2.1
	"If T is zero, DIFF is set to zero"*/
	/* Sice DIFF = 0 we do not nedd to
	dequantize nor update zz0 */
	if (T==0) return 0;
	else{
		/*Receive ----*/
		while (*f <= 24)
			if (*p + 1 <= l){
				*buf |= d[*p++] << (24 - *f);
				*f += 8;
			}
			else break;
		/*We need at least 'T' bits*/
		if (*f < T) return 1;

		udiff = f16(*buf & ((1 << T)-1));

		*buf = *buf << T;
		*f -= T;
		/*---- Receive*/

		diff = extend(udiff,T);
	}

	/*F.2.1.3.1: ZZ(0) = PRED + DIFF*/
	*zz0 += diff;

	du[0] = (int16_t)(*zz0 << Al);

	return 0;
}


/*
To refine a 'DC' for component 'c':

Read a bit from the data.

Add (1 << Al) to the DC coefficient
to be refined.

*/

int refine_DC(int16_t du[64], int16_t Albit,
	int16_t *zz0, int8_t *f, uint32_t *buf,
	uint8_t *d, uint64_t *p, uint64_t l)
{
	int32_t boolean;

	/*Receive ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);
			*f += 8;
		}
		else break;
	/*We need at least 1 bit*/
	if (*f < 1) return 1;

	boolean = *buf & 1;

	*buf = *buf >> 1;
	*f -= 1;
	/*---- Receive*/

	/*Next note and procedure taken from
	libjpeg-turbo*/
	/* Note: since we use |=, repeating
	the assignment later is safe */
	if (boolean) du[0] |= Albit;

	return 0;	
}

/*Table G.1 – EOBn code run length extensions
	EOBn code   Run length
	  EOB0       1
	  EOB1       2,3
	  EOB2       4 .. 7
	  EOB3       8 .. 15
	  EOB4       16 .. 31
	  EOB5       32 .. 63
	  EOB6       64 .. 127
	  EOB7       128 .. 255
	  EOB8       256 .. 511
	  EOB9       512 .. 1023
	  EOB10      1024 .. 2047
	  EOB11      2048 .. 4095
	  EOB12      4096 .. 8191
	  EOB13      8192 .. 16383
	  EOB14      16384 .. 32767
*/
/*Figure G.2 - Two dimensional value array for Huffman coding

	                   SSSS	
	           0     1   2  . . . 13  14
	      0  EOB0 | 
	      1  EOB1 |
	      2  EOB2 |  
	RRRR  .       |  Composite values
	      .       |
	      .       |
	      14 EOB14|
	      15 ZRL  |
*/

/*
To read an 'AC' for component 'c':

Read from the data a huffman code,
this decoded value is RRRR SSSS.

if (SSSS == 0)
	if (RRRR == 15)
		The next 15 coefficients for
		the current band are zero.

		Store zero.
	else if (RRRR == 0)
		All the remaining coefficients for
		the current band are zero.

		End.
	else
		eobrun = (1 << RRRR)

		Read RRRR bits into 'correction'.

		eobrun = eobrun + correction

		ieob = 1

		All the remaining coefficients for
		the current band are zero.

		End.
else if (SSSS != 0)
	Store RRRR zeroes.

	Read SSSS bits into udiff.

	Extend udiff into diff.

	ac = diff << Al

	If the next AC value to be stored
	is the k-th value.

	zig = dezigzag[k]

	if (k <= Se)
		Restart the process to
		read an 'AC' for component 'c'.
	else 
		End.
*/

int initialize_AC(int16_t du[64],
	uint8_t Al, uint8_t Ss, uint8_t Se,
	uint16_t eobrun, uint16_t ieob,
	int16_t *zz0, int8_t *f, uint32_t *buf,
	uint8_t *d, uint64_t *p, uint64_t l)
{
	uint16_t code;

	uint16_t udiff;
	uint16_t correction;

	/* G.1.2.2	
	"An EOB run of length 5 means that
	the current block and the next four
	blocks have an end-of-band with no
	intervening non-zero coefficients.
	*/
	if (ieob < eobrun){
		ieob++;
		return 0;
	}

	/*B.2.3
	"Ss: Start of spectral or predictor
	selection – in the DCT modes of operation, this
	parameter specifies the first DCT coefficient
	in each block in zig-zag order which shall
	be coded in the scan. This parameter shall be set to
	zero for the sequential DCT processes."
	*/
	uint8_t k = Ss;
ac:
	/*Huffman decoding ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);//XXX *p++, *d
			*f += 8;
		}
		else break;
	/*The smallest huffman code is 1 but long*/
	if (*f <= 0) return 1;
	// TODO decode into 'code'
	/*---- Huffman decoding*/

	uint8_t r = (uint8_t)(code >> 4);  /*RRRR Figure G.2*/
	uint8_t s = (uint8_t)(code & 0x0f);/*SSSS Figure G.2*/

	/*Figure G.2*/
	if (15 < r) return 1;
	if (14 < s) return 1;

	/*Figure G.2*/	
	if (s == 0){
		/* 3.2 Symbols
		"ZRL value in HUFFVAL assigned to run
		of 16 zero coefficients"*/
		/* 15 == ZRL by Table G.2 */
		if (r == 15){
			k+=16;
			goto ac;
		}
		/*Table G.1*/
		eobrun = (1 << r);
		/* G.1.2.2
		"Each EOBn code is followed by an
		extension field similar to the extension
		field for the coefficient amplitudes
		(but with positive numbers only).
		The number of bits appended to the EOBn
		code is the minimum number required to
		specify the run length."*/
		if (r){
			/*Receive ----*/
			while (*f <= 24)
				if (*p + 1 <= l){
					*buf |= d[*p++] << (24 - *f);
					*f += 8;
				}
				else break;
			/*We need at least 'r' bits*/
			if (*f < r) return 1;

			correction = f16(*buf & ((1 << r) - 1));

			*buf = *buf >> r;
			*f -= r;
			/*---- Receive*/

			eobrun += correction;

			/*This accounts for the current run*/
			ieob = 1;
		}
		return 0;
	}
	/*Figure F.13*/
	k += r;

	/*Figure F.14 Decode_ZZ(k) ----*/

	/*Receive ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);
			*f += 8;
		}
		else break;
	/*We need at least 's' bits*/
	if (*f < s) return 1;

	udiff = f16(*buf & ((1 << s) - 1));

	*buf = *buf >> s;
	*f -= s;
	/*---- Receive*/
	int16_t diff = extend(udiff, s);

	/*---- Figure F.14 Decode_ZZ(k)*/

	/*A.3.6 and Figure A.6*/
	uint8_t zig = dezigzag[k];

	/*TODO explicar ints y demás turbo usa int, yo int16_t*/
	du[zig] = (int16_t)(diff << Al);

	/*B.2.3
	"Se: End of spectral selection – Specifies
	the last DCT coefficient in each block in
	zig-zag order which shall be coded in the scan.
	This parameter shall be set to 63 for the
	sequential DCT processes."*/
	if (k <= Se) goto ac;

	return 0;
}

/*
Before the process to read an 'AC' for
component 'c:

Let k = Ss be our initial position inside
the current block.

if (ieob < eobrun)
	This means all the coefficients
	ahead of our current position are
	zero.

	ieob++

	Correct coefficients.

	End.

To read an 'AC' for component 'c':

if (Se <= k)
	End.

Read from the data a huffman code,
this decoded value is RRRR SSSS.

if (SSSS == 0)
	if (RRRR == 15)
		The next 15 coefficients ahead
		of our current position are zero.

		Correct coefficients.

		Store zero.

		Restart the process to
		read an 'AC' for component 'c'.

	else if (RRRR == 0)
		All the coefficients ahead
		of our current position are zero.

		Correct coefficients.

		End.
	else
		eobrun = (1 << RRRR)

		Read RRRR bits into 'correction'.

		eobrun = eobrun + correction

		ieob = 1

		All the coefficients ahead
		of our current position are zero.

		Correct coefficients.

		End.

else if (SSSS != 0)

	The next RRRR coefficients ahead
	of our current position are zero.

	Correct coefficients.

	Read one bit into 'value'

	zig = dezigzag[k]

	if (value == 0)

		Store 1 << Al at position zig.

	else (value != 0)

		Store -1 << Al at position zig.

	Restart the process to
	read an 'AC' for component 'c'.




Before the process to correct coefficients
let r be the amount of zeroes ahead of 
our current position.

To correct coefficients:

Let 'coeff' be the coefficient stored at
position 'k'.

if (coeff != 0)
	Read one bit into 'boolean'.

	if (boolean != 0)
		if (0 <= coeff)
			coeff = coeff + (1 << Al)
		else
			coeff = coeff + (-1 << Al)
else
	R = R + 1

if (R < r)
	Restrart the process to correct
	coefficients.
else
	End.
*/

int refine_AC(int16_t du[64],
	uint8_t Albit, uint8_t Ss, uint8_t Se,
	uint16_t eobrun, uint16_t ieob,
	int16_t *zz0, int8_t *f, uint32_t *buf,
	uint8_t *d, uint64_t *p, uint64_t l)
{
	uint16_t code;

	uint16_t correction;
	uint32_t boolean;

	int16_t lastcoeff = 0;
	/*To iterate over the number of zeroes*/
	uint8_t R;

	uint8_t r;

	uint8_t k = Ss;

	/* G.1.2.2	
	"An EOB run of length 5 means that
	the current block and the next four
	blocks have an end-of-band with no
	intervening non-zero coefficients.
	*/
	if (ieob < eobrun){
		ieob++;
		/*Take the whole block as zeroes*/
		r = 63;		
		R = 0;
		goto zeroes;
	}
ac:
	R = 0;
	/*Huffman decoding ----*/
	while (*f <= 24)
		if (*p + 1 <= l){
			*buf |= d[*p++] << (24 - *f);//XXX *p++, *d
			*f += 8;
		}
		else break;
	/*The smallest huffman code is 1 but long*/
	if (*f <= 0) return 1;
	// TODO decode into 'code'
	/*---- Huffman decoding*/

	r = (uint8_t)(code >> 4);  /*RRRR Figure G.2*/
	uint8_t s = (uint8_t)(code & 0x0f);/*SSSS Figure G.2*/

	if (15 < r) return 1;
	/*G.1.2.3
	"The four least significant bits, SSSS, provide the
	magnitude category of the non-zero coefficient;
	for a given component the value of SSSS can only be one"*/
	if (1 < s) return 1;

	/*Figure G.2*/	
	if (s == 0){
		if (r != 15){
			if (r){
				eobrun = (1U << r);

				/*Receive ----*/
				while (*f <= 24)
					if (*p + 1 <= l){
						*buf |= d[*p++] << (24 - *f);
						*f += 8;
					}
					else break;
				/*We need at least 'r' bits*/
				if (*f < r) return 1;

				correction = f16(*buf & ((1 << r) - 1));

				*buf = *buf >> r;
				*f -= r;
				/*---- Receive*/

				eobrun += correction;

				/*This accounts for the current run*/
				ieob = 1;
			}
			/*Take the remaining coefficients as zeroes*/
			r = 63;
			goto zeroes;
		}
		/*r == 15 && s == 0 codes ZRL then we should
		write 16 zeroes. By making lastcoeff = 0
		and running 15 zeroes we achieve this.*/
	}
	else{
		/*Receive ----*/
		while (*f <= 24)
			if (*p + 1 <= l){
				*buf |= d[*p++] << (24 - *f);
				*f += 8;
			}
			else break;
		/*We need at least 1 bit*/
		if (*f < 1) return 1;

		boolean = *buf & 1;

		*buf = *buf >> 1;
		*f -= 1;
		/*---- Receive*/

		if (boolean)
			lastcoeff += Albit;
		else
			lastcoeff -= Albit;
	}
zeroes:
	if (Se < k) return 0;

	/*G.1.2.3
	"whenever zero runs are coded with ZRL or EOBn codes,
	correction bits for those coefficients with non-zero
	history contained within the zero run are appended
	according to rule b above"

	b above:
	"b) For each coefficient with a non-zero history, one
	bit is used to code the correction. A 0-bit means no
	correction and a 1-bit means that one shall be added
	to the (scaled) decoded magnitude of the coefficient."*/

	/*G.1.2.3
	"Coefficients with non-zero history (a non-zero value
	coded in a previous scan) are skipped over when	counting
	the zero coefficients."*/
	int16_t coeff = du[dezigzag[k]];
	if (coeff != 0)/* non-zero history */
		/*Receive ----*/
		while (*f <= 24)
			if (*p + 1 <= l){
				*buf |= d[*p++] << (24 - *f);
				*f += 8;
			}
			else break;
		/*We need at least 1 bit*/
		if (*f < 1) return 1;

		boolean = *buf & 1;

		*buf = *buf >> 1;
		*f -= 1;
		/*---- Receive*/
		if (boolean)
			/*TODO esto está copiado de turbo
			no tengo idea de por qué hacen esto.
			Cuando escriba el encoder para
			progressive jpegs tengo que decifrarlo.*/
			if ((coeff & Albit) == 0)
				if (0 <= coeff)
					du[dezigzag[k]] += Albit;
				else
					du[dezigzag[k]] -= Albit;
	else
		if (++R < r){
			k++;
			goto zeroes;
		}

	du[dezigzag[k++]] = lastcoeff;
	goto ac;
}













/*
Source image size
========================
1. We do not support lossless mode of operation.
2. We do not support hierarchical mode of operation.
------------
"4.10 Image, frame, and scan
An image contains only one frame in the cases of sequential and progressive coding processes;
an image contains multiple frames for the hierarchical mode".
=>
This implementation will handle images that contain only one frame.
------------
1.- By table B.2 in sequential mode the number of components in a frame 'Nf'
must be an integer in [1,255] and in progressive mode 'Nf' must be an integer in [1,4].
2.- By definition 3.1.116 in section 3 during sequential coding all the components of the
encoded image must be encoded in a single scan.
3.- libjpeg-turbo defines the "JPEG limit on # of components in one scan" to be 4
(MAX_COMPS_IN_SCAN' in 'jpeglib.h').
=>
The number of components in an image after the decoding process must be an integer in [1,4].

It is possible to have 4 < Nf. If the scan headers use at most 4 of those 'Nf'
components we can decode a source image without contradicting the upper bound of 4 components
for the decoded image.

When reading the frame header we should then be ready to store more than 4 components with
their corresponding attributes (Ci,Hi,Vi,Tqi in Table B.2) but this increases the complexity
of the program so we only accept integer values in [1,4] for 'Nf'.
This is not compliant with the standard but it seems to not interfere with real life jpeg files
and other encoders do the same (e.g. the golang built in jpeg decoder).
------------
Per Table B.2 the number of lines in the source image 'Y' is a 16 bit unsigned integer and
the number of samples per line 'X' is also a 16 bit unsigned integer.
=>
The upper bound for the number of samples in a jpeg image is (2**16)*(2**16) = 2**32.
------------
The size of a sample is specified in the frame header by the sample precision (see section
4.7 for additional details). We support only 8-bit precision.
------------
The source image size in bits is
	Number of components * Sample precision * Y * X
by the preciding discussion this is bounded by
	4 * 8 * (2 ** 16) * (2**16)
in bytes this is
	(2**2) * 1 * (2 ** 16) * (2**16) = 2**34
=>
The size of the source image fits in a uint64_t.
*/
/*
a     -> memory segment where we read the file to be decoded
l     -> the total size of 'a'
b     -> if successful this points to the decoded image
szb   -> pointer to the total size of 'b'
bcomp -> pointer to how many components the source image has

This function returns 0 if there were no errors in the decoding process.
*/

int read(uint8_t *a, uint64_t l, uint8_t *b, uint64_t *szb, uint8_t *bcomp)
{
	/*Defined in tag 'frame' ----*/
	/*Is it sequential mode?*/ 
	uint8_t isprogressive = 0;
	/*All of these appear in Table B.2 ----*/
	/*Number of lines in the source image, "Y" in Table B.2 */
	uint16_t y;
	/*Number of samples in a line in the source image, "X" in Table B.2*/
	uint16_t x;
	/*Number of components defined in the frame header, "Nf" in Table B.2*/
	uint8_t Nf;
	/* c[j] - jth component identifier defined in the frame header*/
	uint8_t c[MC];
	/* h[j] - horizontal sampling factor for frame's component j*/
	uint8_t h[MC];
	/* v[j] - vertical sampling factor for frame's component j*/
	uint8_t v[MC];
	/* t[j] - jth quantization table destination selector*/
	uint8_t tq[MC];
	/*---- All of these appear in Table B.2*/
	/*Number of horizontal MCUs*/
	uint16_t mcux;
	/*Number of vertical MCUs*/
	uint16_t mcuy;
	/* xc[j] - number of horizontal samples for frame's component j*/
	uint16_t xc[MC];
	/* xc[j] - number of vertical samples for frame's component j*/
	uint16_t yc[MC];
	//TODO descripción
	uint16_t line[MC];
	/*Here we store PRED (see F.1.1.5.1) for every
	component defined in the frame header. */
	int16_t zz0[MC];
	/*---- Defined in tag 'frame'*/

	/*Defined in tag 'scan' ----*/
	/*Number of components defined in the scan header
	'Ns' in Table B.3*/
	uint8_t Ns;
	/* if order[j] == cc then c[cc] is equal to
	the jth scan component selector*/
	uint8_t order[MC];
	/* if td[j] == i the DC coefficients of the
	jth component in the scan are to be decoded
	using the ith DC entropy table defined in the
	frame header*/
	uint8_t td[MC];
	/* if ta[j] == i the AC coefficients of the
	jth component in the scan are to be decoded
	using the ith AC entropy table defined in the
	frame header*/
	uint8_t ta[MC];
	/*Ss - Start of spectral or predictor
	selection, "Ss" in Table B.3 */
	uint8_t Ss;
	/*Se - End of spectral selection,
	"Se" in Table B.3*/
	uint8_t Se;
	/*Ah -  Successive approximation bit position
	high, 'Ah' in Table B.3*/
	uint8_t Ah;
	/*Al - Successive approximation bit position
	low or point transform, 'Al' in Table B.3 */	
	uint8_t Al;
	/*Here we store (int16_t)(1U << Al)*/
	int16_t Albit;
	/*---- Defined in tag 'scan'*/

	/*Free bits in 'buf'*/
	int8_t f; //XXX init
	/*Buffer into which we read new coded data*/
	uint32_t buf; //XXX init al mismo tiempo que f


	/*About 'ieob' and 'eobrun'.

	In progressive mode when reading AC coefficients
	from the first scan for a given component or from
	subsequent scans for a given component there is the
	posibility that a previous scan indicated that all
	the coefficients we are about to read are zero.

	This is indicated when we receive runs of End-Of-Bands
	(EOBs):

	G.1.2.2
	"An EOB run of length 5 means that the current block
	and the next four blocks have an end-of-band with no
	intervening non-zero coefficients."

	We store in 'eobrun' how many blocks are to be zero.

	We track how many EOBs we have processed by incrementing
	by one 'ieob'.

	When ieob == eobrun we have no pending zero blocks to
	process.

	At the beginning of each restart interval the EOB
	run count is set to zero:
	*/
	/*Table G.1 
	eobrun is an integer in [1,32767]

	G.1.2.2
	"If an EOB run is greater than 32 767, it is coded
	as a sequence of EOB runs of length 32 767 followed
	by a final EOB run sufficient to complete the run."
	=>
	eobrun can be stored in a 16-bit unsigned integer*/
	uint16_t eobrun;
	uint16_t ieob;













	/* We will use this array as an auxiliary
	array in both 'frame' and 'scan' */
	uint8_t id[256];
























	/*Restart interval*/
	uint16_t ri = 0;

	/*Quantization tables*/
	int8_t qtable [4][64];
	/*if (quant[i] == 1) then qtable[i] has been initialized*/
	uint8_t quant[4] = {0,0,0,0};

	//TODO htable[]
	/*if (huff*[i] == 1) then htable[i] has been initialized*/
	uint8_t huffdc[4] = {0,0,0,0};
	uint8_t huffac[4] = {0,0,0,0};










	/*Used to iterate over short loops*/
	uint8_t i;

	/*defframe == 1 if we have read
	a frame header.*/
	uint8_t defframe = 0;

	/*We store the two-byte code
	assigned to a marker here.*/
	uint8_t m;

	/*Position inside the file we last
	read.*/
	uint64_t p = 0;

	if (l < p + 1) goto error;
	m = a[p++];
	if (m != 0xff) goto error;
soiff:
	if (l < p + 1) goto error;
	m = a[p++];
	if (m == 0xff) goto soiff;

	/* SOI: Start of image marker */
	if (m != 0xd8) goto error;
loop:
	if (l < p + 1) goto error;
	m = a[p++];
	if (m != 0xff) goto error;
ff:
	if (l < p + 1) goto error;
	m = a[p++];
	if (m == 0xff) goto ff;

marker:
	/*dqt - define quantization table(s)*/
	if (m == 0xdb)
		goto dqt;
	/*dht - define huffman table(s)*/
	else if (m == 0xc4)
		goto dht;
	/*dac - define arithmetic coding conditioning(s)*/
	else if (m == 0xcc)
		goto error;
	/*dri - define restart interval*/
	else if (m == 0xdd)
		goto restart;
	/*com - comment'*/
	else if (m == 0xfe)
		goto loop;//TODO skip comment
	/*app - application data*/
	else if (0xe0 <= m && m <= 0xef)
		goto application;
	/*SOF: start of frame marker*/
	else if (0xc0 <= m && m <= 0xcf)
		goto frame;
	/*SOS: start of scan marker*/
	else if (m == 0xDA)
		goto scan;
	
	/*unkwon marker*/
	goto error;
dht:
	//XXX
	huffac[0] = 1;
	huffdc[0] = 1;
dqt:	
	/*
	Table B.4
	Lq | Pq | Tq
	 2 + 1 nibble +  1 nibble = 3
	*/
	if (l < p + 3) goto error;

	/*
	At most we can have 4 quantization tables.
	Each table needs a byte for Pq|Tq and 64
	quantization elements. Each quantization
	element is at most 2 bytes, then
	4 * (1 + (64*2)) = 516 is a bound for Lq.
	*/
	uint16_t Lq = u16(a); p+=2;
	if (Lq < 2) goto error;
	if (520 < Lq) goto error;
	Lq -= 2;//Lq's length

	uint16_t len = 0;
qn:
	if (Lq <= len) goto loop;

	if (l < p + 1) goto error;

	uint8_t PqTq = a[p]; p++;
	uint8_t Pq = (0x0f & PqTq);
	uint8_t Tq = (0xf0 & PqTq);
	if (1 < Pq) goto error;
	if (3 < Tq) goto error;

	/* "B.2.4.1 An 8-bit DCT-based process shall not use a
	16-bit precision quantization table"*/
	/*We only support 8-bit precision*/
	if (Pq != 0) goto error;

	if (l < p + 64) goto error;
	for (i = 0; i < 64; i++)
		qtable[Tq][dezigzag[i]] = a[p++];
	len += 65;//64 (length of Qk) + 1 (length of PqTq)
	quant[Tq] = 1;
	goto qn;

restart:
	/*
	Table B.7
	Lr | Ri
	 2 +  2 = 4	
	*/
	if (l < p + 4) goto error;

	/* We don't need  the restart interval segment length*/
	p += 2;

	/*libjpeg-turbo does not limit
	this value so neither do we*/
	ri = u16(a); p+=2;
	goto loop;

application:
	if (x == 0xe0)
		goto loop;
	else if (x== 0xef)
		goto loop;
	goto loop;

frame:
	/*
	There can't be more than one frame
	in non-hierarchical modes of operation
	and we forbid redefining the frame header
	*/
	if (defframe) goto error;
	else defframe = 1;

	/*Lossless mode*/
	if (m == 0xc3) return 1;
	/*0xc4 - Huffman table.	Covered in 'loop'*/
	/*hierarchical+huffman*/
	else if (0xc5 <= m && m <= 0xc7) goto error;
	/*Reserved for JPEG extensions*/
	else if (m == 0xc8) goto error;
	/*Arithmetic encoding*/
	else if (0xc9 <= m && m <= 0xcB) goto error;
	/*hierarchical+arithmetic*/
	else if (0xcD <= m && m <= 0xcF) goto error;

	/*According to Table 1 the difference
	between baseline DCT decoding (0xc0)
	and extended DCT decoding (0xc1) is that
	baseline can use at most 2 AC tables and
	2 DC tables as opposed to 4 AC tables and
	4 DC tables. Our implementation does not
	distinguish between this two options*/

	/*Progressive DCT*/
	if (m == 0xc2)
		/*G.1.1.1.1 "For each component, a first DC
		scan shall precede any AC scans." */
		isprogressive = 1;

	/*Processing the frame header -----*/
	/*
	"B.2.2
	The value of 'Ci' shall be different from
	the values of 'C1' through 'Ci − 1'"
	*/
	/*
	By Table B.2 'Ci' is in [0,255] and there
	are at most 'Nf' of them.
	*/
	for (i = 0; i < 255; i++) id[i] = 0;

	/*
	Table B.2
	Lf | P | Y | X | Nf
	2 + 1 + 2 + 2 + 1 = 8
	*/
	if (l < p + 8) goto error;

	uint16_t Lf = u16(a); p+=2;
	if (Lf < 11) goto error;
	Lf -= 2;//Lf's length

	/*
	4.7 Sample precision
	"For DCT-based processes, two alternative
	sample precisions are specified: either
	8 bits or 12 bits per sample"
	*/
	/*
	We only support 8 bits of precision.
	*/
	uint8_t P = a[p]; p++;
	if (P != 8) goto error;

	uint16_t _y = u16(a); p+=2;
	uint16_t _x = u16(a); p+=2;

	y = (uint32_t)y;
	x = (uint32_t)x;

	/*
	B.2.2
	"Value 0 indicates that the number of
	lines shall be defined by the DNL marker
	and parameters at the end of the first scan"

	libjpeg-turbo does not support this
	('get_sof' in 'jdmarker.c') so neither do we.
	*/
	/*
	We don't check for an upper bound for the
	height or width, perhaps we should.
	*/
	if (y == 0) goto error;
	if (x == 0) goto error;

	Nf = a[p]; p++;
	if (Nf == 0) goto error;
	if (MC < Nf) goto error;
	if (Lf != (8 + (3 * Nf))) goto error;

	/*
	Per component we have (table B.2)
	  Ci  |   Hi    |   Vi    |   Tqi
	1 byte + 1 nibble + 1 nibble + 1 byte = 3 bytes
	*/
	if (l < p + (3 * Nf)) goto error;

	uint32_t totalHV = 0;

	for (i = 0; i < Nf; i++){

		uint8_t C = a[p]; p++;

		/*
		The value of 'Ci' shall be different
		from the values of 'C1' through 'Ci−1'
		*/
		if (id[C] != 0) goto error;
		id[C] = 1;

		/*
		libjepeg-turbo checks these bounds in 'jdinput.c'
		and defines the maximum value for 'H' and 'V'
		('MAX_SAMP_FACTOR' in 'jpeglib.h') to 4.
		*/
		uint8_t HV = a[p]; p++;
		uint8_t H = (HV & 0x0f);
		uint8_t V = (HV & 0xf0);
		if (H == 0 || SP < H) goto error;
		if (V == 0 || SP < V) goto error;

		totalHV += H*V;

		uint8_t Tq = a[p]; p++;
		if (3 < Tq) goto error;

		/*
		Component identifier for the ith component
		specified in the frame header.
		*/
		c[i] = C;

		/*
		Horizontal sampling factor for the component 'Ci'
		Vertical sampling factor for the component 'Ci'
		*/
		h[i] = H; 
		v[i] = V;

		/*
		Quantization table to use for dequantization
		of the coefficients of component 'Ci'
		*/
		tq[i] = Tq;
	}

	/*
	Section B.2.3 states that if there is more than
        one component then the sum of all H*V values in
	a scan must be <= 10.
	*/
	if (10 < totalHV) goto error;

	/*----- Processing the frame header*/

	/*Verification of the frame's header dimensions -----*/
	/*
	Let * be a data unit and let hmax = 3, vmax = 2.

	 Source     Source divided
	 image      into MCUs

	*******    ***   ***   * 
	*******    ***   ***   *
	******* -> 
	*******    ***   ***   *
		   ***   ***   *

	Lets assume the source image has two
	components.

	Component 1 with h[1] = 3, v[1] = 1
	***   ***   *


	***   ***   *

	Component 2 with h[2] = 2, v[2] = 2
	**    **    *
	**    **    *

	**    **    *
	**    **    *

	Data units per MCU
	3 * 2 | hmax * vmax

	Samples per MCU
	(3 * 8) * (2 * 8) | (hmax * 8) * (vmax * 8)

	thus

	mcu_h_samples = hmax * 8
	(Horizontal samples per MCU)

	mcu_v_samples = vmax * 8
	(Vertical samples per MCU)

	Lets assume the source image has 'x' horizontal
	samples and 'y' vertical samples.

	If
		mcu_h_samples % _x == 0
	there are
		x/mcu_h_samples
	horizontal MCUs

	"A.2.3 Completion of partial MCU
	If the component is to be interleaved, the encoding process
	shall also extend the number of samples by one or more
	additional blocks, if necessary, so that the number of
	blocks is an integer multiple of Hi."
	('Hi' is 'h[i]' in our notation)

	This means that if
		mcu_h_samples % _x != 0
	we just round up the number of horizonatal samples:
		mcux = (x + mcu_h_samples - 1)/mcu_h_samples

	For each MCU component i will have '_h[i]' horizontal
	data units.
	=>
	There are
		mcux * h[i]
	horizontal data units for component i
	=>
	There are
		mcux * h[i] * 8
	horizontal samples for component i
	=>
	if
		amcu[i][0]
	is the first sample component 'i'
		amcu[i][_mcux*_h[i]*8]
	is the next sample in the same column

	Following the same reasoning if
		mcuy = (y + mcu_v_samples - 1)/mcu_v_samples
	there are
		_mcuy * _v[i] * 8
	samples per column
	=>
	component 'i' has
		(mcuy * v[i] * 8) * (_mcux * h[i] * 8)
	samples

	We can calculate this as
		(mcuy * v[i] * 8) * line[i]
	*/
	uint32_t hmax = 0;
	uint32_t vmax = 0;
	for (i = 0; i < Nf; i++){
		uint32_t th = (uint32_t)(h[i]);
		uint32_t tv = (uint32_t)(v[i]);
		if (hmax < th) hmax = th;
		if (hmax < tv) vmax = tv;
	}

	uint32_t mcu_h_samples = hmax * 8;
	uint32_t mcu_v_samples = vmax * 8;

	uint32_t mcux32 = (x + mcu_h_samples - 1) / mcu_h_samples;
	uint32_t mcuy32 = (y + mcu_v_samples - 1) / mcu_v_samples;

	mcux = (uint16_t)(mcux & 65535);
	mcuy = (uint16_t)(mcuy & 65535);

	for (i = 0; i < Nf; i++){

		uint32_t x32 = ((x * h[i]) + (hmax - 1))/hmax;
		uint32_t y32 = ((y * v[i]) + (vmax - 1))/vmax;

		xc[i] = (uint16_t)(x32 & 65535);
		yc[i] = (uint16_t)(y32 & 65535);
		/*
		We need at least three vertical samples
		and three horizontal samples for the
		upsample process.
		*/
		if ((xc[i] < 4)||(yc[i] < 4)) return 1;

		uint32_t line32 = mcux * h[i] << 3;
		line[i] = (uint16_t)(line32 & 65535);
	}
	/*----- Verification of the frame's header dimensions*/
	goto loop;
scan:
	/* {ecs rst} ecs [dnl]( eoi | [tm] sos {ecs rst} ecs eoi ) */

	/*Processing the scan header -----*/
	/*
	"B.2.3 Frame header syntax
	The value of 'Csj' shall be different
	from the values of 'Cs1' to 'Csj – 1'"
	*/
	/*
	By Table B.2 'Csj' is in [0,255] and there
	are at most 'Nf' of them.
	*/
	for (uint8_t i = 0; i < 255; i++) id[i] = 0;

	/*
	Table B.3
	L_s | N_s
	2   +   1 = 3 bytes
	*/
	if (l < p + 3) return 1;

	uint16_t Ls = u16(a); p+=2;
	if (Ls < 8) return 1;

	Ns = a[p]; p++;
	/*
	If there are no components in the
	scan there is no image data.
	*/
	if (Ns == 0) return 1;
	if (Nf < Ns) return 1;
	if (Ls != (6 + (2 * Ns))) return 1;


	/*
	Per component we have (table B.3)
	Cs_j | Td_j | Ta_j
	1 + 1 nibble + 1 nibble = 2 bytes
	*/
	if (l < p + (2 * Ns)) return 1;

	for (uint8_t i = 0; i < Ns; i++){

		uint8_t Cs = a[p]; p++;

		/*Check if this Cs is duplicated*/
		if (id[Cs] != 0) return 1;
		id[Cs] = 1;

		/*
		Check if this scan component selector
		matches with a component identifier
		*/
		uint8_t cc;
		for(cc = 0; cc < Nf; cc++)
			if (c[cc] == Cs) break;
		if (cc == Nf) return 1;

		/*
		Store the component identifier of the ith
		component in the scan.
		*/
		order[i] = cc;

		uint8_t TdTa = a[p]; p++;
		uint8_t Td = (0x0f & TdTa);
		uint8_t Ta = (0xf0 & TdTa);
		if (3 < Td) return 1;
		if (3 < Ta) return 1;

		td[i] = Td;
		ta[i] = Ta;
	}

	/*
	Table B.3
	Ss | Se | Ah | Al
	1 + 1 + 1 nibble + 1 nibble = 3 bytes
	*/
	if (l < p + 3) return 1;

	if (isprogressive){

		Ss = a[p]; p++;
		Se = a[p]; p++;
		if (63 < Ss) return 1;
		if (63 < Se) return 1;
		if (Se < Ss) return 1;

		/*G.1.1.1.1 "DC coefficients are always coded
		separately from AC coefficients"*/
		/*B.2.3 "Ss: Start of spectral or predictor
		selection – In the DCT modes of operation,
		this parameter specifies the first DCT
		coefficient in each block in zig-zag order
		which shall be coded in the scan."*/
		/*B.2.3 "Se: End of spectral selection – Specifies
		the last DCT coefficient in each block in zig-zag
		order which shall be coded in the scan"*/
		/*So if Ss == 0 this scan has the DC coeffiecients
		of a component. If Se where to be not zero this
		scan would also contain AC coefficients.*/		
		if (Ss == 0 && Se != 0) return 1;
		/*G 1.1.1.1 "only scans which code DC
		coefficients may have interleaved blocks
		from more than one component."*/
		if (Ss != 0 && Ns != 1) return 1;

		uint8_t AhAl = a[p]; p++;
		Ah = (0x0f & AhAl);
		Al = (0xf0 & AhAl);
		if (13 < Ah) return 1;
		if (13 < Al) return 1;

		/* This condition is checked in
		'start_pass_phuff_decoder' in 'jdphuff.c'*/
		/*G.1.1.1.2 "If there are no preceding scans
		for the band, Ah is zero"*/
		/*G.1.1.1.2 "The successive approximation bit
		position parameter Al specifies the actual point
		transform, and the high four bits (Ah) – if
		there are preceding scans for the band – contain
		the value of the point transform used in those
		preceding scans."*/
		if (Ah != 0 && Ah != Al + 1) return 1;

		uint16_t ubit = 1U;
		ubit <<= Al;
		Albit = (int16_t)(ubit);

		eobrun = 0;
		ieob = 0;

	}else p+=3; /* Skip Ss,Se,Ah,Al */
	/*Table B.3
	For sequential mode:
	Ss = 0
	Se = 63
	Ah = 0
	Al = 0
	We do not use this values and they are
	not needed for sequential mode so we
	just skip them.*/

	/*----- Processing the scan header*/

	//TODO initialize components

	/*F.1.1.5.1
	"At the beginning of the scan and at the beginning
	of each restart interval, the prediction for the DC
	coefficient is initialized to zero."*/
	for (i = 0; i < MC; i++) zz0[MC] = 0;

	/*total number of MCUs in the image*/
	uint64_t tmcu = 0;

	/*Used to iterate over the number
	of vertical MUCs in an image*/
	uint16_t mcuv = 0;

	/*Used to iterate over the number
	of horizontal MUCs in an image*/
	uint16_t mcuh = 0;

	/*----- Initialization necessary for processing the scan*/

ecs: //TODO esta etiqueta se puede mover (¿?)


	/*Used to iterate over the number
	of vertical data units inside a MCU
	for a given component*/
	uint8_t duv = 0;
	/*Used to iterate over the number
	of horizontal data units inside a MCU
	for a given component*/
	uint8_t duh = 0;

	/*Used to iterate over the number
	of components in a scan*/
	uint8_t ns = 0;

	//XXX
	uint8_t comp;


	/*Most implementations check for the possibility of
	a marker inside the entropy coded data. We do not
	because according to my understanding of the standard
	such a marker would be misplaced. We then do not look
	for bytes of the form 0xff or 0x00. If there are bytes
	of this kind inside the entropy coded data we treat
	them as regular bytes.*/
	/*At this point we read each MCU. There are three parameters
	that modify how we read the MCUs. 
	1) Restart intervals (ri != 0)
	2) How many components does the scan has.
		If there is only one component within the
		scan the standard
		mandates that we disregard the vertical and horizontal
		sampling factors.
	*/
	/*
	"A.2.1 Minimum coded unit (MCU)
	For non-interleaved data the MCU is one
	data unit. For interleaved data the MCU
	is the sequence of data units defined by
	the sampling factors of the components
	in the scan.
	A.2.2 Non-interleaved order (Ns = 1)
	When Ns = 1 (where Ns is the number of
	components in a scan), the order of data
	units within a scan shall be left-to-right
	and top-to-bottom, as shown in Figure A.2.
	This ordering applies whenever Ns = 1,
	regardless of the values of H1 and V1."
	*/
	//TODO explicar estos checks
	if (Ns == 1)
		if (!quant[0]||!huffdc[0]||!huffac[0]) goto error;
	else
		for (ns = 0; ns < Ns; ns++){
			comp = order[ns];
			if (!huffdc[ns]||!huffac[ns]||!quant[comp])
				goto error;
		}

	/*This is a block before IDCT.
	In sequential mode we store one DC
	coefficient followed by 63 AC
	coefficients here before performing
	the discrete cosine transform.*/
	int16_t du[64];

	/*
	ri == 0 -> restart intervals enabled
	Ss == 0 -> scan with dc coefficients
	Ns == 1 -> scan with only one component
	Ah == 0 -> first AC or DC coefficients for a band
	*/
	if (isprogressive){
		if (Ah == 0){
			if (Ss == 0){
				if      (ri != 0 && Ns == 1) goto p1;
				else if (ri != 0 && Ns != 1) goto p2;
				else if (ri == 0 && Ns == 1) goto p3;
				else if (ri == 0 && Ns != 1) goto p4;
			}
			/*Ss != 0 && Ns != 1 is not allowed*/
			if (ri != 0) goto p5;
			goto p6;
		}
		if (Ss == 0){
			if      (ri != 0 && Ns == 1) goto p7;
			else if (ri != 0 && Ns != 1) goto p8;
			else if (ri == 0 && Ns == 1) goto p9;
			else if (ri == 0 && Ns != 1) goto p10;
		}
		/*Ss != 0 && Ns != 1 is not allowed*/
		if (ri != 0) goto p11;
		goto p12;
	}else{
		if      (ri != 0 && Ns == 1) goto s1;
		else if (ri != 0 && Ns != 1) goto s2;
		else if (ri == 0 && Ns == 1) goto s3;
		else if (ri == 0 && Ns != 1) goto s4;
	}

	//TODO rowIDCT
	//TODO colIDCT



/*squential, with restart interval, non-interleaved*/
s1:
	/*one MCU ----*/
	if (DU(du,qtable[0],&zz0[0],&f,&buf,a,&p,l)) goto error;
	/*---- one MCU*/	
	if (++tmcu == ri) goto prov;
	if (++mcuh < mcux) goto s1; mcuh = 0;
	if (++mcuv < mcuy) goto s1; mcuv = 0;
	goto afterscan;

/*squential, with restart interval, interleaved*/
s2:
	/*one MCU ----*/
	comp = order[ns];
	if (DU(du,qtable[comp],&zz0[comp],&f,&buf,a,&p,l)) goto error;
	if (++duh < h[comp]) goto s2; duh = 0;
	if (++duv < v[comp]) goto s2; duv = 0;
	if (++ns < Ns) goto s2; ns = 0;
	/*---- one MCU*/
	if (++tmcu == ri) goto prov;
	if (++mcuh < mcux) goto s2; mcuh = 0;
	if (++mcuv < mcuy) goto s2; mcuv = 0;
	goto afterscan;	

/*squential,no restart interval, non-interleaved*/
s3:
	/*one MCU ----*/
	if (DU(du,qtable[0],&zz0[0],&f,&buf,a,&p,l)) goto error;
	/*---- one MCU*/	
	if (++mcuh < mcux) goto s3; mcuh = 0;
	if (++mcuv < mcuy) goto s3; mcuv = 0;
	goto afterscan;

/*squential,no restart interval, interleaved*/
s4:
	/*one MCU ----*/
	comp = order[ns];
	if (DU(du,qtable[comp],&zz0[comp],&f,&buf,a,&p,l)) goto error;
	if (++duh < h[comp]) goto s4; duh = 0;
	if (++duv < v[comp]) goto s4; duv = 0;
	if (++ns < Ns) goto s4; ns = 0;
	/*---- one MCU*/
	if (++mcuh < mcux) goto s4; mcuh = 0;
	if (++mcuv < mcuy) goto s4; mcuv = 0;
	goto afterscan;	

/*progressive, initialize DC coefficients, no restart interval, non-interleaved*/
p1:
/*progressive, initialize DC coefficients, no restart interval, interleaved*/
p2:
/*progressive, initialize DC coefficients, with restart interval, non-interleaved*/
p3:
/*progressive, initialize DC coefficients, with restart interval, interleaved*/
p4:
/*progressive, initialize AC coefficients, no restart interval, non-interleaved*/
p5:
/*progressive, initialize AC coefficients, with restart interval, non-interleaved*/
p6:
/*progressive, refine DC coefficients, no restart interval, non-interleaved*/
p7:
/*progressive, refine DC coefficients, no restart interval, interleaved*/
p8:
/*progressive, refine DC coefficients, with restart interval, non-interleaved*/
p9:
/*progressive, refine DC coefficients, with restart interval, interleaved*/
p10:
/*progressive, refine AC coefficients, no restart interval, non-interleaved*/
p11:
/*progressive, refine AC coefficients, with restart interval, non-interleaved*/
p12:

	/* B.1.1.5

	"Entropy-coded data segments [...] consists of an
	integer number of bytes"

	"In order to ensure that a marker does not occur
	within an entropy-coded segment, any X’FF’ byte
	generated by either a Huffman or arithmetic encoder,
	or an X’FF’ byte that was generated by the padding
	of 1-bits [...] is followed by a “stuffed” zero byte"
	*/
	/*We reach this point after processing all entropy
	coded data. There are two options. The first option
	is that we have unprocessed bits in the current byte,
	in that case the remaining bits are padding and we can
	ignore them. The second option is that the data ends
	with the byte. In both cases it is safe to just
	go to the next byte*/
prov:
	//XXX
	//XXX zz0 = 0

	//XXX Progressive
	//XXX G.1.2.2
	//XXX	"At the beginning of each restart interval
	//XXX   the EOB run count, EOBRUN, is set to zero."

	//XXX turbo en jdhuff.c, process restart reinicia el
	//XXX restart counter


	/*G.1.2.2
	"At the beginning of each restart interval the EOB
	run count, EOBRUN, is set to zero."
	*/

afterscan:
	if (l < p + 1) goto error;
	x = a[p++];
	if (x != 0xff) goto error;
afterscanff:
	if (l < p + 1) goto error;
	x = a[p++];
	if (x == 0xff) goto afterscanff;

	/*dri - define restart interval*/
	if (x == 0xdd){
		/*Figure B.16 specifies that if restart was not
		enabled before the SOS marker we should not expect
		a restart segment after the scan data*/
		if (ri == 0) goto error;
		/*
		Table B.7
		Lr | Ri
		 2 +  2 = 4	
		*/
		if (l < p + 4) goto error;

		/* We don't need  the restart interval segment length*/
		p += 2;

		/*libjpeg-turbo does not limit
		this value so neither do we*/
		ri = u16(a); p+=2;
		goto ecs;
	}
	/*dnl - define number of lines*/
	else if (m == 0xdc){
		//dnl can't appear when using progressive mode
		if (isprogressive) goto error;
		/*
		Table B.10
		Lr | Ri
		 2 +  2 = 4	
		*/
		if (l < p + 4) goto error;
		p += 4; //skip marker
	}
	else goto eoimarker;
eoi:
	if (l < p + 1) goto error;
	m = a[p++];
	if (m != 0xff) goto error;
eoiff:
	if (l < p + 1) goto error;
	m = a[p++];
	if (m == 0xff) goto eoiff;
eoimarker:
	/*EOI: end of image marker */
	if (m == 0xd9)
		goto end;

	if (isprogressive) goto marker;
	goto error;
end:
	//TODO. Si tenemos un archivo bien
	// formado pero que tenga información de más
	// regresar un error.

	//TODO
	return 0;
error:
	//TODO
	return 1;
}







