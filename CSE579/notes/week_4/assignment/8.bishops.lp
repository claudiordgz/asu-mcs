% inputs n size of chessboard (nxn)

{full_board(1..n,1..n)}.

% no two bishops on same diagonal 
:- full_board(R1, C1), full_board(R2, C2), R1<R2, |R1-R2|=|C1-C2|.

% count them
bishops(N) :-N=#count{X, Y: full_board(X, Y)}.

% get the max bishops
#maximize{N: bishops(N)}.
