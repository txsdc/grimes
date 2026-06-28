import pandas as pd

# primera temporada
# https://es.wikipedia.org/wiki/Torneo_Apertura_2018_(M%C3%A9xico)

# Siempre 17 semanas con
# 9 partidos salvo por
# 2019 con 19 semanas.
# 2020 - Enero, no hay. Covid.

# 0,1,2 <- 19 semanas
# 12 temporadas

_mx = pd.read_csv('./data/SMEX.csv')

# Data frame con los goles por partido
# 'Home goals', 'Away goals'
#goles = _mx[['Home','Away','HG','AG']]

# Agregan columnas de semana y temporada
def add_season_week(d):
	res = d.copy()
	cweek = []
	cseason = []
	x = 0
	week = 0
	season = 0
	for i in range(d.shape[0]):
		cweek.append(week)
		cseason.append(season)
		if x == 8:
			week = week + 1
			x = 0
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	res.insert(2, "Week", cweek, True)
	res.insert(2, "Season", cseason, True)
	return res

e = add_season_week(_mx)

e.to_csv('./data/NMEX.csv', index=False)


def empates_semana(df):
	r = pd.DataFrame(columns=['Season', 'Week','Empates'])
	empates = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	for i in range(dfx):
		if df.loc[i,'HG'] == df.loc[i,'AG']:
			empates = empates + 1
		if x == 8:
			r.loc[i,'Season']  = season
			r.loc[i,'Week']    = week
			r.loc[i,'Empates'] = empates
			empates = 0
			week = week + 1
			x = 0
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

f = ganados_semana(_mx)




def ceros_semana(df):
	r = pd.DataFrame(columns=['Season', 'Week','Ceros'])
	ceros = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	new_i = 0
	for i in range(dfx):
		if df.loc[i,'HG'] == 0 and df.loc[i,'AG'] == 0:
			ceros = ceros + 1
		if x == 8:
			r.loc[new_i,'Season'] = season
			r.loc[new_i,'Week']   = week
			r.loc[new_i,'Ceros']  = ceros
			ceros = 0
			week = week + 1
			x = 0
			new_i = new_i + 1
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

zeros = ceros_semana(_mx)





def un_cero_semana(df):
	r = pd.DataFrame(columns=['Season', 'Week','Ceros'])
	ceros = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	new_i = 0
	for i in range(dfx):
		if df.loc[i,'HG'] == 0 or df.loc[i,'AG'] == 0:
			ceros = ceros + 1
		if x == 8:
			r.loc[new_i,'Season'] = season
			r.loc[new_i,'Week']   = week
			r.loc[new_i,'Ceros']  = ceros
			ceros = 0
			week = week + 1
			x = 0
			new_i = new_i + 1
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

zeros = un_cero_semana(_mx)

# betway tiene ambos anotan y
# total de goles en tres categorias
# mayor o menor a 1.5, 2.5 o 3.5
# caliente ambos anotan y
# mayor o menor a 0.5, 1.5, 2.5, 3.5, 4.5


_mx = pd.read_csv('./data/NMEX.csv')


def lve(df):
	r = pd.DataFrame(columns=['Season', 'Week','Locales', 'Visitantes', 'Empates'])
	locales = 0
	visitantes = 0
	empates = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	new_i = 0
	for i in range(dfx):
		if  df.loc[i,'AG'] < df.loc[i,'HG']:
			locales = locales + 1
		elif  df.loc[i,'HG'] < df.loc[i,'AG']:
			visitantes = visitantes + 1
		elif df.loc[i,'HG'] == df.loc[i,'AG']:
			empates = empates + 1
		if x == 8:
			r.loc[new_i,'Season']  = season
			r.loc[new_i,'Week']    = week
			r.loc[new_i,'Locales'] = locales
			r.loc[new_i,'Visitantes'] = visitantes
			r.loc[new_i,'Empates'] = empates
			locales = 0
			visitantes = 0
			empates = 0
			week = week + 1
			x = 0
			new_i = new_i + 1
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

x = lve(_mx)


def ngoles(df):
	r = pd.DataFrame(columns=['Season', 'Week','0-1', '2-3', '4 o más'])
	cero_uno = 0
	dos_tres = 0
	cuatro_mas = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	new_i = 0
	for i in range(dfx):
		if  (df.loc[i,'AG'] + df.loc[i,'HG']) <= 1:
			cero_uno = cero_uno + 1
		elif (df.loc[i,'HG'] + df.loc[i,'AG']) <= 3:
			dos_tres = dos_tres + 1
		else:
			cuatro_mas = cuatro_mas + 1
		if x == 8:
			r.loc[new_i,'Season']  = season
			r.loc[new_i,'Week']    = week
			r.loc[new_i,'0-1'] = cero_uno
			r.loc[new_i,'2-3'] = dos_tres
			r.loc[new_i,'4 o más'] = cuatro_mas
			cero_uno = 0
			dos_tres = 0
			cuatro_mas = 0
			week = week + 1
			x = 0
			new_i = new_i + 1
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

x = ngoles(_mx)

def goles(df,bound):
	r = pd.DataFrame(columns=['Season', 'Week','A', 'B'])
	grupoa = 0
	grupob = 0
	dfx = df.shape[0]
	x = 0
	week = 0
	season = 0
	new_i = 0
	for i in range(dfx):
		if  (df.loc[i,'AG'] + df.loc[i,'HG']) <= bound:
			grupoa = grupoa + 1
		else:
			grupob = grupob + 1
		if x == 8:
			r.loc[new_i,'Season']  = season
			r.loc[new_i,'Week']    = week
			r.loc[new_i,'A'] = grupoa
			r.loc[new_i,'B'] = grupob
			grupoa = 0
			grupob = 0
			week = week + 1
			x = 0
			new_i = new_i + 1
		else:
			x = x + 1
		if 17 <= week:
			if season != 2:
				season = season + 1
				week = 0
			else:
				if week == 19:
					season = season + 1
					week = 0
	return r

x = goles(_mx,2.5)



# 18 combinations 8 = 43758
# Total de goles + Ambos anotan
# 
# Generar una semana de ambos anotan
# Generar una semana de goles
# Elegir 8 al azar de los 19 generados
# 
# ¿Cómo se ve el generado?
#
#
#
#
#
#
#
#
#





def compare(d,f):
	














# Hay que hacer con la distribución general
# Por equipo elo
	# Simular muchos partidos
	# 	elegir si la simulación sobre 70%








"""
De SMEX -> tGEN1.csv
   Country,League,Season,Date,Time,Home,Away,HG,AG,Res,PH,PD,PA,MaxH,MaxD,MaxA,AvgH,AvgD,AvgA
0  Mexico,Liga MX,2023/2024,30/03/2024,01:00,Puebla,U.A.N.L.- Tigres,2,3,A,4.49,3.94,1.79,4.49,4.2,1.83,4.18,3.85,1.76
1  Mexico,Liga MX,2023/2024,30/03/2024,02:00,Club America,Atl. San Luis,2,1,H,1.49,4.76,6.4,1.57,5,6.4,1.5,4.47,5.6
2  Mexico,Liga MX,2023/2024,30/03/2024,03:00,Mazatlan FC,Club Tijuana,2,0,H,2.34,3.78,2.93,2.36,3.8,3,2.26,3.61,2.87
3  Mexico,Liga MX,2023/2024,31/03/2024,02:00,Monterrey,Guadalajara Chivas,0,2,A,1.83,3.76,4.5,1.83,3.85,4.88,1.76,3.67,4.37
4  Mexico,Liga MX,2023/2024,31/03/2024,02:00,Pachuca,Toluca,2,3,A,2.28,3.68,3.1,2.31,3.8,3.1,2.21,3.62,2.92
5  Mexico,Liga MX,2023/2024,31/03/2024,04:05,U.N.A.M.- Pumas,Cruz Azul,0,0,D,3.2,3.6,2.26,3.22,3.75,2.27,3.08,3.51,2.16
6  Mexico,Liga MX,2023/2024,01/04/2024,00:00,Atlas,Queretaro,2,3,A,2.1,3.41,3.78,2.15,3.42,3.85,2.07,3.27,3.57
7  Mexico,Liga MX,2023/2024,01/04/2024,02:00,Necaxa,Club Leon,1,2,A,2.77,3.48,2.6,2.81,3.65,2.6,2.7,3.45,2.46
8  Mexico,Liga MX,2023/2024,01/04/2024,02:36,Juarez,Santos Laguna,2,1,H,2.28,3.52,3.2,2.35,3.55,3.2,2.24,3.42,3.02
"""
#Puebla
#Club America
#Queretaro
#U.N.A.M.- Pumas
#Juarez
#Club Leon
#Toluca
#Cruz Azul
#U.A.N.L.- Tigres
#Club Tijuana
#Pachuca
#Mazatlan FC
#Atlas
#Guadalajara Chivas
#Necaxa
#Monterrey
#Santos Laguna
#Atl. San Luis



# julio 19-julio 21
#Atl. San Luis
#Club Tijuana
#Puebla
#Atlas
#Guadalajara Chivas
#Mazatlan FC
#Santos Laguna
#U.A.N.L.- Tigres
#Monterrey
#Queretaro
#Cruz Azul
#Toluca
#Juarez
#Club America
#U.N.A.M.- Pumas
#Pachuca
#Club Leon
#Necaxa



#Pachuca
#Atl. San Luis
#Queretaro
#Guadalajara Chivas
#Atlas
#Santos Laguna
#Puebla
#Club Leon
#Cruz Azul
#Club Tijuana
#Necaxa
#Monterrey
#Juarez
#U.N.A.M.- Pumas
#U.A.N.L.- Tigres
#Club America
#Toluca
#Mazatlan FC

"""
Ambos anotan + Número de goles
Local, visitante l,v
Ambos anotan     1,0|0,0
ngoles           n,0
"""
import pandas as pd
_mx = pd.read_csv('./data/tGEN1.csv')

#Ambos anotan + Número de goles
#anotan 1|0 1 = ambos
#ngoles n
# ['Elegido', 'Resultado']

def make_dict(df):
	r = {}
	xdf = df.shape[0]
	for i in range(xdf):
		#[1 si home team, n goles]
		#r['Atlas'] = [1,2]
		r[df.loc[i,'Home']] = [1,df.loc[i,'HG']]
		r[df.loc[i,'Away']] = [0,df.loc[i,'AG']]
	return r	
make_dict(_mx)

# Puebla,Club America, ... , Atl. San Luis
#            1                1
#     [7 más de estas que son ambos anotan]
#    n       0         
#     [7 más de estas que son ngoles]

# Cuando desgloce por equipo el número de goles
# servirá tener n y 0.

# Donde no haya valores sólo voy a poner comas

#===============================

import pandas as pd
_mx = pd.read_csv('./data/tGEN1.csv')

dfapuestas = pd.read_csv('./data/apuesta.csv')

def rate(d,e,napuestas):
	xd = d.shape[0]
	batch = 0
	i = 0
	j = 0
	result = 0
	aciertos = 0
	while i < xd:
		for subApuesta in range(9):
			ambos = d.loc[i + subApuesta, 'AG'] != 0 and d.loc[i + subApuesta,'HG'] != 0
			unoNo = d.loc[i + subApuesta, 'AG'] == 0 or d.loc[i + subApuesta,'HG'] == 0
			x = e.loc[j,'Resultados']
			if x == 1:# 1 = ambos
				if ambos:
					aciertos = aciertos + 1
			elif x == 0:# 0 = uno no
				if unoNo:
					aciertos = aciertos + 1
			#else (apuesta no jugada), no hacemos nada
			j = j + 1
		for subApuesta in range(9):
			suma = d.loc[i + subApuesta, 'HG'] + d.loc[i + subApuesta,'AG']
			x = e.loc[j,'Resultados']
			if x == 0:#0-1
				if  0 <= suma and suma <= 1:
					aciertos = aciertos + 1
			elif x == 1:#2-3
				if  2 <= suma and suma <= 3:
					aciertos = aciertos + 1
			elif x == 2:#4-
				if  4 <= suma:
					aciertos = aciertos + 1
			#else (apuesta no jugada), no hacemos nada
			j = j + 1
		batch = batch + 1
		if 8 == aciertos:
			result = result + 1
		aciertos = 0
		if napuestas <= batch:
			batch = 0
			i = i + 9
			j = 0 #XXX
	return result

rate(_mx,dfapuestas,100)
































