package cse511

object HotzoneUtils {
  def parseStringOfDoubles(pointString: String): Array[Double] = {
    val pointArray = pointString.split(",").map(_.toDouble)
    return pointArray
  }

  def ST_Contains(queryRectangle: String, pointString: String ): Boolean = {
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

  // YOU NEED TO CHANGE THIS PART

}
