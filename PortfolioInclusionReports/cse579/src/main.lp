%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% sort and object declaration
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
move(0, 1;0, -1;-1, 0;1, 0).

nodeAt(NodeId, pair(X, Y)) :- init(object(node, NodeId), value(at, pair(X, Y))).
pair(X, Y) :- init(object(node, NodeId), value(at, pair(X, Y))).
node(NodeId) :- init(object(node, NodeId), value(at, pair(X, Y))).

highway(NodeId) :- init(object(highway, NodeId), value(at, pair(X, Y))).

onNode(pickingStation(PSId), NodeId) :- init(object(pickingStation, PSId), value(at, pair(X, Y))), init(object(node, NodeId), value(at, pair(X, Y))).
pickingStation(PSId) :- init(object(pickingStation, PSId), value(at, pair(X, Y))), init(object(node, NodeId), value(at, pair(X, Y))).

onNode(robot(RobotId), NodeId, 0) :- init(object(robot, RobotId), value(at, pair(X, Y))), nodeAt(NodeId, pair(X, Y)).
robot(RobotId) :- init(object(robot, RobotId), value(at, pair(X, Y))).

onNode(shelf(ShelfId), NodeId, 0) :- init(object(shelf, ShelfId), value(at, pair(X, Y))), nodeAt(NodeId, pair(X, Y)).
shelf(ShelfId) :- init(object(shelf, ShelfId), value(at, pair(X, Y))).

onShelf(ProductId, ShelfId, Units, 0) :- init(object(product, ProductId), value(on, pair(ShelfId, Units))).
product(ProductId) :- init(object(product, ProductId), value(on, pair(ShelfId, Units))).

goal(OrderId, NodeId, ProductId, Units, 0) :- 
  init(object(order, OrderId), value(pickingStation, PSId)),
  onNode(pickingStation(PSId), NodeId),
  init(object(order, OrderId), value(line, pair(ProductId, Units))).
order(OrderId) :- init(object(order, OrderId), value(pickingStation, PSId)).

numberOfColumns(NR) :- NR=#count{ X:init(object(node, I), value(at, pair(X, Y))) }.
numberOfRows(NC) :- NC=#count{ Y:init(object(node, I), value(at, pair(X, Y))) }.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Actions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

{ robotMove(RobotId, move(DX, DY), T) : move(DX, DY) } 1 :- robot(RobotId), T=0..maxstep-1.
{ pickUp(RobotId, ShelfId, T) : shelf(ShelfId) } 1 :- robot(RobotId), T=0..maxstep-1.
{ putDown(RobotId, ShelfId, T) : shelf(ShelfId) } 1 :- robot(RobotId), T=0..maxstep-1.

% Goal units are different from Shelf units
{ deliver(RobotId, OrderId, ShelfId, ProductId, UnitsToDeliver, T) : 
    goal(OrderId, NodeId, ProductId, GoalUnits, T), 
    onShelf(ProductId, ShelfId, Units, T), 
    UnitsToDeliver=1..Units
} 1 :- robot(RobotId), T=0..maxstep-1.

%converting them to the necessary output
occurs(object(robot, RobotId), action(move, (DX, DY)), T) :- robotMove(RobotId, move(DX, DY), T).

occurs(object(robot, RobotId), action(pickup, ()), T) :- pickUp(RobotId, _, T).

occurs(object(robot, RobotId), action(putdown, ()), T) :- putDown(RobotId, _, T).

occurs(object(robot, RobotId), action(deliver, (OrderId, ProductId, Units)), T) :- deliver(RobotId, OrderId, ShelfId, ProductId, Units, T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% action constraints
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Two actions cannot occur at the same time
:- occurs(object(robot, RobotId), A1, T), occurs(object(robot, RobotId), A2, T), A1!=A2.

% Limit movement of robot
:- onNode(robot(RobotId), NodeId, T), 
   robotMove(RobotId, move(DX, DY), T),
   nodeAt(NodeId, pair(X, Y)), 
   X + DX < 1.
:- onNode(robot(RobotId), NodeId, T), 
   robotMove(RobotId, move(DX, DY), T),
   nodeAt(NodeId, pair(X, Y)), 
   Y + DY<1.
:- onNode(robot(RobotId), NodeId, T), 
   robotMove(RobotId, move(DX, DY), T),
   nodeAt(NodeId, pair(X, Y)), 
   X + DX > NC,
   numberOfColumns(NC).
:- onNode(robot(RobotId), NodeId, T), 
   robotMove(RobotId, move(DX, DY), T),
   nodeAt(NodeId, pair(X, Y)), 
   Y + DY > NR,
   numberOfRows(NR).

% Robot carrying a Shelf can't pass through other Shelves 
% Shelves may need to be moved out of the way first.
:- onNode(shelf(ShelfId1), NewNodeId, T+1),
    robotMove(RobotId, move(DX, DY), T),
    onRobot(ShelfId2, RobotId, T),
    onNode(robot(RobotId), NodeId, T),
    nodeAt(NodeId, pair(X, Y)),
    nodeAt(NewNodeId, pair(X+DX, Y+DY)),
    ShelfId1!=ShelfId2.

% A shelf cant be picked up by 2 robots
:- pickUp(RobotId1, ShelfId, T), pickUp(RobotId2, ShelfId, T), RobotId1!=RobotId2.

% A robot cannot pickup a shelf if it already has one.
:- pickUp(RobotId, ShelfId1, T), onRobot(ShelfId2, RobotId, T).

% A robot cannot pickup a shelf a shelf is already on a robot
:- pickUp(RobotId1, ShelfId, T), onRobot(ShelfId, RobotId2, T).

% A robot can pick up shelf only if it is on the node containing that shelf
:- pickUp(RobotId, ShelfId, T), onNode(shelf(ShelfId), NodeId, T), not onNode(robot(RobotId), NodeId, T). 

% A robot can put down a shelf only if it has one.
:- putDown(RobotId, ShelfId, T), not onRobot(ShelfId, RobotId, T).

% A robot cannot putdown a shelf on a highway
:- putDown(RobotId, ShelfId, T), onNode(robot(RobotId), NodeId, T), highway(NodeId). 

% Can only deliver if robot is on picking station
:- deliver(RobotId, OrderId, _, ProductId, _, T), goal(OrderId, NodeId, ProductId, _, T), not onNode(robot(RobotId), NodeId, T).

% Can only deliver if robot has the shelf containing product
:- deliver(RobotId, OrderId, ShelfId, ProductId, _, T), onShelf(ProductId, ShelfId, _, T), not onRobot(ShelfId, RobotId, T).

% Cannot deliver more quantities than the order.
:- deliver(RobotId, OrderId, ShelfId, ProductId, ShelfUnits, T),
    goal(OrderId, NodeId, ProductId, GoalUnits, T),
    ShelfUnits > GoalUnits.

% Cannot deliver more quantities than the product.
:- deliver(RobotId, OrderId, ShelfId, ProductId, ShelfUnits, T),
    onShelf(ProductId, ShelfId, GoalUnits, T), ShelfUnits > GoalUnits.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% state constraints
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Picking Station cannot be a highway
:- onNode(pickingStation(_), NodeId), highway(NodeId).

% Shelf cannot be on a highway.
:- onNode(shelf(ShelfId), NodeId, _), highway(NodeId).

% Robots cant swap places
:- onNode(robot(RobotId1), NodeId1, T),
   onNode(robot(RobotId1), NodeId2, T+1),
   onNode(robot(RobotId2), NodeId2, T),
   onNode(robot(RobotId2), NodeId1, T+1),
   RobotId1!=RobotId2.

% 1 Shelf per robot 
:- onRobot(ShelfId, RobotId1, T), onRobot(ShelfId, RobotId2, T), RobotId1!=RobotId2.

% 1 Shelf per node
:- onNode(shelf(ShelfId), NodeId1, T), onNode(shelf(ShelfId), NodeId2, T), NodeId1!=NodeId2.

% No robot on 2 nodes
:- onNode(robot(RobotId), NodeId1, T), onNode(robot(RobotId), NodeId2, T), NodeId1!=NodeId2.

% No 2 robots on the same node
:- onNode(robot(RobotId1), NodeId, T), onNode(robot(RobotId2), NodeId, T), RobotId1!=RobotId2.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% action effects
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Effect of picking up a shelf
onRobot(ShelfId, RobotId, T+1) :- pickUp(RobotId, ShelfId, T), onNode(shelf(ShelfId), NodeId, T), onNode(robot(RobotId), NodeId, T).

%Effect of putting down a shelf
onNode(shelf(ShelfId), NodeId, T+1) :- putDown(RobotId, ShelfId, T), onRobot(ShelfId, RobotId, T), onNode(robot(RobotId), NodeId, T).

% Effect of Delivering a product
% Remove from Goal Units
goal(OrderId, NodeId, ProductId, GoalUnits - FromShelf, T+1) :-
  deliver(RobotId, OrderId, ShelfId, ProductId, FromShelf, T),
  goal(OrderId, NodeId, ProductId, GoalUnits, T).

% Remove from Shelf Units
onShelf(ProductId, ShelfId, ShelfUnits - DeliveredUnits, T+1) :- 
  deliver(RobotId, OrderId, ShelfId, ProductId, DeliveredUnits, T),
  onShelf(ProductId, ShelfId, ShelfUnits, T).

% Effect of moving a robot
onNode(robot(RobotId), NewNodeId, T+1) :-
  onNode(robot(RobotId), NodeId, T),
  nodeAt(NodeId, pair(X, Y)),
  nodeAt(NewNodeId, pair(X+DX, Y+DY)),
  robotMove(RobotId, move(DX, DY), T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% domain independent axioms
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

onNode(robot(RobotId), NodeId, T+1) :- onNode(robot(RobotId), NodeId, T), not robotMove(RobotId, move(_, _), T), T<maxstep.

onNode(shelf(ShelfId), NodeId, T+1) :- onNode(shelf(ShelfId), NodeId, T), not pickUp(_, ShelfId, T), T<maxstep.

onRobot(ShelfId, RobotId, T+1) :- onRobot(ShelfId, RobotId, T), not putDown(RobotId, ShelfId, T), T<maxstep.

goal(OrderId, NodeId, ProductId, Units, T+1) :- 
  goal(OrderId, NodeId, ProductId, GoalUnits, T),
  onShelf(ProductId, ShelfId, Units, T),
  not deliver(_, OrderId, ShelfId, ProductId, _, T),
  T < maxstep.

onShelf(ProductId, ShelfId, Units, T+1) :- 
  onShelf(ProductId, ShelfId, Units, T),
  not deliver(_, _, ShelfId, ProductId, _, T),
  T < maxstep.

% On MaxStep Goal Units must be 0 to be Fullfilled
:- not goal(OrderId, _, ProductId, 0, maxstep), goal(OrderId, _, ProductId, _, 0).

totalTimestamp(N) :- N = #count{ T : occurs(O, A, T) } .
#minimize{ N : totalTimestamp(N) } .

#show init/2.
#show occurs/3.