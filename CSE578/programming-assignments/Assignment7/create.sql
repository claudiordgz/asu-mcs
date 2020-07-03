DROP TABLE checkin;
DROP TABLE sequences;
DROP TABLE attraction;
DROP TABLE visitor;
CREATE TABLE attraction (
  AttractionID INTEGER PRIMARY KEY,
  Name TEXT,
  Region TEXT,
  Category TEXT,
  attractionType TEXT
);
CREATE TABLE visitor (
  visitorID INTEGER PRIMARY KEY
);
CREATE TABLE checkin (
  id INTEGER PRIMARY KEY,
  visitorID INTEGER REFERENCES visitor (visitorID),
  checkingTime TIMESTAMP,
  attraction INTEGER REFERENCES attraction (AttractionID),
  duration TEXT,
  checkinType TEXT
);
CREATE TABLE sequences (
  id INTEGER PRIMARY KEY,
  visitorID INTEGER REFERENCES visitor (visitorID),
  attractionSequence TEXT[]
);