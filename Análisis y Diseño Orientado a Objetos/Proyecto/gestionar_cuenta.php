<?php 
    require 'sesion.php';
    
    if(!isset($_SESSION['usuario']))
    {
        header('location: index.php');
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
    <title>Mercury | Gestionar cuenta</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/gestionar_cuenta.css">
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
                    <a href="./cuenta.php" class="boton_cuenta"><p><?php echo $datos['Nombre']." ".substr ($datos['Apellido_P'], 0, 1)."."; ?></p></a>
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
                <h1>GESTIONAR CUENTA</h1>
            </div>
            <a href="pedidos.php">
                <div class="seccion">
                    <h2>Consultar pedidos</h2><i class="fas fa-receipt"></i>
                </div>
            </a>
            <a href="modificar_datos_inicio_sesion.php">
                <div class="seccion">
                    <h2>Cambiar datos de inicio de sesión</h2><i class="fas fa-user"></i>
                </div>
            </a>
            <a href="modificar_domicilio.php">
                <div class="seccion">
                    <h2>Cambiar datos del domicilio</h2><i class="fas fa-map-marked-alt"></i>
                </div>
            </a>
            <a href="modificar_datos_bancarios.php">
                <div class="seccion">
                    <h2>Cambiar datos bancarios</h2><i class="fas fa-money-bill-wave"></i>
                </div>
            </a>
            <a href="eliminar_cuenta.php">
                <div class="seccion">
                    <h2>Eliminar cuenta</h2><i class="fas fa-times-circle"></i>
                </div>
            </a>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>