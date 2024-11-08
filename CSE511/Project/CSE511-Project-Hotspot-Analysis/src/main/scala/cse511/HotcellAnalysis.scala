package cse511

import org.apache.log4j.{Level, Logger}
import org.apache.spark.sql.{DataFrame, SparkSession, Row, types}
import org.apache.spark.sql.functions.udf
import org.apache.spark.rdd.RDD


object HotcellAnalysis {
  Logger.getLogger("org.spark_project").setLevel(Level.WARN)
  Logger.getLogger("org.apache").setLevel(Level.WARN)
  Logger.getLogger("akka").setLevel(Level.WARN)
  Logger.getLogger("com").setLevel(Level.WARN)

  def loadData(spark: SparkSession, pointPath: String): DataFrame = {
    var pickupInfo = spark.read.format("com.databricks.spark.csv").option("delimiter",";").option("header","false").load(pointPath)
    return pickupInfo
  }

  def assignCellCoordinatesBasedOnPickupPoints(spark: SparkSession): DataFrame = {
    spark.udf.register("CalculateX",(pickupPoint: String)=>((
      HotcellUtils.CalculateCoordinate(pickupPoint, 0)
      )))
    spark.udf.register("CalculateY",(pickupPoint: String)=>((
      HotcellUtils.CalculateCoordinate(pickupPoint, 1)
      )))
    spark.udf.register("CalculateZ",(pickupTime: String)=>((
      HotcellUtils.CalculateCoordinate(pickupTime, 2)
      )))
    var pickupInfo = spark.sql("select CalculateX(nyctaxitrips._c5),CalculateY(nyctaxitrips._c5), CalculateZ(nyctaxitrips._c1) from nyctaxitrips")
    var newCoordinateName = Seq("x", "y", "z")
    return pickupInfo.toDF(newCoordinateName:_*)
  }

  def loadDataSilently(spark: SparkSession, pointPath: String): DataFrame  = {
    var pickupInfo = loadData(spark, pointPath)
    pickupInfo.createOrReplaceTempView("nyctaxitrips")

    // Assign cell coordinates based on pickup points
    return assignCellCoordinatesBasedOnPickupPoints(spark)
  }

  def runHotcellAnalysis(spark: SparkSession, pointPath: String): DataFrame =
  {
    // Load the original data from a data source
    var pickupInfo = loadData(spark, pointPath)
    pickupInfo.createOrReplaceTempView("nyctaxitrips")
    pickupInfo.show()

    // Assign cell coordinates based on pickup points
    pickupInfo = assignCellCoordinatesBasedOnPickupPoints(spark)
    pickupInfo.show()

    // Define the min and max of x, y, z
    val results = HotcellUtils.hotcellAnalysis(spark, pickupInfo)

    return results
  }
}
 