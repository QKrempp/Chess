void Arbitre::threatPawn(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    switch(PIECE_COLOR(b->pieces[c]))
    {
        case WHITE:
            if(i + 1 <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 1), j)]) == EMPTY)
            {
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), j));
                if(i == 1 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + 2), j)]) == EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 2), j));
                }
            }
            if(i + 1 <= 7 && j + 1 <= 7)
            {
                swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + 1), (j + 1))]) == BLACK)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
                }
            }
            if(i + 1 <= 7 && j - 1 >= 0){
                swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + 1), (j - 1))]) == BLACK)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
                }
            }
            break;
        case BLACK:
            if(i - 1 >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 1), j)]) == EMPTY)
            {
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), j));
                if(i == 6 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - 2), j)]) == EMPTY)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 2), j));
                }
            }
            if(i - 1 >= 0 && j + 1 <= 7)
            {
                swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - 1), (j + 1))]) == BLACK)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
                }
            }
            if(i - 1 >= 0 && j - 1 >= 0){
                swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
                if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - 1), (j - 1))]) == BLACK)
                {
                    swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
                }
            }
            break;
    }
}

void Arbitre::threatRook(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    byte color = PIECE_COLOR(b->pieces[c]);
    byte k = 1;
    while(i - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        k++;
    }
    if(i - k >= 0){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i - k), j)]) != color){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
    }
    k = 1;
    while(i + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        k++;
    }
    if(i + k <= 7){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE((i + k), j)]) != color){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
    }
    k = 1;
    while(j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        k++;
    }
    if(j - k >= 0){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE(i, (j - k))]) != color){
            swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
    }
    k = 1;
    while(j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        k++;
    }
    if(j + k <= 7){
        if(PIECE_COLOR(b->pieces[INT_TO_BYTE(i, (j + k))]) != color){
            swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
    }
}

void Arbitre::threatKnight(Board* b, byte c)
{
    byte i = GET_LINE(c);
    byte j = GET_COL(c);
    byte p = 0;
    byte color = PIECE_COLOR(b->pieces[c]);
    if(i - 2 >= 0){
        if(j - 1 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 2), (j - 1)));
            p = b->pieces[INT_TO_BYTE((i - 2), (j - 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 2), (j - 1)));
            }
        }
        if(j + 1 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 2), (j + 1)));
            p = b->pieces[INT_TO_BYTE((i - 2), (j + 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 2), (j + 1)));
            }
        }
    }
    if(i - 1 >= 0){
        if(j - 2 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j - 2)));
            p = b->pieces[INT_TO_BYTE((i - 1), (j - 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 2)));
            }
        }
        if(j + 2 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i - 1), (j + 2)));
            p = b->pieces[INT_TO_BYTE((i - 1), (j + 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 2)));
            }
        }
    }
    if(i + 2 <= 7){
        if(j - 1 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 2), (j - 1)));
            p = b->pieces[INT_TO_BYTE((i + 2), (j - 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 2), (j - 1)));
            }
        }
        if(j + 1 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 2), (j + 1)));
            p = b->pieces[INT_TO_BYTE((i + 2), (j + 1))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 2), (j + 1)));
            }
        }
    }
    if(i + 1 <= 7){
        if(j - 2 >= 0){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j - 2)));
            p = b->pieces[INT_TO_BYTE((i + 1), (j - 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 2)));
            }
        }
        if(j + 2 <= 7){
            swapThreat(b, INT_TO_MOVE(i, j, (i + 1), (j + 2)));
            p = b->pieces[INT_TO_BYTE((i + 1), (j + 2))];
			if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
                swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 2)));
            }
        }
    }
}

void Arbitre::threatBishop(Board* b, byte c)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
    byte p = 0;
    byte k = 1;
    while(i - k >= 0 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i + k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i - k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
    }
}

void Arbitre::threatQueen(Board* b, byte c)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
    byte p =0;
    byte k = 1;
    while(i - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        k++;
    }
    if(i - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), j)];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), j));
    }
    k = 1;
    while(i + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), j)]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        k++;
    }
    if(i + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), j)];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), j));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), j));
    }
    k = 1;
    while(j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        k++;
    }
    if(j - k >= 0){
        p = b->pieces[INT_TO_BYTE(i, (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, i, (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j - k)));
    }
    k = 1;
    while(j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE(i, (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        k++;
    }
    if(j + k <= 7){
        p = b->pieces[INT_TO_BYTE(i, (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, i, (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, i, (j + k)));
    }
    k = 1;
    while(i - k >= 0 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        k++;
    }
    if(i - k >= 0 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i - k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j - k)));
    }
    k = 1;
    while(i + k <= 7 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        k++;
    }
    if(i + k <= 7 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i + k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j + k)));
    }
    k = 1;
    while(i + k <= 7 && j - k >= 0 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i + k), (j - k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        k++;
    }
    if(i + k <= 7 && j - k >= 0){
        p = b->pieces[INT_TO_BYTE((i + k), (j - k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i + k), (j - k)));
    }
    k = 1;
    while(i - k >= 0 && j + k <= 7 && PIECE_TYPE(b->pieces[INT_TO_BYTE((i - k), (j + k))]) == EMPTY){
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        k++;
    }
    if(i - k >= 0 && j + k <= 7){
        p = b->pieces[INT_TO_BYTE((i - k), (j + k))];
		if(PIECE_COLOR(p) != color || PIECE_TYPE(p) == EMPTY){
            swapMove(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
        }
        swapThreat(b, INT_TO_MOVE(i, j, (i - k), (j + k)));
    }
}

void Arbitre::threatKing(Board* b, byte c)
{
    byte i = GET_LINE(c);
	byte j = GET_COL(c);
	byte color = PIECE_COLOR(b->pieces[c]);
	byte p = 0;
    if(i - 1 >= 0){
        swapThreat(b, INT_TO_MOVE( i, j, (i - 1), j));
        p = b->pieces[INT_TO_BYTE((i - 1), j)];
        std::cout << b->threats[!color][INT_TO_BYTE((i - 1), j)] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i - 1), j)] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i - 1), j));
        }
    }
    if(i + 1 <= 7){
        swapThreat(b, INT_TO_MOVE( i, j, (i + 1), j));
        p = b->pieces[INT_TO_BYTE((i + 1), j)];
        std::cout << b->threats[!color][INT_TO_BYTE((i + 1), j)] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i + 1), j)] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i + 1), j));
        }
    }
    if(j - 1 >= 0){
        swapThreat(b, INT_TO_MOVE( i, j, i, (j - 1)));
        p = b->pieces[INT_TO_BYTE(i, (j - 1))];
        std::cout << b->threats[!color][INT_TO_BYTE(i, (j - 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE(i, (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, i, (j - 1)));
        }
    }
    if(j + 1 <= 7){
        swapThreat(b, INT_TO_MOVE( i, j, i, (j + 1)));
        p = b->pieces[INT_TO_BYTE(i, (j + 1))];
        std::cout << b->threats[!color][INT_TO_BYTE(i, (j + 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE(i, (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, i, (j + 1)));
        }
    }
    if(i - 1 >= 0 && j - 1 >= 0){
        swapThreat(b, INT_TO_MOVE( i, j, (i - 1), (j - 1)));
        p = b->pieces[INT_TO_BYTE((i - 1), (j - 1))];
        std::cout << b->threats[!color][INT_TO_BYTE((i - 1), (j - 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i - 1), (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j - 1)));
        }
    }
    if(i + 1 <= 7 && j + 1 <= 7){
        swapThreat(b, INT_TO_MOVE( i, j, (i + 1), (j + 1)));
        p = b->pieces[INT_TO_BYTE((i + 1), (j + 1))];
        std::cout << b->threats[!color][INT_TO_BYTE((i + 1), (j + 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i + 1), (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j + 1)));
        }
    }
    if(i + 1 <= 7 && j - 1 >= 0){
        swapThreat(b, INT_TO_MOVE( i, j, (i + 1), (j - 1)));
        p = b->pieces[INT_TO_BYTE((i + 1), (j - 1))];
        std::cout << b->threats[!color][INT_TO_BYTE((i + 1), (j - 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i + 1), (j - 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i + 1), (j - 1)));
        }
    }
    if(i - 1 >= 0 && j + 1 <= 7){
        swapThreat(b, INT_TO_MOVE( i, j, (i - 1), (j + 1)));
        p = b->pieces[INT_TO_BYTE((i - 1), (j + 1))];
        std::cout << b->threats[!color][INT_TO_BYTE((i - 1), (j + 1))] << ' ';
		if(!b->threats[!color][INT_TO_BYTE((i - 1), (j + 1))] &&  (PIECE_TYPE(p) == EMPTY || PIECE_COLOR(p) != color)){
            swapMove(b, INT_TO_MOVE(i, j, (i - 1), (j + 1)));
        }
    }
    std::cout << std::endl;
}
