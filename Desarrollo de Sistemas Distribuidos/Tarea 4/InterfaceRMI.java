/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 22 de abril del 2024
 */

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface InterfaceRMI extends Remote {
    public long[][] multiplicarMatrices(long[][] matrizA, long[][] matrizB) throws RemoteException;
}
