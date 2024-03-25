import java.io.DataOutputStream;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

public class Cliente
{
    public static void main(String[] args) {
        System.setProperty("javax.net.ssl.trustStore", "./clientTruststore.jks");
        System.setProperty("javax.net.ssl.trustStorePassword", "123456");
        SSLSocketFactory cliente = (SSLSocketFactory)SSLSocketFactory.getDefault();
        SSLSocket conexion = null;

        try {
            // Creación del socket del cliente
            conexion = (SSLSocket)cliente.createSocket("localhost", 50000);
            //conexion = new Socket("localhost", 50000);

            System.out.println("Conexión con el servidor exitosa.");

            // Stream para enviar datos al servidor
            DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());

            salida.writeDouble(2.718281828);

            try {
                Thread.sleep(1000);
            } catch (Exception e2) {
                System.out.println(e2);
            }

            // Liberamos los recursos del sistema
            conexion.close();
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
