/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 22 de abril del 2024
 */

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class ClienteRMI {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String respuesta = new String("si");
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

            try {
                System.out.print("Ingrese el valor de M para el nodo 1: ");
                M = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.err.println("\nError: entrada no numérica.");
                scanner.close();
                return;
            }

            System.out.println();

            // #############################################################

            List<long[][]> matrices = generarMatrices(N, M);
            List<long[][]> submatricesA = dividirMatriz(matrices.get(0), 6, N, M);
            List<long[][]> submatricesTranspuestaB = dividirMatriz(matrices.get(2), 6, N, M);
            List<long[][]> resultados = new ArrayList<>();
            List<Producto> productos = new ArrayList<>();
            long[][] matrizC = null;


            if (N <= 15 && M <= 15) {
                System.out.println("Matriz A:");
                mostrarMatriz(matrices.get(0));
                
                System.out.println("Matriz B:");
                mostrarMatriz(matrices.get(1));
            }

            int i = 1;
            String url = "";
            for (long[][] submatrizA: submatricesA) {
                for (long[][] submatrizBT: submatricesTranspuestaB) {
                    if (i <= 12) {
                        url = "rmi://192.168.0.4/producto";
                    } else if (i <= 24) {
                        url = "rmi://192.168.0.5/producto";
                    } else {
                        url = "rmi://192.168.0.6/producto";
                    }

                    productos.add(new Producto(submatrizA, submatrizBT, url));
                    i++;
                }
            }

            System.out.println("Calculando producto...");

            for (Producto producto: productos) {
                producto.start();
            }

            try {
                for (Producto producto: productos) {
                    producto.join();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            for (Producto producto: productos) {
                resultados.add(producto.getResultado());
            }

            matrizC = unirMatrices(resultados, 6, N);

            if (N <= 15 && M <= 15) {
                System.out.println("\nMatriz C:");
                mostrarMatriz(matrizC);
            }

            System.out.println("Calculando checksum...");

            double chksm = checksum(matrizC);

            System.out.println("\nChecksum: " + chksm);

            // #############################################################

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
    }

    private static class Producto extends Thread {
        String url;
        private long[][] submatrizA;
        private long[][] submatrizBT;
        private long[][] submatrizC;

        public Producto(long[][] submatrizA, long[][] submatrizBT, String url) {
            this.url = url;
            this.submatrizA = submatrizA;
            this.submatrizBT = submatrizBT;
        }

        public void run() {
            try {
                InterfaceRMI referencia = (InterfaceRMI)Naming.lookup(url);
                submatrizC = referencia.multiplicarMatrices(submatrizA, submatrizBT);
            } catch (NotBoundException | RemoteException | MalformedURLException e) {
                e.printStackTrace();
            }
        }

        public long[][] getResultado() {
            return submatrizC;
        }
    }

    public static void mostrarMatriz(long[][] matriz) {
        int i = 0, j = 0;

        for (i = 0; i < matriz.length; i++) {
            for (j = 0; j < matriz[0].length; j++) {
                System.out.print(matriz[i][j] + "\t");
            }
            System.out.println();
        }
        System.out.println();
    }
    
    public static List<long[][]> generarMatrices(int N, int M) {
        long[][] matrizA = new long[N][M];
        long[][] matrizB = new long[M][N];
        long[][] transpuestaB = new long[N][M];
        List<long[][]> resultado = new ArrayList<>();
        int i = 0, j = 0;

        // Llenar matriz A
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                matrizA[i][j] = i + 2 * j;
            }
        }

        // Llenar matriz B
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                matrizB[i][j] = 3 * i - j;
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

    public static List<long[][]> dividirMatriz(long[][] matriz, int numeroDivisiones, int N, int M) {
        if (N < numeroDivisiones) {
            throw new IllegalArgumentException("El número de divisiones debe ser igual o mayor a N.");
        }

        int filasPorNodo = 0, residuo = 0, inicioFila = 0;
        List<long[][]> resultado = new ArrayList<>();

        filasPorNodo = N / numeroDivisiones;
        residuo = N % numeroDivisiones;

        for (int i = 0; i < (numeroDivisiones-1); i++) {
            resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo + (residuo > i ? 1 : 0)));
            inicioFila += filasPorNodo + (residuo > i ? 1 : 0);
        }

        resultado.add(obtenerSubmatriz(matriz, M, inicioFila, filasPorNodo));

        return resultado;
    }

    private static long[][] obtenerSubmatriz(long[][] matriz, int M, int inicioFila, int filasSubmatriz) {
        long[][] submatriz = new long[filasSubmatriz][M];
        int i = 0, j = 0;

        for (i = 0; i < filasSubmatriz; i++) {
            for (j = 0; j < M; j++) {
                submatriz[i][j] = matriz[inicioFila + i][j];
            }
        }

        return submatriz;
    }

    private static long[][] unirMatrices(List<long[][]> matrices, int numeroDivisiones, int N) {
        int i = 0, j = 0;

        // Calcular el número total de filas y columnas
        int numColumnas = N, numFilas = N;

        // Crear la matriz resultado con el tamaño calculado
        long[][] resultado = new long[numFilas][numColumnas];

        // Variables para rastrear la posición de inserción en la matriz resultado
        int filaInicio = 0;
        int columnaInicio = 0;

        // Matriz auxiliar
        long[][] matriz = null;

        // Unir las matrices en la matriz resultado
        for (i = 0; i < matrices.size(); i += numeroDivisiones) {
            columnaInicio = 0;
            for (j = 0; j < numeroDivisiones; j++) {
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

    private static double checksum(long[][] matriz) {
        double resultado = 0;
        
        for (long[] fila: matriz) {
            for (long elemento: fila) {
                resultado += elemento;
            }
        }

        return resultado;
    }
}
