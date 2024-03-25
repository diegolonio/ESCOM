import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Cliente
{
    public static void main(String[] args) {
        Socket conexion = null;

        while (true) {
            try {
                // Creación del socket del cliente
                conexion = new Socket("localhost", 50000);
                System.out.println("Conexión con el servidor exitosa.");

                // Stream para enviar datos al servidor
                DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());

                // Stream para recibir datos del servidor
                DataInputStream entrada = new DataInputStream(conexion.getInputStream());

                // Enviando un String
                salida.write("Diego Armando Apolonio Villegas".getBytes());
                
                // A la espera de la confirmación de recepción
                int confirmacion = 0;
                while (confirmacion != 202)
                    confirmacion = entrada.readInt();

                // Enviando 5 Doubles de manera eficiente
                ByteBuffer b = ByteBuffer.allocate(8*5);
                b.putDouble(1.0);
                b.putDouble(1.0);
                b.putDouble(2.0);
                b.putDouble(3.0);
                b.putDouble(5.0);
                byte[] a = b.array();
                salida.write(a);

                // Liberamos los recursos del sistema
                conexion.close();
                salida.close();
                entrada.close();
                break;
            } catch (Exception e1) {
                System.out.println("No se pudo conectar al servidor. Reintentando conexión.");

                try {
                    Thread.sleep(1000);
                } catch (Exception e2) {
                    System.out.println(e2);
                }
            }
        }
    }
}
