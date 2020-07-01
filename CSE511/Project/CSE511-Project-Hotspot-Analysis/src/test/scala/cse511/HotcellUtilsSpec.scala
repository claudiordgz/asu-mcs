import org.specs2.mutable.Specification
import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.SaveMode

object HotcellUtilsSpec extends Specification {
  val minX = -74.50/cse511.HotcellUtils.coordinateStep
  val maxX = -73.70/cse511.HotcellUtils.coordinateStep
  val minY = 40.50/cse511.HotcellUtils.coordinateStep
  val maxY = 40.90/cse511.HotcellUtils.coordinateStep
  val minZ = 1
  val maxZ = 31

  "stWithin" should {
    "check that it falls inside points" in {
      var x = -7399
      var y = 4075
      var z = 25
      var distance = 1
      var result = cse511.HotcellUtils.stWithin(
        minX.toLong, maxX.toLong,
        minY.toLong, maxY.toLong,
        minZ.toLong, maxZ.toLong,
        x, y, z)
      result mustEqual true

      x = -7406
      y = 4076
      z = 13
      distance = 1
      result = cse511.HotcellUtils.stWithin(
        minX.toLong, maxX.toLong,
        minY.toLong, maxY.toLong,
        minZ.toLong, maxZ.toLong,
        x, y, z)
      result mustEqual true
    }
  }

  "hotcellAnalysis" should {
    "check the results dataframe" in {
      val spark = SparkSession.builder().master("local").appName("testHotcell").getOrCreate()
      val df = cse511.HotcellAnalysis.loadDataSilently(spark, "src/resources/yellow_trip_sample_100000.csv")
      val results = cse511.HotcellUtils.hotcellAnalysis(spark, df, true)
      results.limit(50).show()
      true mustEqual true
    }
  }
}