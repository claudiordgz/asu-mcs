% Each number 1..16 is assigned to one cell in each box
1 { a(X,Y,N) : X=1..16, Y=1..16, X1<=X, X<=X1+3, Y1<=Y, Y<=Y1+3 } 1 :- N=1..16, X1=4*(0..3)+1, Y1=4*(0..3)+1.

% no two different numbers given a row and a column
:- a(X,Y,N), a(X,Y,N1), N!=N1.

% no two different columns given a row and a number
:- a(X,Y,N), a(X,Y1,N), Y!=Y1.

% no two different rows given a column and a number
:- a(X,Y,N), a(X1,Y,N), X!=X1.
