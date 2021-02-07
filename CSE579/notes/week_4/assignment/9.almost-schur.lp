% input : positive integers n , k.
% in(I,K) means that I belongs to the K-th subset.
{in(I ,1..k )} =1 :- I=1..n.

% schur numbers but remove duplicate
:- in(I,X), in(J,X), in(I+J,X), I!=J.

