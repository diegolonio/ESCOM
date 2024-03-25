import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.nio.ByteBuffer;

public class Servidor {
    static class Worker extends Thread {
        Socket conexion;

        Worker (Socket conexion) {
            this.conexion = conexion;
        }

        public void run() {
            try {
                DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());
                DataInputStream entrada = new DataInputStream(conexion.getInputStream());

                byte[] buffer = new byte[32];
                read(entrada, buffer, 0, 32);
                System.out.println(new String(buffer, "UTF-8"));

                salida.writeInt(202);
            
                byte[] a = new byte[8*5];
                read(entrada, a, 0, 8*5);
                ByteBuffer b = ByteBuffer.wrap(a);

                for (int i = 0; i < 5; i++)
                    System.out.println(b.getDouble());

                try {
                    Thread.sleep(10000);
                    System.out.println("Hecho está.");
                } catch (Exception e3) {
                    System.err.println(e3.getMessage());
                }

            } catch (Exception e1) {
                System.err.println(e1.getMessage());
            } finally {
                try {
                    conexion.close();
                } catch (Exception e2) {
                    System.err.println(e2.getMessage());
                }
            }
        }
    }

    public static void main(String[] args) throws Exception{
        ServerSocket servidor = new ServerSocket(50000);

        while (true) {
            Socket conexion = servidor.accept();
            Worker hilo = new Worker(conexion);
            hilo.start();            
        }
    }

    public static void read(DataInputStream entrada, byte[] buffer, int posicion, int longitud) throws Exception {
        int n = 0;

        while (longitud > 0) {
            n = entrada.read(buffer, posicion, longitud);
            posicion = posicion + n + 1;
            longitud = longitud - n - 1;
        }
    }
}
