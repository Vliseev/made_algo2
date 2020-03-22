import scala.collection.mutable

object Soltion {
  /*def binPowMod(a: Long, b: Long, n: Long): Long = {
    var bigA = BigInt(a)
    val bigN = BigInt(n)
    var mut_b = b
    var res: BigInt = 1

    while (mut_b > 0) {
      if ((mut_b & 1) == 1L) {
        res = (res * bigA) % bigN
        mut_b -= 1
      } else {
        bigA = (bigA * bigA) % bigN
        mut_b = mut_b >> 1
      }
    }
    (res % bigN).toLong
  }

  def findFactors(n: Long): ArrayBuffer[Long] = {
    var result = ArrayBuffer[Long]()
    var tmp_n = n
    var i: Long = 2
    while (i * i < n) {
      if ((tmp_n % i) == 0) {
        result += i
        tmp_n /= i
      }
      while ((tmp_n % i) == 0) {
        result += i
        tmp_n /= i
      }
      i += 1
    }
    if (tmp_n != 1) {
      result += tmp_n
    }
    result
  }*/

  def solution(inp: scala.io.Source): Unit = {
    val line = inp.getLines().next()
    val inp_arr = line.split(" ").map(_.toLong)

    val (a, b, n) = inp_arr match {
      case Array(a: Long, b: Long, c: Long, _*) => (a, b, c)
    }
    val bigA: BigInt = a
    val q = n - 1

    val k: Long = (math.sqrt(n).toLong + 1) % q
    var r = q - k

    val a1 = bigA.modPow(r, n)
    var key: BigInt = b

    val table = mutable.HashMap[Long, Long]()
    table(key.toLong) = 0

    for (i: Long <- 1L to k) {
      key = (key * a1) % n
      table(key.toLong) = i
    }

    key = 1
    for (i: Long <- 0L to k + 1) {
      if (table.contains(key.toLong)) {
        println((k * table(key.toLong) + i) % q)
        return
      }
      key = (key * bigA) % n
    }
    println(-1)
  }


  def main(args: Array[String]): Unit = {
    val inp_str = scala.io.Source.fromInputStream(System.in)
    solution(inp_str)

  }
}