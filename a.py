import pandas as pd

_fut = pd.read_csv('./data/E0.csv')

_mx = pd.read_csv('./data/MEX.csv')

# Data frame con los goles por partido
# 'Home goals', 'Away goals'
goles = _mx[['HG','AG']]

#In [11]: _mx[['HG','AG']].max()
#Out[11]: 
#HG    9
#AG    6
#dtype: int64
#
#In [12]: _mx[['HG','AG']].mean()
#Out[12]: 
#HG    1.490156
#AG    1.149419
#dtype: float64


"""
-> Contar cuántos partidos hay en una categoría por temporada
	y simular una distribución que esté dentro de esas
	cotas.
	-> Podemos hacer algo como una distribución que
		tenga una media en medio de las cuotas y
		que la varianza no salga de las cuotas o
		algo así.
	-> Simular hasta que tengamos los parámetros
		que queremos.
"""

import pandas as pd

# Quité la temporada 2008 porque había un valor 'E'
# y no tengo idea de qué sea eso.
_nfl = pd.read_csv('./data/Protouch.csv')

#n2 = _nfl[['R1','R2','R3','R4','R5','R6','R7','R8','R9','R10','R11','R12','R13']]

n1 = _nfl[['R1','R2','R3','R4','R5','R6','R7','R8','R9','R10','R11','R12','R13','FECHA']]

#Cuenta el número de L,V,E por temporada
def cluster_date(d):
	r = pd.DataFrame(columns=['L','E','V', 'FECHA'])
	newi = 0
	L = 0
	E = 0
	V = 0
	premonth = d.loc[0,'FECHA'][3:5]
	preseason = d.loc[0,'FECHA'][6:]
	for i in range(d.shape[0]):
		curseason = d.loc[i,'FECHA'][6:]
		curmonth = d.loc[i,'FECHA'][3:5]
		if curmonth != premonth:
			if premonth == "07" or premonth == "08" or premonth == "09":
				if curmonth == "02" or curmonth == "03" or curmonth == "01" or curmonth == "12":
					r.loc[newi, 'L'] = L
					r.loc[newi, 'D'] = E
					r.loc[newi, 'V'] = V 
					r.loc[newi, 'FECHA'] = preseason
					L = 0
					E = 0
					V = 0
					newi = newi + 1
		for j in range(d.shape[1]-1):
			a = d.iloc[i][j]
			if a == 'L':
				L = L + 1
			elif a == 'E' or a == 'D':
				E = E + 1
			elif a == 'V':
				V = V + 1
		premonth = curmonth
		preseason = curseason
	return r

cluster_date(n1)

"""
Ha cambiado el número de juegos con los años

In [14]: x['L']+x['E']+x['V']
Out[14]: 
0     286
1     286
2     286
3     234
4     286
5     286
6     286
7     286
8     286
9     286
10    286
11    286
12    286
13    286
14    286
15    286
16    286
17    286
18    286
19    286
20    286
21    273
22    273
23    273
24    273
25    273
26    273
27    273
28    273
29    234
30    221
31    221
32    221
33    208
dtype: object

"""

#Cuenta el número de L,V,E por temporada y lo divide entre
#cuántos juegos hubo esa temporada.
def normalized_cluster(d):
	r = pd.DataFrame(columns=['L','E','V', 'FECHA'])
	newi = 0
	L = 0
	E = 0
	V = 0
	T = 0
	premonth = d.loc[0,'FECHA'][3:5]
	preseason = d.loc[0,'FECHA'][6:]
	for i in range(d.shape[0]):
		curseason = d.loc[i,'FECHA'][6:]
		curmonth = d.loc[i,'FECHA'][3:5]
		if curmonth != premonth:
			if premonth == "07" or premonth == "08" or premonth == "09":
				if curmonth == "02" or curmonth == "03" or curmonth == "01" or curmonth == "12":
					r.loc[newi, 'L'] = L/T
					r.loc[newi, 'E'] = E/T
					r.loc[newi, 'V'] = V/T
					r.loc[newi, 'FECHA'] = preseason
					L = 0
					E = 0
					V = 0
					T = 0
					newi = newi + 1
		for j in range(d.shape[1]-1):
			T = T + 1
			a = d.iloc[i][j]
			if a == 'L':
				L = L + 1
			elif a == 'E' or a == 'D':
				E = E + 1
			elif a == 'V':
				V = V + 1
		premonth = curmonth
		preseason = curseason
	return r

normalized_cluster(n1)


def perday(d):
	r = pd.DataFrame(columns=['L','E','V', 'FECHA'])
	L = 0
	E = 0
	V = 0
	T = 0
	for i in range(d.shape[0]):
		for j in range(d.shape[1]-1):
			T = T + 1
			a = d.iloc[i][j]
			if a == 'L':
				L = L + 1
			elif a == 'E' or a == 'D':
				E = E + 1
			elif a == 'V':
				V = V + 1
		r.loc[i, 'L'] = L/T
		r.loc[i, 'E'] = E/T
		r.loc[i, 'V'] = V/T
		r.loc[i, 'FECHA'] = d.loc[i,'FECHA']
		L = 0
		E = 0
		V = 0
		T = 0
	return r

perday(n1)

#===============================================================

import pandas as pd

_gato = pd.read_csv('./data/GanaGato.csv')
dgato = _gato.iloc[:,2:10]

#Divide en grupos de 'n' elementos los datos y cuenta cuantas
#ocurrencias de cada número hay
def cgato(d,n):
	r = pd.DataFrame(columns=['1','2','3','4','5'])
	ld = d.shape[0]
	newi = 0
	loop = 0
	r1 = 0
	r2 = 0
	r3 = 0
	r4 = 0
	r5 = 0
	for i in range(ld):
		for j in range(d.shape[1]):
			a = d.iloc[ld - 1 - i][j]
			if a == 1:
				r1 = r1 + 1
			elif a == 2:
				r2 = r2 + 1
			elif a == 3:
				r3 = r3 + 1
			elif a == 4:
				r4 = r4 + 1
			elif a == 5:
				r5 = r5 + 1
		if n == loop + 1:
			r.loc[newi, '1'] = r1
			r.loc[newi, '2'] = r2
			r.loc[newi, '3'] = r3 
			r.loc[newi, '4'] = r4 
			r.loc[newi, '5'] = r5 
			r1 = 0
			r2 = 0
			r3 = 0
			r4 = 0
			r5 = 0
			loop = 0
			newi = newi + 1
		else:
			loop = loop + 1
	if loop != 0:
		r.loc[newi, '1'] = r1
		r.loc[newi, '2'] = r2
		r.loc[newi, '3'] = r3 
		r.loc[newi, '4'] = r4 
		r.loc[newi, '5'] = r5 
	return r.iloc[::-1]

f = cgato(dgato,10)

#Igual que el anterior pero normalizado
def ncgato(d,n):
	r = pd.DataFrame(columns=['1','2','3','4','5'])
	ld = d.shape[0]
	newi = 0
	loop = 0
	r1 = 0
	r2 = 0
	r3 = 0
	r4 = 0
	r5 = 0
	N = 0
	for i in range(ld):
		for j in range(d.shape[1]):
			N = N + 1
			a = d.iloc[ld - 1 - i][j]
			if a == 1:
				r1 = r1 + 1
			elif a == 2:
				r2 = r2 + 1
			elif a == 3:
				r3 = r3 + 1
			elif a == 4:
				r4 = r4 + 1
			elif a == 5:
				r5 = r5 + 1
		if n == loop + 1:
			r.loc[newi, '1'] = r1/N
			r.loc[newi, '2'] = r2/N
			r.loc[newi, '3'] = r3/N
			r.loc[newi, '4'] = r4/N 
			r.loc[newi, '5'] = r5/N 
			r1 = 0
			r2 = 0
			r3 = 0
			r4 = 0
			r5 = 0
			N = 0
			loop = 0
			newi = newi + 1
		else:
			loop = loop + 1
	if loop != 0:
		r.loc[newi, '1'] = r1/N
		r.loc[newi, '2'] = r2/N
		r.loc[newi, '3'] = r3/N
		r.loc[newi, '4'] = r4/N
		r.loc[newi, '5'] = r5/N
	return r.iloc[::-1]

g = ncgato(dgato,10)


#===============================================================

import pandas as pd

# Quité la temporada 2008 porque había un valor 'E'
# y no tengo idea de qué sea eso.
_nfl = pd.read_csv('./data/Protouch.csv')

nfl = _nfl[['R1','R2','R3','R4','R5','R6','R7','R8','R9','R10','R11','R12','R13']]

nfl = _nfl[['R1','R2','R3','R4','R5','R6','R7','R8','R9','R10','R11','R12','R13','FECHA']]

#Cuántos L,E,V por temporada
def cluster_date(d):
	r = pd.DataFrame(columns=['L','E','V', 'FECHA'])
	newi = 0
	L = 0
	E = 0
	V = 0
	premonth = d.loc[0,'FECHA'][3:5]
	preseason = d.loc[0,'FECHA'][6:]
	for i in range(d.shape[0]):
		curseason = d.loc[i,'FECHA'][6:]
		curmonth = d.loc[i,'FECHA'][3:5]
		if curmonth != premonth:
			if premonth == "07" or premonth == "08" or premonth == "09":
				if curmonth == "02" or curmonth == "03" or curmonth == "01" or curmonth == "12":
					r.loc[newi, 'L'] = L
					r.loc[newi, 'E'] = E
					r.loc[newi, 'V'] = V 
					r.loc[newi, 'FECHA'] = preseason
					newi = newi + 1
					L = 0
					E = 0
					V = 0
		for j in range(d.shape[1]-1):
			a = d.iloc[i][j]
			if a == 'L':
				L = L + 1
			elif a == 'E' or a == 'D':
				E = E + 1
			elif a == 'V':
				V = V + 1
		premonth = curmonth
		preseason = curseason
	return r


f = cluster_date(nfl)
"""
In [7]: f
Out[7]: 
      L    E   V FECHA
0    90  124  72  2023
1   104  127  55  2022
2    92  109  85  2021
3    75   98  61  2020
4    90  103  93  2019
5    97  116  73  2018
6   102  118  66  2017
7   103  113  70  2016
8    99  120  67  2015
9   114   97  75  2014
10  106  110  70  2013
11   99  116  71  2012
12  108  107  71  2011
13   96  124  66  2010
14  114   96  76  2009
15  105  105  76  2007
16   95  108  83  2006
17  110  112  64  2005
18  114  104  68  2004
19  111  106  69  2003
20  103  113  70  2002
21  101  111  61  2001
22  102  102  69  2000
23  111  102  60  1999
24  100  110  63  1998
25  100  108  65  1997
26  113   97  63  1996
27   92  115  66  1995
28  101  107  65  1994
29   82   92  60  1993
30   91   83  47  1992
31   75   93  53  1991
32   82   85  54  1990
33   70   83  55  1989

In [8]: f.max()
Out[8]: 
L         114
E         127
V          93
FECHA    2023
dtype: object

In [9]: f.min()
Out[9]: 
L          70
E          83
V          47
FECHA    1989
dtype: object

In [10]: (114-70)*(127-83)*(93-47)
Out[10]: 89056

"""
