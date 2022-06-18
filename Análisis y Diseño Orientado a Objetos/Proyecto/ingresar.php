<?php 
    session_start ();

    if (isset ($_SESSION['usuario']))
    {
        header ('Location: index.php');
    }

    require "conexion.php";
    require_once 'recaptcha/src/autoload.php';

    $recaptcha = new \ReCaptcha\ReCaptcha('6Le_-A4bAAAAAGEYSdD2bgs3_k6zywFsBZS9uzqV');

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        $response = $recaptcha->verify($_POST['g-recaptcha-response']);
        
        if($response->isSuccess())
        {
            $correo = trim ($_POST['correo']);
            $correo = filter_var ($correo, FILTER_SANITIZE_STRING);
            
            $contrasenia = trim ($_POST['contrasenia']);
            $contrasenia = filter_var ($contrasenia, FILTER_SANITIZE_STRING);
            
            $consulta = $conexion->prepare("SELECT IdUsuario FROM Usuario WHERE Correo = :correo AND Contrasena = :contrasenia LIMIT 1");
            $consulta->execute(array(':correo' => $correo, ':contrasenia' => $contrasenia));
            $datos = $consulta->fetch();
            
            if (isset ($datos) && !empty ($datos))
            {
                $_SESSION['usuario'] = $datos['IdUsuario'];                    
                echo "<script>alert('Inicio de sesión exitoso.');window.location.href='index.php';</script>";
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
    <title>Mercury | Ingresar</title>
    <link rel="stylesheet" href="./css/general.css">
    <link rel="stylesheet" href="./css/encabezado_ingresar.css">
    <link rel="stylesheet" href="./css/ingresar.css">
    
    <link href="./css/validetta.css" rel="stylesheet" type="text/css" media="screen">
    <script src="./js/jquery-3.6.0.min.js"></script>
    <script type="text/javascript" src="http://code.jquery.com/jquery-latest.min.js"></script>
    <script type="text/javascript" src="./js/validetta.js"></script>
    <script type="text/javascript" src="./js/validettaLang-es-ES.js"></script>
    <script src="https://kit.fontawesome.com/552da2da03.js" crossorigin="anonymous"></script>

    <script>
        $(document).ready(function () {
            $("#registro").validetta({
                bubblePosition: 'bottom',
                bubbleGapTop: 10,
                bubbleGapLeft: -5,
                validators: {
                regExp: {
                regemail:
                {
                    pattern: /^(\W|^)[\w.\-]{0,25}@(hotmail|gmail)\.com(\W|$)$/,
                    errorMessage: 'Correo electrónico no válido'
                },
                regpass:
                {
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
            <div class="contenedor_logo">
                <div class="logo">
                    <a href="index.php"><img src="./img/mercury_logo.jpg" alt="Mercury Logo"></a>
                </div>
            </div>
            <h1>INICIAR SESIÓN</h1>
        </header>
        <div class="contenido">
            <div class="contenedor_sujeto">
                <img src="./img/sujeto_1.png" alt="Sujeto de morado">
            </div>
            <form id="registro" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST">
                <div class="contenedor_correo">
                    <label for="usuario">Usuario</label>
                    <input type="text" name="correo" id="usuario" placeholder="Correo electrónico" data-validetta="required,regExp[regemail]">
                </div>
                <div class="contenedor_contrasenia">
                    <label for="contrasenia">Contraseña</label>
                    <input type="password" name="contrasenia" id="contrasenia" placeholder="Contraseña" data-validetta="required,minLength[8],maxLength[16]">
                    <a href="#">¿Has olvidado tu contraseña?</a>
                </div>
                <div class="contenedor_captcha">
                    <div class="g-recaptcha" data-sitekey="6Le_-A4bAAAAAGT22nMEv0Ak-A9Zj_wKtbKYrINw"></div>
                </div>
                <?php if (isset ($datos) && empty ($datos)): ?>
                    <div class="mensaje_error">
                        <p>Correo o contraseña incorrectos.</br>Por favor inténtalo de nuevo.</p>
                    </div>
                <?php endif; ?>
                <?php if (isset ($response) && !$response->isSuccess()): ?>
                    <div class="mensaje_error">
                        <p>Por favor ingresa el captcha.</p>
                    </div>
                <?php endif; ?>
                <div class="contenedor_botones">
                    <a href="registrarse.php">Registrarse</a>
                    <button type="submit" onclick = "validar()">Continuar</button>
                </div>
            </form>
        </div>
        <footer class="pie">
            <p>MERCURY 2021 &copy; Reservados todos los derechos.</p>
        </footer>
    </div>
    <script src="https://www.google.com/recaptcha/api.js" async defer></script>
</body>
</html>
