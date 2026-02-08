#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //pour sleep, a retirer apres debugage
#include <string.h> //utile pour memcpy pour copier la struction PosPiece
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <time.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

void tetris_change_monde (struct Monde *monde){
	int key;
	struct PosPiece posPiece;
	monde->time++;
	if (monde->startMenu){
		key = tetris_get_key( monde );
		switch (key){
			case 10:
				monde->startMenu = 0;
				monde->firstDraw = 1;
				break;
			case 'q':
				monde->quit = 1;
				break;
			case KEY_RESIZE:
				handle_resize();
				break;
			default:
				monde->lastKey = TETRIS_NO_KEY ;
				break;
		}
	}else if (monde->quit){
// quitte le if et else if
	}else if (monde->perdu){
		key = tetris_get_key( monde );
		switch (key){
			case 10:
				monde->startMenu = 0;
				break;
			case 'q':
				monde->quit = 1;
				break;
			case KEY_RESIZE:
				handle_resize();
				break;
			default:
				monde->lastKey = TETRIS_NO_KEY ;
				break;
		}
	}else{
//si on est en jeu
		key = tetris_get_key( monde );
		switch (key){
			case 'q':
			case 'n':
				monde->quit = 1;
				break;
			case KEY_ENTER:
			case KEY_DOWN:
				monde->lastKey = TETRIS_DOWN ;
				break;
			case KEY_UP:
			case ' ':
				monde->lastKey = TETRIS_ROTATE ;
				break;
			case KEY_LEFT:
				monde->lastKey = TETRIS_LEFT ;
				break;
			case KEY_RIGHT:
				monde->lastKey = TETRIS_RIGHT ;
				break;
			case KEY_RESIZE:
				handle_resize();
				break;
			default:
				monde->lastKey = TETRIS_NO_KEY ;
				break;
		}
		if (monde->piecePosee){
//verifie qu'une ligne n'est pas à éliminer
			tetris_nettoie_ligne_complete(monde);
//met une nouvelle pièce dans le plan en haut si la précedente pice vient d'être posée

			monde->lastMoveTime = monde->time;
			monde->posPiece.index = monde->prochainePiece ;
			monde->posPiece.rot = 0;
			monde->posPiece.posX = (TETRIS_WIDTH /2) - 2;
			monde->posPiece.posY = 0;
	
			monde->prochainePiece = (int)((tetris_random(monde))*NOMBRE_PIECES);

			//vérifie qu'elle peut aller sur la case et une case en dessous et pose la pièce
			memcpy ((void *)&posPiece,(void *) &(monde->posPiece), sizeof (struct PosPiece));
			monde->perdu = tetris_verifie_place_piece(monde, &posPiece);
			posPiece.posY++;
			monde->perdu = (monde->perdu)?1:tetris_verifie_place_piece(monde, &posPiece);
			
			
			if (!monde->perdu){
				tetris_pose_piece(monde);
			}
			monde->piecePosee = 0;

		}else if ( ((monde->lastKey == TETRIS_DOWN) && ((monde->time - monde->lastMoveTime ) > MAX_SPEED )) ||
				((monde->time - monde->lastMoveTime ) > vitesseNiveau[monde->niveau]) ){
//deplace le bloc d'une case vers le bas
//soit le temps de mouvement est dépassé, soit on a appuyé sur la touche bas et le laps de temps rapide est dépassé
			monde->lastMoveTime = monde->time;
			tetris_efface_piece(monde);

			memcpy ((void *)&posPiece,(void *) &(monde->posPiece), sizeof (struct PosPiece));
			posPiece.posY++;
			if( tetris_verifie_place_piece(monde, &posPiece)){
			//Si la piece ne peut pas descendre
				monde->piecePosee = 1;
				tetris_pose_piece(monde);
			}else{
				monde->posPiece.posY++;
				tetris_pose_piece(monde);
			}
		}else if (monde->lastKey == TETRIS_LEFT || monde->lastKey == TETRIS_RIGHT){
			//il y a un soucis, c'est qu'il faut pouvoir mettre une piece en dessous et à G ou à D en meme temps dans certains cas pour débloquer certaines situations
		
			tetris_efface_piece(monde);

			memcpy ((void *)&posPiece,(void *) &(monde->posPiece), sizeof (struct PosPiece));
			if (monde->lastKey == TETRIS_LEFT)
				posPiece.posX--;
			if (monde->lastKey == TETRIS_RIGHT)
				posPiece.posX++;
			if( tetris_verifie_place_piece(monde, &posPiece)){
			//Si la piece ne peut pas aller à gauche ou à droite, on la remet la ou elle est 
				tetris_pose_piece(monde);
			}else{
				monde->posPiece.posX= posPiece.posX;
				tetris_pose_piece(monde);
			}
		}else if (monde->lastKey == TETRIS_ROTATE){
			tetris_efface_piece(monde);

			memcpy ((void *)&posPiece,(void *) &(monde->posPiece), sizeof (struct PosPiece));
			posPiece.rot--;
			if (posPiece.rot < 0)
				posPiece.rot = 3;
			if( tetris_verifie_place_piece(monde, &posPiece)){
			//Si la piece ne peut pas aller à gauche ou à droite, on la remet la ou elle est 
				tetris_pose_piece(monde);
			}else{
				monde->posPiece.rot= posPiece.rot;
				tetris_pose_piece(monde);
			}
		}

	}

}


int tetris_get_key (struct Monde *monde){

	int ch;

	noecho();
	cbreak();

	nodelay(stdscr, TRUE);   // getch() devient non-bloquant
	
	ch = getch();
	if (ch != ERR) {
		return ch;
	}
	return 0;
}

int tetris_millisec_sleep(){
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 1 * 1000 * 1000; // 1 ms = 1 000 000 ns

	nanosleep(&ts, NULL);
	return 0;
}


int tetris_verifie_place_piece(struct Monde *monde, struct PosPiece *posPiece ){
//verifie que la piece peut être positionnée 
	int i, j, pris = TETRIS_LIBRE, pt_piece, pt_plan;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[posPiece->index][posPiece->rot][i][j];
			pt_plan = monde->plan[posPiece->posX+i][posPiece->posY+j];

			if (pt_piece != 0){
				if (pt_plan || ( (posPiece->posY + j) >= TETRIS_HEIGHT ) )
					pris = TETRIS_PRIS;
				if (pt_plan || ((posPiece->posX + i) < 0) || ((posPiece->posX + i) >= TETRIS_WIDTH ))
					pris = TETRIS_WALL;
			}
		}
	}
	return pris;
}

int tetris_pose_piece(struct Monde *monde){
//place la pièce
	int i, j, posee = 0, pt_piece, pt_plan, indexPiece;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[monde->posPiece.index][monde->posPiece.rot][i][j];
			pt_plan = monde->plan[monde->posPiece.posX+i][monde->posPiece.posY+j];

			if (pt_piece != 0){
				if(pt_plan !=0)
					posee = 1;
				indexPiece = monde->posPiece.index;
				monde->plan[monde->posPiece.posX+i][monde->posPiece.posY+j] = indexPiece + 1 ;
				monde->planColor[monde->posPiece.posX+i][monde->posPiece.posY+j] = indexPiece + 1;
			}
		}
	}
	return posee;
}


void tetris_efface_piece(struct Monde *monde){
//efface la piece de sa position précédente
	int i, j, pt_piece;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[monde->posPiece.index][monde->posPiece.rot][i][j];

			if (pt_piece != 0){
				monde->plan[monde->posPiece.posX+i][monde->posPiece.posY+j] = 0;
				monde->planColor[monde->posPiece.posX+i][monde->posPiece.posY+j] = DEFAULT_COLOR_PAIR ;
			}
		}
	}
}

void handle_resize(void){

	resizeterm(0, 0);     // synchronise ncurses

	clear();
	wnoutrefresh(stdscr);

	// IMPORTANT : redessiner TOUTES les fenêtres
	// (ou les recréer)
}


//verifie qu'une ligne n'est pas à éliminer
void tetris_nettoie_ligne_complete(struct Monde *monde){
	int i=0, j=0, k=0, l=0, complet;
	for (i = TETRIS_HEIGHT -1; i >= 0 ; (complet)?i:i--){
		//refresh();
		complet = 1;
		for (j = 0 ; j < TETRIS_WIDTH ; j++){
			if (monde->plan[j][i] == 0)
				complet = 0;
		}
		if (complet){
			monde->lines++;
			monde->niveau = monde->lines / LIGNES_PAR_NIVEAU;
			if (monde->niveau >= NIVEAU_MAX)
				monde->niveau = NIVEAU_MAX-1;
			for (k = i ; k >= 1 ; k--){
				for (l = 0 ; l < TETRIS_WIDTH ; l++){
					monde->plan[l][k]=monde->plan[l][k-1];
					monde->planColor[l][k]=monde->planColor[l][k-1];
					
				}
			}
			for (l = 0 ; l < TETRIS_WIDTH ; l++){
				monde->plan[l][0]=0;
				monde->planColor[l][0]=DEFAULT_COLOR_PAIR;
			}
		}
	}
	return;
}
