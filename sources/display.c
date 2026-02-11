#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ncursesw/ncurses.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

#define RESIZE_CHECK (( COLS != termData->prevCOLS) || (LINES != termData->prevLINES) )
#define RESIZE_SAVE {(termData->prevCOLS = COLS) ;(termData->prevLINES = LINES);}


void tetris_display( struct Monde *monde, struct TermData *termData){
	int i, j, tropPetit, etat;
	etat = monde->etat;
	switch (etat){
		case TETRIS_START_MENU:
		case TETRIS_PAUSE:
	// Si on est dans StartMenu
			if (RESIZE_CHECK || monde->firstDraw){
		//si il y a redimensionnement du terminal on redessine tout
				RESIZE_SAVE;
				monde->firstDraw = 0;
				tropPetit = tetris_creer_fenetre( termData, 0);
				if (tropPetit){
		// si le terminal est trop petit
					//redessine les fenetre 
					termData->termTropPetit = 1;
					mvprintw ( 5,0, "error, too small terminal");
					refresh();
				}else{
					termData->termTropPetit = 0;
					//redessine les fenetre 
					
					tetris_creer_fenetre( termData, 0);
					for (i=0; i < TETRIS_WIDTH; i++){
						for (j=0 ; j< TETRIS_HEIGHT; j++){
							mvwprintw( termData->tetrisWin , j+1 , i*2+1 , "--");
						}	
					}
					if (etat == TETRIS_START_MENU)
						mvwprintw( termData->tetrisWin , TETRIS_HEIGHT / 2, (TETRIS_WIDTH * 2 - 11)/ 2 + 1, "press ENTER");
					if (etat == TETRIS_PAUSE)
						mvwprintw( termData->tetrisWin , TETRIS_HEIGHT / 2, (TETRIS_WIDTH * 2 - 5)/ 2 + 1, "Pause");
					wrefresh(termData->tetrisWin);
					tetris_dessine_score(monde, termData);
				}

			}else{
		//sinon on redessine que le contenu qui change
				RESIZE_SAVE;
				tetris_dessine_score(monde, termData);
			}
			break;
		case TETRIS_GAME:
			case TETRIS_PERDU:
		// Si on est dans le jeu
			if (RESIZE_CHECK || monde->firstDraw){
		//si il y a redimensionnement du terminal on redessine tout
				RESIZE_SAVE;
				monde->firstDraw = 0;
				tropPetit = tetris_creer_fenetre( termData, 0);
				if (tropPetit){
		// si le terminal est trop petit
					termData->termTropPetit = 1;
					mvprintw ( 5,0, "error, too small terminal");
					refresh();
				}else{
					termData->termTropPetit = 0;
					//redessine les fenetre 
					tetris_creer_fenetre( termData, 0);
					tetris_redessine_plan(monde, termData);
					
	//redessinne la fenetre des scores
					tetris_dessine_score(monde, termData);
				}

			}else{
		//sinon on redessine que le contenu qui change
				RESIZE_SAVE;
				tetris_redessine_plan(monde, termData);
	//redessinne la fenetre des scores
				tetris_dessine_score(monde, termData);
			}
		break;
		default:
		break;
	}
}

void tetris_redessine_plan(struct Monde *monde,struct TermData *termData){
	int i, j;
	for (i=0; i < TETRIS_WIDTH; i++){
		for (j=0 ; j< TETRIS_HEIGHT; j++){
			if(monde->plan[i][j]){
				wattron(termData->tetrisWin, COLOR_PAIR( monde->planColor[i][j]));
				mvwprintw( termData->tetrisWin , j+1 , i*2+1, "  ");
				wattroff(termData->tetrisWin, COLOR_PAIR( monde->planColor[i][j]));
			}else{
				mvwprintw( termData->tetrisWin , j+1 , i*2+1, "  ");
			}
		}
	}
	wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
	doupdate();
}


void tetris_dessine_score(struct Monde *monde,struct TermData *termData){

//redessinne la fenetre des scores
	int i, j, c, etat ;
	mvwprintw ( termData->scoreWin, 2, 2, "Level: %d   ",monde->niveau + 1);
	mvwprintw ( termData->scoreWin, 3, 2, "Lines: %d   ",monde->lines);
	etat = monde->etat;
	switch (etat){
		case TETRIS_PERDU:
			mvwprintw ( termData->scoreWin, 4, 2, "You loose! ");
			for (i=0; i<4; i++){
				for (j=0; j<4; j++){
					wattron(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
					mvwprintw( termData->scoreWin, 6+j , 8+i*2, "  ");
					wattroff(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
				}
			}
			break;
		default:
			mvwprintw ( termData->scoreWin, 4, 2, "next:    ");
			wattron(termData->scoreWin,COLOR_PAIR( monde->prochainePiece+1));
			for (i=0; i<4; i++){
				for (j=0; j<4; j++){
					c = pieces[monde->prochainePiece][0][i][j];
					if (c){
						wattron(termData->scoreWin,COLOR_PAIR( monde->prochainePiece + 1));
						mvwprintw( termData->scoreWin, 6+j , 8+i*2, "  ");
						wattroff(termData->scoreWin,COLOR_PAIR( monde->prochainePiece + 1));
					}else{ 
						wattron(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
						mvwprintw( termData->scoreWin, 6+j , 8+i*2, "  ");
						wattroff(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
					}
				}
			}
			break;
	}
	
	mvwprintw( termData->scoreWin, 11, 19, " ");
	wrefresh(termData->scoreWin);
}
