#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

#define RESIZE_CHECK (( COLS != termData->prevCOLS) || (LINES != termData->prevLINES) || (termData->firstDisplay))
#define RESIZE_SAVE {(termData->prevCOLS = COLS) ;(termData->prevLINES = LINES); (termData->firstDisplay = 0);}


void tetris_display( struct Monde *monde, struct TermData *termData){
	int posXTWin, posYTWin, i, j;


	if (monde->startMenu){
// Si on est dans StartMenu
		if (RESIZE_CHECK || monde->firstDraw){
	//si il y a redimensionnement du terminal on redessine tout
			RESIZE_SAVE;
			monde->firstDraw = 0;
			posYTWin = (LINES - (TETRIS_HEIGHT + 2))/2 ;
			posXTWin = (COLS - (TETRIS_WIDTH + 2))/2 ;
			if ((TETRIS_HEIGHT + 2 > LINES) || (TETRIS_WIDTH + 2) > COLS){
	// si le terminal est trop petit
				termData->termTropPetit = 1;
			}else{
				termData->termTropPetit = 0;

				//efface tout

				werase(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
				//redessine la fenetre principale
				mvwin( termData->tetrisWin , posYTWin, posXTWin);
				wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
				wresize( termData->tetrisWin , TETRIS_HEIGHT + 2, TETRIS_WIDTH + 2);
				wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
				box(termData->tetrisWin, ACS_VLINE, ACS_HLINE);
				wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
//				mvwin( termData->scoreWin , posY,posX);
//				wresize( termData->scoreWin , height, width);

	//			box(termData->scoreWin, ACS_VLINE, ACS_HLINE);

	//			wrefresh(termData->scoreWin);
				
				wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
				for (i=0; i < TETRIS_WIDTH; i++){
					for (j=0 ; j< TETRIS_HEIGHT; j++){
						mvwprintw( termData->tetrisWin , j+1 , i+1 , "r");
					}	
				}
				wrefresh(termData->tetrisWin);
				tetris_dessine_score(monde, termData);
			}



		}else{
	//sinon on redessine que le contenu qui change
			RESIZE_SAVE;
		}
	}else if(! monde->startMenu){
	// Si on est dans le jeu
		if (RESIZE_CHECK || monde->firstDraw){
	//si il y a redimensionnement du terminal on redessine tout
			RESIZE_SAVE;
			monde->firstDraw = 0;
			posYTWin = (LINES - (TETRIS_HEIGHT + 2))/2 ;
			posXTWin = (COLS - (TETRIS_WIDTH + 2))/2 ;
			if ((TETRIS_HEIGHT + 2 > LINES) || (TETRIS_WIDTH + 2) > COLS){
	// si le terminal est trop petit
				termData->termTropPetit = 1;
				mvprintw ( 10,0, "erreur terminal trop petit");
				refresh();
			}else{
				termData->termTropPetit = 0;

				//efface tout
				//clear();
				werase(termData->tetrisWin); //sert à effacer les données de fenetre en tampon

				mvwin( termData->tetrisWin , posYTWin, posXTWin);
				wresize( termData->tetrisWin , TETRIS_HEIGHT + 2, TETRIS_WIDTH + 2);
				box(termData->tetrisWin, ACS_VLINE, ACS_HLINE);
				
				wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
	
				wrefresh(termData->tetrisWin);
//				mvwin( termData->scoreWin , posY,posX);
//				wresize( termData->scoreWin , height, width);
	//			box(termData->scoreWin, ACS_VLINE, ACS_HLINE);
	//			wrefresh(termData->scoreWin);
//				wrefresh(termData->tetrisWin);
				//refresh();
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
	}
}

void tetris_redessine_plan(struct Monde *monde,struct TermData *termData){
	int i, j;
	for (i=0; i < TETRIS_WIDTH; i++){
		for (j=0 ; j< TETRIS_HEIGHT; j++){
			if(monde->plan[i][j]){
				wattron(termData->tetrisWin, COLOR_PAIR( monde->planColor[i][j]));
				mvwprintw( termData->tetrisWin , j+1 , i+1, " ");
				wattroff(termData->tetrisWin, COLOR_PAIR( monde->planColor[i][j]));
			}else{
				mvwprintw( termData->tetrisWin , j+1 , i+1, " ");
			}
		}
	}
	//met au bon endroit le curseur
	wnoutrefresh(termData->tetrisWin); //sert à effacer les données de fenetre en tampon
	doupdate();
	//refresh();
}


void tetris_dessine_score(struct Monde *monde,struct TermData *termData){

//redessinne la fenetre des scores
	int i, j ,c, c1, posYSWin, posXSWin, posXTWin;
	posXTWin = (COLS - (TETRIS_WIDTH + 2))/2 ;
	posYSWin = (LINES / 2) - ( TETRIS_SCORE_HEIGHT / 2 );
	posXSWin = (posXTWin ) - ( TETRIS_SCORE_WIDTH  + 10);
	mvwin( termData->scoreWin , posYSWin, posXSWin);
	wresize( termData->scoreWin , TETRIS_SCORE_HEIGHT, TETRIS_SCORE_WIDTH);
	box(termData->scoreWin, ACS_VLINE, ACS_HLINE);
	wrefresh(termData->scoreWin);
	mvwprintw ( termData->scoreWin, 2, 2, "Niveau: %d   ",monde->niveau + 1);
	mvwprintw ( termData->scoreWin, 3, 2, "Lignes: %d   ",monde->lines);
	if (monde->perdu){
		mvwprintw ( termData->scoreWin, 4, 2, "Perdu!           ");
	}else{
		mvwprintw ( termData->scoreWin, 4, 2, "prochaine piece: ");
	}
	wattron(termData->scoreWin,COLOR_PAIR( monde->prochainePiece+1));
	for (i=0; i<4; i++){
		for (j=0; j<4; j++){
			c = pieces[monde->prochainePiece][0][i][j];
			c1 = c?c:' ';
			if (c && ! monde->perdu){
				wattron(termData->scoreWin,COLOR_PAIR( monde->prochainePiece + 1));
			}else{ 
				wattron(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
			}
			if (monde->perdu)
				c1 = ' ';
			mvwprintw( termData->scoreWin, 6+j , 8+i, "%c", c1);
			if (c && ! monde->perdu){
				wattroff(termData->scoreWin,COLOR_PAIR( monde->prochainePiece + 1));
			}else{
				wattroff(termData->scoreWin,COLOR_PAIR( DEFAULT_COLOR_PAIR));
			}
		}
	}
	mvwprintw( termData->scoreWin, 11, 19, " ");
	wrefresh(termData->scoreWin);
}
