CREATE TABLE users( 
  userid INTEGER PRIMARY KEY,
  name TEXT NOT NULL
);
CREATE TABLE movies( 
  movieid INTEGER PRIMARY KEY,
  title TEXT NOT NULL
);
CREATE TABLE taginfo( 
  tagid INTEGER PRIMARY KEY,
  content text
);
CREATE TABLE genres( 
  genreid INTEGER PRIMARY KEY,
  name TEXT  NOT NULL
);
CREATE TABLE ratings(
  userid INTEGER REFERENCES users (userid),
  movieid INTEGER REFERENCES movies (movieid),
  rating NUMERIC CHECK (rating <= 5),
  timestamp BIGINT,
  PRIMARY KEY (userid, movieid)
);
CREATE TABLE tags( 
  userid INTEGER REFERENCES users (userid),
  movieid INTEGER REFERENCES movies (movieid),
  tagid INTEGER REFERENCES taginfo (tagid),
  timestamp BIGINT,
  PRIMARY KEY (userid, movieid, tagid)
);
CREATE TABLE hasagenre( 
  movieid INTEGER REFERENCES movies (movieid),
  genreid INTEGER REFERENCES genres (genreid),
  PRIMARY KEY (movieid, genreid)
);