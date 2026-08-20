CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP 
# -MMD génère les fichiers de dépendances .d
#-MP évite les cas d'erreurs lorsque le fichier .h a été renommé
TARGET = mon_programme
#DEPS = $(SRCS:.c=.d) # Liste des fichiers .d (ex: main.d, utils.d)

# Détection automatique de tous les fichiers .c
SRCS = $(wildcard *.c)

# Transformation automatique de la liste des .c en .o
OBJS = $(SRCS:.c=.o)

# Le fichier d'en-tête commun à votre projet
HEADER = utils.h

all: $(TARGET)

# Liaison des objets pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
# $^ = toutes les dépendances ; $@ = la cible (ici mon_programme)

# LA LIGNE UNIQUE : Compile chaque .c en .o automatiquement
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# INCLUSION MAGIQUE : Inclut les fichiers de dépendances s'ils existent
#-include $(DEPS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)