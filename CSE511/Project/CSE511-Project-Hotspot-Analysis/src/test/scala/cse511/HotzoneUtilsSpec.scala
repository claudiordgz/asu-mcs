import org.specs2.mutable.Specification

object HotzoneUtilsSpec extends Specification {
  "ST_Contains" should {
    "check the queryRectangle does not contain point" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-88.331492,32.324142"
      val result = cse511.HotzoneUtils.ST_Contains(rectangle, point)
      result mustEqual false
    }
    "check inside" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.4560,33.15489"
      val result = cse511.HotzoneUtils.ST_Contains(rectangle, point)
      result mustEqual true
    }
    "check on-boundary" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.63173,33.0183"
      val result = cse511.HotzoneUtils.ST_Contains(rectangle, point)
      result mustEqual true
    }
    "check Y contains but not X" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.73173,33.0183"
      val result = cse511.HotzoneUtils.ST_Contains(rectangle, point)
      result mustEqual false
    }
    "check X contains but not Y" in {
      val rectangle = "-93.63173,33.0183,-93.359203,33.219456"
      val point = "-93.73173,33.1183"
      val result = cse511.HotzoneUtils.ST_Contains(rectangle, point)
      result mustEqual false
    }
  }
}