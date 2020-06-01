CREATE TABLE query1 AS
  SELECT genres.name as name, COUNT(movies.movieid) as moviecount
  FROM hasagenre
  LEFT JOIN movies 
  ON hasagenre.movieid = movies.movieid
  LEFT JOIN genres
  ON hasagenre.genreid = genres.genreid
  GROUP BY genres.name;

CREATE TABLE query2 AS
  SELECT genres.name as name, AVG(ratings.rating) as rating
  FROM hasagenre
  LEFT JOIN movies 
  ON hasagenre.movieid = movies.movieid
  LEFT JOIN genres
  ON hasagenre.genreid = genres.genreid
  LEFT JOIN ratings
  ON hasagenre.movieid = ratings.movieid
  GROUP BY genres.name;

CREATE TABLE query3 AS
  SELECT title, CountOfRatings
  FROM (SELECT movies.title as title, COUNT(ratings.rating) as CountOfRatings
    FROM movies 
    LEFT JOIN ratings
    ON movies.movieid = ratings.movieid
    GROUP BY title
  ) as rating_counts
  WHERE CountOfRatings >= 10
  ORDER BY title;
  
CREATE TABLE query4 AS
  SELECT movies.movieid as movieid, movies.title as title
  FROM (SELECT *
    FROM genres
    WHERE genres.name LIKE 'Comedy') as genres
  LEFT JOIN hasagenre
  ON hasagenre.genreid = genres.genreid
  LEFT JOIN movies 
  ON hasagenre.movieid = movies.movieid;

CREATE TABLE query5 AS
  SELECT movies.title as title, AVG(ratings.rating) as average
  FROM ratings
  LEFT JOIN movies
  ON ratings.movieid = movies.movieid
  GROUP BY movies.title
  ORDER BY movies.title;

CREATE TABLE query6 AS
  SELECT AVG(ratings.rating) as average
  FROM (SELECT *
  FROM genres
  WHERE genres.name LIKE 'Comedy') as genres
  LEFT JOIN hasagenre
  ON genres.genreid = hasagenre.genreid
  LEFT JOIN ratings
  ON hasagenre.movieid = ratings.movieid;

CREATE TABLE query7 AS
  SELECT AVG(rating) as average
  FROM (
    SELECT r.movieid
    FROM genres g, hasagenre h, ratings r
    WHERE g.genreid = h.genreid AND r.movieid = h.movieid
      AND (g.name = 'Comedy')
    INTERSECT
    SELECT r.movieid
    FROM genres g, hasagenre h, ratings r
    WHERE g.genreid = h.genreid AND r.movieid = h.movieid
      AND (g.name = 'Romance')
  ) AS movies, ratings r
  WHERE movies.movieid = r.movieid;

CREATE TABLE query8 AS
  SELECT AVG(rating) as average
  FROM (
    SELECT r.movieid
    FROM genres g, hasagenre h, ratings r
    WHERE g.genreid = h.genreid AND r.movieid = h.movieid
      AND (g.name = 'Romance')
    EXCEPT 
    SELECT r.movieid
    FROM genres g, hasagenre h, ratings r
    WHERE g.genreid = h.genreid AND r.movieid = h.movieid
      AND (g.name = 'Comedy')
  ) AS movies, ratings r
  WHERE movies.movieid = r.movieid;

CREATE TABLE query9 AS
  SELECT r.movieid as movieid, r.rating as rating
  FROM ratings r
  WHERE r.userid = :v1;

--------------- QUESTION 10 -------------------
CREATE TABLE movie_ratings AS 
  SELECT movies.movieid as movieid, AVG(ratings.rating) as rating
    FROM ratings
    LEFT JOIN movies
    ON ratings.movieid = movies.movieid
    GROUP BY movies.movieid
    ORDER BY movies.movieid;

CREATE TABLE similarity_base AS
  SELECT 
    m1.movieid as movie1,
    m1.rating as rating1,
    m2.movieid as movie2,
    m2.rating as rating2
  FROM movie_ratings m1
  CROSS JOIN movie_ratings m2
  WHERE m1.movieid != m2.movieid;

CREATE TABLE similarities AS
  SELECT movie1 as movieid1, movie2 as movieid2, (1 - (ABS(rating1 - rating2) / 5)) as sim
  FROM similarity_base;

CREATE TABLE recommendation AS
  SELECT m.title as title
  FROM (
    SELECT movieid, (SUM(similarity) / SUM(dividend)) as probability
    FROM (
      SELECT sim.movieid1 as movieid, sim.sim as similarity, sim.sim * r.rating as dividend
      FROM similarities sim, ratings r
      WHERE r.userid = :v1 AND sim.movieid2 = r.movieid
      GROUP BY sim.movieid1, sim.sim, r.rating
      ORDER BY sim.movieid1
    ) AS recommendations_base
    GROUP BY movieid
  ) AS recommendations_movies, movies m
  WHERE probability > 0.3 and recommendations_movies.movieid = m.movieid;