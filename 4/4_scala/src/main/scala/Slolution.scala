import scala.util.Random


object Slolution {

  @scala.annotation.tailrec
  def GCD(x: Long, y: Long): Long = {
    if (y > 0)
      GCD(y, x % y)
    else
      x
  }

  def isPrimeMR(p: Long, q: BigInt, n: BigInt, b: BigInt): Boolean = {

    @scala.annotation.tailrec
    def CalcLoop(rem: BigInt, n_1: BigInt, n: BigInt, from: Long, to: Long): Boolean = {
      if (from == to) {
        false
      } else if (rem == n_1) {
        true
      } else {
        CalcLoop((rem * rem) % n, n_1, n, from + 1, to)
      }
    }

    val rem = b.modPow(q, n)
    if (rem == 1 || rem == n - 1) {
      true
    } else {
      CalcLoop((rem * rem) % n, n - 1, n, 1, p)
    }
  }

  def MRTest(l: Long): Boolean = {
    var q = l - 1
    var p = 0L
    while (q != 0L && q % 2L == 0L) {
      p += 1
      q /= 2
    }

    val rng = new Random(123)

    @scala.annotation.tailrec
    def CalcLoop(p: Long, q: Long, rng: Random, n: Long, from: Int, to: Int): Boolean = {
      if (from == to) {
        true
      } else {
        var b = 0L
        while (b == 0L) {
          b = rng.nextLong()
          b = if (b > 0) b else -b
          b = b % l
        }
        if (GCD(b, n) != 1) {
          false
        } else if (!isPrimeMR(p, q, l, b)) {
          false
        } else {
          CalcLoop(p, q, rng, n, from + 1, to)
        }
      }
    }

    CalcLoop(p, q, rng, l, 1, 10)
  }

  def solution(inp: scala.io.Source): Unit = {
    val lines_it = inp.getLines()

    val n: Int = lines_it.next().toInt
    for (i <- 1 to n) {
      val cur_n = lines_it.next().toLong
      if (cur_n == 1) {
        println("NO")
      } else if (cur_n == 2) {
        println("YES")
      } else if (cur_n % 2 == 0) {
        println("NO")
      } else {
        val res = MRTest(cur_n)
        if (res) {
          println("YES")
        } else {
          println("NO")
        }
      }

    }

  }

  def main(args: Array[String]): Unit = {
    val inp_str = scala.io.Source.fromInputStream(System.in)
    solution(inp_str)
  }
}