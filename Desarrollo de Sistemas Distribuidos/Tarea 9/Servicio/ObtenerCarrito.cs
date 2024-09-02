using System.Data;
using System.Data.Common;
using System.Net;
using System.Text;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using MySql.Data.MySqlClient;
using Newtonsoft.Json;

namespace ServicioWeb
{
    public static class ObtenerCarrito
    {
        [Function("obtener_carrito")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "obtener_carrito")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("ObtenerCarrito");

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                // No se utiliza el requestBody en este caso, ya que no se envía ningún parámetro en la solicitud

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");

                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    await connection.OpenAsync();

                    string sql = @"SELECT cc.ID AS ID, a.nombre AS nombre, a.descripcion AS descripcion, a.precio AS precio, i.imagen AS imagen, cc.cantidad AS cantidad
                               FROM carrito cc
                               INNER JOIN articulos a ON cc.ID = a.ID
                               LEFT JOIN imagenes i ON cc.ID = i.ID";

                    using (MySqlCommand cmd = new MySqlCommand(sql, connection))
                    {
                        using (DbDataReader reader = await cmd.ExecuteReaderAsync())
                        {
                            List<ArticuloCarrito> articulos = new List<ArticuloCarrito>();

                            // Convertir a MySqlDataReader explícitamente
                            MySqlDataReader rs = (MySqlDataReader)reader;

                            while (await rs.ReadAsync())
                            {
                                ArticuloCarrito articulo = new ArticuloCarrito();
                                articulo.ID = rs.GetInt32("ID");
                                articulo.nombre = rs.GetString("nombre");
                                articulo.descripcion = rs.GetString("descripcion");
                                articulo.precio = rs.GetFloat("precio");

                                // Verificar si el campo imagen no es DBNull antes de obtenerlo como byte[]
                                if (!await rs.IsDBNullAsync("imagen"))
                                {
                                    articulo.foto = (byte[])rs["imagen"];
                                }

                                articulo.cantidad = rs.GetInt32("cantidad");
                                articulos.Add(articulo);
                            }

                            if (articulos.Count == 0)
                            {
                                return await ConstructBadRequestResponseAsync(req, "No hay artículos en el carrito de compras.");
                            }

                            var response = req.CreateResponse(HttpStatusCode.OK);
                            await response.WriteStringAsync(JsonConvert.SerializeObject(articulos));
                            return response;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                logger.LogError($"Error general: {ex.Message}");
                return await ConstructBadRequestResponseAsync(req, ex.Message);
            }
        }

        private static async Task<HttpResponseData> ConstructBadRequestResponseAsync(HttpRequestData req, string message)
        {
            var response = req.CreateResponse(HttpStatusCode.BadRequest);
            response.Headers.Add("Content-Type", "application/json");
            await response.WriteStringAsync(JsonConvert.SerializeObject(new { error = message }), Encoding.UTF8);
            return response;
        }
    }
}
