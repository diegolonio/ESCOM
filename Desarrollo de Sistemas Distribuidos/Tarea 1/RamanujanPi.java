import java.math.BigDecimal;
import java.math.MathContext;

public class RamanujanPi {
    public static void main(String[] args) {
        long t1 = System.currentTimeMillis();
        BigDecimal pi = calculatePi(0, 15);
        long t2 = System.currentTimeMillis();
        System.out.println("Valor aproximado de PI: " + pi);
        System.out.println(t2-t1);
    }

    public static BigDecimal calculatePi(int numero1, int numero2) {
        BigDecimal sum = BigDecimal.ZERO;
        BigDecimal sqrt2 = BigDecimal.valueOf(Math.sqrt(2));
        BigDecimal a, b;

        MathContext mc = new MathContext(100); // Precisión de 100 dígitos
        BigDecimal term;

        for (int k = numero1; k < numero2; k++) {
            a = factorial(4 * k).multiply(BigDecimal.valueOf(1103 + 26390 * k));
            b = factorial(k).pow(4).multiply(BigDecimal.valueOf(396).pow(4 * k));
            term = a.divide(b, mc);
            sum = sum.add(term);
        }

        BigDecimal result = BigDecimal.ONE.divide(sqrt2.multiply(BigDecimal.valueOf(9801)), mc).multiply(sum);
        return BigDecimal.ONE.divide(result, mc).divide(BigDecimal.valueOf(4),mc);
    }

    public static BigDecimal factorial(int n) {
        BigDecimal result = BigDecimal.ONE;
        for (int i = 2; i <= n; i++) {
            result = result.multiply(BigDecimal.valueOf(i));
        }
        return result;
    }
}