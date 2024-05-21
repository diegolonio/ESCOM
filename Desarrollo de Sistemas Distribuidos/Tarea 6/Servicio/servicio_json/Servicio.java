// Alumno: Apolonio Villegas Diego Armando
// Boleta: 2020630050
// Grupo: 4CV11
// Fecha: 8 de mayo del 2024

package servicio_json;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Consumes;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.QueryParam;
import javax.ws.rs.FormParam;
import javax.ws.rs.core.Response;

import java.lang.Error;
import java.sql.*;
import javax.sql.DataSource;
import javax.naming.Context;
import javax.naming.InitialContext;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.google.gson.*;

@Path("ws")
public class Servicio {
    static DataSource pool = null;
    static {
        try {
            Context ctx = new InitialContext();
            pool = (DataSource) ctx.lookup("java:comp/env/jdbc/datasource_Servicio");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    static Gson j = new GsonBuilder().registerTypeAdapter(byte[].class, new AdaptadorGsonBase64()).setDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS").create();

    @POST
    @Path("insertar_articulo")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response capturar(String json) throws Exception {
        ParamInsertarArticulo p = (ParamInsertarArticulo) j.fromJson(json, ParamInsertarArticulo.class);
        Connection conexion = pool.getConnection();
        Articulo articulo = p.articulo;

        if (articulo.nombre == null || articulo.nombre.equals(""))
            return Response.status(400).entity(j.toJson(new Error("Se debe ingresar el nombre del producto."))).build();

        if (articulo.descripcion == null || articulo.descripcion.equals(""))
            return Response.status(400).entity(j.toJson(new Error("Se debe ingresar la descripción del producto."))).build();

        if (articulo.precio < 0)
            return Response.status(400).entity(j.toJson(new Error("Precio de producto no válido."))).build();

        if (articulo.existencia < 0)
            return Response.status(400).entity(j.toJson(new Error("Existencias de producto no válidas."))).build();

        if (articulo.existencia < 1 || articulo.relevancia > 100)
            return Response.status(400).entity(j.toJson(new Error("La relevancia del producto debe estar entre 1 y 100."))).build();
        
        try {
            conexion.setAutoCommit(false);

            PreparedStatement stmt_1 = conexion.prepareStatement("INSERT INTO articulos(ID, nombre, descripcion, precio, existencia, cantidad) VALUES (0, ?, ?, ?, ?, ?)");

            try {
                stmt_1.setString(1, articulo.nombre);
                stmt_1.setString(2, articulo.descripcion);
                stmt_1.setFloat(3, articulo.precio);
                stmt_1.setInt(4, articulo.existencia);
                stmt_1.setInt(5, articulo.relevancia);
                stmt_1.executeUpdate();
            } finally {
                stmt_1.close();
            }

            if (articulo.foto != null) {
                PreparedStatement stmt_2 = conexion.prepareStatement("INSERT INTO imagenes(ID, imagen) VALUES ((SELECT ID FROM articulos WHERE nombre=?), ?)");

                try {
                    stmt_2.setString(1, articulo.nombre);
                    stmt_2.setBytes(2, articulo.foto);
                    stmt_2.executeUpdate();
                } finally {
                    stmt_2.close();
                }
            }

            conexion.commit();
        } catch (Exception e) {
            conexion.rollback();
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.setAutoCommit(true);
            conexion.close();
        }

        return Response.ok().build();
    }

    @POST
    @Path("busqueda_articulo")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response busqueda(String json) throws Exception {
        ParamBusqueda p = (ParamBusqueda)j.fromJson(json, ParamBusqueda.class);
        Connection conexion = pool.getConnection();
        Busqueda busqueda = p.busqueda;
        String[] palabras = eliminarPreposiciones(busqueda.busqueda);
        String consultaSQL = generarConsultaSQL(palabras, busqueda.orden);

        try {
            PreparedStatement stmt_1 = conexion.prepareStatement(consultaSQL);

            try {
                ResultSet rs = stmt_1.executeQuery();

                try {
                    // Estructurar los resultados en una lista de objetos
                    List<Articulo> articulos = new ArrayList<>();
                    while (rs.next()) {
                        Articulo articulo = new Articulo();
                        articulo.ID = rs.getInt("ID");
                        articulo.nombre = rs.getString("nombre");
                        articulo.descripcion = rs.getString("descripcion");
                        articulo.precio = rs.getFloat("precio");
                        articulo.existencia = rs.getInt("existencia");
                        articulo.relevancia = rs.getInt("cantidad");
                        articulo.foto = rs.getBytes("imagen");
                        articulos.add(articulo);
                    }

                    if (articulos.size() == 0)
                        return Response.status(400).entity(j.toJson(new Error("Busqueda sin resultados."))).build();
                    
                    return Response.ok().entity(j.toJson(articulos)).build();
                } finally {
                    rs.close();
                }
            } finally {
                stmt_1.close();
            }
        } catch (Exception e) {
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.close();
        }
    }

    @POST
    @Path("agregar_carrito")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response agregar_carrito(String json) throws Exception {
        ParamCarrito p = (ParamCarrito)j.fromJson(json, ParamCarrito.class);
        Connection conexion = pool.getConnection();
        Carrito carrito = p.carrito;

        try {
            conexion.setAutoCommit(false);

            int existenciaActual = obtenerExistencia(conexion, carrito.ID);

            if (carrito.cantidad <= existenciaActual) {
                int cantidadExistente = obtenerCantidadExistente(conexion, carrito.ID);

                if (cantidadExistente == 0) {
                    insertarNuevoRegistro(conexion, carrito.ID, carrito.cantidad);
                } else {
                    actualizarCantidadExistente(conexion, carrito.ID, cantidadExistente, carrito.cantidad);
                }

                disminuirExistenciaArticulo(conexion, carrito.ID, carrito.cantidad);
            } else {
                return Response.status(400).entity(j.toJson(new Error("Solo hay " + existenciaActual + " items disponibles del producto que quieres agregar a tu Carrito de Compras."))).build();
            }

            conexion.commit();
        } catch (SQLException e) {
            conexion.rollback();
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.setAutoCommit(true);
            conexion.close();
        }

        return Response.ok().build();
    }

    @POST
    @Path("obtener_carrito")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response obtener_carrito() throws Exception {
        Connection conexion = pool.getConnection();
        
        try {
            PreparedStatement stmt_1 = conexion.prepareStatement("SELECT cc.ID AS ID, a.nombre AS nombre, a.descripcion AS descripcion, a.precio AS precio, i.imagen AS imagen, cc.cantidad AS cantidad FROM carrito_compra cc INNER JOIN articulos a ON cc.ID = a.ID LEFT JOIN imagenes i ON cc.ID = i.ID;");

            try {
                ResultSet rs = stmt_1.executeQuery();

                try {
                    // Estructurar los resultados en una lista de objetos
                    List<ArticuloCarrito> articulos = new ArrayList<>();
                    while (rs.next()) {
                        ArticuloCarrito articulo = new ArticuloCarrito();
                        articulo.ID = rs.getInt("ID");
                        articulo.nombre = rs.getString("nombre");
                        articulo.descripcion = rs.getString("descripcion");
                        articulo.precio = rs.getFloat("precio");
                        articulo.foto = rs.getBytes("imagen");
                        articulo.cantidad = rs.getInt("cantidad");
                        articulos.add(articulo);
                    }

                    if (articulos.size() == 0)
                        return Response.status(400).entity(j.toJson(new Error("No hay artículos en el carrito de compras."))).build();
                    
                    return Response.ok().entity(j.toJson(articulos)).build();
                } finally {
                    rs.close();
                }
            } finally {
                stmt_1.close();
            }
        } catch (Exception e) {
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.close();
        }
    }

    @POST
    @Path("eliminar_articulo")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response eliminar_articulo(String json) throws Exception {
        ParamEliminarArticulo p = (ParamEliminarArticulo)j.fromJson(json, ParamEliminarArticulo.class);
        Connection conexion = pool.getConnection();

        try {
            conexion.setAutoCommit(false);

            int cantidad = obtenerCantidadArticulo(conexion, p.ID);

            eliminarArticuloCarrito(conexion, p.ID);
            incrementarExistenciaArticulo(conexion, p.ID, cantidad);

            conexion.commit();
        } catch (SQLException e) {
            conexion.rollback();
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.setAutoCommit(true);
            conexion.close();
        }

        return Response.ok().build();
    }

    @POST
    @Path("articulo_toast")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response articulo_toast(String json) throws Exception {
        ParamArticuloToast p = (ParamArticuloToast)j.fromJson(json, ParamArticuloToast.class);
        Connection conexion = pool.getConnection();
        
        try {
            PreparedStatement stmt_1 = conexion.prepareStatement("SELECT cc.ID AS ID, a.nombre AS nombre, a.descripcion AS descripcion, a.precio AS precio, i.imagen AS imagen, cc.cantidad AS cantidad FROM carrito_compra cc INNER JOIN articulos a ON cc.ID = a.ID LEFT JOIN imagenes i ON cc.ID = i.ID WHERE cc.ID = ?;");

            try {
                stmt_1.setInt(1, p.ID);

                ResultSet rs = stmt_1.executeQuery();

                try {
                    if (rs.next()) {
                        ArticuloCarrito articulo = new ArticuloCarrito();
                        articulo.ID = rs.getInt("ID");
                        articulo.nombre = rs.getString("nombre");
                        articulo.descripcion = rs.getString("descripcion");
                        articulo.precio = rs.getFloat("precio");
                        articulo.foto = rs.getBytes("imagen");
                        articulo.cantidad = rs.getInt("cantidad");

                        return Response.ok().entity(j.toJson(articulo)).build();
                    }
                    
                    return Response.status(400).entity(j.toJson(new Error("No existe ningún artículo en tu carrito de compras con el ID ingresado."))).build();
                } finally {
                    rs.close();
                }
            } finally {
                stmt_1.close();
            }
        } catch (Exception e) {
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.close();
        }
    }

    @POST
    @Path("eliminar_carrito")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response eliminar_carrito() throws Exception {
        Connection conexion = pool.getConnection();
        
        try {
            conexion.setAutoCommit(false);
            
            // Obtener los productos en el carrito_compra y su cantidad
            ResultSet rs = obtenerProductosEnCarrito(conexion);

            while (rs.next())
                // Incrementar la cantidad en la tabla "articulos"
                incrementarExistenciaArticulo(conexion, rs.getInt("ID"), rs.getInt("cantidad"));

            // Eliminar todos los productos del carrito_compra
            vaciarCarrito(conexion);
            
            // Confirmar la transacción
            conexion.commit();
        } catch (SQLException e) {
            conexion.rollback();
            return Response.status(400).entity(j.toJson(new Error(e.getMessage()))).build();
        } finally {
            conexion.setAutoCommit(true);
            conexion.close();
        }
            
        return Response.ok().build();
    }

    // Método para eliminar preposiciones
    public static String[] eliminarPreposiciones(String busqueda) {
        // Lista de preposiciones en español
        List<String> preposiciones = Arrays.asList("a", "ante", "bajo", "cabe", "con", "contra", "de", "desde", "en", "entre", "hacia", "hasta", "para", "por", "según", "sin", "so", "sobre", "tras");
        
        // Dividir la cadena de búsqueda en palabras
        String[] palabras = busqueda.split("\\s+");
        
        // Eliminar preposiciones
        List<String> palabrasFiltradas = new ArrayList<>();
        for (String palabra : palabras) {
            if (!preposiciones.contains(palabra.toLowerCase())) {
                palabrasFiltradas.add(palabra);
            }
        }
        
        // Convertir la lista de palabras filtradas a un array
        String[] resultado = new String[palabrasFiltradas.size()];
        return palabrasFiltradas.toArray(resultado);
    }

    public static String generarConsultaSQL(String[] palabras, int orden) {
        // Generar la cláusula WHERE de la consulta SQL
        StringBuilder whereClause = new StringBuilder();
        for (int i = 0; i < palabras.length; i++) {
            if (i > 0) {
                whereClause.append(" OR ");
            }
            whereClause.append("nombre LIKE '%").append(palabras[i]).append("%' OR descripcion LIKE '%").append(palabras[i]).append("%'");
        }
        
        // Generar la consulta SQL completa con la cláusula WHERE y el orden
        String consultaSQL = "SELECT articulos.*, imagenes.imagen FROM articulos LEFT JOIN imagenes ON articulos.ID = imagenes.ID WHERE " + whereClause.toString();
        
        switch (orden) {
            case 1:
                consultaSQL += " ORDER BY precio ASC";
                break;
            case 2:
                consultaSQL += " ORDER BY precio DESC";
                break;
            case 3:
                consultaSQL += " ORDER BY cantidad DESC";
                break;
            default:
                // No se especifica orden
                break;
        }
        
        return consultaSQL;
    }
    
    private static int obtenerExistencia(Connection conexion, int ID) throws SQLException {
        String consultaExistencia = "SELECT existencia FROM articulos WHERE ID = ?";
        try (PreparedStatement stmtExistencia = conexion.prepareStatement(consultaExistencia)) {
            stmtExistencia.setInt(1, ID);
            try (ResultSet rsExistencia = stmtExistencia.executeQuery()) {
                if (rsExistencia.next()) {
                    return rsExistencia.getInt("existencia");
                } else {
                    throw new SQLException("No se encontró el artículo en la base de datos.");
                }
            }
        }
    }

    private static int obtenerCantidadExistente(Connection conexion, int ID) throws SQLException {
        String consultaCantidad = "SELECT cantidad FROM carrito_compra WHERE ID = ?";
        try (PreparedStatement stmtCantidad = conexion.prepareStatement(consultaCantidad)) {
            stmtCantidad.setInt(1, ID);
            try (ResultSet rsCantidad = stmtCantidad.executeQuery()) {
                return rsCantidad.next() ? rsCantidad.getInt("cantidad") : 0;
            }
        }
    }

    private static void insertarNuevoRegistro(Connection conexion, int ID, int cantidad) throws SQLException {
        String consultaInsertar = "INSERT INTO carrito_compra (ID, cantidad) VALUES (?, ?)";
        try (PreparedStatement stmtInsertar = conexion.prepareStatement(consultaInsertar)) {
            stmtInsertar.setInt(1, ID);
            stmtInsertar.setInt(2, cantidad);
            stmtInsertar.executeUpdate();
        }
    }

    private static void actualizarCantidadExistente(Connection conexion, int ID, int cantidadExistente, int cantidad) throws SQLException {
        String consultaActualizar = "UPDATE carrito_compra SET cantidad = ? WHERE ID = ?";
        try (PreparedStatement stmtActualizar = conexion.prepareStatement(consultaActualizar)) {
            stmtActualizar.setInt(1, cantidadExistente + cantidad);
            stmtActualizar.setInt(2, ID);
            stmtActualizar.executeUpdate();
        }
    }

    private static void disminuirExistenciaArticulo(Connection conexion, int ID, int cantidad) throws SQLException {
        String consultaDisminuirExistencia = "UPDATE articulos SET existencia = existencia - ? WHERE ID = ?";
        try (PreparedStatement stmtDisminuirExistencia = conexion.prepareStatement(consultaDisminuirExistencia)) {
            stmtDisminuirExistencia.setInt(1, cantidad);
            stmtDisminuirExistencia.setInt(2, ID);
            stmtDisminuirExistencia.executeUpdate();
        }
    }

    private static int obtenerCantidadArticulo(Connection conexion, int ID) throws SQLException {
        String consultaCantidad = "SELECT cantidad FROM carrito_compra WHERE ID = ?";
        try (PreparedStatement stmtCantidad = conexion.prepareStatement(consultaCantidad)) {
            stmtCantidad.setInt(1, ID);
            try (ResultSet rsCantidad = stmtCantidad.executeQuery()) {
                if (rsCantidad.next()) {
                    return rsCantidad.getInt("cantidad");
                } else {
                    throw new SQLException("No se encontró el artículo en el carrito de compra.");
                }
            }
        }
    }

    private static void eliminarArticuloCarrito(Connection conexion, int ID) throws SQLException {
        String eliminarArticulo = "DELETE FROM carrito_compra WHERE ID = ?";
        try (PreparedStatement stmtEliminar = conexion.prepareStatement(eliminarArticulo)) {
            stmtEliminar.setInt(1, ID);
            stmtEliminar.executeUpdate();
        }
    }

    private static void incrementarExistenciaArticulo(Connection conexion, int ID, int cantidad) throws SQLException {
        String incrementarExistencia = "UPDATE articulos SET existencia = existencia + ? WHERE ID = ?";
        try (PreparedStatement stmtIncrementar = conexion.prepareStatement(incrementarExistencia)) {
            stmtIncrementar.setInt(1, cantidad);
            stmtIncrementar.setInt(2, ID);
            stmtIncrementar.executeUpdate();
        }
    }

    private static ResultSet obtenerProductosEnCarrito(Connection conexion) throws SQLException {
        String consulta = "SELECT ID, cantidad FROM carrito_compra";
        PreparedStatement stmt = conexion.prepareStatement(consulta);
        return stmt.executeQuery();
    }

    private static void vaciarCarrito(Connection conexion) throws SQLException {
        String consulta = "DELETE FROM carrito_compra";
        PreparedStatement stmt = conexion.prepareStatement(consulta);
        stmt.executeUpdate();
    }
}
