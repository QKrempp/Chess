#include "Board.hpp"
#include "Arbitre.hpp"
#include "Player.hpp"

#include <iostream>
#include <fstream>

byte inpute_mode    = 0;
byte play_once      = 0;
byte get_moves      = 0;
byte get_board      = 0;
byte update         = 0;
byte cli_mode       = 0;

const char* game_f_name = "data/game.txt";

int parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    Board b;
    b.initDefault();
    parseArgs(argc, argv);
	Player* p1;
	Player* p2;
	if(cli_mode)
	{
        p1 = new Human(&b, WHITE);
        p2 = new AlphaBeta(&b, BLACK);
	}
	else
	{
        p1 = new AlphaBeta(&b, WHITE);
        p2 = new AlphaBeta(&b, BLACK);
	}
	Arbitre a(p1, p2);
    a.initMenaces(&b);
    a.initHash(&b);
    if(inpute_mode)
    {
        std::ifstream game_f;
        game_f.open(game_f_name, std::ios::in);
        if(game_f)
        {
            std::string s;
            while(getline(game_f, s))
            {
                a.playRequest(&b, STR_TO_MOVE(s));
                b.nextTurn();
            }
            game_f.close();
        }
        else
        {
            std::cout << "Erreur, impossible d'ouvrir le fichier spécifié" << std::endl;
        }
    }
    else
    {
        remove(game_f_name);
    }
    if(play_once)
    {
        while(b.getWinner() == RUNNING && play_once)
        {
            std::cout << (int) b.getTurn() << std::endl;
			if(cli_mode)
			{
				std::cout << b << std::endl;
			}
            a.play(&b);
            b.nextTurn();
            play_once--;
        }
    }
    else if(update)
    {
    }
    else
    {
        while(b.getWinner() == RUNNING)
        {
            std::cout << (int) b.getTurn() << std::endl;
			if(cli_mode)
			{
				std::cout << b << std::endl;
			}
            a.play(&b);
            b.nextTurn();
        }
    }
    if(get_board)
    {
        std::ofstream game_b;
        game_b.open("data/board.txt", std::ios::trunc);
        if(game_b)
        {
            std::string s;
            for(byte i = 0; i < 64; i++)
            {
                game_b << (int) b.getPiece(i) << std::endl;
            }
            game_b.close();
        }
        else
        {
            std::cout << "Erreur, impossible d'ouvrir le fichier d'export du board" << std::endl;
        }
    }
    if(get_moves)
    {
        std::ofstream game_m;
        game_m.open("data/moves.txt", std::ios::trunc);
        if(game_m)
        {
            std::string s;
            for(byte i = 0; i < 64; i++)
            {
                uint64_t mv = b.getAltMoves(i, b.getPlayer());
                game_m << mv << std::endl;
            }
            game_m.close();
        }
        else
        {
            std::cout << "Erreur, impossible d'ouvrir le fichier d'export des moves" << std::endl;
        }
    }
    switch(b.getWinner())
    {
        case WHITE:
            std::cout << 'W' << std::endl;
            break;
        case BLACK:
            std::cout << 'B' << std::endl;
            break;
        case DRAW:
            std::cout << 'D' << std::endl;
            break;
    }
    return EXIT_SUCCESS;
}

int parseArgs(int argc, char* argv[])
{
    int i = 0;
    while(i < argc)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'b':
                {
                    get_board = 1;
                    i++;
                    break;
                }
				case 'c':
				{
					cli_mode = 1;
					i++;
					break;
				}	
                case 'i':
                {
                    inpute_mode = 1;
                    i++;
                    if(i < argc && argv[i][0] != '-')
                    {
                        game_f_name = argv[i];
                        i++;
                    }
                    break;
                }
                case 'm':
                {
                    get_moves = 1;
                    i++;
                    break;
                }
                case 's':
                {
                    i++;
                    if(i < argc && argv[i][0] != '-')
                    {
                        byte j = 0;
                        while(argv[i][j] != '\0')
                        {
                            play_once *= 10;
                            play_once += (argv[i][j] - '0');
                            j++;
                        }
                        i++;
                    }
                    else
                    {
                        play_once = 1;
                    }
                    break;
                }
                case 'u':
                {
                    inpute_mode = 1;
                    get_moves = 1;
                    get_board = 1;
                    update = 1;
                    i++;
                    if(i < argc && argv[i][0] != '-')
                    {
                        game_f_name = argv[i];
                        i++;
                    }
                    break;
                }
                default:
                    std::cout << "Erreur, option inconnue" << std::endl;
            }
        }
        else
        {
            i++;
        }
    }
    return EXIT_SUCCESS;
}
