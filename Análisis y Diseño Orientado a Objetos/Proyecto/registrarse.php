<?php
session_start();

if (isset($_SESSION['usuario'])) {
    header('Location: index.php');
}

require 'conexion.php';

include_once './conexion.php';

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

require './PHPMailer/Exception.php';
require './PHPMailer/PHPMailer.php';
require './PHPMailer/SMTP.php';

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $rol = $_POST['rol'];
    $nombre = $_POST['nombre'];
    $apellido_paterno = $_POST['apellido_paterno'];
    $apellido_materno = $_POST['apellido_materno'];
    $fecha_nacimiento = $_POST['fecha_nacimiento'];
    $email = $_POST['email'];
    $contrasenia = $_POST['contrasenia'];
    $telefono = $_POST['telefono'];
    $rfc = $_POST['rfc'];

    $rfc_valido = preg_grep('/^([A-ZÑ\x26]{3,4}([0-9]{2})(0[1-9]|1[0-2])(0[1-9]|1[0-9]|2[0-9]|3[0-1])([A-Z]|[0-9]){2}([A]|[0-9]){1})?$/', [$rfc]);

    $aceptacion_terminos = $_POST['aceptacion_terminos'];

    if (empty($rfc)) {
        $codigo = $conexion->prepare("SELECT Codigo FROM codigo WHERE IdCodigo = :id");
        $codigo->execute(
            array(
                ':id' => rand(1, 4)
            )
        );
        $codigo = $codigo->fetch();

        $temporal = $conexion->prepare("INSERT INTO temporal (Correo, Nombre, Apellido_P, Apellido_M, Fecha, Telefono, Contrasena, RFC, Vendedor, Codigo) VALUES(:Correo, :Nombre, :Apellido_P, :Apellido_M, :Fecha, :Telefono, :Contrasena, :RFC, :Vendedor, :Codigo)");
        $temporal->execute(
            array(
                ':Correo' => $email,
                ':Nombre' => $nombre,
                ':Apellido_P' => $apellido_paterno,
                ':Apellido_M' => $apellido_materno,
                ':Fecha' => $fecha_nacimiento,
                ':Telefono' => $telefono,
                ':Contrasena' => $contrasenia,
                ':RFC' => $rfc,
                ':Vendedor' => $rol,
                ':Codigo' => $codigo['Codigo']
            )
        );

        echo "<script>window.location.href='confirmacion.php';</script>";
    } else {
        if (isset($rfc_valido) && !empty($rfc_valido)) {
            $codigo = $conexion->prepare("SELECT Codigo FROM codigo WHERE IdCodigo = :id");
            $codigo->execute(
                array(
                    ':id' => rand(1, 4)
                )
            );
            $codigo = $codigo->fetch();

            $temporal = $conexion->prepare("INSERT INTO temporal (Correo, Nombre, Apellido_P, Apellido_M, Fecha, Telefono, Contrasena, RFC, Vendedor, Codigo) VALUES(:Correo, :Nombre, :Apellido_P, :Apellido_M, :Fecha, :Telefono, :Contrasena, :RFC, :Vendedor, :codigo)");
            $temporal->execute(
                array(
                    ':Correo' => $email,
                    ':Nombre' => $nombre,
                    ':Apellido_P' => $apellido_paterno,
                    ':Apellido_M' => $apellido_materno,
                    ':Fecha' => $fecha_nacimiento,
                    ':Telefono' => $telefono,
                    ':Contrasena' => $contrasenia,
                    ':RFC' => $rfc,
                    ':Vendedor' => $rol,
                    ':codigo' => $codigo['Codigo']
                )
            );

            echo "<script>window.location.href='confirmacion.php';</script>";
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
    <title>Mercury | Registrarse</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado_registro.css">
    <link rel="stylesheet" href="./css/registro.css">

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
                validators: {
                    regExp: {
                        regname: {
                            pattern: /^[a-zA-Z]{1,45}$/,
                            errorMessage: 'Solo se permiten letras del abecedario: Mayúsculas y minúsculas'
                        },
                        regemail: {
                            pattern: /^(\W|^)[\w.\-]{0,25}@(hotmail|gmail)\.com(\W|$)$/,
                            errorMessage: 'Correo electrónico no válido'
                        },
                        regpass: {
                            pattern: /^(?=.*\d)(?=.*[\u0021-\u002b\u003c-\u0040])(?=.*[A-Z])(?=.*[a-z])\S{8,16}$/,
                            errorMessage: 'La contraseña debe tener al entre 8 y 16 caracteres, al menos un dígito, al menos una minúscula, al menos una mayúscula y al menos un caracter no alfanumérico'
                        }
                    }
                }
            });
        });
    </script>

</head>

<body>
    <div class="contenedor_principal">
        <header class="encabezado">
            <div class="logo">
                <a href="index.php"><img src="./img/mercury_logo.png" alt="Mercury Logo">
            </div>
            </a>
            <h1>REGISTRO</h1>
        </header>
        <div class="contenido">
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <div class="roles">
                    <div class="rol_comprador">
                        <label for="comprador">Comprador</label>
                        <input type="radio" name="rol" value="0" id="comprador" onclick="changeStyle()">
                    </div>
                    <div class="rol_vendedor">
                        <label for="vendedor">Vendedor</label>
                        <input type="radio" name="rol" value="1" id="vendedor" onclick="changeStyle2()" checked>
                    </div>
                </div>
                <div class="datos_personales">
                    <input type="text" name="nombre" id="nombre" placeholder="Nombre(s)" data-validetta="required,maxLength[25],regExp[regname]">
                    <input type="text" name="apellido_paterno" id="apellido_paterno" placeholder="Apellido paterno" data-validetta="required,maxLength[25],regExp[regname]">
                    <input type="text" name="apellido_materno" id="apellido_materno" placeholder="Apellido materno" data-validetta="maxLength[25],regExp[regname]">
                    <div class="fecha">
                        <label for="fecha_nacimiento">Fecha de nacimiento</label>
                        <input type="date" name="fecha_nacimiento" id="fecha_nacimiento" data-validetta="required" max="2003-06-03">
                    </div>
                </div>
                <div class="datos_contacto">
                    <input type="text" name="email" id="email" placeholder="Correo electrónico" data-validetta="required,regExp[regemail]">
                    <input type="password" name="contrasenia" id="contrasenia" placeholder="Contraseña" data-validetta="required,minLength[8],maxLength[16],regExp[regpass]">
                    <input type="tel" name="telefono" id="telefono" placeholder="Teléfono" data-validetta="required,number,minLength[10],maxLength[10]">
                    <input type="text" name="rfc" id="rfc" placeholder="RFC">
                </div>
                <?php if (isset($rfc_valido) && empty($rfc_valido)) : ?>
                    <div class="rfc_no_valido">
                        <p>El RFC no es válido.</p>
                    </div>
                <?php endif; ?>
                <div class="confirmacion">
                    <div class="declaracion">
                        <input type="checkbox" name="aceptacion_terminos" id="aceptacion_terminos" data-validetta="required">
                        <div class="declaracion_texto">
                            <label for="aceptacion_terminos">Declaro que soy mayor de edad, acepto los Términos y
                                Condiciones y autorizo el uso de mis datos de acuerdo a la Declaración de
                                Privacidad.</label>
                        </div>
                    </div>
                    <button type="submit">Continuar</button>
                </div>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
</body>

</html>

<script>
    function changeStyle() {
        var element = document.getElementById("rfc");
        element.style.display = "none";
    }

    function changeStyle2() {
        var element = document.getElementById("rfc");
        element.style.display = "initial";
    }
</script>