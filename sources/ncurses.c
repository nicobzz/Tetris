#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main (int argc, char ** argv){
	WINDOW *boite;

	initscr();              // Initialise la structure WINDOW et autres paramètres 
	printw("Hello World");  // Écrit Hello World à l'endroit où le curseur logique est positionné

	attron(A_NORMAL);
	printw("Texte sans mise en forme\n");

	attron(A_STANDOUT);
	printw("Texte en meilleur surlignement\n");
	attroff(A_STANDOUT);

	attron(A_REVERSE);
	printw("Texte en inversion video\n");
	attroff(A_REVERSE);

	attron(A_DIM);
	printw("Texte a moitie brillant\n");
	attroff(A_DIM);

	attron(A_BOLD);
	printw("Texte en gras\n");
	attroff(A_BOLD);

	attron(A_UNDERLINE);
	printw("Texte en souligne\n");
	attroff(A_UNDERLINE);

	attron(A_INVIS);
	printw("Texte invisible\n");
	attroff(A_INVIS);

	attron(A_UNDERLINE | A_BOLD); // Pour appliquer plusieurs type de mises en forme, on utilise l'opérateur unaire |
	printw("Texte en gras souligne\n");

	attron(A_NORMAL);
	boite= subwin(stdscr, 10, 10, LINES / 2, COLS /2);

	box(boite, ACS_VLINE, ACS_HLINE); // ACS_VLINE et ACS_HLINE sont des constantes qui génèrent des bordures par défaut


	move(LINES - 1, COLS - 1);
	
	refresh();              // Rafraîchit la fenêtre courante afin de voir le message apparaître
	getch();                // On attend que l'utilisateur appui sur une touche pour quitter
	endwin();               // Restaure les paramètres par défaut du terminal
	
	free(boite);

	return 0;


}
