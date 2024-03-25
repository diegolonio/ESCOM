import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class Servidor {
    static class Worker extends Thread {
        Socket conexion;

        Worker(Socket conexion) {
            this.conexion = conexion;
        }

        public void run() {
            try {
                System.out.println(conexion.getRemoteSocketAddress().toString());

                BufferedReader entrada = new BufferedReader(new InputStreamReader(conexion.getInputStream()));
                PrintWriter salida = new PrintWriter(conexion.getOutputStream());

                String req = entrada.readLine();
                System.out.println(req);

                while (true) {
                    String encabezado = entrada.readLine();
                    System.out.println(encabezado);

                    if (encabezado.equals("")) {
                        break;
                    }
                }

                if (req.startsWith("GET /hola ")) {
                    String contenido = "<html><button onclick='alert(\"Se presionó el botón\")'>Aceptar</button></html>";
                    salida.println("HTTP/1.1 200 OK");
                    salida.println("Content-Type: text/html; charset=utf-8");
                    salida.println("Content-Length: " + contenido.length());
                    salida.println("Server: ServidorHTTP.java");
                    salida.println("Date: " + new Date());
                    salida.println();
                    salida.println(contenido);
                    salida.flush();
                } else {
                    salida.println("HTTP/1.1 404 File Not Found");
                    salida.flush();
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

    public static void main(String[] args) throws Exception {
        ServerSocket servidor = new ServerSocket(80);

        while (true) {
            Socket conexion = servidor.accept();
            Worker hilo = new Worker(conexion);
            hilo.start();
        }        
    }
}
