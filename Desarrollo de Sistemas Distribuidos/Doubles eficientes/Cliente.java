import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Cliente {
    public static void main(String[] args) {
        try {
            Socket conexion = new Socket("localhost", 50000);
            DataOutputStream salida = new DataOutputStream(conexion.getOutputStream());

            ByteBuffer b = ByteBuffer.allocate(8*10000);

            for (int i = 0; i < 10000; i++) {
                b.putDouble(i);
            }

            byte[] a = b.array();

            salida.write(a);

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e2) {
                System.err.println(e2.getMessage());
            }

            conexion.close();
        } catch(IOException e) {
            System.err.println(e.getMessage());
        }
    }
}
