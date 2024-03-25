import java.io.DataInputStream;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLServerSocket;

public class Servidor {
    public static void main(String[] args) {
        System.setProperty("javax.net.ssl.trustStore", "./serverKeystore.jks");
        System.setProperty("javax.net.ssl.trustStorePassword", "1234567");
        SSLServerSocketFactory socket_factory = (SSLServerSocketFactory)SSLServerSocketFactory.getDefault();

        try {
            // Creación del socket del servidor
            SSLServerSocket servidor = (SSLServerSocket)socket_factory.createServerSocket(50000);
            //ServerSocket servidor = new ServerSocket(50000);

            // A la espera de una conexión
            SSLSocket conexion = (SSLSocket)servidor.accept();

            // Stream para recibir datos del cliente
            DataInputStream entrada = new DataInputStream(conexion.getInputStream());
                
            double x = entrada.readDouble();
            System.out.println(x);

            // Liberamos los recursos del sistema
            servidor.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
