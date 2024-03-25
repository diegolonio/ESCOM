import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class Servidor {
    public static void main(String[] args) {
        try {
            ServerSocket servidor = new ServerSocket(50000);
            Socket conexion = servidor.accept();
            DataInputStream entrada = new DataInputStream(conexion.getInputStream());

            long t1 = 0, t2 = 0;
            byte[] a = new byte[8*10000];
            
            t1 = System.currentTimeMillis();
            read(entrada, a, 0, 8*10000);
            t2 = System.currentTimeMillis();

            System.out.println("Se han enviado los 10000 doubles en " + (t2 - t1) + " milisegundos");

            servidor.close();
        } catch(IOException e) {
            System.err.println(e.getMessage());
        }
    }

    public static void read(DataInputStream entrada, byte[] buffer, int posicion, int longitud) throws IOException {
        int n = 0;

        while (longitud > 0) {
            n = entrada.read(buffer, posicion, longitud);
            posicion = posicion + n + 1;
            longitud = longitud - n - 1;
        }
    }
}
