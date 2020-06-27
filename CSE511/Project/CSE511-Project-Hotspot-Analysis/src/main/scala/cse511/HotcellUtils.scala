package cse511

import java.sql.Timestamp
import java.text.SimpleDateFormat
import java.util.Calendar

import org.apache.spark.sql.{DataFrame, Row, RowFactory}
import org.apache.spark.rdd.RDD

object HotcellUtils {
  class Bounds(_minX: Double, _maxX: Double, _minY: Double, _maxY: Double, _minZ: Double, _maxZ: Double) extends Serializable {
  var minX: Double = _minX
  var maxX: Double = _maxX
  var minY: Double = _minY
  var maxY: Double = _maxY
  var minZ: Double = _minZ
  var maxZ: Double = _maxZ
}

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

  def calculateSpatialWeight(i: Row, j: Row): Int = {
    val xi = i.getAs[Int]("x")
    val yi = i.getAs[Int]("y")
    val zi = i.getAs[Int]("z")
    val xj = j.getAs[Int](0)
    val yj = j.getAs[Int](1)
    val zj = j.getAs[Int](2)
    if ((xj == xi+1 || xj == xi-1) && yi == yj && zi == zj) {
      return 1
    }
    if ((yj == yi+1 || yj == yi-1) && xi == xj && zi == zj) {
      return 1
    }
    if ((zj == zi+1 || zj == zi-1) && yi == yj && xi == xj) {
      return 1
    }
    return 0
  }

  def calculateGetisOrd(cellsArray: Array[Row], bounds: Bounds, currentCell: Row, n: Double, S: Double, xbar: Double): Row = { 
    val pointTimesWeightSum = cellsArray.map(cell => {
      val points = cell.getAs[Long]("points")
      val weight = calculateSpatialWeight(currentCell, cell)
      (points * weight)
    }).reduce(_+_)
    // TODO: FIX THE WEIGHTSSUM
    val weightsSum = cellsArray.map(cell => calculateSpatialWeight(currentCell, cell)).reduce(_+_)
    
    val numerator = pointTimesWeightSum - (xbar * weightsSum)
    // no need to do each weight square because they are 1 or 0
    val denominator: Double = S * math.sqrt((n * weightsSum - math.pow(weightsSum, 2)) / (n - 1))
    val zscore: java.lang.Double = numerator / denominator

    val x: java.lang.Integer = currentCell.getAs[java.lang.Integer]("x")
    val y: java.lang.Integer = currentCell.getAs[java.lang.Integer]("y")
    val z: java.lang.Integer = currentCell.getAs[java.lang.Integer]("z")

    return RowFactory.create(x, y, z, zscore)
  }
}
