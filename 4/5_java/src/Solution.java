import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class Solution {

    private static long GCD(long x, long y) {
        long tmp;
        while (y > 0) {
            tmp = x;
            x = y;
            y = tmp % y;
        }
        return x;
    }

    private static boolean isPrimeMRTestIt(int p, BigInteger q, BigInteger n, BigInteger b) {
        var n1 = n.subtract(BigInteger.ONE);
        var rem = b.modPow(q, n);

        if (rem.equals(BigInteger.ONE) || rem.equals(n1)) {
            return true;
        }
        for (int i = 0; i < p; ++i) {
            rem = rem.multiply(rem);
            rem = rem.mod(n);
            if (rem.equals(n1))
                return true;
        }
        return false;
    }


    private static boolean isPrimeMRTest(long l) {
        var bigN = new BigInteger(Long.toString(l));
        var q = l - 1;
        int p = 0;

        while (q != 0 && q % 2 == 0) {
            p++;
            q /= 2;
        }

        var rng = new Random(123);
        var bigQ = new BigInteger(Long.toString(q));

        for (int i = 0; i < 10; i++) {
            long b;

            do {
                b = rng.nextLong();
                b = b > 0 ? b : -b;
                b = b % l;
            } while (b == 0);
            var bigB = new BigInteger(Long.toString(b));
            if (GCD(b, l) != 1) {
                return false;
            }
            if (!isPrimeMRTestIt(p, bigQ, bigN, bigB)) {
                return false;
            }
        }

        return true;
    }

    /*
    * void sieve(int n, vi& lp, vi& primes) {
    lp.resize(n+1);
    for (int i = 2; i <= n; i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > lp[i] || i * p >= n + 1) {
                break;
            }
            lp[i * p] = p;
        }
    }
}*/

    void Sieve(long n, ArrayList<Long> lp, ArrayList<Long> primes) {
        lp.set(0, 1L);
    }


    public static void main(String[] args) {


       /* var scanner = new Scanner(System.in);
        int n = scanner.nextInt();
*/

        var lp = new ArrayList<Long>(100);
        System.out.println(lp.size());

    }
}