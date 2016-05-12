#include <ncurses.h>
void imprimirCartas();
void crearMapaCorazon(char ,int);
void crearMapaPalo(char ,int);
void imprimirCarta(char[20][27], int, char);
int main(){
	/* code */
	imprimirCartas();
	return 0;
}
void imprimirCartas(){

	initscr();
	start_color(); //to add color effect 
	// init_pair(2,COLOR_WHITE	,COLOR_RED); //declaring color(<pair no>,<text color>, <background color>) 
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	while(true){
		printw("%d", COLOR_WHITE);
		char cartas[14] = "A23456789DJQK";
		crearMapaCorazon('4', 0);
		crearMapaPalo('5', 1);
		crearMapaCorazon('4', 2);
		crearMapaCorazon('A', 3);
	 	crearMapaCorazon('D', 4);
		// continue;
		// char palo[20][35] = {
		// 	"----------------------------------",
		// 	"----------------------------------",
		// 	"----------------------------------",
		// 	"----------------------------------",
		// 	"---------------    ---------------",
		// 	"--------------      --------------",
		// 	"------------          ------------",
		// 	"----------             -----------",
		// 	"--------                 ---------",
		// 	"------                    --------",
		// 	"----                        ------",
		// 	"---                           ----",
		// 	"---                            ---",
		// 	"--------------      --------------",
		// 	"--------------      --------------",
		// 	"--------------      --------------",
		// 	"--------------      --------------",
		// 	"----------------------------------",
		// 	"----------------------------------",
		// 	"----------------------------------"
		// };
		// attrset (COLOR_PAIR(2)); //set the colors move(12,30);
		// // numeroCarta = cartas;
		// for(int i = 0; i < 20; i++){
		// 	for(int j = 0; j < 35; j++){
		// 		move(i + 5,j + 40);
		// 		printw("%c", palo[i][j]);
		// 	}
		// }


		// printw("----------------------------------\n");
		// printw("----------------------------------\n");
		// printw("----------------------------------\n");
		// printw("----------------------------------\n");
		// printw("------        -----        -------\n");
		// printw("----           ---           -----\n");
		// printw("---             -             ----\n");
		// printw("---                           ----\n");
		// printw("---                           ----\n");
		// printw("----                         -----\n");
		// printw("------                     -------\n");
		// printw("--------                  --------\n");
		// printw("----------              ----------\n");
		// printw("------------          ------------\n");
		// printw("-------------        -------------\n");
		// printw("---------------     --------------\n");
		// printw("----------------   ---------------\n");
		// printw("----------------- ----------------\n");
		// printw("----------------------------------\n");
		// printw("----------------------------------\n");
		refresh();
	}
	endwin();
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
	imprimirCarta(corazon, posicion, numeroCarta);
}
void crearMapaPalo(char numeroCarta, int posicion){
	char palo[20][27] = {
		"--------------------------",
		"-|     |----------|     |-",
		"-|  N  |----------|  N  |-",
		"-|     |--     ---|     |-",
		"---------       ----------",
		"--------         ---------",
		"-------           --------",
		"------             -------",
		"-----      ---      ------",
		"----      | N |      -----",
		"---        ---        ----",
		"--      --     --      ---",
		"--     ----  ----      ---",
		"----------    ------------",
		"---------      -----------",
		"--------------------------",
		"-|     |----------|     |-",
		"-|  N  |----------|  N  |-",
		"-|     |----------|     |-",
		"--------------------------"
		// "-------------------------"
	};
	attrset (COLOR_PAIR(2));
	imprimirCarta(palo, posicion, numeroCarta);
}
void imprimirCarta(char mapa[20][27], int posicion, char numeroCarta){

		for(int i = 0; i < 20; i++){
			for(int j = 0; j < 27; j++){
				move(i+10,j + 4 + (27 * posicion));
				if(mapa[i][j] == 'N'){
					if(numeroCarta == 'D'){
						move(i+10,j + 3 + (27 * posicion));
						// j++;
						printw("10");
					}else{
						printw("%c", numeroCarta);
					}
				}else{
					printw("%c", mapa[i][j]);
				}
			}
			// printw(" | ");
		}
}