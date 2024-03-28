/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 27 de marzo del 2024
 */

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

public class Nodo {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Uso: java Nodo [numeroNodo]");
            return;
        }

        int numeroNodo = Integer.parseInt(args[0]);

        switch (numeroNodo) {
            case 1:
                Scanner scanner = new Scanner(System.in);
                String respuesta = new String("si");
                NodoCliente nodo1 = new NodoCliente();
                int N = 0, M = 0;

                while (respuesta.equals("si")) {
                    System.out.print("\033[H\033[2J");  
                    System.out.flush();

                    try {
                        System.out.print("Ingrese el valor de N para el nodo 1: ");
                        N = Integer.parseInt(scanner.nextLine());
                    } catch (NumberFormatException e) {
                        System.err.println("\nError: entrada no numérica.");
                        scanner.close();
                        return;
                    }

                    if (N < 4) {
                        System.out.println("\nError: el valor de N debe ser igual o mayor a 4.");
                        scanner.close();
                        return;
                    }

                    try {
                        System.out.print("Ingrese el valor de M para el nodo 1: ");
                        M = Integer.parseInt(scanner.nextLine());
                    } catch (NumberFormatException e) {
                        System.err.println("\nError: entrada no numérica.");
                        scanner.close();
                        return;
                    }

                    System.out.println();

                    try {
                        nodo1.ejectuarNodo(N, M);
                    } catch (IOException | InterruptedException e) {
                        e.printStackTrace();
                    }

                    System.out.print("\n¿Desea calcular un nuevo producto? [si|no]: ");
                    respuesta = scanner.nextLine();

                    while (!respuesta.equals("no") && !respuesta.equals("si")) {
                        System.out.println("\nError: solo se admite [si] o [no] como respuestas validas.");
                        System.out.print("\n¿Desea calcular una nueva suma? [si|no]: ");
                        respuesta = scanner.nextLine();
                    }

                    if (respuesta.equals("no")) {
                        scanner.close();
                        return;
                    }
                }
            case 2:
                NodoServidor nodo2 = new NodoServidor(2);
                nodo2.iniciarServidor(12346); // Puerto para el nodo 2
                nodo2.iniciarServidor(12347); // Puerto para el nodo 2
                break;
            case 3:
                NodoServidor nodo3 = new NodoServidor(3);
                nodo3.iniciarServidor(12348); // Puerto para el nodo 3
                nodo3.iniciarServidor(12349); // Puerto para el nodo 3
                break;
            default:
                System.out.println("Número de nodo inválido. Debe ser 1, 2 o 3.");
        }
    }
}

class NodoCliente {
    public void ejectuarNodo(int N, int M) throws IOException, InterruptedException {
        List<float[][]> matrices = generarMatrices(N, M);
        List<float[][]> submatricesA = dividirMatriz(matrices.get(0), N, M);
        List<float[][]> submatricesTranspuestaB = dividirMatriz(matrices.get(2), N, M);
        List<float[][]> resultados = new ArrayList<>();
        float[][] matrizC = null;

        if (N <= 9 && M <= 9) {
            System.out.println("Matriz A:");
            mostrarMatriz(matrices.get(0));

            System.out.println("Matriz B:");
            mostrarMatriz(matrices.get(1));
        }

        HiloProducto A1xBTj = new HiloProducto("localhost", 12346, submatricesA.get(0), submatricesTranspuestaB);
        HiloProducto A2xBTj = new HiloProducto("localhost", 12347, submatricesA.get(1), submatricesTranspuestaB);
        HiloProducto A3xBTj = new HiloProducto("localhost", 12348, submatricesA.get(2), submatricesTranspuestaB);
        HiloProducto A4xBTj = new HiloProducto("localhost", 12349, submatricesA.get(3), submatricesTranspuestaB);

        System.out.println("Calculando producto...");

        double inicio = (double)System.currentTimeMillis();

        A1xBTj.start();
        A2xBTj.start();
        A3xBTj.start();
        A4xBTj.start();

        A1xBTj.join();
        A2xBTj.join();
        A3xBTj.join();
        A4xBTj.join();

        for (float[][] matriz: A1xBTj.getMatrices()) {
            resultados.add(matriz);
        }

        for (float[][] matriz: A2xBTj.getMatrices()) {
            resultados.add(matriz);
        }

        for (float[][] matriz: A3xBTj.getMatrices()) {
            resultados.add(matriz);
        }

        for (float[][] matriz: A4xBTj.getMatrices()) {
            resultados.add(matriz);
        }

        matrizC = unirMatrices(resultados, N);
        double fin = (double)System.currentTimeMillis();

        if (N <= 9 && M <= 9) {
            System.out.println("\nMatriz C:");
            mostrarMatriz(matrizC);
        }

        System.out.println("Calculando checksum...");

        double chksm = checksum(matrizC);

        System.out.println("\nChecksum: " + chksm);
        System.out.println("Tiempo de cálculo: " + ((fin - inicio) / 1000.0) + " segundos");
    }

    class HiloProducto extends Thread {
        private String host;
        private int puerto;
        private float[][] submatrizA;
        private List<float[][]> transpuestaB;
        private List<float[][]> resultados;

        public HiloProducto(String host, int puerto, float[][] submatrizA, List<float[][]> transpuestaB) {
            this.host = host;
            this.puerto = puerto;
            this.submatrizA = submatrizA;
            this.transpuestaB = transpuestaB;
            this.resultados = new ArrayList<>();
        }

        @Override
        public void run() {
            try {
                Socket conexion = new Socket(host, puerto);
                ObjectOutputStream salida = new ObjectOutputStream(conexion.getOutputStream());
                ObjectInputStream entrada = new ObjectInputStream(conexion.getInputStream());
                float[][] resultado = null;

                salida.writeObject(submatrizA);

                salida.writeObject(transpuestaB.get(0));
                salida.writeObject(transpuestaB.get(1));
                salida.writeObject(transpuestaB.get(2));
                salida.writeObject(transpuestaB.get(3));

                for (int i = 0; i < 4; i++) {
                    resultado = (float[][])entrada.readObject();
                    resultados.add(resultado);
                }

                conexion.close();
            } catch (IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }
        }

        public List<float[][]> getMatrices() {
            return resultados;
        }
    }

    public List<float[][]> generarMatrices(int N, int M) {
        if (N < 4) {
            throw new IllegalArgumentException("El valor de N debe ser igual o mayor a 4.");
        }

        float[][] matrizA = new float[N][M];
        float[][] matrizB = new float[M][N];
        float[][] transpuestaB = new float[N][M];
        List<float[][]> resultado = new ArrayList<>();
        int i = 0, j = 0;

        // Llenar matriz A
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                matrizA[i][j] = 3 * i + 2 * j;
            }
        }

        // Llenar matriz B
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                matrizB[i][j] = 2 * i - 4 * j;
            }
        }

        // Llenar transpuesta de la matriz B
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                transpuestaB[i][j] = matrizB[j][i];
            }
        }

        resultado.add(matrizA);
        resultado.add(matrizB);
        resultado.add(transpuestaB);

        return resultado;
    }

    public List<float[][]> dividirMatriz(float[][] matriz, int N, int M) {
        int filasPorNodo = 0, residuo = 0, inicioFila = 0;
        List<float[][]> resultado = new ArrayList<>();

        filasPorNodo = N / 4;
        residuo = N % 4;

        resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo + (residuo > 0 ? 1 : 0)));
        inicioFila += filasPorNodo + (residuo > 0 ? 1 : 0);
        resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo + (residuo > 1 ? 1 : 0)));
        inicioFila += filasPorNodo + (residuo > 1 ? 1 : 0);
        resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo + (residuo > 2 ? 1 : 0)));
        inicioFila += filasPorNodo + (residuo > 2 ? 1 : 0);
        resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo));

        return resultado;
    }

    private float[][] obtenerSubmatriz(float[][] matriz, int M, int inicioFila, int filasSubmatriz) {
        float[][] submatriz = new float[filasSubmatriz][M];
        int i = 0, j = 0;

        for (i = 0; i < filasSubmatriz; i++) {
            for (j = 0; j < M; j++) {
                submatriz[i][j] = matriz[inicioFila + i][j];
            }
        }

        return submatriz;
    }

    private float[][] unirMatrices(List<float[][]> matrices, int N) {
        int i = 0, j = 0;

        // Calcular el número total de filas y columnas
        int numColumnas = N, numFilas = N;

        // Crear la matriz resultado con el tamaño calculado
        float[][] resultado = new float[numFilas][numColumnas];

        // Variables para rastrear la posición de inserción en la matriz resultado
        int filaInicio = 0;
        int columnaInicio = 0;

        // Matriz auxiliar
        float[][] matriz = null;

        // Unir las matrices en la matriz resultado
        for (i = 0; i < matrices.size(); i += 4) {
            columnaInicio = 0;
            for (j = 0; j < 4; j++) {
                matriz = matrices.get(i + j);
                for (int k = 0; k < matriz.length; k++) {
                    System.arraycopy(matriz[k], 0, resultado[filaInicio + k], columnaInicio, matriz[k].length);
                }
                columnaInicio += matriz[0].length;
            }
            filaInicio += matrices.get(i).length;
        }

        return resultado;
    }

    private double checksum(float[][] matriz) {
        double resultado = 0;
        
        for (float[] fila: matriz) {
            for (float elemento: fila) {
                resultado += elemento;
            }
        }

        return resultado;
    }
    
    public static void mostrarMatriz(float[][] matriz) {
        int i = 0, j = 0;

        for (i = 0; i < matriz.length; i++) {
            for (j = 0; j < matriz[0].length; j++) {
                System.out.print(matriz[i][j] + "\t");
            }
            System.out.println();
        }
        System.out.println();
    }
}

class NodoServidor {
    private int numeroNodo;

    public NodoServidor(int numeroNodo) {
        this.numeroNodo = numeroNodo;
    }

    public void iniciarServidor(int puerto) {
        HiloServidor nuevoServidor = new HiloServidor(puerto);
        nuevoServidor.start();
    }

    private class HiloServidor extends Thread {
        private int puerto;

        public HiloServidor(int puerto) {
            this.puerto = puerto;
        }

        @Override
        public void run() {
            DateTimeFormatter dtf = DateTimeFormatter.ofPattern("dd/MM/yyyy|HH:mm:ss");

            try {
                ServerSocket servidor = new ServerSocket(puerto);
                
                System.out.println("[" + numeroNodo + "|" + puerto + "|" + dtf.format(LocalDateTime.now()) + "] Esperando conexiones.");
    
                while (true) {
                    Socket cliente = servidor.accept();
    
                    System.out.println("[" + numeroNodo + "|" + puerto + "|" + dtf.format(LocalDateTime.now()) + "] Conexión aceptada desde " + cliente.getInetAddress().toString().substring(1) + ".");
    
                    HiloCliente hiloCliente = new HiloCliente(cliente);
                    hiloCliente.start();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class HiloCliente extends Thread {
        private Socket cliente;

        public HiloCliente(Socket cliente) {
            this.cliente = cliente;
        }

        @Override
        public void run() {
            try {
                ObjectInputStream entrada = new ObjectInputStream(cliente.getInputStream());
                ObjectOutputStream salida = new ObjectOutputStream(cliente.getOutputStream());

                float[][] submatrizA = (float[][])entrada.readObject();
                
                HiloProducto AjxBT1 = new HiloProducto(submatrizA, (float[][])entrada.readObject());
                HiloProducto AjxBT2 = new HiloProducto(submatrizA, (float[][])entrada.readObject());
                HiloProducto AjxBT3 = new HiloProducto(submatrizA, (float[][])entrada.readObject());
                HiloProducto AjxBT4 = new HiloProducto(submatrizA, (float[][])entrada.readObject());

                AjxBT1.start();
                AjxBT2.start();
                AjxBT3.start();
                AjxBT4.start();

                try {
                    AjxBT1.join();
                    AjxBT2.join();
                    AjxBT3.join();
                    AjxBT4.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                salida.writeObject(AjxBT1.getResultado());
                salida.writeObject(AjxBT2.getResultado());
                salida.writeObject(AjxBT3.getResultado());
                salida.writeObject(AjxBT4.getResultado());
            } catch (IOException | ClassNotFoundException e) {
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

    private class HiloProducto extends Thread {
        private float[][] submatrizA;
        private float[][] submatrizBTj;
        private float[][] resultado;

        public HiloProducto(float[][] submatrizA, float[][] submatrizBTj) {
            this.submatrizA = submatrizA;
            this.submatrizBTj = submatrizBTj;
        }

        @Override
        public void run() {
            this.resultado = multiplicarMatrices(submatrizA, submatrizBTj);
        }

        public float[][] getResultado() {
            return resultado;
        }
    }

    private float[][] multiplicarMatrices(float[][] matrizA, float[][] matrizB) {
        int filasA = matrizA.length;
        int columnasA = matrizA[0].length;
        int filasB = matrizB.length;
        int columnasB = matrizB[0].length;
        int i = 0, j = 0;

        if (columnasA != columnasB) {
            throw new IllegalArgumentException("Las dimensiones de las matrices no son válidas para la multiplicación.");
        }

        float[][] resultado = new float[filasA][filasB];

        for (i = 0; i < filasA; i++) {
            for (j = 0; j < filasB; j++) {
                for (int k = 0; k < columnasA; k++) {
                    resultado[i][j] += matrizA[i][k] * matrizB[j][k];
                }
            }
        }

        return resultado;
    }
}
