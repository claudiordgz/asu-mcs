% Each number 1..9 is assigned to one cell in each box
1 { a(X,Y,N) : X=1..9, Y=1..9, X1<=X, X<=X1+2, Y1<=Y, Y<=Y1+2 } 1 :- N=1..9, X1=3*(0..2)+1, Y1=3*(0..2)+1.

% no two different numbers given a row and a column
:- a(X,Y,N), a(X,Y,N1), N!=N1.

% no two different columns given a row and a number
:- a(X,Y,N), a(X,Y1,N), Y!=Y1.

% no two different rows given a column and a number
:- a(X,Y,N), a(X1,Y,N), X!=X1.

% regions must contain 1..9
:- a(R,C,N), a(R1,C1,N), R\3 == R1\3, C\3==C1\3, 1{R!= R1; C!=C1}.