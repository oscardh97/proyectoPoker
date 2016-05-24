#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
void iniciarMesa(int**, int*);
void crearMapaCorazon(char ,int);
void crearMapaEspada(char ,int);
void crearMapaDiamante(char ,int);
void crearMapaTrebol(char ,int);
void revolver(int*[], int, int*, bool);
int tieneJuego(int*[]);
int tieneEscalera(int**);
bool tieneFullHouse(int**);
bool sobreMesa(int*[], int, int);
int hayPares(int**);
bool tieneTrio(int**);
bool tieneFullColor(int**);
bool tienePoker(int**);
char getValueChar(int );
void ordenarMesa(int**, const int);
void imprimirCarta(int, char, char[20][27] = NULL);
void imprimirOpciones(bool = false);
void imprimirMensaje(char[]);
void imprimirDinero(int, int);
int pedirDatos(char*, bool);
bool indexOf(int*, int, int);
void liberarMemoriaMesa(int**);
void copiarMatriz(int**, int** const, const int, const int);
int main(){
	srand(time(NULL));
	int** mesa = NULL; 
	mesa = new int*[5];
	int* cartasCongeladas = new int[5];
	char input[100];
	int dinero, apuesta = 0, codigoTrampa = 0;
	char caracter;
	/**Iniciar Pantalla**/
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	iniciarMesa(mesa, cartasCongeladas);
	char mensaje[] = "Desea jugar con trampa? S/N";
	imprimirMensaje(mensaje);
	char esTramposo = getch();
	/**Cargar dinero del archivo*/
	ifstream archivoEntrada;
	if(ifstream("dinero.txt")){
		archivoEntrada.open("dinero.txt");
		archivoEntrada >> dinero;
	}else{
		dinero = 0;
	}
	imprimirDinero(dinero, apuesta);
	archivoEntrada.close();
	
	while(true){
		if(dinero != 0){
			strcpy(mensaje, "Desea modificar la cantidad de dinero? S/N");
			imprimirMensaje(mensaje);
			caracter = getch();
		}else{
			caracter = 's';
		}
		if(caracter == 's' || caracter == 'S'){
			strcpy(mensaje, "Ingrese la cantidad de dinero?");
			imprimirMensaje(mensaje);
			dinero = pedirDatos(input, true);
			imprimirDinero(dinero, apuesta);
		}
		if(esTramposo == 's' || esTramposo == 'S'){
			imprimirOpciones(true);
			strcpy(mensaje, "Ingrese el codigo tramposo. Ver lista ----->>");
			imprimirMensaje(mensaje);
			codigoTrampa = (int)(getch() - '0');
		}
		strcpy(mensaje, "Ingrese la cantidad a apostar");
		imprimirMensaje(mensaje);
		do{
			apuesta = pedirDatos(input, true);
			if(apuesta > dinero || apuesta <= 0){
				strcpy(mensaje, "La apuesta debe estar entre dinero disponible y mayor a cero");
				imprimirMensaje(mensaje);
				refresh();
			}else{
				break;
			}
		}while(1);
		dinero -= apuesta;
		imprimirDinero(dinero, apuesta);
		revolver(mesa, codigoTrampa, cartasCongeladas, false);
		strcpy(mensaje, "Ingrese las cartas que desea congelar");
		imprimirMensaje(mensaje);
		int cartasACongelar = pedirDatos(input, false);
		int contadorCartas = 0;
		for(int i = 0; i < cartasACongelar; i++){
			cartasCongeladas[contadorCartas] = (int)(input[i]- '0') - 1;
			contadorCartas++;
		}
		revolver(mesa, codigoTrampa, cartasCongeladas, true);
		dinero += tieneJuego(mesa) * apuesta;
		apuesta = 0;
		imprimirDinero(dinero, apuesta);
		if(dinero == 0 ){
			strcpy(mensaje, "Usted ha quedado en la ruina! Presione una tecla");	
			imprimirMensaje(mensaje);
			getch();
		}
		strcpy(mensaje, "\tDesea continuar? S/N");
		imprimirMensaje(mensaje);
		caracter = getch();
		if(caracter != '\n' && caracter != 's' && caracter != 'S'){
			strcpy(mensaje, "Visitanos: http://fia.unitec.edu/wiki/index.php/11511308,OSCAR_DAVID_DIAZ_HERNANDEZ");
			imprimirMensaje(mensaje);
			getch();
			break;
		}
		iniciarMesa(mesa, cartasCongeladas);
		refresh();
	}
	endwin();
	for(int i = 0; i < 5; i++){
		delete[] mesa[i];
	}
	delete[] mesa;
	delete[] cartasCongeladas;
	return 0;
}
int pedirDatos(char* input, bool esEntero){
	for(int i = 0; i < 100; i++){
		input[i] = -1;
	}
	int contadorTeclas = 0;
	noecho();
	char caracter = getch();
	while(caracter != '\n'){
		noecho();
		if(caracter >= 48 && caracter <= 57){
			addch(caracter);
			input[contadorTeclas] = caracter;
			contadorTeclas++;
		}
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
void imprimirDinero(int dinero, int apuesta){
	ofstream archivoSalida;
	archivoSalida.open("dinero.txt");
	archivoSalida << dinero;
	archivoSalida.close();

	attrset (COLOR_PAIR(2));
	for(int i = 25; i < 150; i++){
		move(2, i);
		printw("%c", ' ');
	}
	move(2, 28);
	printw("%d", dinero);
	move(2, 133);
	printw("%d", apuesta);
	move(40,80);
}
void imprimirMensaje(char mensaje[]){

	attrset (COLOR_PAIR(2));
	move(35,55);
	addstr("---------------------------------------------------------------");
	move(37,49);
	for(int i = 50; i < 127	; i++){
		move(37, i);
		printw(" ");
		move(40, i);
		printw(" ");
	}
	// move(37,55);
	// addstr("\t");
	move(37,67);
	addstr(mensaje);
	move(39,55);
	addstr("---------------------------------------------------------------");
	move(41,55);
	addstr("---------------------------------------------------------------");
	move(40,80);
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
void revolver(int** mesa, int codigoTrampa, int* cartasCongeladas, bool segundaVez){
	int* numeros = new int[5];
	int** mesaActual = new int*[5];
	copiarMatriz(mesaActual, mesa, 5, 2);
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
				if(codigoTrampa == 6){
					palo = i;
				}
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
		}while(sobreMesa(segundaVez ? mesaActual : mesa, palo, numeroCarta) && (codigoTrampa == 0));
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
	move(4,70);
	addstr("                        ");
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
// 	Par Jack o mejor	*1		+
// Dos Pares			*2		+-
// Trio				*3		+
// Escalera			*4		+
// Mismo Mejor			*5
// Full House			*9
// Cuatro Iguales		*25
// Escalera Mejor		*50		+
// Escalera Final		*250

	attrset (COLOR_PAIR(2));
	move(5, 25);
	int escalera = tieneEscalera(mesa);
	int pares = hayPares(mesa);
	move(3,70);
	addstr("------------------------");
	move(5,70);
	addstr("------------------------");
	move(4,70);
	for(int i = 70; i < 90; i++){
		move(4, i);
		printw(" ");
	}
	// addstr("------------------------");
	move(4,76);
	if(escalera){
		if(escalera == 1){
			addstr("ESCALERA DE COLOR");
			return 50;
		}else if(escalera == 2){
			addstr("ESCALERA NORMAL");
			return 4;
		}else{
			addstr("ESCALERA REAL");
			return 250;
		}
	}else if(tienePoker(mesa)){
		addstr("TIENE POKER");
		return 25;
	}else if(tieneFullHouse(mesa)){
		addstr("TIENE FULL HOUSE");
		return 9;
	}else if(tieneFullColor(mesa)){
		addstr("TIENE FULL COLOR");
		return 5;
	}else if(tieneTrio(mesa)){
		addstr("TIENE TRIO");
		return 3;
	}else if(pares){
		if(pares == 1){
			addstr("HAY PAR");
			return 1;
		}else{
			addstr("HAY DOS PARES");
			return 2;
		}
	}
	addstr("INTENTA DE NUEVO");
	return 0;
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
	liberarMemoriaMesa(mesaOrdenada);
	return cantidadPares == 1 ? esReal : cantidadPares;
}
void liberarMemoriaMesa(int** mesa){
	for(int i = 0; i < 5; i++){
		delete[] mesa[i];
	}
	delete[] mesa;
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
	for(int i = 0; i < 2; i++){
		if(mesaOrdenada[i][1] == mesaOrdenada[i + 3][1]){
			return 1;
		}
	}
	liberarMemoriaMesa(mesaOrdenada);
	return 0;
}
bool tieneFullHouse(int** mesa){
	int** mesaOrdenada = new int*[5];
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	copiarMatriz(mesaOrdenada, mesa, 5, 2);
	ordenarMesa(mesaOrdenada, 5);
	if(mesaOrdenada[0][1] == mesaOrdenada[1][1]){
		if(mesaOrdenada[1][1] == mesaOrdenada[2][1]){
			return mesaOrdenada[3][1] == mesaOrdenada[4][1];
		}else{
			return mesaOrdenada[2][1] == mesaOrdenada[3][1] && mesaOrdenada[2][1] == mesaOrdenada[4][1]; 
		}
	}
	liberarMemoriaMesa(mesaOrdenada);
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
	liberarMemoriaMesa(mesaOrdenada);
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
	tipo = tipo == 1 && mesaOrdenada[1][1] == 10 && mesaOrdenada[0][1] == 1 ? 3 : tipo;
	liberarMemoriaMesa(mesaOrdenada);
	return tipo;
}
void ordenarMesa(int** matriz, const int tamanioMatriz){
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
void copiarMatriz(int** matrizDestino, int** const matrizOriginal, const int tamanioMatriz, const int tamanioHijos){
	for(int i = 0; i < tamanioMatriz; i++){
		matrizDestino[i] = new int[tamanioHijos];
		for(int j = 0; j < tamanioHijos; j++){
			matrizDestino[i][j] = matrizOriginal[i][j];
		}
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
void imprimirOpciones(bool esTramposo){
	attrset (COLOR_PAIR(2));
	for(int i = 5; i <= 37; i++){
		/**BORDES DEL PANEL**/
		move(33, i);
		printw("-");
		move(35, i);
		printw("-");
		move(45, i);
		printw("-");
		move(46, i);
		printw("-");
		/**BORDES DINERO**/
		move(0, i + 10);
		printw("-");
		move(4, i + 10);
		printw("-");
		move(6, i + 10);
		printw("-");
		/**BORDES APUESTA**/
		move(0, i + 113);
		printw("-");
		move(4, i + 113);
		printw("-");
		move(6, i + 113);
		printw("-");
	}
	move(5, 29);
	printw("Dinero");
	move(5, 132	);
	printw("Apuesta");
	move(34, 5);
	printw("|\tPosibles Manos");
	move(34, 37);
	printw("|");
	char panelOpciones[9][25] = {
		"1. Par\t\t\tx 1",
		"2. Doble par\t\tx 2",
		"3. Trio\t\t\tx 3",
		"4. Escalera\t\tx 4",
		"5. Mismo Mejor\t\tx 5",
		"6. Full House\t\tx 9",
		"7. Cuatro Iguales\tx 25",
		"8. Escalera Mejor\tx 50",
		"9. Escalera Final\tx 250",
	};
	char panelTrampa[9][25] = {
		"1. Escalera",
		"2. Escalera de Color",
		"3. Escalera Real",
		"4. Doble Par",
		"5. Full House",
		"6. Mismo Mejor",
		"7. Trio",
		"8. Full Color",
		"9. Par",
	};
	for(int i = 0; i < 9; i++){
		move(36 + i, 5);
		printw("| ");
		printw(panelOpciones[i]);
		move(36 + i, 37);
		printw("|");
		if(esTramposo){
			move(35 + i, 130);
			printw(panelTrampa[i]);
		}
	}
}