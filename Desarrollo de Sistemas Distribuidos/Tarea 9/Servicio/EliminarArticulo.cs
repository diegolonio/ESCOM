using System.Net;
using System.Text;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using MySql.Data.MySqlClient;
using Newtonsoft.Json;

namespace ServicioWeb
{
    public static class EliminarArticulo
    {
        private static readonly string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");

        [Function("eliminar_articulo")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "eliminar_articulo")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("EliminarArticulo");
            var j = new JsonSerializer();

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                var p = JsonConvert.DeserializeObject<ParamEliminarArticulo>(requestBody);

                using MySqlConnection connection = new MySqlConnection(connectionString);
                await connection.OpenAsync();

                try
                {
                    await Task.Run(async () =>
                    {
                        var transaction = connection.BeginTransaction();

                        try
                        {
                            int cantidad = await ObtenerCantidadArticulo(connection, p.ID);

                            await EliminarArticuloCarrito(connection, p.ID);
                            await IncrementarExistenciaArticulo(connection, p.ID, cantidad);

                            transaction.Commit();
                        }
                        catch (Exception e)
                        {
                            transaction.Rollback();
                            logger.LogError($"Error al eliminar el artículo: {e.Message}");
                            await ConstructBadRequestResponse(req, e.Message);
                        }
                    });

                    var response = req.CreateResponse(HttpStatusCode.OK);
                    return response;
                }
                catch (Exception ex)
                {
                    logger.LogError($"Error general: {ex.Message}");
                    return await ConstructBadRequestResponse(req, ex.Message);
                }
            }
            catch (Exception ex)
            {
                logger.LogError($"Error al procesar la solicitud: {ex.Message}");
                return await ConstructBadRequestResponse(req, ex.Message);
            }
        }

        private static async Task<int> ObtenerCantidadArticulo(MySqlConnection connection, int articuloId)
        {
            string sql = "SELECT cantidad FROM carrito WHERE ID = @ID";
            using MySqlCommand cmd = new MySqlCommand(sql, connection);
            cmd.Parameters.AddWithValue("@ID", articuloId);
            object result = await cmd.ExecuteScalarAsync();
            return result != null ? Convert.ToInt32(result) : 0;
        }

        private static async Task EliminarArticuloCarrito(MySqlConnection connection, int articuloId)
        {
            string sql = "DELETE FROM carrito WHERE ID = @ID";
            using MySqlCommand cmd = new MySqlCommand(sql, connection);
            cmd.Parameters.AddWithValue("@ID", articuloId);
            await cmd.ExecuteNonQueryAsync();
        }

        private static async Task IncrementarExistenciaArticulo(MySqlConnection connection, int articuloId, int cantidad)
        {
            string sql = "UPDATE articulos SET existencia = existencia + @cantidad WHERE ID = @ID";
            using MySqlCommand cmd = new MySqlCommand(sql, connection);
            cmd.Parameters.AddWithValue("@ID", articuloId);
            cmd.Parameters.AddWithValue("@cantidad", cantidad);
            await cmd.ExecuteNonQueryAsync();
        }

        private static async Task<HttpResponseData> ConstructBadRequestResponse(HttpRequestData req, string message)
        {
            var response = req.CreateResponse(HttpStatusCode.BadRequest);
            response.Headers.Add("Content-Type", "application/json");
            byte[] responseMessageBytes = Encoding.UTF8.GetBytes(JsonConvert.SerializeObject(new { error = message }));
            await response.Body.WriteAsync(responseMessageBytes, 0, responseMessageBytes.Length);
            return response;
        }
    }
}