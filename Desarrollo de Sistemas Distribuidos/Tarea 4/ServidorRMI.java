/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 22 de abril del 2024
 */

import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.UnknownHostException;
import java.rmi.Naming;
import java.rmi.RemoteException;

public class ServidorRMI {
    public static void main(String[] args) throws RemoteException, MalformedURLException {
        try {
            InetAddress localhost = InetAddress.getLocalHost();
            String url = "rmi://" + localhost.toString().split("/")[1] + "/producto";
            System.out.println("URL del servidor: " + url);
            ClaseRMI obj = new ClaseRMI();
            Naming.rebind(url, obj);
        } catch (UnknownHostException e) {
            e.printStackTrace();
            System.exit(0);
        }
    }
}
