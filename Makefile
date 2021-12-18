CXX = g++
C = gcc
CFLAGS = -W -Wall -ansi -pedantic -std=c++11
CXXFLAGS = -O3 -W -Wall -ansi -pedantic -std=c++11
LDFLAGS =
SRC=./src/
WIN=./win/
EXEC = Chess.xpp
CXXFILES= $(SRC)Arbitre.cpp $(SRC)Player.cpp $(SRC)Board.cpp $(SRC)main.cpp
DEPS= $(SRC)Board.hpp $(SRC)Arbitre.hpp $(SRC)Player.hpp
OBJ= $(CXXFILES:.cpp=.o)


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
	$(C) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(SRC)*.o

mrproper: clean
	rm -rf Chess.xpp win/Chess.exe

$(WIN)Chess.exe : $(WINOBJ)
	x86_64-w64-mingw32-g++ $(CXXFILES) -o win/Chess.exe $(CXXFLAGS)

