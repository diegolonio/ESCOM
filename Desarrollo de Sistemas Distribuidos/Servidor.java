import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Servidor {
    public static void main(String[] args) {
        try {
            // Creación del socket del servidor
            ServerSocket servidor = new ServerSocket(50000);

            // A la espera de una conexión
            Socket conexion = servidor.accept();

            // Stream para enviar datos al cliente
            DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());

            // Stream para recibir datos del cliente
            DataInputStream entrada = new DataInputStream(conexion.getInputStream());
                
            // Recibiendo un String
            byte[] buffer = new byte[32];
            Utilidades.read(entrada, buffer, 0, 32);
            System.out.println(new String(buffer, "UTF-8"));

            // Confirmación de recepción
            salida.writeInt(202);
            
            // Recibiendo 5 Doubles de manera eficiente
            byte[] a = new byte[8*5];
            Utilidades.read(entrada, a, 0, 8*5);
            ByteBuffer b = ByteBuffer.wrap(a);
            for (int i = 0; i < 5; i++)
                System.out.println(b.getDouble());

            // Liberamos los recursos del sistema
            salida.close();
            entrada.close();
            servidor.close();
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}
