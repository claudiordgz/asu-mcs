#include "./family.lp".

sibling(X, Y) :- parent(Z, X), parent(Z, Y), X!=Y.