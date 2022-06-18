<?php 
    session_start ();

    require 'conexion.php';

    if (isset ($_SESSION['usuario']))
    {
        $consulta = $conexion->prepare("SELECT Nombre, Apellido_P FROM usuario WHERE IdUsuario = :IdUsuario LIMIT 1");
        $consulta->execute(array(':IdUsuario' => $_SESSION['usuario']));
        $datos = $consulta->fetch();
    }

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
        if (isset ($_POST['cerrar_sesion']) && $_POST['cerrar_sesion'] == 'ok')
        {
            session_destroy ();
            header ('Location: index.php');
        }
    }
