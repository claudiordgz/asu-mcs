block(a;b).


:- not on(a, table, 0).

:- not on(b, a, 0).

:- not on(a, b, m).

:- not on(b, table, m).

#show move/3.