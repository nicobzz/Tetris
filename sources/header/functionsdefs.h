#ifndef H_STRUCT_H
#include "struct.h"
#endif


// tetris.c
void clear_term ( struct TermData * termData);
void tetris_init_monde_data( struct Monde *monde);
void tetris_init_data( struct Monde *monde, struct TermData *termData);
int tetris_creer_fenetre( struct TermData *termData, int init);
void tetris_main_loop( struct Monde *monde, struct TermData *termData);

//change_monde.c
void tetris_change_monde (struct Monde *monde);
int tetris_get_key (struct Monde *monde);
int tetris_millisec_sleep();
int tetris_verifie_place_piece(struct Monde *monde, struct PosPiece *piece );
int tetris_pose_piece(struct Monde *monde);
void tetris_efface_piece(struct Monde *monde);
void handle_resize(void);
void tetris_nettoie_ligne_complete(struct Monde *monde);


//display.c
void tetris_display( struct Monde *monde, struct TermData *termData);
void tetris_redessine_plan(struct Monde *monde,struct TermData *termData);
void tetris_dessine_score(struct Monde *monde,struct TermData *termData);

//random.c
double tetris_random(struct Monde *monde);
