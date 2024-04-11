/**
 * Alumno: Apolonio Villegas Diego Armando
 * Boleta: 2020630050
 * Grupo: 4CV11
 * Fecha: 10 de abril del 2024
 */

import java.io.OutputStream;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileInputStream;
import java.net.Socket;
import java.net.ServerSocket;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;
import javax.net.ssl.SSLServerSocketFactory;

public class ServidorHTTPS {
    public static void main(String[] args) {
        final int PORT = 443; // Puerto en el que el servidor escuchará las conexiones
        final String WEB_ROOT = "public_html"; // Directorio raíz donde se encuentran los archivos web

        System.setProperty("javax.net.ssl.keyStore", "keystore_servidor.jks");
        System.setProperty("javax.net.ssl.keyStorePassword", "1234567");
        SSLServerSocketFactory socket_factory = (SSLServerSocketFactory)SSLServerSocketFactory.getDefault();
        
        try {
            ServerSocket serverSocket = socket_factory.createServerSocket(PORT);
            System.out.println("Servidor HTTP multihilo iniciado en el puerto " + PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept(); // Espera una conexión entrante

                // Crear un nuevo hilo para manejar la solicitud del cliente
                ClientHandler thread = new ClientHandler(clientSocket, WEB_ROOT);
                thread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

class ClientHandler extends Thread {
    private Socket clientSocket;
    private String webRoot;

    public ClientHandler(Socket clientSocket, String webRoot) {
        this.clientSocket = clientSocket;
        this.webRoot = webRoot;
    }

    @Override
    public void run() {
        try {
            BufferedReader input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            OutputStream output = clientSocket.getOutputStream();

            PrintWriter writer = new PrintWriter(output, true);
            SimpleDateFormat dateFormat = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
            dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));

            StringBuilder requestBuilder = new StringBuilder();
            String line = input.readLine();

            requestBuilder.append(line);

            while ((line = input.readLine()) != null && !line.isEmpty()) {
                if(line.startsWith("If-Modified-Since")) {
                    requestBuilder.append("\r\n");
                    requestBuilder.append(line);
                }
            }

            String request = requestBuilder.toString(); // Leer la solicitud HTTP

            if (request != null) {
                // Extraer la ruta del archivo solicitado (suponiendo que está en la URL)
                String[] parts = request.split(" ");
                String requestedFile = parts[1];

                // Si la ruta contiene parámetros de consulta o fragmento, se eliminan para obtener la ruta del archivo
                int queryIndex = requestedFile.indexOf('?');
                if (queryIndex != -1) {
                    requestedFile = requestedFile.substring(0, queryIndex);
                }
                int fragmentIndex = requestedFile.indexOf('#');
                if (fragmentIndex != -1) {
                    requestedFile = requestedFile.substring(0, fragmentIndex);
                }

                // Si la ruta es solo "/", se asume que el archivo solicitado es index.html
                if (requestedFile.equals("/")) {
                    requestedFile = "/index.html";
                }

                // Construir la ruta completa del archivo solicitado
                String filePath = webRoot + requestedFile;
                String[] requestedFileArray = requestedFile.split("/");
                
                System.out.println("Conexión entrante desde " + clientSocket.getInetAddress() + " solicitando el recurso: " + requestedFileArray[requestedFileArray.length-1]);

                // Si el archivo solicitado es un directorio, intenta servir el archivo index.html dentro de ese directorio
                File file = new File(filePath);
                if (file.isDirectory()) {     
                    // Si es el directorio raíz, intentar servir index.html dentro de ese directorio
                    File indexFile = new File(file, "index.html");
                    if (indexFile.exists() && !indexFile.isDirectory()) {
                        // Si se encuentra index.html, establecerlo como el archivo solicitado
                        file = indexFile;
                        filePath = indexFile.getAbsolutePath();
                    } else {
                        // Si no se encuentra index.html, enviar respuesta 403 Forbidden
                        writer.println("HTTP/1.1 403 Forbidden");
                        writer.println();
                        writer.close();
                        input.close();
                        output.close();
                        clientSocket.close();
                        return;
                    }
                }

                // Verificar si el archivo solicitado existe
                if (file.exists() && !file.isDirectory()) {
                    try {
                        // Leer la fecha de la última modificación del archivo
                        Date lastModified = dateFormat.parse(dateFormat.format(file.lastModified()));
                        
                        // Extraer la fecha If-Modified-Since del encabezado de la solicitud
                        String ifModifiedSinceHeader = getIfModifiedSinceHeader(request);
                        Date ifModifiedSinceDate = parseDate(ifModifiedSinceHeader);
                        
                        // Verificar si el archivo ha sido modificado desde la fecha If-Modified-Since
                        if (ifModifiedSinceDate != null && !lastModified.after(ifModifiedSinceDate)) {
                            // Si no ha sido modificado, enviar respuesta 304 Not Modified
                            writer.println("HTTP/1.1 304 Not Modified");
                            writer.println();
                        } else {
                            // Si ha sido modificado o no se proporciona la fecha, enviar respuesta 200 OK con el contenido del archivo
                            String contentType = getContentType(filePath);
                            writer.println("HTTP/1.1 200 OK");
                            writer.println("Cache-Control: no-cache");
                            writer.println("Content-Type: " + contentType);
                            writer.println("Last-Modified: " + dateFormat.format(lastModified));
                            writer.println();
                            
                            if (isTextFile(contentType)) {
                                sendTextFileContent(writer, filePath);
                            } else {
                                sendBinaryFileContent(output, filePath);
                            }
                        }
                    } catch (ParseException e) {
                        e.printStackTrace();
                    }
                } else {
                    // Si el archivo no se encuentra o es un directorio, enviar respuesta 404 Not Found
                    writer.println("HTTP/1.1 404 Not Found");
                    writer.println();
                }
            }

            // Cerrar los flujos y el socket
            writer.close();
            input.close();
            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String getIfModifiedSinceHeader(String request) {
        String[] lines = request.split("\r\n");
        for (String line : lines) {
            if (line.startsWith("If-Modified-Since:")) {
                return line.substring("If-Modified-Since:".length()).trim();
            }
        }
        return null;
    }

    private Date parseDate(String dateString) {
        if (dateString != null && !dateString.isEmpty()) {
            try {
                SimpleDateFormat dateFormat = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss zzz", Locale.US);
                dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
                return dateFormat.parse(dateString);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    private String getContentType(String filePath) {
        if (filePath.endsWith(".html") || filePath.endsWith(".htm")) {
            return "text/html";
        } else if (filePath.endsWith(".css")) {
            return "text/css";
        } else if (filePath.endsWith(".js")) {
            return "application/javascript";
        } else if (filePath.endsWith(".jpg") || filePath.endsWith(".jpeg")) {
            return "image/jpeg";
        } else if (filePath.endsWith(".png")) {
            return "image/png";
        } else if (filePath.endsWith(".gif")) {
            return "image/gif";
        } else if (filePath.endsWith(".ttf") || filePath.endsWith(".otf")) {
            return "application/font-sfnt"; // Fuentes tipográficas
        } else if (filePath.endsWith(".woff")) {
            return "application/font-woff"; // Web Open Font Format (WOFF)
        } else if (filePath.endsWith(".eot")) {
            return "application/vnd.ms-fontobject"; // Embedded OpenType (EOT)
        } else {
            return "application/octet-stream";
        }
    }

    private boolean isTextFile(String contentType) {
        return contentType.startsWith("text/") || contentType.equals("application/javascript");
    }

    private void sendTextFileContent(PrintWriter writer, String filePath) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                writer.println(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendBinaryFileContent(OutputStream output, String filePath) {
        try (FileInputStream fileInputStream = new FileInputStream(filePath)) {
            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                output.write(buffer, 0, bytesRead);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
