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
	setlocale(LC_ALL, "");
	initscr();              // Initialise la structure WINDOW et autres paramètres
	cbreak();
	keypad(stdscr, TRUE);	// permet de lire les entrées claviers spéciales en une fois( et non trois caractère pour les touches des flèches
	noecho();
	keypad(stdscr, TRUE); // permet de capter le signal KEY_RESIZE dans les caractères tapés pour faire le redimensionnement du terminal
	start_color();

	tetris_init_data ();
	tetris_main_loop ();

//remet bien ncurses
	clear_term ();

//ferme le fichier random utilisé pour avoir du random pour la piece
	if (mySystem.random != -1)
		close (mySystem.random);
	
	return 0;
}

void clear_term (){

	delwin(termData.tetrisWin);
	delwin(termData.scoreWin);
	endwin();               // Restaure les paramètres par défaut du terminal
}

void tetris_init_world_data(){
	int i, j;
//initialise le world
	
	world.etat = TETRIS_START_MENU;
	world.time = 0;
	for (i = 0; i<TETRIS_WIDTH; i++){
		for( j = 0; j<TETRIS_HEIGHT; j++){
			world.plan[i][j]= 0;
			world.planColor[i][j]= DEFAULT_COLOR_PAIR ;
		}
	}
	world.niveau = 0;
	world.posPiece.index = 0;	//le numéro de la pièce actuelle
	world.posPiece.rot = 0;	//la rotation de la pièce actuelle (0 à 3)
	world.posPiece.posX = 0;
	world.posPiece.posY = 0;
	world.piecePosee = 1;
	world.lastMoveTime = 0;
	world.prochainePiece = (int)((tetris_random(world))*NOMBRE_PIECES);
	world.score = 0;
	world.lines = 0;
}

void tetris_init_data(){
//ouvre le fichier /dev/random pour avoir des piece random
	mySystem.random = open("/dev/urandom", O_RDONLY);

	tetris_init_world_data(world);

	termData.prevLINES = LINES;
	termData.prevCOLS = COLS;
	termData.prevEtat = TETRIS_NOWHERE ;
	termData.termTropPetit = 0;
	termData.tetrisWin = NULL;
	termData.scoreWin = NULL;
//Créer les fenetre ncurses
	tetris_creer_fenetre(1);

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

void tetris_creer_fenetre(int init){
	int posYSWin,  posXSWin, posYTWin, posXTWin, heightTWin, widthTWin, heightSWin, widthSWin;
//Créer ou repositionne les fenetre ncurses

//Create Big size window coordonates
	termData.size = SIZE_BIG;
	heightSWin = TETRIS_SCORE_BIG_HEIGHT;
	posYSWin = (LINES / 2) - ( TETRIS_SCORE_BIG_HEIGHT / 2 );
	posXSWin = (COLS - (TETRIS_REAL_BIG_WIDTH + TETRIS_SPACE_BETWEEN + TETRIS_SCORE_BIG_WIDTH))/2 ;
	
	posYTWin = (LINES - (TETRIS_REAL_BIG_HEIGHT ))/2 ;
	posXTWin = posXSWin + TETRIS_SCORE_BIG_WIDTH + TETRIS_SPACE_BETWEEN ;

	heightTWin = TETRIS_REAL_BIG_HEIGHT;
	widthTWin = TETRIS_REAL_BIG_WIDTH;

	widthSWin = TETRIS_SCORE_BIG_WIDTH;

	if (posYSWin < 0 || posXSWin <0 || posYTWin <0 ||posXTWin + heightTWin > COLS || posYTWin + widthTWin > LINES){
//if too big, create smal size window coordonates
		termData.size = SIZE_SMALL;
		heightSWin = TETRIS_SCORE_HEIGHT;
		posYSWin = (LINES / 2) - ( TETRIS_SCORE_HEIGHT / 2 );
		posXSWin = (COLS - (TETRIS_REAL_WIDTH + TETRIS_SPACE_BETWEEN + TETRIS_SCORE_WIDTH))/2 ;
		
		posYTWin = (LINES - (TETRIS_REAL_HEIGHT ))/2 ;
		posXTWin = posXSWin + TETRIS_SCORE_WIDTH + TETRIS_SPACE_BETWEEN ;

		heightTWin = TETRIS_REAL_HEIGHT;
		widthTWin = TETRIS_REAL_WIDTH;

		widthSWin = TETRIS_SCORE_WIDTH;
	
		if (posYSWin < 0 || posXSWin <0 || posYTWin <0 ||posXTWin + heightTWin > COLS || posYTWin + widthTWin > LINES){
//if too big, create very small window coordonates
			termData.size = SIZE_TOO_SMALL;
			posYSWin = posXSWin = posYTWin = posXTWin = 0;
			heightTWin = widthTWin = heightSWin = widthSWin = 1;
		}
	}

//create or resize the windows according to the coordonates
	if (init){
		termData.tetrisWin = subwin(stdscr, heightTWin, widthTWin, posYTWin, posXTWin);
		termData.scoreWin = subwin(stdscr, heightSWin, widthSWin, posYSWin, posXSWin);
	}else{
		werase(termData.tetrisWin); //sert à effacer les données de fenetre en tampon
		mvwin( termData.tetrisWin , posYTWin, posXTWin);
		wnoutrefresh(termData.tetrisWin); //sert à effacer les données de fenetre en tampon
		wresize( termData.tetrisWin , heightTWin , widthTWin );
		wnoutrefresh(termData.tetrisWin); //sert à effacer les données de fenetre en tampon
		
		werase(termData.scoreWin); //sert à effacer les données de fenetre en tampon
		mvwin( termData.scoreWin , posYSWin, posXSWin);
		wnoutrefresh(termData.scoreWin); //sert à effacer les données de fenetre en tampon
		wresize( termData.scoreWin , heightSWin, widthSWin);
		wnoutrefresh(termData.scoreWin); //sert à effacer les données de fenetre en tampon
	}

	box(termData.tetrisWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData.tetrisWin);
	wnoutrefresh(termData.tetrisWin); //sert à effacer les données de fenetre en tampon

	box(termData.scoreWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData.scoreWin);
	wnoutrefresh(termData.scoreWin); //sert à effacer les données de fenetre en tampon
	if( ! (termData.size == SIZE_TOO_SMALL)){
		if (termData.size == SIZE_SMALL){
			mvprintw(posYSWin + TETRIS_SCORE_HEIGHT + 2,posXSWin + 1 , "press q to quit!");
			mvprintw(posYSWin + TETRIS_SCORE_HEIGHT + 3,posXSWin + 1 , "press p to pause!");
		}else{
			mvprintw(posYSWin + TETRIS_SCORE_BIG_HEIGHT + 2,posXSWin + 1 , "press q to quit!");
			mvprintw(posYSWin + TETRIS_SCORE_BIG_HEIGHT + 3,posXSWin + 1 , "press p to pause!");
		}
	}else{
		mvprintw ( 5,0, "error, too small terminal");
		refresh();
	}
	return ;
}

void tetris_main_loop(){
	while (1){
		tetris_display( world, termData );
		tetris_millisec_sleep();
		tetris_change_world ( world );
		if (world.etat == TETRIS_QUIT) break;
	}
}


