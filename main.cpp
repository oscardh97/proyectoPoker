#include <ncurses.h>
#include <cstdlib>
#include <ctime>
void iniciarBaraja(int*[]);
void crearMapaCorazon(char ,int);
void crearMapaEspada(char ,int);
void crearMapaDiamante(char ,int);
void crearMapaTrebol(char ,int);
void revolver(int*[], int);
bool tieneJuego(int*[]);
int tieneEscalera(int**);
bool isOnTable(int*[], int, int);
bool hayPares(int*[], int);
bool hayTrios(int*[], int);
char getValueChar(int );
void ordenarMesa(int**, const unsigned int);
void imprimirCarta(int, char, char[20][27] = NULL);
void copiarMatriz(int**, int** const, unsigned const int, unsigned const int);
int main(){
	srand(time(NULL));
	int** mesa = NULL; 
	mesa = new int*[5];
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	iniciarBaraja(mesa);
	while(true){
		revolver(mesa, 3);
		tieneJuego(mesa);
		char c = getch();
		refresh();
	}
	endwin();
	delete[] mesa;
	return 0;
}
void revolver(int** mesa, int trampa){
	int* numeros = new int[5];
	int palo;
	if(trampa >= 1 && trampa <= 3){
		int inicio = trampa == 3 ? 10 : rand() % 13 + 1;
		for(int i = 0; i < 5; i++){
			if(inicio + i < 13){
				numeros[i] = inicio + i;
			}else{
				numeros[i] = inicio - i;
			}
		}
		palo = trampa == 2 ? rand() % 3 + 1 : 1;
	}

	for(int i = 0; i < 5; i++){
		char charCarta; 
		int numeroCarta = 1;
		int* propiedades = mesa[i];
		do{
			if(trampa >= 1 && trampa <= 3){
				palo = trampa == 1 ? rand() % 3 + 1 : palo;
				numeroCarta = numeros[i];
			}else{
				palo = rand() % 3 + 1;
				numeroCarta = rand() % 13 + 1;
			}
			charCarta = getValueChar(numeroCarta);

			if(palo == 0){
				crearMapaCorazon(charCarta, i);
			}else if(palo == 1){
				crearMapaEspada(charCarta, i);
			}else if(palo == 2){
				crearMapaDiamante(charCarta, i);
			}else{
				crearMapaTrebol(charCarta, i);
			}
		}while(isOnTable(mesa, palo, numeroCarta) && trampa == 0);
		propiedades[0] = palo;
		propiedades[1] = numeroCarta;
	}
}
char getValueChar(int numeroCarta){
	if(numeroCarta == 1){
		return 'A';
	}else if(numeroCarta == 10){
		return 'D';
	}else if(numeroCarta == 11){
		return 'J';
	}else if(numeroCarta == 12){
		return 'Q';
	}else if(numeroCarta == 13){
		return 'K';
	}

	return numeroCarta + '0';
}
void iniciarBaraja(int** mesa){
	for(int k = 0; k < 5; k++){
		int* propiedades = new int[2];
		propiedades[0] = -1;
		propiedades[1] = -1;
		mesa[k] = propiedades;
	}
}
bool procesarData(int numeroCarta){
	return 0;
}
bool isOnTable(int** mesa, int palo, int codigoCarta){
	for(int i = 0; i < 5; i++){
		if(mesa[i][0] == palo && mesa[i][1] == codigoCarta){
			return 1;
		}
	}
	return 0;
}
bool tieneJuego(int** mesa){
	printw("%d", tieneEscalera(mesa));
	for(int i = 0; i < 5; i++){
		// printw("%d", hayTrios(mesa, i));
		hayPares(mesa, i);
	}
}
bool hayPares(int** mesa, int posicion){
	if(mesa[posicion][1] > 1 && mesa[posicion][1] < 11){
		return 0;
	}
	for(int CARTA = posicion + 1; CARTA < 5; CARTA++){
		if(mesa[CARTA][1] == mesa[posicion][1]){
			return 1;
		}
	}
	return 0;
}
bool hayTrios(int** mesa, int posicion){
	int contador = 0;
	for(int CARTA = posicion + 1; CARTA < 5; CARTA++){
		if(mesa[CARTA][1] == mesa[posicion][1]){
		// printw("%d", mesa[CARTA][1]);
			contador++;
		}
		if(contador == 0 && CARTA >= 2){
			return 0;
		}
	}
	return contador == 2;
}
int tieneEscalera(int** mesa){
	int** mesaOrdenada = new int*[5];
	int tipo = 1;
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	for(int i = 0; i < 4; i++){
		if(mesaOrdenada[i][1] == 13 && mesaOrdenada[0][1] == 1 || mesaOrdenada[i][1] != mesaOrdenada[i + 1][1] - 1){
			return 0;
		}
		tipo = tipo == 1 && mesaOrdenada[i][0] == mesaOrdenada[i + 1][0] ? 1 : 2; 
	}
	move(45,10);
	return tipo == 1 && mesaOrdenada[0][1] == 10 ? 3 : tipo;
}
void ordenarMesa(int** matriz, const unsigned int tamanioMatriz){
	int valorMayor, paloMayor;
	for(int i = 0; i < tamanioMatriz; i++){
		for(int j = 0; j < tamanioMatriz -1; j++){
			if(matriz[j][1] > matriz[j + 1][1]){
				paloMayor = matriz[j][0];
				valorMayor = matriz[j][1];
				matriz[j][0] = matriz[j + 1][0];
				matriz[j][1] = matriz[j + 1][1];
				matriz[j + 1][0] = paloMayor;
				matriz[j + 1][1] = valorMayor;
			}
		}
	}
}
void copiarMatriz(int** matrizDestino, int** const matrizOriginal, const unsigned int tamanioMatriz, const unsigned int tamanioHijos){
	for(int i = 0; i < tamanioMatriz; i++){
		// for(int k = 0; k < tamanioHijos; k++){
		// 	matrizDestino[i][k] = 
		// }
		matrizDestino[i] = new int[tamanioHijos];
		for(int j = 0; matrizDestino[i][j] = matrizOriginal[i][j]; j++);
	}
}
void crearMapaCorazon(char numeroCarta, int posicion){
	char corazon[20][27] = {
		"--------------------------",
		"-|     |----------|     |-",
		"-|  N  |----------|  N  |-",
		"-|     |----------|     |-",	
		"--------------------------",
		"----       ---      ------",
		"---         -         ----",
		"--                     ---",
		"--         ---        ----",
		"---       | N |      -----",
		"----       ---      ------",
		"-----              -------",
		"------            --------",
		"-------          ---------",
		"--------        ----------",
		"---------      -----------",
		"-|     |--    ----|     |-",
		"-|  N  |---  -----|  N  |-",
		"-|     |----------|     |-",
		"--------------------------"
	};
	attrset (COLOR_PAIR(1));
	imprimirCarta(posicion, numeroCarta, corazon);
}
void crearMapaEspada(char numeroCarta, int posicion){
	char palo[20][27] = {
		"**************************",
		"*     *------------*     *",
		"*  N  *------------*  N  *",
		"*     *---     ----*     *",
		"*******--       ---*******",
		"*-------         --------*",
		"*------           -------*",
		"*-----             ------*",
		"*----      ===      -----*",
		"*---      | N |      ----*",
		"*--        ===        ---*",
		"*-      --     --      --*",
		"*-     ----  ----      --*",
		"*---------    -----------*",	
		"*--------      ----------*",
		"*******------------*******",
		"*     *------------*     *",
		"*  N  *------------*  N  *",
		"*     *------------*     *",
		"**************************"
		// "-------------------------"
	};
	attrset (COLOR_PAIR(2));
	imprimirCarta(posicion, numeroCarta, palo);
}
void crearMapaTrebol(char numeroCarta, int posicion){
	char palo[20][27] = {
		"**************************",
		"*     *------------*     *",
		"*  N  *------------*  N  *",
		"*     *-          -*     *",
		"*******            *******",
		"*------           -------*",
		"*-------         --------*",
		"*---------      ---------*",
		"*---        ===       ---*",
		"*--        | N |       --*",
		"*--        -===-       --*",
		"*---     --    --     ---*",
		"*---------      ---------*",
		"*--------        --------*",	
		"*-------          -------*",
		"*******------------*******",
		"*     *------------*     *",
		"*  N  *------------*  N  *",
		"*     *------------*     *",
		"**************************"
		// "-------------------------"
	};
	attrset (COLOR_PAIR(2));
	imprimirCarta(posicion, numeroCarta, palo);
}
void crearMapaDiamante(char numeroCarta, int posicion){
	char diamante[20][27] = {
		"**************************",
		"*     *------------*     *",
		"*  N  *----   -----*  N  *",
		"*     *---     ----*     *",
		"*******--       ---*******",
		"*-------         --------*",
		"*------           -------*",
		"*-----      ===    ------*",
		"*-----     | N |    -----*",
		"*------     ===    ------*",
		"*-------          -------*",
		"*--------        --------*",
		"*---------      ---------*",
		"*----------    ----------*",	
		"*-----------  -----------*",
		"*******------------*******",
		"*     *------------*     *",
		"*  N  *------------*  N  *",
		"*     *------------*     *",
		"**************************"
		// "-------------------------"
	};
	attrset (COLOR_PAIR(1));
	imprimirCarta(posicion, numeroCarta, diamante);
}
void imprimirCarta(int posicion, char numeroCarta, char mapa[20][27]){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 27; j++){
			if((i == 0 || i == 4) && j < 26){
				move(i,j + 15 + (27 * posicion));
				printw("-");
			}
			if((j == 0 || j == 25) && (i >0 && i < 4)){
				move(i,j + 15 + (27 * posicion));
				printw("|");	
			}
			if(i == 2 && j == 12){
				move(i,j + 15 + (27 * posicion));
				printw("%d", posicion + 1);					
			}
			move(i+5,j + 15 + (27 * posicion));
			if(mapa == NULL){
				if(j % 2 == 0){
					printw("|");
				}else{
					printw("-");
				}
			}else{
				if(mapa[i][j] == 'N'){
					if(numeroCarta == 'D'){
						move(i+5,j + 14 + (27 * posicion));
						// j++;
						printw("10");
					}else{
						printw("%c", numeroCarta);
					}
				}else{
					printw("%c", mapa[i][j]);
				}
			}
		}
		// printw(" | ");
	}
}