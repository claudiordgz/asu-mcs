--test cases
--testcase1
INSERT INTO users (userid,name) VALUES (44444444,'User Test');
INSERT INTO genres (genreid,name) VALUES (9999,'Genre Test');
INSERT INTO movies (movieid,title) VALUES (99999999,'Movie Test');
INSERT INTO taginfo (tagid,content) VALUES (77777777,'Test');
INSERT INTO hasagenre (movieid,genreid) VALUES (99999999,9999);
INSERT INTO ratings (userid,movieid,rating,timestamp) VALUES (44444444,99999999,5.0,1);
INSERT INTO tags (userid,movieid,tagid,timestamp) VALUES (44444444,99999999,77777777,1);
​
--testcase2
INSERT INTO hasagenre (movieid, genreid) VALUES (88888888,9999);
​
--testcase3
INSERT INTO ratings (userid,movieid,rating,timestamp) VALUES (44444444,99999999,5.0,2);
​
--testcase4
INSERT INTO hasagenre (movieid,genreid) VALUES (99999999,4444);
​
--testcase5
INSERT INTO ratings (userid,movieid,rating,timestamp) VALUES (44444444,99999999,8.0,1);