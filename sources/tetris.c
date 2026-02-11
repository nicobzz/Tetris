#include <stdio.h>
#include <unistd.h> //pour sleep, a retirer apres debugage
#include <string.h> //utile pour memcpy pour copier la struction PosPiece
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <time.h>
#include <fcntl.h> //pour open et read pour le fichier /dev/random

#include "header/struct.h"
#include "header/functionsdefs.h"


int main (int argc, char ** argv){
	struct Monde monde;
	struct TermData termData;
	setlocale(LC_ALL, "");
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

void tetris_init_monde_data( struct Monde *monde){
	int i, j;
//initialise le monde
	monde->startMenu = 1;
	monde->perdu = 0;
	monde->quit = 0;
	monde->pause = 0;
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
	monde->firstDraw = 1;


}

void tetris_init_data( struct Monde *monde, struct TermData *termData){
//ouvre le fichier /dev/random pour avoir des piece random
	monde->random = open("/dev/random", O_RDONLY);

	tetris_init_monde_data( monde);

	termData->prevLINES = LINES;
	termData->prevCOLS = COLS;
	termData->termTropPetit = 0;
	termData->tetrisWin = NULL;
	termData->scoreWin = NULL;
//Créer les fenetre ncurses
	tetris_creer_fenetre(termData, 1);

//initiatise les couleurs
	init_color( COLOR_PINK, 976, 375, 554);
	init_color( COLOR_BROWN, 340, 230, 220 );

	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(5, COLOR_WHITE, COLOR_CYAN);
	init_pair(6, COLOR_WHITE, COLOR_PINK);
	init_pair(7, COLOR_WHITE, COLOR_BROWN);
	init_pair(DEFAULT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);

}

int tetris_creer_fenetre( struct TermData *termData, int init){
	int tropPetit = 0, posYSWin,  posXSWin, posYTWin, posXTWin;
//Créer ou repositionne les fenetre ncurses
	posYSWin = (LINES / 2) - ( TETRIS_SCORE_HEIGHT / 2 );
	posXSWin = (COLS - (TETRIS_REAL_WIDTH + TETRIS_SPACE_BETWEEN + TETRIS_SCORE_WIDTH))/2 ;
	
	posYTWin = (LINES - (TETRIS_REAL_HEIGHT ))/2 ;
	posXTWin = posXSWin + TETRIS_SCORE_WIDTH + TETRIS_SPACE_BETWEEN ; 
	
	if(posYSWin < 0 || posXSWin <0 || posYTWin <0 ||posXTWin + TETRIS_REAL_WIDTH > COLS || posYTWin + TETRIS_REAL_HEIGHT > LINES){
// si les fenetre n'ont pas la bonne taille on les mete de taille 1
		tropPetit = 1;
		if (init){
//si on est dans la fonction init on crée les fenetre
			termData->tetrisWin = subwin(stdscr, 0, 0, 1, 1);
			termData->scoreWin = subwin(stdscr, 0, 0, 1, 1);
		}else{
//sinon on les redimensionne
			werase(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			mvwin( termData->tetrisWin , 0, 0);
			wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			wresize( termData->tetrisWin , 1, 1);
			wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			
			werase(termData->scoreWin); //sert à effacer les données de fenetre en tampon
			mvwin( termData->scoreWin , 0, 0);
			wnoutrefresh(termData->scoreWin); //sert à effacer les données de fenetre en tampon
			wresize( termData->scoreWin , 1, 1);
			wnoutrefresh(termData->scoreWin); //sert à effacer les données de fenetre en tampon
		}
	}else{
		if (init){
			termData->tetrisWin = subwin(stdscr, TETRIS_REAL_HEIGHT ,TETRIS_REAL_WIDTH , posYTWin, posXTWin);
			termData->scoreWin = subwin(stdscr, TETRIS_SCORE_HEIGHT, TETRIS_SCORE_WIDTH, posYSWin, posXSWin);
		}else{
			werase(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			mvwin( termData->tetrisWin , posYTWin, posXTWin);
			wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			wresize( termData->tetrisWin , TETRIS_REAL_HEIGHT , TETRIS_REAL_WIDTH );
			wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
			
			werase(termData->scoreWin); //sert à effacer les données de fenetre en tampon
			mvwin( termData->scoreWin , posYSWin, posXSWin);
			wnoutrefresh(termData->scoreWin); //sert à effacer les données de fenetre en tampon
			wresize( termData->scoreWin , TETRIS_SCORE_HEIGHT, TETRIS_SCORE_WIDTH);
			wnoutrefresh(termData->scoreWin); //sert à effacer les données de fenetre en tampon
		}
	}

	box(termData->tetrisWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData->tetrisWin);
	wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon

	box(termData->scoreWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData->scoreWin);
	wnoutrefresh(termData->scoreWin); //sert à effacer les données de fenetre en tampon
	if( ! tropPetit){
		mvprintw(posYSWin + TETRIS_SCORE_HEIGHT + 2,posXSWin + 1 , "press q to quit!");
		mvprintw(posYSWin + TETRIS_SCORE_HEIGHT + 3,posXSWin + 1 , "press p to pause!");
		
		}
	return tropPetit;
}

void tetris_main_loop( struct Monde *monde, struct TermData *termData){
	while (1){
		tetris_display( monde, termData );
		tetris_millisec_sleep();
		tetris_change_monde ( monde );
		if (monde->quit) break;
	}
}


