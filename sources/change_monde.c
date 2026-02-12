#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //pour sleep, a retirer apres debugage
#include <string.h> //utile pour memcpy pour copier la struction PosPiece
#include <locale.h>
#include <ncursesw/ncurses.h>
#include <time.h>

#include "header/struct.h"
#include "header/functionsdefs.h"

void tetris_change_world (){
	int key,etat;
	struct PosPiece posPiece;
	world.time++;
	etat = world.etat;
	switch (etat){
		case TETRIS_START_MENU:
			key = tetris_get_key( world );
			switch (key){
				case 10:
					world.etat = TETRIS_GAME;
					break;
				case 'q':
					world.etat = TETRIS_QUIT;
					break;
				case KEY_RESIZE:
					handle_resize();
					break;
				default:
					world.lastKey = TETRIS_NO_KEY ;
					break;
			}
			break;
		case TETRIS_QUIT:
			break;
// quitte le if et else if
		case TETRIS_PERDU:
			key = tetris_get_key( world );
			switch (key){
				case 10:
					world.etat = TETRIS_START_MENU;
					tetris_init_world_data( world );
					break;
				case 'q':
					world.etat = TETRIS_QUIT;
					break;
				case KEY_RESIZE:
					handle_resize();
					break;
				default:
					world.lastKey = TETRIS_NO_KEY ;
					break;
			}
			break;
		case TETRIS_PAUSE:
			key = tetris_get_key( world );
			switch (key){
				case 'q':
					world.etat = TETRIS_QUIT;
					break;
				case 10:
				case 'p':
					world.etat = TETRIS_GAME;
					break;
				case KEY_RESIZE:
					handle_resize();
					break;
				default:
					world.lastKey = TETRIS_NO_KEY ;
					break;
			}
		break;
		case TETRIS_GAME:
//si on est en jeu
			key = tetris_get_key( world );
			switch (key){
				case 'q':
					world.etat = TETRIS_QUIT;
					break;
				case 'p':
					world.etat = TETRIS_PAUSE;
					break;
				case KEY_ENTER:
				case KEY_DOWN:
					world.lastKey = TETRIS_DOWN ;
					break;
				case KEY_UP:
				case ' ':
					world.lastKey = TETRIS_ROTATE ;
					break;
				case KEY_LEFT:
					world.lastKey = TETRIS_LEFT ;
					break;
				case KEY_RIGHT:
					world.lastKey = TETRIS_RIGHT ;
					break;
				case KEY_RESIZE:
					handle_resize();
					break;
				default:
					world.lastKey = TETRIS_NO_KEY ;
					break;
			}
			if (world.piecePosee){
	//verifie qu'une ligne n'est pas à éliminer
				tetris_nettoie_ligne_complete(world);
	//met une nouvelle pièce dans le plan en haut si la précedente pice vient d'être posée

				world.lastMoveTime = world.time;
				world.posPiece.index = world.prochainePiece ;
				world.posPiece.rot = 0;
				world.posPiece.posX = (TETRIS_WIDTH /2) - 2;
				world.posPiece.posY = 0;
		
				world.prochainePiece = (int)((tetris_random(world))*NOMBRE_PIECES);


				//vérifie qu'elle peut aller sur la case et une case en dessous et pose la pièce
				memcpy ((void *)&posPiece,(void *) &(world.posPiece), sizeof (struct PosPiece));
				if (tetris_verifie_place_piece(&posPiece))
					world.etat = TETRIS_PERDU;
				posPiece.posY++;
				if( (world.etat == TETRIS_PERDU)?1:tetris_verifie_place_piece(&posPiece))
					world.etat = TETRIS_PERDU;
				if (! (world.etat == TETRIS_PERDU)){
					tetris_pose_piece(world);
				}
				world.piecePosee = 0;

			}else if ( ((world.lastKey == TETRIS_DOWN) && ((world.time - world.lastMoveTime ) > MAX_SPEED )) ||
					((world.time - world.lastMoveTime ) > vitesseNiveau[world.niveau]) ){
	//deplace le bloc d'une case vers le bas
	//soit le temps de mouvement est dépassé, soit on a appuyé sur la touche bas et le laps de temps rapide est dépassé
				world.lastMoveTime = world.time;
				tetris_efface_piece(world);

				memcpy ((void *)&posPiece,(void *) &(world.posPiece), sizeof (struct PosPiece));
				posPiece.posY++;
				if( tetris_verifie_place_piece(&posPiece)){
				//Si la piece ne peut pas descendre
					world.piecePosee = 1;
					tetris_pose_piece(world);
				}else{
					world.posPiece.posY++;
					tetris_pose_piece(world);
				}
			}else if (world.lastKey == TETRIS_LEFT || world.lastKey == TETRIS_RIGHT){
				//il y a un soucis, c'est qu'il faut pouvoir mettre une piece en dessous et à G ou à D en meme temps dans certains cas pour débloquer certaines situations
			
				tetris_efface_piece(world);

				memcpy ((void *)&posPiece,(void *) &(world.posPiece), sizeof (struct PosPiece));
				if (world.lastKey == TETRIS_LEFT)
					posPiece.posX--;
				if (world.lastKey == TETRIS_RIGHT)
					posPiece.posX++;
				if( tetris_verifie_place_piece(&posPiece)){
				//Si la piece ne peut pas aller à gauche ou à droite, on la remet la ou elle est 
					tetris_pose_piece(world);
				}else{
					world.posPiece.posX= posPiece.posX;
					tetris_pose_piece(world);
				}
			}else if (world.lastKey == TETRIS_ROTATE){
				tetris_efface_piece(world);

				memcpy ((void *)&posPiece,(void *) &(world.posPiece), sizeof (struct PosPiece));
				posPiece.rot--;
				if (posPiece.rot < 0)
					posPiece.rot = 3;
				if( tetris_verifie_place_piece(&posPiece)){
				//Si la piece ne peut pas aller à gauche ou à droite, on la remet la ou elle est 
					tetris_pose_piece(world);
				}else{
					world.posPiece.rot= posPiece.rot;
					tetris_pose_piece(world);
				}
			}
			break;
	}

}


int tetris_get_key (){

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


int tetris_verifie_place_piece(struct PosPiece *posPiece ){
//verifie que la piece peut être positionnée 
	int i, j, pris = TETRIS_LIBRE, pt_piece, pt_plan;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[posPiece->index][posPiece->rot][i][j];
			pt_plan = world.plan[posPiece->posX+i][posPiece->posY+j];

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

int tetris_pose_piece(){
//place la pièce
	int i, j, posee = 0, pt_piece, pt_plan, indexPiece;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[world.posPiece.index][world.posPiece.rot][i][j];
			pt_plan = world.plan[world.posPiece.posX+i][world.posPiece.posY+j];

			if (pt_piece != 0){
				if(pt_plan !=0)
					posee = 1;
				indexPiece = world.posPiece.index;
				world.plan[world.posPiece.posX+i][world.posPiece.posY+j] = indexPiece + 1 ;
				world.planColor[world.posPiece.posX+i][world.posPiece.posY+j] = indexPiece + 1;
			}
		}
	}
	return posee;
}


void tetris_efface_piece(){
//efface la piece de sa position précédente
	int i, j, pt_piece;
	for( i = 0 ; i < 4 ; i++){
		for ( j = 0 ; j < 4 ; j++){
			pt_piece = pieces[world.posPiece.index][world.posPiece.rot][i][j];

			if (pt_piece != 0){
				world.plan[world.posPiece.posX+i][world.posPiece.posY+j] = 0;
				world.planColor[world.posPiece.posX+i][world.posPiece.posY+j] = DEFAULT_COLOR_PAIR ;
			}
		}
	}
}

void handle_resize(){

	resizeterm(0, 0);     // synchronise ncurses

	clear();
	wnoutrefresh(stdscr);

	// IMPORTANT : redessiner TOUTES les fenêtres
	// (ou les recréer)
}


//verifie qu'une ligne n'est pas à éliminer
void tetris_nettoie_ligne_complete(){
	int i=0, j=0, k=0, l=0, complet;
	for (i = TETRIS_HEIGHT -1; i >= 0 ; (complet)?i:i--){
		//refresh();
		complet = 1;
		for (j = 0 ; j < TETRIS_WIDTH ; j++){
			if (world.plan[j][i] == 0)
				complet = 0;
		}
		if (complet){
			world.lines++;
			world.niveau = world.lines / LIGNES_PAR_NIVEAU;
			if (world.niveau >= NIVEAU_MAX)
				world.niveau = NIVEAU_MAX-1;
			for (k = i ; k >= 1 ; k--){
				for (l = 0 ; l < TETRIS_WIDTH ; l++){
					world.plan[l][k]=world.plan[l][k-1];
					world.planColor[l][k]=world.planColor[l][k-1];
					
				}
			}
			for (l = 0 ; l < TETRIS_WIDTH ; l++){
				world.plan[l][0]=0;
				world.planColor[l][0]=DEFAULT_COLOR_PAIR;
			}
		}
	}
	return;
}
