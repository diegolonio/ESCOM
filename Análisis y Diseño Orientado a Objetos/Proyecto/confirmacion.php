<?php

require 'conexion.php';

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $d1 = $_POST['digito_1'];
    $d2 = $_POST['digito_2'];
    $d3 = $_POST['digito_3'];
    $d4 = $_POST['digito_4'];
    $codigo_ingresado = $d1 . $d2 . $d3 . $d4;

    $id_temporal = $conexion->prepare("SELECT MAX(IdTemporal) AS ID FROM temporal");
    $id_temporal->execute();
    $id_temporal = $id_temporal->fetch();

    $obtener_temporal = $conexion->prepare("SELECT * FROM temporal WHERE IdTemporal = :id");
    $obtener_temporal->execute(
        array (
            ':id' => $id_temporal[0]
        )
    );
    $obtener_temporal = $obtener_temporal->fetch();

    if ($codigo_ingresado == $obtener_temporal['Codigo']) {
        $registrarUsuario = $conexion->prepare("INSERT INTO usuario (Correo, Nombre, Apellido_P, Apellido_M, Fecha, Telefono, Contrasena, RFC, Vendedor) VALUES(:Correo, :Nombre, :Apellido_P, :Apellido_M, :Fecha, :Telefono, :Contrasena, :RFC, :Vendedor)");
        $registrarUsuario->execute(
            array(
                ':Correo' => $obtener_temporal['Correo'],
                ':Nombre' => $obtener_temporal['Nombre'],
                ':Apellido_P' => $obtener_temporal['Apellido_P'],
                ':Apellido_M' => $obtener_temporal['Apellido_M'],
                ':Fecha' => $obtener_temporal['Fecha'],
                ':Telefono' => $obtener_temporal['Telefono'],
                ':Contrasena' => $obtener_temporal['Contrasena'],
                ':RFC' => $obtener_temporal['RFC'],
                ':Vendedor' => $obtener_temporal['Vendedor']
            )
        );

        $obtener_id = $conexion->prepare("SELECT IdUsuario FROM usuario WHERE IdUsuario = (SELECT MAX(IdUsuario) FROM usuario)");
        $obtener_id->execute();
        $obtener_id = $obtener_id->fetch();

        $registrarDireccion = $conexion->prepare("INSERT INTO direccion (IdUsuario) VALUES(:IdUsuario)");
        $registrarDireccion->execute(
            array(
                ':IdUsuario' => $obtener_id[0]
            )
        );

        $registrarBanco = $conexion->prepare("INSERT INTO banco (IdUsuario) VALUES(:IdUsuario)");
        $registrarBanco->execute(
            array(
                ':IdUsuario' => $obtener_id[0]
            )
        );

        echo "<script>alert('Su cuenta ha sido creada con éxito.');window.location.href='index.php';</script>";
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
    <title>Mercury | Confirmación del correo electrónico</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado_confirmacion.css">
    <link rel="stylesheet" href="./css/confirmacion.css">
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>
</head>

<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="index.php">
                    <img src="./img/mercury_logo.png" alt="Mercury Logo">
                </a>
            </div>
            <h1>INICIAR SESIÓN</h1>
        </header>
        <div class="contenido">
            <section class="tarjeta_digitos">
                <p>Ingresa el código que te hemos enviado<br>por e-mail.</p>
                <p>Lo enviaremos a correo@correo.com para confirmar que te pertenece.</p>
                <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                    <div class="inputs_digitos">
                        <input type="text" name="digito_1" id="digito_1" maxlength="1">
                        <input type="text" name="digito_2" id="digito_2" maxlength="1">
                        <input type="text" name="digito_3" id="digito_3" maxlength="1">
                        <input type="text" name="digito_4" id="digito_4" maxlength="1">
                    </div>
                    <button type="submit">Continuar</button>
                </form>
            </section>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>

</html>