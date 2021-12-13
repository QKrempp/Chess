CXX = g++
C = gcc
CFLAGS = -W -Wall -ansi -pedantic -std=c++11
LDFLAGS =
BIN=./src/
EXEC = Chess.xpp
SRC= $(BIN)Arbitre.cpp $(BIN)Player.cpp $(BIN)Board.cpp $(BIN)main.cpp
DEPS= $(BIN)Board.hpp $(BIN)Arbitre.hpp $(BIN)Player.hpp
OBJ= $(SRC:.cpp=.o)

# $@	Le nom de la cible
# $<	Le nom de la première dépendance
# $^	La liste des dépendances
# $?	La liste des dépendances plus récentes que la cible
# $*	Le nom du fichier sans suffixe

all : $(EXEC)

Chess.xpp : $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN)main.o : $(BIN)Arbitre.hpp $(BIN)Player.hpp $(BIN)Board.hpp

$(BIN)%.o: $(BIN)%.cpp $(DEPS)
	$(CXX) -o $@ -c $< $(CFLAGS)

$(BIN)%.o: $(BIN)%.c
	$(C) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN)*.o

mrproper: clean
	rm -rf Chess.xpp
