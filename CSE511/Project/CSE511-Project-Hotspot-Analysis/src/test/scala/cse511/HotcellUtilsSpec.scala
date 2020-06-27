import org.specs2.mutable.Specification

object HotcellUtilsSpec extends Specification {
  val minX = -74.50/cse511.HotcellUtils.coordinateStep
  val maxX = -73.70/cse511.HotcellUtils.coordinateStep
  val minY = 40.50/cse511.HotcellUtils.coordinateStep
  val maxY = 40.90/cse511.HotcellUtils.coordinateStep
  val minZ = 1
  val maxZ = 31

  "stWithin" should {
    "check that it falls inside points" in {
      val x = -7399
      val y = 4075
      val z = 25
      val distance = 1
      val result = cse511.HotcellUtils.stWithin(
        minX.toLong, maxX.toLong,
        minY.toLong, maxY.toLong,
        minZ.toLong, maxZ.toLong,
        x, y, z)
      result mustEqual true
    }
    "check that it falls inside points" in {
      val x = -7406
      val y = 4076
      val z = 13

      val distance = 1
       val result = cse511.HotcellUtils.stWithin(
        minX.toLong, maxX.toLong,
        minY.toLong, maxY.toLong,
        minZ.toLong, maxZ.toLong,
        x, y, z)
      result mustEqual true
    }
  }
}