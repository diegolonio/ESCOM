<?php
require 'sesion.php';

$subtotal = 0;

$productos = $conexion->prepare("SELECT producto.Imagen, producto.Nombre, carrito.Cantidad, producto.Precio FROM carrito, producto WHERE idUsuario = :id AND carrito.IdProducto = producto.IdProducto;");
$productos->execute(
    array(
        ':id' => $_SESSION['usuario'],
    )
);
$productos = $productos->fetchAll();

$direcciones = $conexion->prepare("SELECT IdDireccion, Direccion FROM direccion WHERE IdUsuario = :id");
$direcciones->execute(
    array(
        ':id' => $_SESSION['usuario'],
    )
);
$direcciones = $direcciones->fetchAll();

$formas_pago = $conexion->prepare("SELECT IdBanco, NoTarjeta FROM banco WHERE IdUsuario = :id");
$formas_pago->execute(
    array(
        ':id' => $_SESSION['usuario'],
    )
);
$formas_pago = $formas_pago->fetchAll();

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $direccion = $_POST['direccion'];
    $direccion = trim($direccion);
    $forma_pago = $_POST['forma_pago'];
    $forma_pago = trim($forma_pago);
    $CVV = $_POST['cvv'];
    $CVV = trim($CVV);

    if (isset($direccion) && !empty($direccion) && isset($forma_pago) && !empty($forma_pago) && isset($CVV) && !empty($CVV)) {
        $obtener_cvv = $conexion->prepare("SELECT CVV FROM banco WHERE IdUsuario = :id LIMIT 1");
        $obtener_cvv->execute(
            array(
                ':id' => $_SESSION['usuario']
            )
        );
        $obtener_cvv = $obtener_cvv->fetch();

        if ($CVV == $obtener_cvv['CVV']) {
            $carrito = $conexion->prepare("SELECT * FROM carrito WHERE IdUsuario = :id");
            $carrito->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );
            $carrito = $carrito->fetchAll();

            $total = 0;
            $aux = 0;

            foreach ($carrito as $producto) {
                $id_producto = $producto['IdProducto'];
                $cantidad = $producto['Cantidad'];

                $info_producto = $conexion->prepare("SELECT IdProducto, Existencias, Precio FROM producto WHERE IdProducto = :id LIMIT 1");
                $info_producto->execute(
                    array(
                        ':id' => $id_producto
                    )
                );
                $info_producto = $info_producto->fetch();

                if ($cantidad > $info_producto['Existencias']) {
                    $eliminar_existencias = $conexion->prepare("DELETE FROM carrito WHERE IdProducto = :producto AND IdUsuario = :id");
                    $eliminar_existencias->execute(
                        array(
                            ':producto' => $id_producto,
                            ':id' => $_SESSION['usuario']
                        )
                    );
                    $eliminar_existencias = $eliminar_existencias->rowCount();
                } else {
                    $existencias_nuevas = $info_producto['Existencias'] - $cantidad;

                    $actualizar_carrito = $conexion->prepare("UPDATE producto SET Existencias = :cantidad WHERE IdProducto = :producto");
                    $actualizar_carrito->execute(
                        array(
                            ':cantidad' => $existencias_nuevas,
                            ':producto' => $info_producto['IdProducto']
                        )
                    );
                    $actualizar_carrito = $actualizar_carrito->rowCount();

                    $aux = $cantidad * $info_producto['Precio'];
                    $total += $aux;
                }
            }

            $carrito = $conexion->prepare("INSERT INTO venta (IdUsuario, Total, Estado, Fecha) VALUES (:id, :total, :estado, CURRENT_TIMESTAMP())");
            $carrito->execute(
                array(
                    ':id' => $_SESSION['usuario'],
                    ':total' => $total,
                    ':estado' => 0
                )
            );
            $carrito = $carrito->rowCount();

            $id_venta = $conexion->lastInsertId();

            $asociar = $conexion->prepare("SELECT IdProducto, Cantidad FROM carrito WHERE IdUsuario = :id");
            $asociar->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );
            $asociar = $asociar->fetchAll();

            foreach ($asociar as $dato) {
                $nueva_compra = $conexion->prepare("INSERT INTO compra (IdVenta, IdProducto, Cantidad) VALUES(:venta, :producto, :cantidad)");
                $nueva_compra->execute(
                    array(
                        ':venta' => $id_venta,
                        ':producto' => $dato['IdProducto'],
                        ':cantidad' => $dato['Cantidad']
                    )
                );
            }

            $limpiar_carrito = $conexion->prepare("DELETE FROM carrito WHERE IdUsuario = :id");
            $limpiar_carrito->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );

            echo "<script>alert('Compra exitosa.');window.location.href='index.php';</script>";
        } else {
            echo "<script>alert('Debes revisar los datos bancarios');</script>";
        }
    }
}
?>

<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="./img/favicon.ico" type="image/x-icon">
    <link rel="icon" href="./img/favicon.ico" type="image/x-icon">
    <title>Mercury | Compra</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/compra.css">

    <link href="./css/validetta.css" rel="stylesheet" type="text/css" media="screen">
    <script src="./js/jquery-3.6.0.min.js"></script>
    <script type="text/javascript" src="http://code.jquery.com/jquery-latest.min.js"></script>
    <script type="text/javascript" src="./js/validetta.js"></script>
    <script type="text/javascript" src="./js/validettaLang-es-ES.js"></script>
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>

    <script>
        $(document).ready(function() {
            $("#registro").validetta({
                bubblePosition: 'bottom',
                bubbleGapTop: 10,
                bubbleGapLeft: -5,
            });
        });
    </script>

</head>

<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="./index.php"><img src="./img/mercury_logo.png" alt="Mercury Logo" width="80%" height="80%"></a>
            </div>
            <nav class="categorias">
                <a href="#" class="boton_categorias">Categorías</a>
                <div class="enlaces">
                    <a href="electronicos.php">Electrónicos</a>
                    <a href="salud_belleza.php">Salud y belleza</a>
                    <a href="alimentos.php">Alimentos</a>
                    <a href="ropa_accesorios.php">Ropa y accesorios</a>
                    <a href="hogar.php">Hogar</a>
                    <a href="juguetes.php">Juguetes</a>
                    <a href="deportes.php">Deportes</a>
                    <a href="entretenimiento.php">Entretenimiento</a>
                </div>
            </nav>
            <div class="barra_navegacion">
                <form action="busqueda.php" method="GET" autocomplete="off">
                    <div class="icono_lupa">
                        <i class="fas fa-search"></i>
                    </div>
                    <input type="text" name="busqueda" autocomplete="off" placeholder="Busca los productos que necesitas">
                </form>
            </div>
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="carrito">
                    <a href="./carrito.php" class="boton_carrito"><i class="fas fa-shopping-cart"></i></a>
                </div>
            <?php else : ?>
                <div class="ingresar">
                    <a href="./ingresar.php" class="boton_ingresar">Ingresar</a>
                </div>
            <?php endif; ?>
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="cuenta">
                    <a href="./gestionar_cuenta.php" class="boton_cuenta">
                        <p><?php echo $datos['Nombre'] . " " . substr($datos['Apellido_P'], 0, 1) . "."; ?></p>
                    </a>
                    <div class="imagen_cuenta">
                        <img src="./img/user.png" alt="Imagen usuario">
                    </div>
                </div>
            <?php else : ?>
                <div class="registro">
                    <a href="./registrarse.php" class="boton_registro">Registrarse</a>
                </div>
            <?php endif; ?>
            <?php if (isset($_SESSION['usuario'])) : ?>
                <div class="cerrar_sesion">
                    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                        <button type="submit" name="cerrar_sesion" value="ok">Cerrar Sesión</button>
                    </form>
                </div>
            <?php endif; ?>
        </header>
        <div class="contenido">
            <div class="cesta">
                <div class="texto">
                    <h2>Cesta</h2>
                    <hr>
                </div>
                <div class="panel_productos">
                    <?php foreach ($productos as $producto): ?>
                    <?php $subtotal += $producto['Cantidad'] * $producto['Precio']; ?>
                        <div class="tarjeta_producto">
                            <div class="img_producto">
                                <img src="./img/<?php echo $producto['Imagen']; ?>" alt="<?php echo $producto['Nombre']; ?>">
                            </div>
                            <p><?php echo $producto['Nombre']; ?></p>
                            <p class="cantidad">x(<?php echo $producto['Cantidad']; ?>)</p>
                            <p>$<?php echo $producto['Cantidad'] * $producto['Precio']; ?>.00 MXN</p>
                        </div>
                    <?php endforeach; ?>
                </div>
            </div>
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <h2>Dirección de envío</h2>
                <div class="contenedor_direccion">
                    <label for="direccion">Elegir dirección:</label>
                    <select name="direccion" id="direccion" data-validetta="required">
                        <option value="">-- Elegir dirección --</option>
                        <?php foreach ($direcciones as $direccion): ?>
                            <?php if (!empty ($direccion['Direccion'])): ?>
                                <option value="<?php echo $direccion['IdDireccion']; ?>"><?php echo $direccion['Direccion']; ?></option>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </select>
                    <a href="modificar_domicilio.php">Registrar dirección</a>
                </div>
                <h2>Pago</h2>
                <div class="contenedor_pago">
                    <label for="forma_pago">Elegir forma de pago:</label>
                    <select name="forma_pago" id="forma_pago" data-validetta="required">
                        <option value="">-- Elegir forma de pago --</option>
                        <?php foreach ($formas_pago as $forma_pago): ?>
                            <?php if (!empty ($forma_pago['NoTarjeta'])): ?>
                                <option value="<?php echo $forma_pago['IdBanco']; ?>"><?php echo $forma_pago['NoTarjeta']; ?></option>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </select>
                    <a href="modificar_datos_bancarios.php">Registrar forma de pago</a>
                </div>
                <div class="contenedor_cvv">
                    <label for="cvv">CVV</label>
                    <input type="text" name="cvv" id="cvv" placeholder="CVV" data-validetta="required,minLength[3],maxLength[3],number">
                </div>
                <div class="contenedor_subtotal">
                    <p>Subtotal (* artículos) productos:</p>
                    <p>$<?php echo $subtotal; ?>.00 MXN</p>
                </div>
                <div class="contenedor_costo_envio">
                    <p>Costo de envío:</p>
                    <p>Gratis</p>
                </div>
                <hr>
                <div class="contenedor_total">
                    <p>Total:</p>
                    <p>$<?php echo $subtotal; ?>.00 MXN</p>
                </div>
                <div class="contenedor_botones">
                    <button type="submit">FINALIZAR PEDIDO</button>
                    <a href="carrito.php">CANCELAR</a>
                </div>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>

</html>