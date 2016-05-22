#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
void iniciarMesa(int**, int*);
void crearMapaCorazon(char ,int);
void crearMapaEspada(char ,int);
void crearMapaDiamante(char ,int);
void crearMapaTrebol(char ,int);
void revolver(int*[], int, int*);
int tieneJuego(int*[]);
int tieneEscalera(int**);
bool tieneFullHouse(int**);
bool sobreMesa(int*[], int, int);
int hayPares(int**);
bool tieneTrio(int**);
bool tieneFullColor(int**);
bool tienePoker(int**);
char getValueChar(int );
void ordenarMesa(int**, const unsigned int);
void imprimirCarta(int, char, char[20][27] = NULL);
void imprimirOpciones();
void imprimirMensaje(char[]);
void imprimirDinero(int);
int pedirDatos(char*, bool);
bool indexOf(int*, int, int);
void copiarMatriz(int**, int** const, unsigned const int, unsigned const int);
int main(){
	srand(time(NULL));
	int** mesa = NULL; 
	mesa = new int*[5];
	int* cartasCongeladas = new int[5];
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	iniciarMesa(mesa, cartasCongeladas);
	char mensaje[] = "¿Desea jugar con trampa? S/N";
	imprimirMensaje(mensaje);
	char esTramposo = getch();
	strcpy(mensaje, "¿Ingrese la cantidad de dinero?");
	imprimirMensaje(mensaje);
	char input[100];
	int dinero = pedirDatos(input, true);
	imprimirDinero(dinero);
	int codigoTrampa = 0;
	while(true){
		char c = '0';
		if(esTramposo == 's' || esTramposo == 'S'){
			strcpy(mensaje, "Ingrese el codigo tramposo. Ver lista ->");
			imprimirMensaje(mensaje);
			codigoTrampa = (int)(getch() - '0');
		}
		strcpy(mensaje, "Ingrese la cantidad a apostar");
		imprimirMensaje(mensaje);
		pedirDatos(input, false);
		// strcpy(mensaje, input);
		imprimirOpciones();
		revolver(mesa, codigoTrampa, cartasCongeladas);
		strcpy(mensaje, "Ingrese las cartas que dejara");
		imprimirMensaje(mensaje);
		int cartasACongelar = pedirDatos(input, false);
		int contadorCartas = 0;
		for(int i = 0; i < cartasACongelar; i++){
			cartasCongeladas[contadorCartas] = (int)(input[i]- '0') - 1;
			contadorCartas++;
		}
		revolver(mesa, codigoTrampa, cartasCongeladas);
		tieneJuego(mesa);
		c = getch();
		iniciarMesa(mesa, cartasCongeladas);
		refresh();
	}
	endwin();
	delete[] mesa;
	return 0;
}
int pedirDatos(char* input, bool esEntero){
	for(int i = 0; i < 100; i++){
		input[i] = -1;
	}
	int contadorTeclas = 0;
	char caracter = getch();
	while(caracter != '\n'){
		input[contadorTeclas] = caracter;
		contadorTeclas++;
		caracter = getch();
	}
	if(esEntero){
		int valorEntero = 0;
		for(int i = 0; i < contadorTeclas; i++){
			valorEntero += (int)(input[i] - '0') * pow(10, contadorTeclas - 1 - i);
		}
		return valorEntero;
	}
	return contadorTeclas;
}
void imprimirDinero(int dinero){
	move(2, 25);
	printw("%d", dinero);
	
}
void imprimirMensaje(char mensaje[]){
	move(35,60);
	addstr("----------------------------------------");
	for(int i = 60; i < 150; i++){
		move(37, i);
		printw(" ");
		move(40, i);
		printw(" ");
	}
	move(37,60);
	addstr("\t");
	addstr(mensaje);
	move(39,60);
	addstr("----------------------------------------");
	move(41,60);
	addstr("----------------------------------------");
	move(40,76);
}
/**
	codigoTrampa:{ 
		1-> Escalera
		2-> Escalera de Color
		3-> Escalera Real
		4-> Doble Par
		5-> Full House
		6-> Poker
		7-> Tiene trio
		8-> Full Color
	}
*/
void revolver(int** mesa, int codigoTrampa, int* cartasCongeladas){
	int* numeros = new int[5];
	int	palo = rand() % 3 + 1;
	//codigoTrampa
	if(codigoTrampa >= 1 && codigoTrampa <= 3){
		int inicio = codigoTrampa == 3 ? 10 : rand() % 13 + 1;
		for(int i = 0; i < 5; i++){
			if(inicio + i <= 13){
				numeros[i] = inicio + i;
			}else if(inicio == 10 && inicio + i == 14){
				numeros[i] = 1;
			}else{
				numeros[i] = 13 - i;
			}
		}
		palo = codigoTrampa != 1 ? rand() % 3 + 1 : 1;
	}else if(codigoTrampa >= 4 && codigoTrampa <= 7){
		int carta = rand() % 13 + 1;
		numeros[0] = codigoTrampa == 7 ? rand() % 13 + 1 : carta;
		numeros[2] = carta;
		carta = codigoTrampa == 6 ? carta : rand() % 13 + 1;
		numeros[1] = carta;
		numeros[3] = carta;
		numeros[4] = codigoTrampa == 5 || codigoTrampa == 7 ? carta : rand() % 13 + 1;
	}

	for(int i = 0; i < 5; i++){
		if(indexOf(cartasCongeladas, i, 5)){
			continue;
		}
		char charCarta; 
		int numeroCarta = 1;
		int* propiedades = mesa[i];
		do{
			if(codigoTrampa >= 1 && codigoTrampa <= 3){
				palo = codigoTrampa == 1 ? rand() % 3 + 1 : palo;
				numeroCarta = numeros[i];
			}else if(codigoTrampa >= 4 && codigoTrampa <= 7){
				numeroCarta = numeros[i];
				palo = rand() % 3 + 1;
			}else{
				numeroCarta = rand() % 13 + 1;
				palo = codigoTrampa != 8 ? rand() % 3 + 1 : palo;
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
		}while(sobreMesa(mesa, palo, numeroCarta) && codigoTrampa == 0);
		propiedades[0] = palo;
		propiedades[1] = numeroCarta;
	}
		delete[] numeros;
}
bool indexOf(int* arreglo, int elemento, int tamanio){
	for(int i = 0; i < tamanio; i++){
		if(arreglo[i] == elemento && elemento != -1){
			return 1;
		}
	}
	return 0;
}
/**
	Convierte el numero de carta al caracter que corresponde.
*/
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
/**
	Inicia las propiedades de todas las cartas de la mesa. Les asigna palo = -1 y numero de carta = -1
*/
void iniciarMesa(int** mesa, int* cartasCongeladas){
	for(int k = 0; k < 5; k++){
		int* propiedades = new int[2];
		propiedades[0] = -1;
		propiedades[1] = -1;
		mesa[k] = propiedades;
		cartasCongeladas[k] = -1;
		imprimirCarta(k, '*');
	}
	imprimirOpciones();
}
/**
	Verifica si hay una carta con las propiedades recibidas sobre la mesa 
*/
bool sobreMesa(int** mesa, int palo, int codigoCarta){
	for(int i = 0; i < 5; i++){
		if(mesa[i][0] == palo && mesa[i][1] == codigoCarta){
			return 1;
		}
	}
	return 0;
}
/**
	Comprueba la mano que tiene, en caso de tener retorna el valor por el cual hay que multiplicar la apuesta
*/
int tieneJuego(int** mesa){
	int escalera = tieneEscalera(mesa);
	move(3,70);
	addstr("------------------------");
	move(5,70);
	addstr("------------------------");
	move(4,70);
	addstr("------------------------");
	move(4,76);
	if(escalera){
		if(escalera == 1){
			addstr("ESCALERA DE COLOR");
		}else if(escalera == 2){
			addstr("ESCALERA NORMAL");
		}else{
			addstr("ESCALERA REAL");
		}
	}else if(tienePoker(mesa)){
		addstr("TIENE POKER");
	}else if(tieneFullHouse(mesa)){
		addstr("TIENE FULL HOUSE");
	}else if(tieneFullColor(mesa)){
		addstr("TIENE FULL COLOR");
	}else if(tieneTrio(mesa)){
		addstr("TIENE TRIO");
	}else if(hayPares(mesa)){
		addstr("Hay pares");
	}
}
int hayPares(int** mesa){
	int** mesaOrdenada = new int*[5];
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	int cantidadPares = 0;
	bool esReal = false;
	for(int i = 0; i < 4; i++){
		if(mesaOrdenada[i][1] == mesaOrdenada[i + 1][1]){
			if(mesaOrdenada[i][1] > 10 || mesaOrdenada[i][1] == 1){
				esReal = true;
			}
			cantidadPares++;
			i++;
		}
	}
	return cantidadPares == 1 ? esReal : cantidadPares;
}
bool tieneFullColor(int** mesa){
	int color = mesa[0][0];
	for(int i = 1; i < 5; i++){
		if(color != mesa[i][0]){
			return 0;
		}
	}
	return 1;
}
bool tienePoker(int** mesa){
	int** mesaOrdenada = new int*[5];
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	bool tiene = 1;
	for(int i = 0; i < 2; i++){
		for(int j = i + 1; j < 5; j++){
			if(mesaOrdenada[i][1] != mesaOrdenada[j][1]){
				tiene = 0;
				break;
			}
		}
	}
	return tiene;
}
bool tieneFullHouse(int** mesa){
	int** mesaOrdenada = new int*[5];
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	if(mesaOrdenada[0][1] == mesaOrdenada[1][1]){
		if(mesaOrdenada[1][1] == mesaOrdenada[2][1]){
			return mesaOrdenada[3][1] == mesaOrdenada[4][1];
		}else{
			return mesaOrdenada[2][1] == mesaOrdenada[3][1] && mesaOrdenada[2][1] == mesaOrdenada[4][1]; 
		}
	}
	return 0;
}
bool tieneTrio(int** mesa){
	int** mesaOrdenada = new int*[5];
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	for(int i = 0; i < 3; i++){
		if(mesaOrdenada[i][1] == mesaOrdenada[i + 2][1]){
			return 1;
		}
	}
	return 0;
}
int tieneEscalera(int** mesa){
	int** mesaOrdenada = new int*[5];
	int tipo = 1;
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	for(int i = 0; i < 4; i++){
		if(mesaOrdenada[i][1] == 1 && (mesaOrdenada[4][1] != 13 && mesaOrdenada[4][1] != 5)){
			return 0;
		}else if(mesaOrdenada[i][1] != 1 && mesaOrdenada[i][1] != mesaOrdenada[i + 1][1] - 1){
			return 0;
		}
		tipo = tipo == 1 && mesaOrdenada[i][0] == mesaOrdenada[i + 1][0] ? 1 : 2; 
	}
	return tipo == 1 && mesaOrdenada[1][1] == 10 && mesaOrdenada[0][1] == 1 ? 3 : tipo;
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
				move(i + 8,j + 15 + (27 * posicion));
				printw("-");
			}
			if((j == 0 || j == 25) && (i >0 && i < 4)){
				move(i + 8,j + 15 + (27 * posicion));
				printw("|");	
			}
			if(i == 2 && j == 12){
				move(i + 8,j + 15 + (27 * posicion));
				printw("%d", posicion + 1);					
			}
			move(i + 13,j + 15 + (27 * posicion));
			if(mapa == NULL){
				if(j % 2 == 0){
					printw("|");
				}else{
					printw("-");
				}
			}else{
				if(mapa[i][j] == 'N'){
					if(numeroCarta == 'D'){
						move(i + 13,j + 14 + (27 * posicion));
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
void imprimirOpciones(){
	attrset (COLOR_PAIR(2));
	for(int i = 5; i <= 30; i++){
		move(33, i);
		printw("-");
		move(35, i);
		printw("-");
		move(45, i);
		printw("-");
		move(46, i);
		printw("-");
		move(0, i + 10);
		printw("-");
		move(4, i + 10);
		printw("-");
		move(6, i + 10);
		printw("-");
		move(0, i + 118);
		printw("-");
		move(4, i + 118);
		printw("-");
		move(6, i + 118);
		printw("-");
	}
// 	Par Jack o mejor	*1		+
// Dos Pares			*2		+-
// Trio				*3		+
// Escalera			*4		+
// Mismo Mejor			*5
// Full House			*9
// Cuatro Iguales		*25
// Escalera Mejor		*50		+
// Escalera Final		*250
	move(5, 25);
	printw("Dinero");
	move(5, 133	);
	printw("Apuesta");
	move(34, 5);
	printw("|\tPosibles Manos");
	move(34, 30);
	printw("|");
	char panelOpciones[9][25] = {
		"Par\t\tx 1",
		"Doble par\tx 2",
		"Trio\t\tx 3",
		"Escalera\t\tx 4",
		"Mismo Mejor\tx 5",
		"Full House\tx 9",
		"Cuatro Iguales\tx 25",
		"Escalera Mejor\tx 50",
		"Escalera Final\tx 250",
	};
	for(int i = 0; i < 9; i++){
		move(36 + i, 5);
		printw("| ");
		printw(panelOpciones[i]);
		move(36 + i, 30);
		printw("|");
	}
}