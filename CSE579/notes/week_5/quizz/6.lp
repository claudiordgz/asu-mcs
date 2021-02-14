% initial condition

% :- not loc(monkey, l1, 0).
% :- not loc(bananas, l1, 0).
% :- not loc(box, l2, 0).

% :- not hasBananas(f, 0).
% :- not hasBananas(t, m).


:- not loc(monkey, l1, 0; bananas, l1, 0; box, l2, 0).

:- not hasBananas(f, 0; t, m).

