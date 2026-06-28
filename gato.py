import pandas as pd

_gato = pd.read_csv('./data/GanaGato.csv', sep=',')
dgato = _gato.iloc[:,2:10]

df1 = pd.read_csv('./data/zpos.csv', sep=',', header=None)
df2 = pd.read_csv('./data/zneg.csv', sep=',', header=None)

df1 = pd.read_csv('./data/be1.csv', sep=',', header=None)
df2 = pd.read_csv('./data/be2.csv', sep=',', header=None)

df1 = pd.read_csv('~/1b.csv', sep=',', header=None)
df2 = pd.read_csv('~/1c.csv', sep=',', header=None)

df1 = pd.read_csv('~/1a.csv', sep=',', header=None)
df2 = pd.read_csv('~/2a.csv', sep=',', header=None)


result = df1.sub(df2, fill_value=0)

result = df2.sub(df1, fill_value=0)


r2 = result.abs()

r2.to_csv('~/t.csv', index=False)


s1 = df1.diff()

grep -Fvxf zneg.csv zpos.csv > 4.csv




"""
      -------------- x -------------

      F1  F2  F3  F4  F5  F6  F7  F8
0      2   2   2   2   2   1   1   4   |
1      5   2   2   5   1   2   5   4   |
2      2   4   3   5   5   5   3   1   |
3      4   1   4   2   2   4   3   2   |
4      1   1   4   4   3   5   4   2   |
...   ..  ..  ..  ..  ..  ..  ..  ..   y
2963   4   5   4   2   2   5   5   4   |
2964   3   3   3   2   1   1   4   3   |
2965   1   1   5   4   5   5   1   5   |
2966   1   4   5   2   5   3   1   5   |
2967   3   5   4   5   2   2   4   1   |
"""



def cgato(d,n,c):
	r = pd.DataFrame(columns=['1','2','3','4','5'])
	y = d.shape[0]
	x = d.shape[1]
	newi = 0
	loop = 0
	loc1 = []
	loc2 = []
	loc3 = []
	loc4 = []
	loc5 = []
	a = 0
	for row in range(y):
		if a == 1:
			loc1.append(r1)
		elif a == 2:
			loc2.append(r2)
		elif a == 3:
			loc3.append(r3)
		elif a == 4:
			loc4.append(r4)
		elif a == 5:
			loc5.append(r5)
		r1 = 0
		r2 = 0
		r3 = 0
		r4 = 0
		r5 = 0
		for i in range(x):
			a = d.iloc[y-1-row][i]
			if a == 1:
				r1 = r1 + 1
				loc2.append(r2)
				loc3.append(r3)
				loc4.append(r4)
				loc5.append(r5)
				r2 = 0
				r3 = 0
				r4 = 0
				r5 = 0
			elif a == 2:
				r2 = r2 + 1
				loc1.append(r1)
				loc3.append(r3)
				loc4.append(r4)
				loc5.append(r5)
				print("row",row)
				print("col",i)
				print(r2)
				r1 = 0
				r3 = 0
				r4 = 0
				r5 = 0
			elif a == 3:
				r3 = r3 + 1
				loc1.append(r1)
				loc2.append(r2)
				loc4.append(r4)
				loc5.append(r5)
				r1 = 0
				r2 = 0
				r4 = 0
				r5 = 0
			elif a == 4:
				r4 = r4 + 1
				loc1.append(r1)
				loc2.append(r2)
				loc3.append(r3)
				loc5.append(r5)
				r1 = 0
				r2 = 0
				r3 = 0
				r5 = 0
			elif a == 5:
				r5 = r5 + 1
				loc1.append(r1)
				loc2.append(r2)
				loc3.append(r3)
				loc4.append(r4)
				r1 = 0
				r2 = 0
				r3 = 0
				r4 = 0
		if n == loop + 1:
			i1 = 0
			i2 = 0
			i3 = 0
			i4 = 0
			i5 = 0
			print(loc2)
			for e in loc1:
				if e == c:
					i1 = i1 + 1
			for e in loc2:
				if e == c:
					i2 = i2 + 1
			for e in loc3:
				if e == c:
					i3 = i3 + 1
			for e in loc4:
				if e == c:
					i4 = i4 + 1
			for e in loc5:
				if e == c:
					i5 = i5 + 1
			r.loc[newi, '1'] = i1
			r.loc[newi, '2'] = i2
			r.loc[newi, '3'] = i3 
			r.loc[newi, '4'] = i4 
			r.loc[newi, '5'] = i5 
			loc1 = []
			loc2 = []
			loc3 = []
			loc4 = []
			loc5 = []
			loop = 0
			newi = newi + 1
		else:
			loop = loop + 1
	if loop != 0:
		i1 = 0
		i2 = 0
		i3 = 0
		i4 = 0
		i5 = 0
		for e in loc1:
			if e == c:
				i1 = i1 + 1
		for e in loc2:
			if e == c:
				i2 = i2 + 1
		for e in loc3:
			if e == c:
				i3 = i3 + 1
		for e in loc4:
			if e == c:
				i4 = i4 + 1
		for e in loc5:
			if e == c:
				i5 = i5 + 1
		r.loc[newi, '1'] = i1
		r.loc[newi, '2'] = i2
		r.loc[newi, '3'] = i3 
		r.loc[newi, '4'] = i4 
		r.loc[newi, '5'] = i5 
	return r.iloc[::-1]
	#return r

f = cgato(dgato,10,2)

g = cgato(dgato,20,2)

h = cgato(dgato,20,3)

i = cgato(dgato,10,3)

z = f.sum(axis=1)




"""
Cuenta cuántos de cada número han sucedido
"""

def indiv(d,n):
	r = pd.DataFrame(columns=['1','2','3','4','5'])
	y = d.shape[0]
	x = d.shape[1]
	newi = 0
	loop = 0
	a = 0
	r1 = 0
	r2 = 0
	r3 = 0
	r4 = 0
	r5 = 0
	for row in range(y):
		for i in range(x):
			a = d.iloc[y-1-row][i]
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


a = indiv(dgato,10)
b = indiv(dgato,20)

















