#SquareDL

A very simple game, commonly played with pen and paper, implemented in SDL with a computer opponent.

Players take it in turns to draw in (click) one of the edges of a square on the grid. If they complete the fourth side of a square they claim that square and it remains their turn; otherwise it becomes the turn of their opponent. The player with the most squares at the end of the game&mdash;i.e. when all squares are claimed&mdash;wins the game.

Compile with `make`. Requires libsdl2-dev and libsdl2-gfx-dev to compile.
