import java.io.DataInputStream;
import java.io.IOException;

public class Utilidades
{
    public static void read(DataInputStream entrada, byte[] buffer, int posicion, int longitud) throws IOException {
        int n = 0;

        while (longitud > 0) {
            n = entrada.read(buffer, posicion, longitud);
            posicion = posicion + n + 1;
            longitud = longitud - n - 1;
        }
    }
}
