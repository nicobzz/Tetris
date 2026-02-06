#define H_STRUCT_H
#include <ncurses.h>

#define TETRIS_HEIGHT 22
#define TETRIS_WIDTH 10

#define TETRIS_SCORE_HEIGHT 12
#define TETRIS_SCORE_WIDTH 20 

#define TETRIS_REAL_HEIGHT ( TETRIS_HEIGHT + 2)
#define TETRIS_REAL_WIDTH ( TETRIS_WIDTH * 2 + 2)

#define TETRIS_SPACE_BETWEEN 8

#define TETRIS_NO_KEY 0
#define TETRIS_DOWN 1
#define TETRIS_LEFT 2
#define TETRIS_RIGHT 3
#define TETRIS_ROTATE 4

#define TETRIS_LIBRE 0
#define TETRIS_PRIS 1
#define TETRIS_WALL 2



#define COLOR_PINK 8
#define COLOR_BROWN 9

#define DEFAULT_COLOR_PAIR 128

struct PosPiece {
	int index;	//le numéro de la pièce actuelle
	int rot;	//la rotatation de la pièce actuelle (0 à 3)
	int posX;
	int posY;
};

struct Monde {
	int time;
	int startMenu;
	int plan[TETRIS_WIDTH][TETRIS_HEIGHT];	//le plan ou les pieces sont mises
	int planColor[TETRIS_WIDTH][TETRIS_HEIGHT];	//le plan ou les couleurs sont mises
	int niveau;
	struct PosPiece posPiece;
	int piecePosee;
	int prochainePiece;
	int lastMoveTime;
	int lastKey;
	int lines;
	int score;
	int perdu;
	int quit;
	int random;
	int firstDraw;
};

struct TermData {
	int prevLINES;
	int prevCOLS;
	int termTropPetit;
	WINDOW *tetrisWin;
	WINDOW *scoreWin;
};


//pieces.c

#define NOMBRE_PIECES 7

extern int pieces[NOMBRE_PIECES][4][4][4];

//niveaux.c

#define LIGNES_PAR_NIVEAU 10
#define NIVEAU_MAX 10

extern int vitesseNiveau[10];

#define MAX_SPEED 30 // vitesse en milliseconde
