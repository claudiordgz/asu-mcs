package cse511

import org.apache.spark.sql.SparkSession

object SpatialQuery extends App{
  def parseStringOfDoubles(pointString: String): Array[Double] = {
    val pointArray = pointString.split(",").map(_.toDouble)
    return pointArray
  }

  def _stContains(queryRectangle:String, pointString:String): Boolean  = {
    val queryRectangleArray = parseStringOfDoubles(queryRectangle)
    val xMin = queryRectangleArray(0)
    val yMin = queryRectangleArray(1)
    val xMax = queryRectangleArray(2)
    val yMax = queryRectangleArray(3)
    val p = parseStringOfDoubles(pointString)
    if (p(0) < xMin || p(0) > xMax || p(1) < yMin || p(1) > yMax) {
      return false
    }
    return true
  }

  def _stWithin(pointString1:String, pointString2:String, distance:Double): Boolean  = {
    val p1 = parseStringOfDoubles(pointString1)
    val p2 = parseStringOfDoubles(pointString2)
    val distanceBetweenP1AndP2 = math.sqrt( math.pow(p1(0) - p2(0), 2) +  math.pow(p1(1) - p2(1), 2) )
    return (distance > distanceBetweenP1AndP2)
  }

  def stContains = (queryRectangle:String, pointString:String) => _stContains(queryRectangle, pointString)
  def stWithin = (pointString1:String, pointString2:String, distance:Double) => _stWithin(pointString1, pointString2, distance)

  def runRangeQuery(spark: SparkSession, arg1: String, arg2: String): Long = {

    val pointDf = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg1);
    pointDf.createOrReplaceTempView("point")

    // YOU NEED TO FILL IN THIS USER DEFINED FUNCTION
    spark.udf.register("ST_Contains",stContains)

    val resultDf = spark.sql("select * from point where ST_Contains('"+arg2+"',point._c0)")
    resultDf.show()

    return resultDf.count()
  }

  def runRangeJoinQuery(spark: SparkSession, arg1: String, arg2: String): Long = {

    val pointDf = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg1);
    pointDf.createOrReplaceTempView("point")

    val rectangleDf = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg2);
    rectangleDf.createOrReplaceTempView("rectangle")

    // YOU NEED TO FILL IN THIS USER DEFINED FUNCTION
    spark.udf.register("ST_Contains",stContains)

    val resultDf = spark.sql("select * from rectangle,point where ST_Contains(rectangle._c0,point._c0)")
    resultDf.show()

    return resultDf.count()
  }

  def runDistanceQuery(spark: SparkSession, arg1: String, arg2: String, arg3: String): Long = {

    val pointDf = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg1);
    pointDf.createOrReplaceTempView("point")

    // YOU NEED TO FILL IN THIS USER DEFINED FUNCTION
    spark.udf.register("ST_Within",stWithin)

    val resultDf = spark.sql("select * from point where ST_Within(point._c0,'"+arg2+"',"+arg3+")")
    resultDf.show()

    return resultDf.count()
  }

  def runDistanceJoinQuery(spark: SparkSession, arg1: String, arg2: String, arg3: String): Long = {

    val pointDf = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg1);
    pointDf.createOrReplaceTempView("point1")

    val pointDf2 = spark.read.format("com.databricks.spark.csv").option("delimiter","\t").option("header","false").load(arg2);
    pointDf2.createOrReplaceTempView("point2")

    // YOU NEED TO FILL IN THIS USER DEFINED FUNCTION
    spark.udf.register("ST_Within",stWithin)
    val resultDf = spark.sql("select * from point1 p1, point2 p2 where ST_Within(p1._c0, p2._c0, "+arg3+")")
    resultDf.show()

    return resultDf.count()
  }
}
