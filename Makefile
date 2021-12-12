CXX = g++
C = gcc
CFLAGS = -W -Wall -ansi -pedantic -std=c++11
LDFLAGS =
EXEC = Chess.xpp
SRC= Arbitre.cpp Player.cpp Board.cpp main.cpp
DEPS= Board.hpp Arbitre.hpp Player.hpp
OBJ= $(SRC:.cpp=.o)

# $@	Le nom de la cible
# $<	Le nom de la première dépendance
# $^	La liste des dépendances
# $?	La liste des dépendances plus récentes que la cible
# $*	Le nom du fichier sans suffixe

all : $(EXEC)

Chess.xpp : $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o : Arbitre.hpp Player.hpp Board.hpp

%.o: %.cpp $(DEPS)
	$(CXX) -o $@ -c $< $(CFLAGS)

%.o: %.c
	$(C) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf Chess.xpp
