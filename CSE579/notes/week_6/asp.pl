 unsat(1,"2", Y, X):- smoke( X), influence( X, Y),not  smoke( Y).
 smoke( Y):- smoke( X), influence( X, Y),not  unsat(1,"2", Y, X).
:~ unsat(1,"2", Y, X).[2000@0,1, Y, X]
 unsat(2,"1"):-not  smoke( alice).
 smoke( alice):-not  unsat(2,"1").
:~ unsat(2,"1").[1000@0,2]
 unsat(3,"99"):-not  influence( alice, bob).
 influence( alice, bob):-not  unsat(3,"99").
:~ unsat(3,"99").[99000@0,3]
