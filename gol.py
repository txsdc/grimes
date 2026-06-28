import pandas as pd

_df = pd.read_csv('./data/Progol.csv')

df = _df[['R1','R2','R3','R4','R5','R6','R7','R8','R9','R10','R11','R12','R13','R14']]


def rowresults(d):
	r = pd.DataFrame(columns=['L','E','V'])
	for i in range(d.shape[0]):
		L = 0
		E = 0
		V = 0
		for j in range(d.shape[1]):
			a = d.iloc[i][j]
			if a == 'L':
				L = L + 1
			elif a == 'E':
				E = E + 1
			elif a == 'V':
				V = V + 1
		r.loc[i, 'L'] = L
		r.loc[i, 'E'] = E
		r.loc[i, 'V'] = V
	return r

x = rowresults(df)


