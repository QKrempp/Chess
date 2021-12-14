CXX = g++
C = gcc
CFLAGS = -W -Wall -ansi -pedantic -std=c++11
CXXFLAGS = -O3 -W -Wall -ansi -pedantic -std=c++11
LDFLAGS =
SRC=./src/
EXEC = Chess.xpp
CPPFILES= $(SRC)Arbitre.cpp $(SRC)Player.cpp $(SRC)Board.cpp $(SRC)main.cpp
DEPS= $(SRC)Board.hpp $(SRC)Arbitre.hpp $(SRC)Player.hpp
OBJ= $(CPPFILES:.cpp=.o)

# $@	Le nom de la cible
# $<	Le nom de la première dépendance
# $^	La liste des dépendances
# $?	La liste des dépendances plus récentes que la cible
# $*	Le nom du fichier sans suffixe

all : $(EXEC)

Chess.xpp : $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(SRC)main.o : $(SRC)Arbitre.hpp $(SRC)Player.hpp $(SRC)Board.hpp

$(SRC)%.o: $(SRC)%.cpp $(DEPS)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(SRC)%.o: $(SRC)%.c
	$(C) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(SRC)*.o

mrproper: clean
	rm -rf Chess.xpp
