import java.math.BigInteger;
import java.util.Random;
import java.util.Scanner;

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

            while (true) {
                b = rng.nextLong();
                b = b > 0 ? b : -b;
                b = b % l;
                if (b != 0)
                    break;
            }
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

    public static void main(String[] args) {


        var scanner = new Scanner(System.in);
        int n = scanner.nextInt();

        for (int i = 0; i < n; i++) {
            long val = scanner.nextLong();
            if (val == 1) {
                System.out.println("NO");
                continue;
            } else if (val == 2) {
                System.out.println("YES");
                continue;
            } else if (val % 2 == 0) {
                System.out.println("NO");
                continue;
            }

            var res = isPrimeMRTest(val);
            if (res) {
                System.out.println("YES");
            } else {
                System.out.println("NO");
            }
        }
    }
}