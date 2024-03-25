/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 17 de marzo del 2024
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.math.BigDecimal;
import java.math.MathContext;

public class ServidorB {
    public static void main(String[] args) {
        int puerto = 80;
        int n = 0;

        try {
            ServerSocket servidor = new ServerSocket(puerto);
            System.out.println("Servidor B escuchando en el puerto " + puerto);

            while (true) {
                Socket cliente = servidor.accept();
                System.out.println(++n + " Conexion recibida: " + cliente.getInetAddress());

                Thread nuevoHilo = new Thread(new HiloCliente(cliente));
                nuevoHilo.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static class HiloCliente extends Thread {
        private Socket cliente;

        public HiloCliente(Socket cliente) {
            this.cliente = cliente;
        }

        @Override
        public void run() {
            try {
                clienteHilo(cliente);
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    cliente.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        private void clienteHilo(Socket clientSocket) throws IOException {
            OutputStream salida = clientSocket.getOutputStream();
            BigDecimal sumatorio = BigDecimal.ZERO, raiz2 = BigDecimal.valueOf(Math.sqrt(2));
            BigDecimal resultado1 = BigDecimal.ZERO, resultado2 = BigDecimal.ZERO, resultado3 = BigDecimal.ZERO;
            MathContext mathContext = new MathContext(100); // Precisión de 100 dígitos
            String respuesta = null;

            try {
                Thread hilo1 = new SolicitudServidorA("172.174.233.253", 8080, 0, 333);
                Thread hilo2 = new SolicitudServidorA("172.174.239.122", 8080, 334, 666);
                Thread hilo3 = new SolicitudServidorA("40.90.238.22", 8080, 667, 1000);

                hilo1.start();
                hilo2.start();
                hilo3.start();

                hilo1.join();
                hilo2.join();
                hilo3.join();

                resultado1 = ((SolicitudServidorA) hilo1).getResultado();
                resultado2 = ((SolicitudServidorA) hilo2).getResultado();
                resultado3 = ((SolicitudServidorA) hilo3).getResultado();

                sumatorio = sumatorio.add(resultado3).add(resultado2).add(resultado1);
                sumatorio = BigDecimal.ONE.divide(raiz2.multiply(BigDecimal.valueOf(9801)), mathContext).multiply(sumatorio);
                sumatorio = BigDecimal.ONE.divide(sumatorio, mathContext).divide(BigDecimal.valueOf(4),mathContext);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // Envía la suma al cliente
            respuesta = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" + sumatorio.toString();
            salida.write(respuesta.getBytes());
            salida.flush();
        }
    }

    static class SolicitudServidorA extends Thread {
        private String host;
        private int puerto;
        private int inicio;
        private int fin;
        private BigDecimal resultado;

        public SolicitudServidorA(String host, int puerto, int inicio, int fin) {
            this.host = host;
            this.puerto = puerto;
            this.inicio = inicio;
            this.fin = fin;
        }

        @Override
        public void run() {
            try {
                Socket socket = new Socket(host, puerto);
                enviarSolicitud(socket, inicio, fin);
                String respuesta = recibirRespuesta(socket);
                resultado = new BigDecimal(respuesta.trim());
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private void enviarSolicitud(Socket socket, int inicio, int fin) throws IOException {
            OutputStream salida = socket.getOutputStream();
            String solicitud = "GET /" + inicio + "," + fin + " HTTP/1.1\r\n" +
                             "Host: localhost\r\n" +
                             "Connection: close\r\n\r\n";
            salida.write(solicitud.getBytes());
            salida.flush();
        }

        private String recibirRespuesta(Socket socket) throws IOException {
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            StringBuilder respuesta = new StringBuilder();
            String linea;
            boolean finEncabezado = false;

            while ((linea = entrada.readLine()) != null) {
                if (!finEncabezado) {
                    if (linea.trim().isEmpty()) {
                        finEncabezado = true;
                    }
                } else {
                    respuesta.append(linea);
                }
            }

            return respuesta.toString();
        }

        public BigDecimal getResultado() {
            return resultado; 
        }
    }
}
