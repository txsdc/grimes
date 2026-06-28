#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>//memset

/*
; x = (2**64)/5
3689348814741910323
7378697629483820646
11068046444225730969
14757395258967641292
*/

/*regresa uint64_t para evitar conversiones después*/
uint64_t partition(uint64_t n)
{
	if      (n <= 3689348814741910323)  return 1;
	else if (n <= 7378697629483820646)  return 2;
	else if (n <= 11068046444225730969) return 3;
	else if (n <= 14757395258967641292) return 4;
	else return 5;
}

/*
https://en.wikipedia.org/wiki/Xorshift#xoshiro256++
*/
uint64_t xoshiro256pp(uint64_t* *s)
{
	uint64_t x;
	uint64_t y;
	x = (s[0]+s[3]) << 23;
	y = (s[0]+s[3]) >> 41
	x |= y;
	x += s[0];

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= s[1] << 17;

	s[3]  = s[3] << 45;
	   y  = s[3] >> 19;
	s[3] |= y;

	return x;
}
int main(){

	uint64_t t;
	uint64_t p;

	uint64_t *s = (uint64_t *)(malloc( 2 * sizeof(uint64_t) ));
	if (!s) goto error;
	s[0] = 599069558410000471ULL;
	s[1] = 6334593251367909448ULL;
	s[2] = 13763014985042389641ULL;
	s[3] = 5577877247334134733ULL;

	t = partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));
	t <<= 8;
	t |= partition(xoshiro256pp(s));




	return 0;
error:
	printf("Error.\n");
	return 1;
}













































