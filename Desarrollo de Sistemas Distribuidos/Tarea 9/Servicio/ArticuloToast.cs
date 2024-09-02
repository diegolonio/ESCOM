using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using MySql.Data.MySqlClient;
using Newtonsoft.Json;
using System.Data;
using System.Net;
using System.Text;

namespace ServicioWeb
{
    public static class ArticuloToast
    {
        [Function("articulo_toast")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "articulo_toast")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("ArticuloToast");
            var j = new JsonSerializer();

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                ParamArticuloToast p = JsonConvert.DeserializeObject<ParamArticuloToast>(requestBody);

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");

                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    await connection.OpenAsync();

                    string query = @"SELECT cc.ID AS ID, a.nombre AS nombre, a.descripcion AS descripcion, a.precio AS precio, i.imagen AS imagen, cc.cantidad AS cantidad 
                                     FROM carrito cc 
                                     INNER JOIN articulos a ON cc.ID = a.ID 
                                     LEFT JOIN imagenes i ON cc.ID = i.ID 
                                     WHERE cc.ID = @ID";

                    using (MySqlCommand cmd = new MySqlCommand(query, connection))
                    {
                        cmd.Parameters.AddWithValue("@ID", p.ID);

                        // Ejecutar la consulta y obtener el MySqlDataReader
                        using (MySqlDataReader reader = (MySqlDataReader)await cmd.ExecuteReaderAsync())
                        {
                            if (reader.Read())
                            {
                                ArticuloCarrito articulo = new ArticuloCarrito();
                                articulo.ID = reader.GetInt32("ID");
                                articulo.nombre = reader.GetString("nombre");
                                articulo.descripcion = reader.GetString("descripcion");
                                articulo.precio = reader.GetFloat("precio");

                                // Verificar si la columna de imagen no es nula
                                if (!reader.IsDBNull("imagen"))
                                {
                                    long bytesLength = reader.GetBytes("imagen", 0, null, 0, 0); // Obtener la longitud de los bytes
                                    byte[] imagenBytes = new byte[bytesLength];
                                    reader.GetBytes(reader.GetOrdinal("imagen"), 0, imagenBytes, 0, (int)bytesLength); // Obtener los bytes de la imagen
                                    articulo.foto = imagenBytes;
                                }

                                articulo.cantidad = reader.GetInt32("cantidad");

                                var response = req.CreateResponse(HttpStatusCode.OK);
                                await response.WriteStringAsync(JsonConvert.SerializeObject(articulo));
                                return response;
                            }
                            else
                            {
                                var response = req.CreateResponse(HttpStatusCode.BadRequest);
                                await response.WriteStringAsync(JsonConvert.SerializeObject(new { error = "No existe ningún artículo en tu carrito de compras con el ID ingresado." }));
                                return response;
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                logger.LogError($"Error general: {ex.Message}");
                var response = req.CreateResponse(HttpStatusCode.BadRequest);
                await response.WriteStringAsync(JsonConvert.SerializeObject(new { error = ex.Message }));
                return response;
            }
        }
    }
}
