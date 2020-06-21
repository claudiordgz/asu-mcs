import org.specs2.mutable.Specification

object SpatialQuerySpec extends Specification {
  "stContains" should {
    "check the queryRectangle does not contain point" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-88.331492,32.324142"
      val result = cse512.SpatialQuery.stContains(rectangle, point)
      result mustEqual false
    }
    "check inside" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.4560,33.15489"
      val result = cse512.SpatialQuery.stContains(rectangle, point)
      result mustEqual true
    }
    "check on-boundary" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.63173,33.0183"
      val result = cse512.SpatialQuery.stContains(rectangle, point)
      result mustEqual true
    }
    "check Y contains but not X" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.73173,33.0183"
      val result = cse512.SpatialQuery.stContains(rectangle, point)
      result mustEqual false
    }
    "check X contains but not Y" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.73173,33.1183"
      val result = cse512.SpatialQuery.stContains(rectangle, point)
      result mustEqual false
    }
  }
  "stWithin" should {
    "check whether the two points are within the given distance" in {
      val point1 = "-88.388954,32.357073"
      val point2 = "-88.331492,32.324142"
      val distance = 1
      val result = cse512.SpatialQuery.stWithin(point1, point2, distance)
      result mustEqual true
    }
    "check on-boundary" in {
      val point1 = "-88.388954,32.357073"
      val point2 = "-88.388954,32.357073"
      val distance = 1
      val result = cse512.SpatialQuery.stWithin(point1, point2, distance)
      result mustEqual true
    }
  }
}