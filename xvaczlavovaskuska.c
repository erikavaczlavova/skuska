#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//----------------------------FUNKCIA V-----------------------------------------------nacitanie a vypis 
void vytvor_vypis(FILE **subor){	
	char meno[52], spzt[52];
	int typauta = 0, datum;
	double cena;
	
	if ((*subor = fopen("autobazar.txt", "r")) == NULL)			//skuska otvorenia suboru 	
			printf("Neotvoreny subor\n");
	
	else{	
		while (fgets(meno, sizeof(meno), *subor) != NULL){		//citanie suboru po riadkoch az po koniec suboru
			meno[strlen(meno) - 1] = '\0';						//nacitanie premennych do premennych na vypis
			fscanf(*subor, "%s\n", spzt);							
			fscanf(*subor, "%d\n", &typauta);
			fscanf(*subor, "%lf", &cena);
			fscanf(*subor, "%d\n", &datum);
			printf("meno priezvisko: %s\nSPZ: %s\ntyp auta: %d\ncena: %.2lf\ndatum: %d\n\n", meno, spzt, typauta, cena, datum);		//vypis do konzoly	
		}
	}
}
//----------------------------FUNKCIA O-----------------------------------------------vypocet odmeny pre zamestnancov
void vypocet_odmeny(FILE **subor){
	if (*subor == NULL)											
		return;													//kontrola otvorenia suboru, ak nie je otvoreny, funkcia nevykona ukon
	
	char meno[52], spzt[52];	
	int typauta = 0, datum;
	double cena, odmena;
	int den;
														
	scanf("%d", &den);
	
	rewind(*subor);												//vratenie sa na zaciatok uz otvorene suboru
										
	while (fgets(meno, sizeof(meno), *subor) != NULL){			//citanie suboru po riadkoch az po koniec suboru
		meno[strlen(meno) - 1] = '\0';
		fscanf(*subor, "%s\n", spzt);								
		fscanf(*subor, "%d\n", &typauta);
		fscanf(*subor, "%lf", &cena);
		fscanf(*subor, "%d\n", &datum);

		if ((den - datum) >= (10000)){							//vypocet odmeny zamestnancom na zaklade podmienky
			if (typauta == 1)											 
				odmena = ((cena / 100)*(2.3));					//odmena ak predajca predal nove auto
			if (typauta == 0)										
				odmena = ((cena / 100)*(5.1));					//odmena ak predajca predal jazdene auto
			printf("%s %s %.2lf \n", meno, spzt, odmena);		//vypis do riadku podla zadania do konzoly		
		}
	}
}	
//--------------------------------FUNKCIA N--------------------------------------------nacitanie
int vytvor_pole(FILE **subor, int **spz){
	if (*subor == NULL)											//kontrola otvorenia suboru, ak nie je otvoreny, funkcia nevykona ukon		
		return 0;
		
	rewind(*subor);												//vratenie sa na zaciatok uz otvorene suboru
											
	int pocetriadkov = 0;
	int velkost = 0, pocitanieZnakov = 0;
	int pomocnyCharakter;
	int *docasnePole;
	
	while ((pomocnyCharakter = getc(*subor)) != EOF){			//prechadzanie suboru po znaku az po jeho koniec
		if (pomocnyCharakter == '\n')							//ak sa charakter rovna znaku konca riadku zvysuje premennú pre pocet riadkov	
			pocetriadkov++;
	}				
	docasnePole = (int*)malloc((sizeof(int)));					//alokacia pomocneho pola 
					
	rewind(*subor);												//vratenie sa na zaciatok uz otvorene suboru
										
	while ((pomocnyCharakter = getc(*subor)) != EOF){			//prechadzanie suboru po znaku az po jeho koniec
		if (pomocnyCharakter == '\n'){							//ak sa charakter rovna zanku konca riadku zvysi pomocnu premennu
			pocitanieZnakov++;
		}
		if (pocitanieZnakov == 1){								//ak sa pomocna premenna rovna 1, nachadazame sa vo vhodnom riadku suboru ktoreho obsah posielame pomocnemu polu
			if (pomocnyCharakter != '\n'){						//citanie charakterov iba po koniec riadku
				docasnePole[velkost] = pomocnyCharakter;									
				velkost++;
				docasnePole = (int*)realloc(docasnePole, (velkost + 1) * sizeof(int));	//realokovanie velkosti pola pre kazdu dalsiu spz
			}
		}		
		if (pocitanieZnakov == 6)								//ak sa pomocna premenna rovna 6 nulujeme ju, prechadzame na dalsi zaznam a pomocnou premennou nasledne na vhodny riadok 
			pocitanieZnakov = 0;		
	}
	*spz = docasnePole;											//po vykonaní cyklu poslanie obsahu pomocneho pola do alokovaneho hlavneho pola v maine																
	return velkost;												//navrat velkosti pola z dovodu vyuzitia tejto premennej aj v dalsich fukciach	
}
//------------------------------------FUNKCIA S---------------------------------------------specialny vypis
void specialny_vypis(int *spz, int velkostPola){
	int pozicia;
	
	if (spz == NULL){
		printf("Pole nie je vytvorene\n");						//ak pole nebolo vytvorene vypise do konzoly spravu a nevykona ukon funkcie
		return;
	}
	for (pozicia = 0; pozicia < velkostPola; pozicia++){
		if (pozicia % 7 == 1)									//pisanie medzery za/pred kokretny prvok radu (modulujeme cislom 7, pretoze vieme ze spz ma prave 7 znakov)	
			printf("%c ", spz[pozicia]);
		else if (pozicia % 7 == 5)									
			printf(" %c", spz[pozicia]);
		else if (pozicia % 7 == 0 && pozicia > 0)						
			printf("\n%c", spz[pozicia]);
		else
			printf("%c", spz[pozicia]);
	}
	printf("\n");												//vypis konca riadku, noveho riadku	
}
//-----------------------------------FUNKCIA M -------------------------------------------najcastejsi znak
void najcastejsi_znak(int *spz, int velkostPola){
	int poziciaPolaAbecedy, poradie, poziciaPomocnehoPola, znak, znakPola, hodnotaPoctu;
	int max = 0;
	char pismenko;
	char znaky[26];
	char pocet[26];
	char *pole;	
	pole = ((char*)malloc(velkostPola * (sizeof(char))));		//alokovanie velkosti docasneho pomocneho pola
		
	if (spz == NULL){
		printf("Pole nie je vytvorene\n");						//ak pole nebolo vytvorene vypise do konzoly spravu a nevykona ukon funkcie	
		return;
	}
	for (poziciaPolaAbecedy = 0; poziciaPolaAbecedy < 26; poziciaPolaAbecedy++){			
		znaky[poziciaPolaAbecedy] = 'A' + poziciaPolaAbecedy;	//nacitanie abecedy do pola pomocou ascii tabulky vo for cykle a nulovanie poctoveho pola
		pocet[poziciaPolaAbecedy] = 0;
	}		
	for (znakPola = 0; znakPola < velkostPola; znakPola++){		//porovnavanie a pocitanie mnozstva rovankych znakov a pridavania ich hodnot do pola s pocetnostou
		pole[znakPola] = spz[znakPola];
		
		for (znak = 0; znak < 26; znak++){
			if (znaky[znak] == pole[znakPola])
				pocet[znak] += 1;
		}
	}	
	for (hodnotaPoctu = 0; hodnotaPoctu < 26; hodnotaPoctu++){	//vyber najpocetnejsieho prvku
		if (pocet[hodnotaPoctu] > max){
			max = pocet[hodnotaPoctu];
			pismenko = znaky[hodnotaPoctu];
		}
	}	
	printf("%c %d\n", pismenko, max);							//vypis najpocetnejsieho prvku a jeho pocetnosti 
}
//---------------------------------FUNKCIA P-------------------------------------------------palindromy 
void palindrom(int *spz, int velkostPola){
	if (spz == NULL){
		printf("Pole nie je vytvorene\n");					//ak pole nebolo vytvorene vypise do konzoly spravu a nevykona ukon funkcie	
		return;
	}	
	int znaky;
	int prvyZnakSpz = 0;
	int poslednyZnakSpz = 6;
	int druhyZnakSpz = 1;
	int predposlednyZnakSpz = 5;
	int tretiZnakSpz = 2;
	int predpredposlednyZnakSpz = 4;
	
	for (znaky = 0; znaky < velkostPola / 7; znaky++){		//cyklus opakujuci sa prave tolkokrat kolko spz je v poli ulozenych
		if ((spz[prvyZnakSpz] == spz[poslednyZnakSpz]) && (spz[druhyZnakSpz] == spz[predposlednyZnakSpz]) && (spz[tretiZnakSpz]== spz [predpredposlednyZnakSpz]))			
			printf("%c%c\n", spz[prvyZnakSpz], spz[druhyZnakSpz]);				//vypis len tych zankov z spz ktore boli vyhodnotene ako palindrom
							
		prvyZnakSpz += 7;									//zvysovanie hodnot pre kontrolu kazdej spz z dovodu kontroly dalsej spz v poradi	
		poslednyZnakSpz+= 7;
		druhyZnakSpz += 7;
		predposlednyZnakSpz += 7;
		tretiZnakSpz += 7;
		predpredposlednyZnakSpz += 7;
	}
}
//--------------------------------FUNKCIA Z-------------------------------------najpocetnejsi  okres
void naj_okres(int *spz, int velkostPola){
	if (spz == NULL)											
		return;	
		
	int spzt, dalsiaspz;
	int prvyZnakSpz= 0;	
	int druhyZnakSpz = 1;
	char stringMax[2];
	int max = -1;
	int pomocMax = 0;
	int help = velkostPola / 7; 							// pomocna premenna kvoli tabulke porovnani, nech sa za kazdym pocet porovani znizi 	
	
	for ( spzt = (velkostPola / 7);  spzt >0;  spzt--){
		int prvyZnakDalsejSpz = prvyZnakSpz + 7; 										//  prvyZnakDalsejSpz a druhyZnakDalsejSpz  stale beru hodnoty z prvyZnakSpz, musia byt tu vytvorene	
		int druhyZnakDalsejSpz = druhyZnakSpz + 7;
		
		for (dalsiaspz = help - 1; dalsiaspz >0; dalsiaspz--){
			if ((spz[prvyZnakSpz] == spz[prvyZnakDalsejSpz]) && (spz[druhyZnakSpz] == spz[druhyZnakDalsejSpz]))
				pomocMax++;
						
			prvyZnakDalsejSpz += 7;
			druhyZnakDalsejSpz+= 7;
		}		
		if (pomocMax == max){
			max = pomocMax;
			printf("%c%c %d", stringMax[0], stringMax[1], max + 1); 	// ak sa najde viac krat to iste maximum, vypise ulozene, a nahradi ho novym
			stringMax[0] = spz[prvyZnakSpz];
			stringMax[1] = spz[druhyZnakSpz];
		}		
		if (pomocMax > max)	{
			max = pomocMax;
			stringMax[0] = spz[prvyZnakSpz];
			stringMax[1] = spz[druhyZnakSpz];
		}
		pomocMax = 0;
		prvyZnakSpz += 7;
		druhyZnakSpz += 7;
		help--;
	}
	printf("\n%c%c %d\n", stringMax[0], stringMax[1], max + 1);
}
//--------------------------------------MAIN-------------------------------------------------
int main(){
	FILE *subor = NULL;												
	char operand = 'a';
	int *spz = NULL;												//pointer na pole spz, vyuzivame vo viacerych fukciach		
	int velkostPola;
	
	while (operand != 'k')											//cyklus potrebny nanukoncenie programu pri vybere hodkoty 'k' 
	{																
		scanf("%c", &operand);
		
		switch (operand){
			case 'v':
					vytvor_vypis(&subor);
					break;
			case 'o':
					vypocet_odmeny(&subor);
					break;
			case 'n':
					velkostPola = vytvor_pole(&subor, &spz);
		      		break;
			case 's':
					specialny_vypis(spz, velkostPola);
					break;
			case 'm':
					najcastejsi_znak(spz, velkostPola);
					break;
			case 'p':
					palindrom(spz, velkostPola);
					break;
			case 'z':
					naj_okres(spz, velkostPola);
					break;
			case 'k':
					if(subor!=NULL) 
					fclose(subor);
					break;
		}
	}
	return 0;
}
