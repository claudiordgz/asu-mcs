#include "./family.lp".

grandparent(X, Z) :- parent(X, Y), parent(Y, Z).