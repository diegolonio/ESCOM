using System;
using System.Data.Common;
using System.IO;
using System.Net;
using System.Text;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using MySql.Data.MySqlClient;
using Newtonsoft.Json;

namespace ServicioWeb
{
    public static class AgregarCarrito
    {
        [Function("agregar_carrito")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "agregar_carrito")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("AgregarCarrito");

            try
            {
                string requestBody = await new StreamReader(req.Body).ReadToEndAsync();
                ParamCarrito p = JsonConvert.DeserializeObject<ParamCarrito>(requestBody);
                Carrito carrito = p.carrito;

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");

                using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    await connection.OpenAsync();
                    MySqlTransaction transaction = await connection.BeginTransactionAsync();

                    try
                    {
                        int existenciaActual = await ObtenerExistencia(connection, carrito.ID);

                        if (carrito.cantidad <= existenciaActual)
                        {
                            int cantidadExistente = await ObtenerCantidadExistente(connection, carrito.ID);

                            if (cantidadExistente == 0)
                            {
                                await InsertarNuevoRegistro(connection, carrito.ID, carrito.cantidad);
                            }
                            else
                            {
                                await ActualizarCantidadExistente(connection, carrito.ID, cantidadExistente, carrito.cantidad);
                            }

                            await DisminuirExistenciaArticulo(connection, carrito.ID, carrito.cantidad);
                        }
                        else
                        {
                            await transaction.RollbackAsync();
                            return await ConstructBadRequestResponseAsync(req, $"Solo hay {existenciaActual} items disponibles del producto que quieres agregar a tu Carrito de Compras.");
                        }

                        await transaction.CommitAsync();
                    }
                    catch (Exception e)
                    {
                        await transaction.RollbackAsync();
                        logger.LogError($"Error al agregar al carrito: {e.Message}");
                        return await ConstructBadRequestResponseAsync(req, e.Message);
                    }
                }

                var response = req.CreateResponse(HttpStatusCode.OK);
                return response;
            }
            catch (Exception ex)
            {
                logger.LogError($"Error general: {ex.Message}");
                return await ConstructBadRequestResponseAsync(req, ex.Message);
            }
        }

        private static async Task<int> ObtenerExistencia(MySqlConnection connection, int articuloId)
        {
            string sql = "SELECT existencia FROM articulos WHERE ID = @ID";
            using (MySqlCommand cmd = new MySqlCommand(sql, connection))
            {
                cmd.Parameters.AddWithValue("@ID", articuloId);
                object result = await cmd.ExecuteScalarAsync();
                return result != null ? Convert.ToInt32(result) : 0;
            }
        }

        private static async Task<int> ObtenerCantidadExistente(MySqlConnection connection, int articuloId)
        {
            string sql = "SELECT COUNT(*) FROM carrito WHERE ID = @ID";
            using (MySqlCommand cmd = new MySqlCommand(sql, connection))
            {
                cmd.Parameters.AddWithValue("@ID", articuloId);
                object result = await cmd.ExecuteScalarAsync();
                return result != null ? Convert.ToInt32(result) : 0;
            }
        }

        private static async Task InsertarNuevoRegistro(MySqlConnection connection, int articuloId, int cantidad)
        {
            string sql = "INSERT INTO carrito (ID, cantidad) VALUES (@ID, @cantidad)";
            using (MySqlCommand cmd = new MySqlCommand(sql, connection))
            {
                cmd.Parameters.AddWithValue("@ID", articuloId);
                cmd.Parameters.AddWithValue("@cantidad", cantidad);
                await cmd.ExecuteNonQueryAsync();
            }
        }

        private static async Task ActualizarCantidadExistente(MySqlConnection connection, int articuloId, int cantidadExistente, int cantidad)
        {
            string sql = "UPDATE carrito SET cantidad = @cantidad WHERE ID = @ID";
            using (MySqlCommand cmd = new MySqlCommand(sql, connection))
            {
                cmd.Parameters.AddWithValue("@ID", articuloId);
                cmd.Parameters.AddWithValue("@cantidad", cantidadExistente + cantidad);
                await cmd.ExecuteNonQueryAsync();
            }
        }

        private static async Task DisminuirExistenciaArticulo(MySqlConnection connection, int articuloId, int cantidad)
        {
            string sql = "UPDATE articulos SET existencia = existencia - @cantidad WHERE ID = @ID";
            using (MySqlCommand cmd = new MySqlCommand(sql, connection))
            {
                cmd.Parameters.AddWithValue("@ID", articuloId);
                cmd.Parameters.AddWithValue("@cantidad", cantidad);
                await cmd.ExecuteNonQueryAsync();
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
