#include <stdint.h>






































/*
space = {\n|\t|0x20}

comment = # utf8 [\n] | # utf8 | ; utf8 [\n]

programa = s-expr|comment|space
*/

static int utf8_comment(uint8_t *a, uint64_t *p, uint64_t l)
{
	uint32_t value = 0;
	uint8_t width = 0;

	if( (a[*p] & 0x80) == 0x00 ){
		width = 1;
		value = a[*p] & 0x7F;
	}
	else if((a[*p] & 0xE0) == 0xC0 ){
		width = 2;
		value = a[*p] & 0x1F;
	}
	else if((a[*p] & 0xF0) == 0xE0 ){
		width = 3;
		value = a[*p] & 0x0F;
	}
	else if((a[*p] & 0xF8) == 0xF0 ){
		width = 4;
		value = a[*p] & 0x0F;
	}
	else return 3;
	
	if(l < *p + width) return 4;

	for (uint8_t k = 1; k < width; k++){
		uint8_t temp = a[*p++];

		/* Check if a->octet has the form 10xxxxxx */
		if ((temp & 0xC0) != 0x80) return 5;

		value = (value << 6) + (temp & 0x3F);
	}

	if (!((width == 1) ||
		(width == 2 && 0x80 <= value) ||
		(width == 3 && 0x800 <= value) ||
		(width == 4 && 0x10000 <= value))) return 6;

	/* We check that the a->value isn't in the range of a surrogate
	   pair 0xD800 - 0xDFFF or above the biggest a->value possible
	   for utf8. */
	if ((0xD800 <= value && value <= 0xDFFF) || 0x10FFFF < value)
		return 7;
	/*
	* Check if the character is in the allowed range:
	*      #x9 | #xA | #xD | [#x20-#x7E]               (8 bit)
	*      | #x85 | [#xA0-#xD7FF] | [#xE000-#xFFFD]    (16 bit)
	*      | [#x10000-#x10FFFF]                        (32 bit)
	*/
	if (! (value == 0x09 || value == 0x0A || value == 0x0D
		|| (0x20 <= value && value <= 0x7E)
		|| (value == 0x85) || (0xA0 <= value && value <= 0xD7FF)
		|| (0xE000 <= value && value <= 0xFFFD)
		|| (0x10000 <= value && value <= 0x10FFFF)))
			return 8;
	return 0;
}

/*
(global x dir)

La memoria en la dirección 'dir' se llama x
en todo el programa.
	(¿o qué solo aplique al archivo usado
	en ese momento?)


Importante:
Si alguien intenta renombrar x hay un error.


(f-alias x y)

Ahora x se llama y en la función actual.
*/




/*
When we read a file we expect
comments, s-expressions and spacing.
There shouldn't be any othher options
so if we find any other values we
return error.

Then file can end when we are reading
a one line comment or spacing. If the file
ends at any other point it is considered an
error.
*/


/*
1 -> Not a comment, s-expression or spacing.
2 -> Control characters are not allowed
3 -> Invalid leading UTF-8 octet
4 -> Incomplete UTF-8 character
5 -> Wrong prefix for non-leading octet
6 -> Width of the character does not match its range
7 -> Invalid range for utf8 value
8 -> Control characters are not allowed
9 -> File ended while inside an s-expression
10 -> File ended while inside a comment that was inside an s-expression
11 -> Non visible ascii in s-expression
12 -> Function not supported
*/

//leaq -8(%rbp), %rax
/*
Parece que todas las instrucciones son a lo más
así de largas/complicadas. Si sólo nos proponemos
parsear este tipo de instrucciones creo que podemos
hacerlo con una estructura fija:

uint64_t ip[4];
uint64_t fp[4];
*/


int t(uint8_t *a, uint64_t l)
{
#define E(value) {e = value; goto error;}

	uint64_t p = 0;

	uint64_t i;

	/*initial position*/
	uint64_t ip[4];
	/*final position*/
	uint64_t fp[4];

	uint8_t s = 0;

	uint64_t col;
	uint64_t line;

	/*Error value*/
	uint8_t e;

/*Spacing*/
space:
	/*End of file*/
	if (l < p)
		if (s) E(9)
		else goto eof;

	/*\t*/
	else if (a[p] == 9) col++;
	/*\n*/
	if (a[p] == 10) line++, col = 0;
	/*space*/
	else if (a[p] == 32)col++;

	else goto ce;

	p++;
	goto space;

/*Comment or expression*/
ce:
	/*';'*/
	if (a[p] == 59) goto com;
	/*'#'*/
	else if (a[p] == 35) goto com;
	/*'('*/
	else if (a[p] == 40) goto sexpr;
	/*Currently inside an s-expression*/
	else if (s) goto sexpr;

	/*Not a comment, s-expression or spacing*/
	E(1);

/*Comment*/
com:
	if (l < p + 1)
		if (s) E(10)
		else goto eof;
	p++; col++;
	/*\n*/
	if (a[p] == 10) goto space;
	if (e = utf8_comment(a, &p, l)) goto error;
	goto com;
sexpr:
	s = 1;
	if (l < p + 1) E(9);
	p++; col++;
	/*')'*/
	if (a[p] == 41) goto exec;
	/*spaces ----*/
	else if (a[p] == 10) goto space;
	else if (a[p] ==  9) goto space;
	else if (a[p] == 32) goto space;
	/*---- spaces*/
	/*check for forbidden characters*/
	else{
		if (a[p] < 20) E(11);
		if (126 < a[p]) E(11);
	}
	//XXX sz++;
	goto sexpr;
exec:
	p++; //para regresar a spacing
	i = ip[0];
	uint64_t len = fp[0] - i;
	if (len == 4){
		if (a[i++] == 'c')
		if (a[i++] == 'a')
		if (a[i++] == 'l')
		if (a[i++] == 'l')
		goto call;

		i = ip[0];
		if (a[i++] == 'x')
		if (a[i++] == 'y')
		if (a[i++] == 'z')
		if (a[i++] == 'q')
		goto xyzq;
	}
	else if (len == 7){
		if (a[i++] == 's')
		if (a[i++] == 'y')
		if (a[i++] == 's')
		if (a[i++] == 'c')
		if (a[i++] == 'a')
		if (a[i++] == 'l')
		if (a[i++] == 'l')
		goto syscall;
	}

	E(12);
call:






syscall:


xyzq:



eof:
	return 0;
error:
	return e;

#undef E
}


/*
(entry func1)

(defun func1
	(mov rax 60)
	(mov rdi 0)
	(syscall))

o

(deftext func1)
*/






/*
(add 5 6)

========================
134 On Lisp Paul Graham


(defmacro for ((var start stop) &body body)
(let ((gstop (gensym)))
‘(do ((,var ,start (1+ ,var))
(,gstop ,stop))
((> ,var ,gstop))
,@body)))

========================
Regreso de valores n ádico

Si uno de los factores es 0 regresa error

(no-zero-mult 5 9) => (mult _ -3)

========================
Pegado de texto
*/

/*
func -> .

block1
::

::
block1



block1::

::block1

funciones (cargar un pedazo de código y regresar el)
Pedazos de código


variable (algo que cabe en un registro)
	Guardar en caso de no caber
*/

/*
section .text
global _start

_start:
    mov rax, 60     ; syscall number for sys_exit
    mov rdi, 0      ; exit code 0
    syscall         ; invoke operating system to exit
*/








