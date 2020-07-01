package cse511

import java.sql.Timestamp
import java.text.SimpleDateFormat
import java.util.Calendar

import org.apache.spark.sql.{DataFrame, SparkSession, Row, RowFactory}
import org.apache.spark.sql.functions.udf
import org.apache.spark.sql.functions.{desc}
import org.apache.spark.rdd.RDD

object HotcellUtils {

  val coordinateStep = 0.01

  def CalculateCoordinate(inputString: String, coordinateOffset: Int): Int =
  {
    // Configuration variable:
    // Coordinate step is the size of each cell on x and y
    var result = 0
    coordinateOffset match
    {
      case 0 => result = Math.floor((inputString.split(",")(0).replace("(","").toDouble/coordinateStep)).toInt
      case 1 => result = Math.floor(inputString.split(",")(1).replace(")","").toDouble/coordinateStep).toInt
      // We only consider the data from 2009 to 2012 inclusively, 4 years in total. Week 0 Day 0 is 2009-01-01
      case 2 => {
        val timestamp = HotcellUtils.timestampParser(inputString)
        result = HotcellUtils.dayOfMonth(timestamp) // Assume every month has 31 days
      }
    }
    return result
  }

  def timestampParser (timestampString: String): Timestamp =
  {
    val dateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss")
    val parsedDate = dateFormat.parse(timestampString)
    val timeStamp = new Timestamp(parsedDate.getTime)
    return timeStamp
  }

  def dayOfYear (timestamp: Timestamp): Int =
  {
    val calendar = Calendar.getInstance
    calendar.setTimeInMillis(timestamp.getTime)
    return calendar.get(Calendar.DAY_OF_YEAR)
  }

  def dayOfMonth (timestamp: Timestamp): Int =
  {
    val calendar = Calendar.getInstance
    calendar.setTimeInMillis(timestamp.getTime)
    return calendar.get(Calendar.DAY_OF_MONTH)
  }

  // We are already grabbing all the points inside some Bounds
  // Because of that
  // if it's not equal to any bounds, means it is inside the zone and it has 27 neighbors
  // 26 neighbors + the cell
  // if it's one one edge, then it has 18
  // two edges 12
  // three 8
  def _stNeighbors(minX: Int, maxX: Int, minY: Int, maxY: Int, minZ: Int, maxZ: Int, x: Int, y: Int, z:Int): Int = {
    val isOnEdgeX = if (x == minX || x == maxX) 1 else 0 
    val isOnEdgeY = if (y == minY || y == maxY) 1 else 0
    val isOnEdgeZ = if (z == minZ || z == maxZ) 1 else 0
    val totalEdges = isOnEdgeX + isOnEdgeZ + isOnEdgeY
    val neighbors = totalEdges match {
      case 1  => 18
      case 2  => 12
      case 3  => 8
      case _  => 27 
    }
    return neighbors
  }

  def stNeighbors = (minX: Int, maxX: Int, minY: Int, maxY: Int, minZ: Int, maxZ: Int, x: Int, y: Int, z:Int) 
    => _stNeighbors(minX, maxX, minY, maxY, minZ, maxZ, x, y, z)
  
  def _stWithin(minX: Double, maxX: Double, minY: Double, maxY: Double, minZ: Double, maxZ: Double, x: Double, y: Double, z:Double): Boolean = {
    if (x >= minX && x <= maxX &&
        y >= minY && y <= maxY &&
        z >= minZ && z <= maxZ) {
      return true
    }
    return false
  } 

  def stWithin = (minX: Double, maxX: Double, minY: Double, maxY: Double, minZ: Double, maxZ: Double, x: Double, y: Double, z:Double) 
    => _stWithin(minX, maxX, minY, maxY, minZ, maxZ, x, y, z)

  def isAdjacent = (i: Int, j: Int) => (i == j+1 || i == j || i == j-1)

  def isNeighbor(i: Row, j: Row): Boolean = {
    val xi = i.getAs[Int]("x")
    val yi = i.getAs[Int]("y")
    val zi = i.getAs[Int]("z")
    val xj = j.getAs[Int](0)
    val yj = j.getAs[Int](1)
    val zj = j.getAs[Int](2)
    return (isAdjacent(xi, xj) && isAdjacent(yi, yj) && isAdjacent(zi, zj))
  }

  def hotcellAnalysis(spark: SparkSession, pickupInfo: DataFrame, withZscores: Boolean = false): DataFrame = {
    import spark.implicits._

    val minX = -74.50/HotcellUtils.coordinateStep
    val maxX = -73.70/HotcellUtils.coordinateStep
    val minY = 40.50/HotcellUtils.coordinateStep
    val maxY = 40.90/HotcellUtils.coordinateStep
    val minZ = 1
    val maxZ = 31
    val numCells = (maxX - minX + 1)*(maxY - minY + 1)*(maxZ - minZ + 1)

    pickupInfo.createOrReplaceTempView("coordinates")
    spark.udf.register("ST_Within", HotcellUtils.stWithin)
    spark.udf.register("ST_Neighbors", HotcellUtils.stNeighbors)
    val cellsDf = spark.sql(s"""
      SELECT 
        x, y, z, count(*) as points,
        ST_Neighbors(${minX}, ${maxX},
                     ${minY}, ${maxY},
                     ${minZ}, ${maxZ}, x, y, z) as w
      FROM coordinates 
      WHERE ST_Within(${minX}, ${maxX},
                      ${minY}, ${maxY},
                      ${minZ}, ${maxZ},
                      coordinates.x, coordinates.y, coordinates.z)
      GROUP BY x,y,z
    """)

    // calculate Xbar
    val cellPointsRdd = cellsDf.select("points").rdd.map(r => r(0).asInstanceOf[Long].toDouble)
    val cellsSum = cellPointsRdd.reduce(_+_)
    val xMean = cellsSum / numCells
    // calculate S
    val cellsSumOfSquares = cellPointsRdd.map(math.pow(_, 2)).reduce(_+_)
    val s = math.sqrt((cellsSumOfSquares / numCells) - math.pow(xMean, 2))
    // TODO: can this be optimized? nested rdd map throws errors
    val cellsArray = cellsDf.rdd.collect()
    val results = cellsDf
      .map(row => {
        val pointTimesWeightSum = cellsArray
          .filter(cell => HotcellUtils.isNeighbor(row, cell))
          .map(cell => cell.getAs[Long]("points").toDouble)
          .reduce(_+_)

        val weightsSum = row.getAs[Int]("w").toDouble
        
        val numerator = pointTimesWeightSum - (xMean * weightsSum)
        val denominator = s * math.sqrt((numCells * weightsSum - math.pow(weightsSum, 2)) / (numCells - 1))
        val zscore = numerator / denominator
        (
          row.getAs[Int]("x"),
          row.getAs[Int]("y"),
          row.getAs[Int]("z"),
          zscore
        )
      })
      .toDF("x","y","z","zscore")
    results.createOrReplaceTempView("zscore")
    val withZscore = if (withZscores) ", zscore" else ""
    val resultsDf = spark.sql(s"""
      SELECT x,y,z ${withZscore}
      FROM zscore 
      ORDER BY zscore DESC, x DESC, y ASC, z DESC
    """)
    return resultsDf.coalesce(1)
  }
}
