%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% sort and object declaration
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% • init(object(node,’n’),value(at,pair(’x’,’y’))).
% The ground term ’n’ labels a grid cell at the coordinates given by
% positive integers ’x’ and ’y’. Taking such facts together, the integers
% for ’x’ and ’y’ each start at 1 and specify a rectangular grid without
% holes.

locationNode(NodeLocation, pair(X,Y)) :- init(object(node, NodeLocation), value(at, pair(X,Y))).
pair(X,Y):- init(object(node, NodeLocation), value(at, pair(X,Y))).
node(NodeLocation):- init(object(node, NodeLocation),value(at, pair(X,Y))).

% Static Locations

% • init(object(highway,’h’),value(at,pair(’x’,’y’))).
% The ground term ’h’ labels a highway cell at the coordinates ’x’ and
% ’y’, where a corresponding fact of the first form exists and specifies
% the grid cell as such.

locationHighway(NodeLocation, pair(X,Y)) :- init(object(highway, NodeLocation), value(at, pair(X, Y))).
highway(NodeLocation):- init(object(highway, NodeLocation), value(at, pair(X,Y))).


% • init(object(pickingStation,’p’),value(at,pair(’x’,’y’))).
% The ground term ’p’ labels a picking station at the cell with coordinates ’x’ and ’y’, 
% where a corresponding fact of the first form exists
% and specifies the grid cell as such.

locationPickingStation(PickingStationLocation, NodeLocation) :- 
  init(object(pickingStation,PickingStationLocation), value(at, pair(X,Y))),
  init(object(node,NodeLocation), value(at, pair(X,Y))).
pickingStation(PickingStationLocation):- 
  init(object(pickingStation, PickingStationLocation), value(at,pair(X,Y))),
  init(object(node, NodeLocation), value(at, pair(X,Y))).

% Locations that depend on Time T

% • init(object(robot,’r’),value(at,pair(’x’,’y’))).
% The ground term ’r’ labels a robot, initially located at the cell with
% coordinates ’x’ and ’y’, where a corresponding fact of the first form
% exists and specifies the grid cell as such.

locationRobot(RobotLocation, object(node, NodeLocation), 0) :-
  init(object(robot, RobotLocation), value(at, pair(X,Y))), locationNode(NodeLocation, pair(X,Y)).
robot(RobotLocation):- init(object(robot, RobotLocation), value(at,pair(X,Y))).

% • init(object(shelf,’s’),value(at,pair(’x’,’y’))).
% The ground term ’s’ labels a shelf, initially located at the cell with
% coordinates ’x’ and ’y’, where a corresponding fact of the first form
% exists and specifies the grid cell as such.

locationShelf(PickingStationLocation, object(node, NodeCoordinates), 0) :- 
  init(object(shelf,PickingStationLocation), value(at, pair(X,Y))), locationNode(NodeCoordinates,pair(X,Y)).
shelf(PickingStationLocation):- init(object(shelf, PickingStationLocation), value(at,pair(X,Y))).

% • init(object(product,’i’),value(on,pair(’s’,’u’))).
% The ground term ’i’ labels a product, of which a positive integer ’u’
% many units are initially stored on shelf ’s’, where a corresponding
% fact of the previous form exists and specifies an initial cell for the
% shelf. Moreover, the number ’u’ of units is unique for each product
% ’i’ and shelf ’s’, i.e., there is not more than one fact referring to the
% same product and shelf.

locationProduct(ProductLocation, object(shelf,PickingStationLocation), has(quantity, ProductQuantity), 0) :- 
  init(object(product, ProductLocation), value(on, pair(PickingStationLocation, ProductQuantity))).
product(ProductLocation) :-
  init(object(product, ProductLocation),value(on,pair(PickingStationLocation,ProductQuantity))).

% • init(object(order,’o’),value(pickingStation,’p’)).
% The ground term ’o’ labels an order, whose included products ought
% to be delivered to picking station ’p’. There is exactly one fact of this
% kind per order ’o’ occurring in facts of the previous form, and a fact
% specifying the cell of picking station ’p’ exists as well.

% • init(object(order,’o’),value(line,pair(’i’,’u’))).
% The ground term ’o’ labels an order, including a positive integer ’u’
% many units of product ’i’, where the number ’u’ of units is unique
% for each order ’o’ and product ’i’, i.e., there is not more than one
% fact referring to the same order and product.

% TODO: Check how to diff between picking station and line
locationOrder(OrderLocation, object(node, NodeLocation), hasProductAndAmount(ProductLocation, ProductQuantity), 0) :-
  init(object(order, OrderLocation), value(pickingStation, PickingStationLocation)), locationPickingStation(PickingStationLocation, NodeLocation),
  init(object(order, OrderLocation), value(line,pair(ProductLocation, ProductQuantity))).
order(OrderLocation):- init(object(order, OrderLocation),value(pickingStation, PickingStationLocation)).

#show locationOrder/4.

%%%%%%%%%%%%%%%%%%%%%%%%%%
% state description
%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% effect and precoNodeLocationtions of action
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

move(0,1;0,-1;-1,0;1,0).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% action constraints
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% domain independent axioms
%%%%%%%%%%%%%%%%%%%%%%%%%%%%