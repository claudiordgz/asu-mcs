roll_dice.

{get(N): N=1..6} :- roll_dice.


:- get(N1), get(N2), N1 != N2.

:- not get(1), not get(2), not get(3), not get(4), not get(5), not get(6).