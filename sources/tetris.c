#include <stdio.h>
#include <unistd.h> //pour sleep, a retirer apres debugage
#include <string.h> //utile pour memcpy pour copier la struction PosPiece
#include <ncurses.h>
#include <time.h>
#include <fcntl.h> //pour open et read pour le fichier /dev/random

#include "header/struct.h"
#include "header/functionsdefs.h"


int main (int argc, char ** argv){
	struct Monde monde;
	struct TermData termData;
	initscr();              // Initialise la structure WINDOW et autres paramètres
	cbreak();
	keypad(stdscr, TRUE);	// permet de lire les entrées claviers spéciales en une fois( et non trois caractère pour les touches des flèches
	noecho();
	keypad(stdscr, TRUE); // permet de capter le signal KEY_RESIZE dans les caractères tapés pour faire le redimensionnement du terminal
	start_color();

	tetris_init_data (&monde , &termData);
	tetris_main_loop (&monde , &termData);

//remet bien ncurses
	clear_term ( &termData);

//ferme le fichier random utilisé pour avoir du random pour la piece
	if (monde.random != -1)
		close (monde.random);
	
	return 0;
}

void clear_term ( struct TermData * termData){

	delwin(termData->tetrisWin);
	delwin(termData->scoreWin);
	endwin();               // Restaure les paramètres par défaut du terminal
}

void tetris_init_data( struct Monde *monde, struct TermData *termData){
	int posXTWin, posYTWin, posXSWin, posYSWin, i, j;

//ouvre le fichier /dev/random pour avoir des piece random
	monde->random = open("/dev/random", O_RDONLY);

//initialise le monde
	monde->startMenu = 1;
	monde->time = 0;
	for (i = 0; i<TETRIS_WIDTH; i++){
		for( j = 0; j<TETRIS_HEIGHT; j++){
			monde->plan[i][j]= 0;
			monde->planColor[i][j]= DEFAULT_COLOR_PAIR ;
		}
	}
	monde->niveau = 0;
	monde->posPiece.index = 0;	//le numéro de la pièce actuelle
	monde->posPiece.rot = 0;	//la rotation de la pièce actuelle (0 à 3)
	monde->posPiece.posX = 0;
	monde->posPiece.posY = 0;
	monde->piecePosee = 1;
	monde->lastMoveTime = 0;
	monde->prochainePiece = (int)((tetris_random(monde))*NOMBRE_PIECES);
	monde->score = 0;
	monde->lines = 0;
	monde->perdu = 0;
	monde->quit = 0;
	monde->firstDraw = 1;

	termData->prevLINES = LINES;
	termData->prevCOLS = COLS;
	termData->firstDisplay = 1;
	termData->termTropPetit = 0;
	termData->tetrisWin = NULL;
	termData->scoreWin = NULL;

//Créer les fenetre ncurses
	posYTWin = (LINES - (TETRIS_HEIGHT + 2))/2 ;
	posXTWin = (COLS - (TETRIS_WIDTH + 2))/2 ;
	refresh();
	termData->tetrisWin = subwin(stdscr,TETRIS_HEIGHT+2 ,TETRIS_WIDTH +2 , posYTWin, posXTWin);
	box(termData->tetrisWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData->tetrisWin);

	posYSWin = (LINES / 2) - ( TETRIS_SCORE_HEIGHT / 2 );
	posXSWin = (posXTWin ) - ( TETRIS_SCORE_WIDTH  + 10);
	termData->scoreWin = subwin(stdscr, TETRIS_SCORE_HEIGHT, TETRIS_SCORE_WIDTH, posYSWin, posXSWin);
	box(termData->scoreWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData->scoreWin);

//initiatise les couleurs
	init_color( COLOR_PINK, 976, 375, 554);
	init_color( COLOR_BROWN, 340, 230, 220 );

	init_pair(1, COLOR_GREEN, COLOR_GREEN);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_BLUE, COLOR_BLUE);
	init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(5, COLOR_CYAN, COLOR_CYAN);
	init_pair(6, COLOR_PINK, COLOR_PINK);
	init_pair(7, COLOR_BROWN, COLOR_BROWN);
	init_pair(DEFAULT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);

}


void tetris_main_loop( struct Monde *monde, struct TermData *termData){
	while (1){
		tetris_display( monde, termData );
		tetris_millisec_sleep();
		tetris_change_monde ( monde );
		if (monde->quit) break;
	}
}


