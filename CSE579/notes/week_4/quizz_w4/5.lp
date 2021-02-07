a(1;2).

b(a;b;c).

{f(X,Y) : b(Y)}=1 :- a(X).

:- f(X, Y), f(X1, Y), X!=X1.
%X=X1 :- f(X,Y), f(X1, Y).

#show f/2.