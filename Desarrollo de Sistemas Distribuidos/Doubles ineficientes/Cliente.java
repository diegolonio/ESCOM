import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class Cliente {
    public static void main(String[] args) {
        try {
            Socket conexion = new Socket("localhost", 50000);
            DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());

            for (int i = 0; i < 10000; i++) {
                salida.writeDouble(i);
            }

            conexion.close();
        } catch(IOException e) {
            System.err.println(e.getMessage());
        }
    }
}
