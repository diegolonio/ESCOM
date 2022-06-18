<?php
	// Datos conexión DB
	$conexionHost = 'localhost';
	$conexionDB = 'tw';
	$conexionUser = 'root';
	$conexionPassword = '';

	// Conexión PDO
	try {
		$conexion = new PDO("mysql:host=$conexionHost;dbname=$conexionDB;charset=utf8", $conexionUser, $conexionPassword);
		$conexion->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	} catch (PDOException $e) {
		echo "Error: ".$e->getMessage();
		die();
	}