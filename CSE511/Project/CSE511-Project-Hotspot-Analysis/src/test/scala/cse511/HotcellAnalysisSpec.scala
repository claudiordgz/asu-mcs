import org.specs2.mutable.Specification
import org.apache.spark.sql.{DataFrame, SparkSession}
import java.nio.file.Paths

trait SparkSessionTestWrapper {
  lazy val spark: SparkSession = {
    SparkSession.builder().master("local").appName("testHotcell").getOrCreate()
  }
}

object HotcellAnalysisSpec extends Specification with SparkSessionTestWrapper  {
  import spark.implicits._

  def afterAll = spark.close()

  "HotcellAnalysis" should {
    "printSomething" in {
      val df = cse511.HotcellAnalysis.loadDataSilently(spark, "src/resources/yellow_trip_sample_100000.csv")
      cse511.HotcellAnalysis.hotcellAnalysis(spark, df)
      val rectangle = 1000
      rectangle mustEqual 1000
    } 
  }
}
