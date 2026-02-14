#define H_STRUCT_H
#include <locale.h>
#include <ncursesw/ncurses.h>

#define TETRIS_HEIGHT 22
#define TETRIS_WIDTH 10

#define TETRIS_SCORE_HEIGHT 12
#define TETRIS_SCORE_BIG_HEIGHT 16
#define TETRIS_SCORE_WIDTH 20 
#define TETRIS_SCORE_BIG_WIDTH 26 

#define TETRIS_REAL_HEIGHT ( TETRIS_HEIGHT + 2)
#define TETRIS_REAL_BIG_HEIGHT ( TETRIS_HEIGHT * 2 + 2)
#define TETRIS_REAL_WIDTH ( TETRIS_WIDTH * 2 + 2)
#define TETRIS_REAL_BIG_WIDTH ( TETRIS_WIDTH * 4 + 2)

#define TETRIS_SPACE_BETWEEN 8

#define TETRIS_NO_KEY 0
#define TETRIS_DOWN 1
#define TETRIS_LEFT 2
#define TETRIS_RIGHT 3
#define TETRIS_ROTATE 4

#define TETRIS_LIBRE 0
#define TETRIS_PRIS 1
#define TETRIS_WALL 2

#define TETRIS_NOWHERE -1
#define TETRIS_START_MENU 0
#define TETRIS_PAUSE 1
#define TETRIS_PERDU 2
#define TETRIS_GAME 3
#define TETRIS_QUIT 4

#define COLOR_PINK 8
#define COLOR_BROWN 9

#define DEFAULT_COLOR_PAIR 128

struct PosPiece {
	int index;	//le numéro de la pièce actuelle
	int rot;	//la rotatation de la pièce actuelle (0 à 3)
	int posX;
	int posY;
};

struct World {
	int time;
	int etat;
	int score;
	int quit;
	int plan[TETRIS_WIDTH][TETRIS_HEIGHT];	//le plan ou les pieces sont mises
	int planColor[TETRIS_WIDTH][TETRIS_HEIGHT];	//le plan ou les couleurs sont mises
	int niveau;
	struct PosPiece posPiece;
	int piecePosee;
	int prochainePiece;
	int lastMoveTime;
	int lastKey;
	int lines;
};

#define SIZE_TOO_SMALL 0
#define SIZE_SMALL 1
#define SIZE_BIG 2

struct TermData {
	int prevLINES;
	int prevCOLS;
	int termTropPetit;
	WINDOW *tetrisWin;
	WINDOW *scoreWin;
	int prevEtat;
	int size;
};

struct System{
	int random;
};


//pieces.c

#define NOMBRE_PIECES 7

extern int pieces[NOMBRE_PIECES][4][4][4];

//niveaux.c

#define LIGNES_PAR_NIVEAU 10
#define NIVEAU_MAX 10

extern int vitesseNiveau[10];

#define MAX_SPEED 30 // vitesse en milliseconde

//main-structs.c

extern struct World world;

extern struct TermData termData;

extern struct System mySystem;
