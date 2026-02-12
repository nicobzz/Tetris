#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ncursesw/ncurses.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

#define RESIZE_CHECK (( COLS != termData.prevCOLS) || (LINES != termData.prevLINES) )
#define RESIZE_SAVE {(termData.prevCOLS = COLS) ;(termData.prevLINES = LINES);}


void tetris_display(){
	int i, j, etat, resizeCheck;
	etat = world.etat;
	//check if screen has been resized
	resizeCheck = RESIZE_CHECK;
	RESIZE_SAVE;
	
	switch (etat){
		case TETRIS_START_MENU:
		case TETRIS_PAUSE:
	// if one are in StartMenu or pause
			if (resizeCheck || (world.etat != termData.prevEtat)){
		//si il y a redimensionnement du terminal on redessine tout
				tetris_creer_fenetre(0);
				switch (termData.size ){
					case SIZE_SMALL:
						for (i=0; i < TETRIS_WIDTH; i++){
							for (j=0 ; j< TETRIS_HEIGHT; j++){
								mvwprintw( termData.tetrisWin , j+1 , i*2+1 , "--");
							}	
						}
						if (etat == TETRIS_START_MENU)
							mvwprintw( termData.tetrisWin , TETRIS_HEIGHT / 2, (TETRIS_WIDTH * 2 - 11)/ 2 + 1, "press ENTER");
						if (etat == TETRIS_PAUSE)
							mvwprintw( termData.tetrisWin , TETRIS_HEIGHT / 2, (TETRIS_WIDTH * 2 - 5)/ 2 + 1, "Pause");
						break;
	
					case SIZE_BIG:
						for (i=0; i < TETRIS_WIDTH; i++){
							for (j=0 ; j< TETRIS_HEIGHT; j++){
								mvwprintw( termData.tetrisWin , j*2+1 , i*4+1 , "----");
								mvwprintw( termData.tetrisWin , j*2+2 , i*4+1 , "----");
							}	
						}
						if (etat == TETRIS_START_MENU)
							mvwprintw( termData.tetrisWin , TETRIS_HEIGHT + 1, (TETRIS_WIDTH * 4 - 11)/ 2 + 1, "press ENTER");
						if (etat == TETRIS_PAUSE)
							mvwprintw( termData.tetrisWin , TETRIS_HEIGHT + 1, (TETRIS_WIDTH * 4 - 5)/ 2 + 1, "Pause");
						
						break;
					default:
						break;
				}
					//redessine les fenetre 
				wrefresh(termData.tetrisWin);
				tetris_dessine_score(world, termData);
			}else{
		//sinon on redessine que le contenu qui change
				if (termData.size != SIZE_TOO_SMALL){
					tetris_dessine_score(world, termData);
				}
			}
			break;
		case TETRIS_GAME:
		case TETRIS_PERDU:
		// Si on est dans le jeu
			if (resizeCheck || (world.etat != termData.prevEtat)){
		//si il y a redimensionnement du terminal on redessine tout
				tetris_creer_fenetre(0);
				if (termData.size != SIZE_TOO_SMALL){
					//redessine les fenetre 
					tetris_redessine_plan();
					
	//redessinne la fenetre des scores
					tetris_dessine_score();
				}

			}else{
		//sinon on redessine que le contenu qui change
				if (termData.size != SIZE_TOO_SMALL){
					tetris_redessine_plan();
		//redessinne la fenetre des scores
					tetris_dessine_score();
				}
			}
		break;
		default:
		break;
	}
	//met à jour ce que représente le précédent état du terminal
	termData.prevEtat = world.etat;
}

void tetris_redessine_plan(){
	int i, j;
	switch (termData.size ){
		case SIZE_SMALL:
			for (i=0; i < TETRIS_WIDTH; i++){
				for (j=0 ; j< TETRIS_HEIGHT; j++){
					if(world.plan[i][j]){
						wattron(termData.tetrisWin, COLOR_PAIR( world.planColor[i][j]));
						mvwprintw( termData.tetrisWin , j+1 , i*2+1, "  ");
						wattroff(termData.tetrisWin, COLOR_PAIR( world.planColor[i][j]));
					}else{
						mvwprintw( termData.tetrisWin , j+1 , i*2+1, "  ");
					}
				}
			}
			break;
		case SIZE_BIG:
			for (i=0; i < TETRIS_WIDTH; i++){
				for (j=0 ; j< TETRIS_HEIGHT; j++){
					if(world.plan[i][j]){
						wattron(termData.tetrisWin, COLOR_PAIR( world.planColor[i][j]));
						mvwprintw( termData.tetrisWin , j*2+1 , i*4+1, "    ");
						mvwprintw( termData.tetrisWin , j*2+2 , i*4+1, "    ");
						wattroff(termData.tetrisWin, COLOR_PAIR( world.planColor[i][j]));
					}else{
						mvwprintw( termData.tetrisWin , j*2+1 , i*4+1, "    ");
						mvwprintw( termData.tetrisWin , j*2+2 , i*4+1, "    ");
					}
				}
			}
			break;
		default:
			break;
	}
	wnoutrefresh(termData.tetrisWin); //sert à effacer les données de fenetre en tampon
	doupdate();
}


void tetris_dessine_score(){

//redessinne la fenetre des scores
	int i, j, c, etat ;
	mvwprintw ( termData.scoreWin, 2, 2, "Level: %d   ",world.niveau + 1);
	mvwprintw ( termData.scoreWin, 3, 2, "Lines: %d   ",world.lines);
	etat = world.etat;
	switch (etat){
		case TETRIS_PERDU:
			mvwprintw ( termData.scoreWin, 4, 2, "You loose! ");
			for (i=0; i<3; i++){
				for (j=0; j<4; j++){
					wattron(termData.scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
					print_piece_point_score_win( i , j);
					wattroff(termData.scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
				}
			}
			break;
		default:
			mvwprintw ( termData.scoreWin, 4, 2, "next:    ");
			wattron(termData.scoreWin,COLOR_PAIR( world.prochainePiece+1));
			for (i=0; i<3; i++){
				for (j=0; j<4; j++){
					c = pieces[world.prochainePiece][0][i][j];
					if (c){
						wattron(termData.scoreWin,COLOR_PAIR( world.prochainePiece + 1));
						print_piece_point_score_win( i, j);
						wattroff(termData.scoreWin,COLOR_PAIR( world.prochainePiece + 1));
					}else{ 
						wattron(termData.scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
						print_piece_point_score_win( i, j);
						wattroff(termData.scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
					}
				}
			}
			break;
	}
	
	mvwprintw( termData.scoreWin, 11, 19, " ");
	wrefresh(termData.scoreWin);
}

void print_piece_point_score_win( int i , int j){

	if (termData.size == SIZE_SMALL){
		mvwprintw( termData.scoreWin, 6+j , 8+i*2, "  ");
	}else if(termData.size == SIZE_BIG){
		mvwprintw( termData.scoreWin, 6+j*2 , 4+i*4, "    ");
		mvwprintw( termData.scoreWin, 6+j*2 +1 , 4+i*4, "    ");
	}

}
