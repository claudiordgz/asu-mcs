%domain(1;2).
%codomain(a;b;c).
domain(1;2;3).
codomain(a;b;c;d;e).

{f(X, Y): codomain(Y)} = 1 :- domain(X).

X=X1 :- f(X,Y), f(X1, Y).

#show f/2.