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
            
            t1 = System.currentTimeMillis();
            for (int i = 0; i < 10000; i++) {
                entrada.readDouble();
            }
            t2 = System.currentTimeMillis();

            System.out.println("Se han enviado los 10000 doubles en " + (t2 - t1) + " milisegundos");

            servidor.close();
        } catch(IOException e) {
            System.err.println(e.getMessage());
        }
    }
}
