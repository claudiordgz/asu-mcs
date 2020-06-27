package cse511

import org.apache.log4j.{Level, Logger}
import org.apache.spark.sql.{DataFrame, SparkSession, Row, types}
import org.apache.spark.sql.functions.udf
import org.apache.spark.sql.functions.{desc}
import org.apache.spark.rdd.RDD
import org.apache.spark.sql.types.{DoubleType, StructType, IntegerType, StructField}

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

  def hotcellAnalysis(spark: SparkSession, pickupInfo: DataFrame): DataFrame = {
    val minX = -74.50/HotcellUtils.coordinateStep
    val maxX = -73.70/HotcellUtils.coordinateStep
    val minY = 40.50/HotcellUtils.coordinateStep
    val maxY = 40.90/HotcellUtils.coordinateStep
    val minZ = 1
    val maxZ = 31
    val numCells = (maxX - minX + 1)*(maxY - minY + 1)*(maxZ - minZ + 1)

    pickupInfo.createOrReplaceTempView("coordinates")
    spark.udf.register("ST_Within", HotcellUtils.stWithin)
    val cellsDf = spark.sql(s"""
      SELECT x, y, z, count(*) as points 
      FROM coordinates 
      WHERE ST_Within(${minX}, ${maxX},
                      ${minY}, ${maxY},
                      ${minZ}, ${maxZ},
                      coordinates.x, coordinates.y, coordinates.z)
      GROUP BY x,y,z
    """)

    // calculate Xbar
    val cellPointsRdd = cellsDf.select("points").rdd.map(r => r(0).asInstanceOf[Long])
    val cellsSum = cellPointsRdd.reduce(_+_)
    val xMean: Double = cellsSum / numCells
    // calculate S
    val cellsSumOfSquares = cellPointsRdd.map(math.pow(_, 2)).reduce(_+_)
    val s: Double  = math.sqrt((cellsSumOfSquares / numCells) - math.pow(xMean, 2))
    // calculate Getis-Ord
    val bounds = new HotcellUtils.Bounds(minX, maxX, minY, maxY, minZ, maxZ)
    // TODO: can this be optimized? nested rdd map throws errors
    val cellsArray = cellsDf.rdd.collect()
    val results: RDD[Row] = cellsDf.rdd.map(row => 
      HotcellUtils.calculateGetisOrd(cellsArray, bounds, row, numCells, s, xMean)
    )
    val columns = new StructType()
      .add(StructField("x", IntegerType))
      .add(StructField("y", IntegerType))
      .add(StructField("z", IntegerType))
      .add(StructField("zscore", DoubleType))
    val resultsDf = spark.createDataFrame(results, columns).orderBy(desc("zscore"))
    return resultsDf.coalesce(1)
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
    val results = hotcellAnalysis(spark, pickupInfo)

    return results // YOU NEED TO CHANGE THIS PART
  }
}
 