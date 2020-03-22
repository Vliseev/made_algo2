package sol

import scala.collection.mutable.ArrayBuffer
import scala.util.Random

object Solution {

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
      if (from >= to) {
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
    if (l == 2 || l == 1)
      true

    var q = l - 1
    var p = 0L
    while (q != 0L && q % 2L == 0L) {
      p += 1
      q /= 2
    }

    val rng = new Random(123)

    @scala.annotation.tailrec
    def CalcLoop(p: Long, q: Long, rng: Random, n: Long, from: Int, to: Int): Boolean = {
      if (from >= to) {
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


  def F(x: BigInt, n: BigInt): BigInt = {
    (x * x + 1) % n
  }

  def PollardRho(a: BigInt, a2: BigInt, n: BigInt): Tuple2[BigInt, BigInt] = {
    val gcd = (a - a2).gcd(n)
    if (1 < gcd && gcd < n) {
      (gcd, n / gcd)
    } else {
      PollardRho(F(a, n), F(F(a, n), n), n)
    }
  }


  def factor(n: Long, rng: Random): Seq[Long] = {
    if (MRTest(n)) {
      Seq[Long](n)
    } else {
      var a = 0L
      while (a == 0L) {
        a = rng.nextLong()
        a = if (a > 0) a else -a
        a = a % n
      }
      val a2 = F(a, n)
      val (f1, f2) = PollardRho(a, a2, n)
      factor(f2.toLong, rng) ++ factor(f1.toLong, rng)
    }
  }

  def solution(inp: scala.io.Source): Unit = {
    val n = inp.getLines().next().toLong
    val rng = new Random(123)
    val res = factor(n, rng)
    res.sorted.foreach(x => print(s"$x "))

  }

  def Sieve(n: Int): (ArrayBuffer[Int], ArrayBuffer[Int]) = {
    var lp = ArrayBuffer.fill(n + 1)(0)
    var primes = ArrayBuffer(0)
    var i = 2
    while (i <= n) {
      if (lp(i) == 0) {
        lp(i) = i
        primes += i
      }

      var break = false
      var j = 0

      def comp(lp: ArrayBuffer[Int], primes: ArrayBuffer[Int], i: Int, n: Int, pos: Int): Unit = {
        if (!(pos >= primes.size || primes(pos) > lp(i) || i * primes(pos) >= n + 1)) {
          lp(i * primes(pos)) = primes(pos)
          comp(lp, primes, i, n, pos + 1)
        }
      }

      def comp1(lp: ArrayBuffer[Int], primes: ArrayBuffer[Int], i: Int, n: Int, pos: Int): Unit = {
        primes match {
          case head ::: tail => head
          case _ =>
        }
        //        if (!(pos >= primes.size || primes(pos) > lp(i) || i * primes(pos) >= n + 1)) {
        //          lp(i * primes(pos)) = primes(pos)
        //          comp(lp, primes, i, n, pos + 1)
        //        }
      }

      comp(lp, primes, i, n, 0)

      i += 1

    }


    (lp, primes)
  }


  def main(args: Array[String]): Unit = {
    Sieve(100)
    /*val inp_str = scala.io.Source.fromString("8")
    solution(inp_str)*/
  }
}
