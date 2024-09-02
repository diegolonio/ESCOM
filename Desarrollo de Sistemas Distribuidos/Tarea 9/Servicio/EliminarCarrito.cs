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
    public static class EliminarCarrito
    {
        [Function("eliminar_carrito")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "eliminar_carrito")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("EliminarCarrito");

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                // No se utiliza el requestBody en este caso, ya que no se envía ningún parámetro en la solicitud

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");

                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    await connection.OpenAsync();
                    using (var transaction = await connection.BeginTransactionAsync())
                    {
                        try
                        {
                            // Obtener los productos en el carrito y su cantidad
                            List<Carrito> productosEnCarrito = await ObtenerProductosEnCarritoAsync(connection, transaction);

                            foreach (var producto in productosEnCarrito)
                            {
                                // Incrementar la cantidad en la tabla "articulos"
                                await IncrementarExistenciaArticuloAsync(connection, transaction, producto.ID, producto.cantidad);
                            }

                            // Eliminar todos los productos del carrito
                            await VaciarCarritoAsync(connection, transaction);

                            // Confirmar la transacción
                            await transaction.CommitAsync();

                            return ConstructOkResponse(req);
                        }
                        catch (Exception ex)
                        {
                            await transaction.RollbackAsync();
                            logger.LogError($"Error en la transacción: {ex.Message}");
                            return await ConstructBadRequestResponseAsync(req, ex.Message);
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

        private static async Task<List<Carrito>> ObtenerProductosEnCarritoAsync(MySqlConnection connection, MySqlTransaction transaction)
        {
            string sql = "SELECT ID, cantidad FROM carrito";

            using (MySqlCommand cmd = new MySqlCommand(sql, connection, transaction))
            {
                using (DbDataReader reader = await cmd.ExecuteReaderAsync())
                {
                    List<Carrito> productos = new List<Carrito>();

                    while (await reader.ReadAsync())
                    {
                        productos.Add(new Carrito
                        {
                            ID = reader.GetInt32("ID"),
                            cantidad = reader.GetInt32("cantidad")
                        });
                    }

                    return productos;
                }
            }
        }

        private static async Task IncrementarExistenciaArticuloAsync(MySqlConnection connection, MySqlTransaction transaction, int idArticulo, int cantidad)
        {
            string sql = "UPDATE articulos SET existencia = existencia + @cantidad WHERE ID = @id";

            using (MySqlCommand cmd = new MySqlCommand(sql, connection, transaction))
            {
                cmd.Parameters.AddWithValue("@cantidad", cantidad);
                cmd.Parameters.AddWithValue("@id", idArticulo);
                await cmd.ExecuteNonQueryAsync();
            }
        }

        private static async Task VaciarCarritoAsync(MySqlConnection connection, MySqlTransaction transaction)
        {
            string sql = "DELETE FROM carrito";

            using (MySqlCommand cmd = new MySqlCommand(sql, connection, transaction))
            {
                await cmd.ExecuteNonQueryAsync();
            }
        }

        private static HttpResponseData ConstructOkResponse(HttpRequestData req)
        {
            var response = req.CreateResponse(HttpStatusCode.OK);
            return response;
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
