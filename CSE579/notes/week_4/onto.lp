domain(1;2;3;4;5;6;7).
codomain(a;b).
%domain(1;2;3).
%codomain(a;b;c;d;e).

{f(X, Y): codomain(Y)} = 1 :- domain(X).

:- {f(X,Y) : domain(X)} = 0, codomain(Y).

#show f/2.