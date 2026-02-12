#ifndef H_STRUCT_H
#include "struct.h"
#endif


// tetris.c
void clear_term ();
void tetris_init_world_data();
void tetris_init_data();
void tetris_creer_fenetre(int init);
void tetris_main_loop();

//change_world.c
void tetris_change_world ();
int tetris_get_key ();
int tetris_millisec_sleep();
int tetris_verifie_place_piece( struct PosPiece *piece );
int tetris_pose_piece();
void tetris_efface_piece();
void handle_resize();
void tetris_nettoie_ligne_complete();

//display.c
void tetris_display();
void tetris_redessine_plan();
void tetris_dessine_score();
void print_piece_point_score_win( int i , int j);

//random.c
double tetris_random();
