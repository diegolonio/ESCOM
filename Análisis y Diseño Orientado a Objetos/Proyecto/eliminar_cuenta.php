<?php 
    require 'sesion.php';
    
    if(!isset($_SESSION['usuario']))
    {
        header('location: index.php');
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        if (isset ($_POST['eliminar']) && $_POST['eliminar'])
        {
            $eliminar_direccion = $conexion->prepare("DELETE FROM direccion WHERE IdUsuario = :id");
			$eliminar_direccion->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );

            $eliminar_tarjeta = $conexion->prepare("DELETE FROM banco WHERE IdUsuario = :id");
			$eliminar_tarjeta->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );

            $eliminar_cuenta = $conexion->prepare("DELETE FROM usuario WHERE IdUsuario = :id");
			$eliminar_cuenta->execute(
                array(
                    ':id' => $_SESSION['usuario']
                )
            );
            $eliminar_cuenta = $eliminar_cuenta->rowCount();

            if ($eliminar_cuenta == 1)
            {
                session_destroy ();

                echo "<script>alert('Su cuenta ha sido eliminada.');window.location.href='index.php';</script>";
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
    <title>Mercury | Gestionar cuenta</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado.css">
    <link rel="stylesheet" href="./css/eliminar_cuenta.css">
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
                        <button type="submit" name="cerrar_sesion" value="TRUE">Cerrar Sesión</button>
                    </form>
                </div>
            </div>
        </header>
        <div class="contenido">
            <div class="contenedor_imagen">
                <img src="./img/seniora_llorando.png" alt="Señora llorando">
            </div>
            <div class="formulario">
                <div class="titulo">
                    <h1>Eliminar cuenta</h1>
                </div>
                <div class="advertencia">
                    <p>Estás a punto de eliminar tu cuenta<br>de forma permanente, si está<br>seguro y desea continuar presione<br>'Aceptar'.</p>
                </div>
                <div class="botones">
                    <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                        <a href="gestionar_cuenta.php">Cancelar</a>
                        <button type="submit" name="eliminar" value="ok">Aceptar</button>
                    </form>
                </div>
            </div>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>
</html>