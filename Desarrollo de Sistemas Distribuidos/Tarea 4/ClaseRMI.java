/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 22 de abril del 2024
 */

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ClaseRMI extends UnicastRemoteObject implements InterfaceRMI {
    public ClaseRMI() throws RemoteException {
        super();
    }

    public long[][] multiplicarMatrices(long[][] matrizA, long[][] matrizB) throws RemoteException {
        int filasA = matrizA.length;
        int columnasA = matrizA[0].length;
        int filasB = matrizB.length;
        int columnasB = matrizB[0].length;
        int i = 0, j = 0;

        if (columnasA != columnasB) {
            throw new IllegalArgumentException("Las dimensiones de las matrices no son válidas para la multiplicación.");
        }

        long[][] resultado = new long[filasA][filasB];

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
