using System;
using System.IO;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Azure.Functions.Worker.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using MySql.Data.MySqlClient;

namespace ServicioWeb
{
    public static class InsertarArticulo
    {
        [Function("insertar_articulo")]
        public static async Task<HttpResponseData> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "post", Route = "insertar_articulo")] HttpRequestData req,
            FunctionContext context)
        {
            var logger = context.GetLogger("InsertarArticulo");

            try
            {
                string requestBody;
                using (StreamReader reader = new StreamReader(req.Body))
                {
                    requestBody = await reader.ReadToEndAsync();
                }

                ParamInsertarArticulo paramInsertarArticulo = JsonConvert.DeserializeObject<ParamInsertarArticulo>(requestBody);
                Articulo articulo = paramInsertarArticulo.articulo;

                if (string.IsNullOrEmpty(articulo.nombre))
                    return await ConstructBadRequestResponseAsync(req, "Se debe ingresar el nombre del producto.");

                if (string.IsNullOrEmpty(articulo.descripcion))
                    return await ConstructBadRequestResponseAsync(req, "Se debe ingresar la descripción del producto.");

                if (articulo.precio < 0)
                    return await ConstructBadRequestResponseAsync(req, "Precio de producto no válido.");

                if (articulo.existencia < 0)
                    return await ConstructBadRequestResponseAsync(req, "Existencias de producto no válidas.");

                if (articulo.relevancia < 1 || articulo.relevancia > 100)
                    return await ConstructBadRequestResponseAsync(req, "La relevancia del producto debe estar entre 1 y 100.");

                string connectionString = Environment.GetEnvironmentVariable("MYSQLConnectionString");
                await using (MySqlConnection connection = new MySqlConnection(connectionString))
                {
                    await connection.OpenAsync();
                    await using (MySqlTransaction transaction = await connection.BeginTransactionAsync())
                    {
                        try
                        {
                            await using (MySqlCommand insertCmd = connection.CreateCommand())
                            {
                                insertCmd.Transaction = transaction;
                                insertCmd.CommandText = @"INSERT INTO articulos(nombre, descripcion, precio, existencia, relevancia) 
                                                          VALUES (@nombre, @descripcion, @precio, @existencia, @relevancia)";
                                insertCmd.Parameters.AddWithValue("@nombre", articulo.nombre);
                                insertCmd.Parameters.AddWithValue("@descripcion", articulo.descripcion);
                                insertCmd.Parameters.AddWithValue("@precio", articulo.precio);
                                insertCmd.Parameters.AddWithValue("@existencia", articulo.existencia);
                                insertCmd.Parameters.AddWithValue("@relevancia", articulo.relevancia);
                                await insertCmd.ExecuteNonQueryAsync();
                            }

                            if (articulo.foto != null)
                            {
                                await using (MySqlCommand insertImgCmd = connection.CreateCommand())
                                {
                                    insertImgCmd.Transaction = transaction;
                                    insertImgCmd.CommandText = @"INSERT INTO imagenes(ID, imagen) 
                                                                VALUES (LAST_INSERT_ID(), @imagen)";
                                    insertImgCmd.Parameters.AddWithValue("@imagen", articulo.foto);
                                    await insertImgCmd.ExecuteNonQueryAsync();
                                }
                            }

                            await transaction.CommitAsync();
                        }
                        catch (Exception ex)
                        {
                            logger.LogError($"Error al insertar el artículo: {ex.Message}");
                            await transaction.RollbackAsync();
                            return await ConstructBadRequestResponseAsync(req, ex.Message);
                        }
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

        private static async Task<HttpResponseData> ConstructBadRequestResponseAsync(HttpRequestData req, string message)
        {
            var response = req.CreateResponse(HttpStatusCode.BadRequest);
            response.Headers.Add("Content-Type", "application/json");
            await response.WriteStringAsync(JsonConvert.SerializeObject(new { error = message }), Encoding.UTF8);
            return response;
        }
    }
}
