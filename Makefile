CC = gcc
CFLAGS = -Wall -g
SOURCESDIR = sources
SOURCESFILES = tetris.c pieces.c display.c niveaux.c change_monde.c random.c
SOURCES = $(addprefix $(SOURCESDIR)/, $(SOURCESFILES))
HEADERSDIR = sources/headers
HEADERSFILES = struc.h functionsdefs.h 
HEADERS =  $(addprefix $(HEADERSDIR)/, $(HEADERSFILES))

OBJECTS = $(SOURCES:.c=.o)
TARGET = Tetris

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ -lncursesw

$(SOURCESDIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJECTS)
