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

public class ServidorA {
    public static void main(String[] args) throws IOException {
        int puerto = 8080;
        int n = 0;
        
        ServerSocket servidor = new ServerSocket(puerto);
        System.out.println("Servidor A escuchando en el puerto " + puerto);
        
        while (true) {
            Socket cliente = servidor.accept();
            System.out.println(++n + " Conexión recibida: " + cliente.getInetAddress());
            
            Thread nuevoHilo = new Thread(new HiloCliente(cliente)); 
            nuevoHilo.start();
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
                hiloCliente(cliente);
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
    }

    private static void hiloCliente(Socket cliente) throws IOException {
        BufferedReader entrada = new BufferedReader(new InputStreamReader(cliente.getInputStream()));
        OutputStream salida = cliente.getOutputStream();
        String solicitudRecibida = entrada.readLine(), uri = null, respuesta = null;
        String[] arregloSolicitudRecibida = solicitudRecibida.split(" "), arregloUri = null, intervalo = null;
        BigDecimal pi = BigDecimal.ZERO;

        if (arregloSolicitudRecibida.length < 2 || !arregloSolicitudRecibida[0].equals("GET")) {
            enviarRespuesta(salida, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }
        
        uri = arregloSolicitudRecibida[1];
        arregloUri = uri.split("/");

        if (arregloUri.length < 2) {
            enviarRespuesta(salida, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }
        
        intervalo = arregloUri[1].split(",");

        if (intervalo.length != 2) {
            enviarRespuesta(salida, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }
        
        int numero1, numero2;

        try {
            numero1 = Integer.parseInt(intervalo[0]);
            numero2 = Integer.parseInt(intervalo[1]);
        } catch (NumberFormatException e) {
            enviarRespuesta(salida, "HTTP/1.1 400 Bad Request\r\n\r\n");
            return;
        }
        
        pi = calcularPi(numero1, numero2);
        respuesta = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" + pi.toString();
        enviarRespuesta(salida, respuesta);
    }
    
    private static BigDecimal calcularPi(int numero1, int numero2) {
        BigDecimal sumatorio = BigDecimal.ZERO, a = BigDecimal.ZERO, b = BigDecimal.ZERO, termino = BigDecimal.ZERO;
        MathContext mathContext = new MathContext(100); // Precisión de 100 dígitos

        for (int k = numero1; k < numero2; k++) {
            a = factorial(4 * k).multiply(BigDecimal.valueOf(1103 + 26390 * k));
            b = factorial(k).pow(4).multiply(BigDecimal.valueOf(396).pow(4 * k));
            termino = a.divide(b, mathContext);
            sumatorio = sumatorio.add(termino);
        }

        return sumatorio;
    }
    
    private static BigDecimal factorial(int n) {
        BigDecimal resultado = BigDecimal.ONE;

        for (int i = 2; i <= n; i++) {
            resultado = resultado.multiply(new BigDecimal(i));
        }

        return resultado;
    }
    
    private static void enviarRespuesta(OutputStream salida, String respuesta) throws IOException {
        salida.write(respuesta.getBytes());
        salida.flush();
    }
}
