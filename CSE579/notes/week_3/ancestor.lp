#include "./family.lp".

ancestor(X, Y) :- parent(X,Y).
ancestor(X, Z) :- ancestor(X, Y), ancestor(Y, Z).