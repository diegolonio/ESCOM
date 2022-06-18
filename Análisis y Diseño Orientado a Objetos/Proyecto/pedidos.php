<?php 
    require 'sesion.php';

    $ventas = $conexion->prepare("SELECT * FROM venta WHERE IdUsuario = :id ORDER BY FECHA DESC");
    $ventas->execute(
        array(
            ':id' => $_SESSION['usuario']
        )
    );
    $ventas = $ventas->fetchAll();
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="./img/favicon.ico" type="image/x-icon">
    <link rel="icon" href="./img/favicon.ico" type="image/x-icon">
    <title>Mercury | Pedidos</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/pedidos.css">
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>
</head>
<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="./index.php"><img src="./img/mercury_logo.png" alt="Mercury Logo" width="80%" height="80%"></a>
            </div>
            <div class="botones_cuenta">
                <div class="cuenta">
                    <a href="./gestionar_cuenta.php" class="boton_cuenta"><p><?php echo $datos['Nombre']." ".substr ($datos['Apellido_P'], 0, 1)."."; ?></p></a>
                    <div class="imagen_cuenta">
                        <img src="./img/user.png" alt="Imagen usuario">
                    </div>
                </div>
                <div class="cerrar_sesion">
                    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                        <button type="submit" name="cerrar_sesion" value="ok">Cerrar Sesión</button>
                    </form>
                </div>
            </div>
        </header>
        <div class="contenido">
            <div class="titulo">
                <h1>PEDIDOS</h1>
            </div>
            <div class="pedidos">
                <?php foreach ($ventas as $venta): ?>
                <?php
                    $productos = $conexion->prepare("SELECT * FROM compra c JOIN producto p ON c.IdProducto = p.IdProducto WHERE IdVenta = :id");
                    $productos->execute(
                        array(
                            ':id' => $venta['IdVenta']
                        )
                    );
                    $productos = $productos->fetchAll();
                ?>
                <div class="pedido">
                    <div class="nombre">
                        <p>Compra #<?php echo $venta['IdVenta']; ?></p>
                    </div>
                <?php foreach ($productos as $producto): ?>
                    <div class="datos_pedido">
                        <p>Producto: <span><?php echo $producto['Nombre']; ?></span></p>
                        <p>Cantidad: <span><?php echo $producto['Cantidad']; ?></span></p>
                    </div>
                <?php endforeach; ?>
                    <div class="total">
                        <p>Total: <span>$<?php echo $venta['Total']; ?>.00</span></p>
                    </div>
                    <div class="estatus">
                        <p>Estatus: <span><?php if ($venta['Estado']) {echo 'Enviado';} else {echo 'Pendiente';} ?></span></p>
                        <!-- <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                            <button type="submit" name="enviado" value="4567">Enviado</button>
                        </form> -->
                    </div>
                </div>
                <?php endforeach; ?>
            </div>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>