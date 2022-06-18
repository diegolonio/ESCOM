drop database if exists tw;
create database tw;
use tw;

create table codigo (
	IdCodigo int auto_increment not null,
	Codigo int(4) NOT NULL,
	PRIMARY KEY (IdCodigo)
);

INSERT INTO codigo (Codigo) VALUES (4567), (8798), (2999), (9489);

create table admin (
	IdAdmin int auto_increment not null,
	Correo varchar(25) NOT NULL,
	Nombre varchar(25) not null,
    Apellido_P varchar(25) not null,
    Apellido_M varchar(25) not null,
	Contrasena varchar(10) NOT NULL,
	PRIMARY KEY (IdAdmin)
);

create table usuario
(
	IdUsuario int auto_increment not null,
    Nombre varchar(25) not null,
    Apellido_P varchar(25) not null,
    Apellido_M varchar(25) not null,
    Correo varchar(25) not null,
    Contrasena varchar(16) not null,
    Telefono varchar(10) not null,
    Vendedor boolean not null,
    Fecha date not null,
    RFC varchar(13),
    primary key(IdUsuario)
);

create table temporal
(
	IdTemporal int auto_increment not null,
    Nombre varchar(25) not null,
    Apellido_P varchar(25) not null,
    Apellido_M varchar(25) not null,
    Correo varchar(25) not null,
    Contrasena varchar(16) not null,
    Telefono varchar(10) not null,
    Vendedor boolean not null,
    Fecha date not null,
    RFC varchar(13),
    Codigo int(4),
    primary key(IdTemporal)
);

create table direccion
(
	IdDireccion int auto_increment not null,
	IdUsuario int not null,
    Ciudad text,
    CP int(5),
    Direccion text,
    Referencias text,
    foreign key(IdUsuario) references Usuario(IdUsuario) on delete cascade on update cascade,
    primary key (IdDireccion)
);

create table banco
(	
	IdUsuario int not null,
    IdBanco int auto_increment not null,
    NoTarjeta bigint(16),
    CVV int(3),
    Tipo boolean,
    foreign key(IdUsuario) references Usuario(IdUsuario) on delete cascade on update cascade,
    primary key (IdBanco)
);

create table categoria
(
	IdCategoria int auto_increment not null,
    Nombre varchar(45) not null,
	primary key(IdCategoria)
);

insert into categoria (Nombre) values('Electrónicos'), ('Salud y belleza'), ('Alimentos'), ('Ropa y accesorios'), ('Hogar'), ('Juguetes'), ('Deportes'), ('Entretenimiento');

create table producto
(
	IdProducto int auto_increment not null,
    IdCategoria int not null,
    Nombre text not null,
    Existencias int not null,
    Precio double not null,
    Descripcion text not null,
    Imagen text not null,
    Marca text not null,
    Estado text not null,
    Caracteristicas text not null,
    Valoracion float not null,
    foreign key(IdCategoria) references Categoria(IdCategoria) on delete cascade on update cascade,
    primary key(IdProducto)
);
insert into producto (IdCategoria, Nombre, Existencias, Precio, Descripcion, Imagen, Marca, Estado, Caracteristicas, Valoracion) values
(1, 'Amazon Echo Dot', 4, 1010, 'Amazon Echo Dot 4th Gen con asistente virtual Alexa charcoal 110V/240V.', 'Amazon_Doth.jpg', 'Amazon', 'Nuevo', '4th Gen con asistente virtual Alexa', 0.5),
(1, 'Xiaomi Redmi 9', 2, 2668, 'Xiaomi Redmi 9 (Global) Dual SIM 32 GB.', 'Red9.png', 'Xiaomi', 'Nuevo', '3 GB RAM', 2.5),
(1, 'Nintendo Switch Lite', 1, 5319, 'Nintendo Switch Lite 32GB color turquesa.', 'Switch.png', 'Nintendo', 'Nuevo', '32 GB', 4.5),
(1, 'Reloj Smart Watch', 7, 499, 'Reloj Smart Watch P8 Band Cardiaco Calorias Pasos Ecg Spo2', 'Reloj.png', 'Sin marca', 'Nuevo', 'Mide el ritmo cardiaco, cuenta los pasos del usuario y es resistente al agua', 0.5),
(1, 'Redlemon Audífonos', 10, 559, 'Audífonos RedLemon Bluetooth True Wireless Stereo.', 'Audifonos.jpg', 'Amazon', 'Nuevo', 'Tecnología bluetooth 5.0 de conexión automática.', 3.5),
(2, 'Serum', 15, 348, 'Serum Ácido Hialurónico Facial + Vitamina B5', 'Serum.png', 'QUOTIDIEN', 'Nuevo', 'Contiene Ácido Hialurónico Reticulado al 1.5% + Vitamina B5 y extracto de Bulbo de Azafrán.', 4.0),
(2, 'Crema Hidratante', 1, 499, 'Anti-líneas de expresion Revitalift Ácido Hialurónico Día de LOréal Paris.', 'Crema.png', 'LOREAL', 'Nuevo', 'Crema anti-líneas de expresión con Ácido Hialurónico Puro.Hidrata intensamente 24h. Protege del sol FPS 20. Uso recomendable a partir de +25 años.', 3.5),
(2, 'Cubrebocas', 25, 191, 'Cubrebocas plisado tricapa termosellado', 'Cubreboca.jpg', 'Sin marca', 'Nuevo', 'Filtro anti-bacterial, resistente a líquidos, desechable,ajuste nasal', 5.0),
(2, 'Cicatricure Aceite Biorestaurador', 5, 136, 'Cicatricure Cara y Cuerpo, piel visiblemente más suave, lisa y flexible.', 'Cicatricure.png', 'Cicatricure', 'Nuevo', 'La inovadora fórmula de Cicatricure Aceite restaurador hecha a base de una cuidadosa selección de aceites y vitamina E.', 2.5),
(2, 'WONU Colágeno', 6, 520, 'Colágeno Hidrolizado Sabor Fresa-Arándano.', 'WONU.png', 'WOMENS NUTRITION', 'Nuevo', 'Ácido Hialurónico + Vitamina C sin Azúcar Añadida', 4.5),
(3, 'Samyang Ramen Carbonara', 23, 48, 'Samyang Ramen Carbonara Buldak 140g.', 'Cubreboca.jpg', 'Samyang', 'Nuevo', 'Harina y Trigo', 3.5),
(3, 'Quaker Avena Old Fashion.', 5 ,67 , 'Sabor Natural, 1190 g.', 'Avena.jpg', 'Quaker', 'Nuevo','Ingredientes: Whole grain rolled oats, calcium carbonate, salt, guar gum caramel color, reduced iron vitamin, a palmitate.', 2.5),
(3, 'Leche Entera, Leche Santa Clara',7,270 , '12 - pack - 1l/caja.', 'Leche.jpg', 'Santa Clara', 'Nuevo', 'Ingredientes: Leche Entera de vaca con vitaminas A y D', 1.5),
(3, '1-2-3 Aceite Vegetal',20 ,34 , 'No contiene colesterol.', 'Aceite.jpg', '1-2-3', 'Nuevo', 'Ingredientes: Aceite de canola y/o girasol, 0.007% antioxidante TBHQ', 5.0),
(4, 'Wilson Tines', 7, 197, ' Tines para Hombre con Tejido Interior de Felpa Completa.', 'Wilson.jpg', 'Wilson', 'Nuevo','Contenido: 1 Paquete con 6 Pares de Tines para Caballero', 4.0),
(4, 'Playera cuello V caballero ', 24, 185 , '2 Piezas.', 'playera.jpg', 'Rinbros', 'Nuevo', '100% Algodón', 3.5),
(4, 'Camisa de Manga Larga ',5 ,613 , 'Botones Frontales, Resistente a Las Arrugas.', 'camisa.jpg', 'Nautica', 'Nuevo', '100 % Algodón.', 4.0),
(4, 'Bolso Crossbody', 2,889 , 'Lleva lo esencial de la vida diaria en esta nueva y elegante bolsa y adorna de estilo tu outfit.', 'bolso.jpg', 'Steve Madden BBolton', 'Nuevo', 'Material y forro sintetico', 3.5),
(5, 'Alfombra de piel sintética',7 ,441 , 'Suave, para la sala, rectangular, para recámara, agradable al tacto.', 'Alfombra.jpg', 'Gorilla Grip', 'Nuevo', 'Fuerte y duradera: perfecta para zonas con mucho tráfico en tu hogar; el fuerte respaldo de goma TP (nunca PVC) es muy resistente e ideal para un uso duradero.', 5.0),
(5, 'Servilletas.',13 , 45, 'Napkins 500 pieces.', 'regio.jpg', 'Regio', 'Nuevo', 'Cada servilleta de papel mide 27.5 por 24.5 centímetros', 2.5),
(5, 'Cobija de Tortilla de Harina',24 , 599, 'Manta de taco Gigante, Doble vista para Niños y Adultos.', 'cobija.jpg', 'Shenzhen Dasong Tech', 'Nuevo', 'Tamaño: 180cm', 5.0),
(5, 'Set Eco 1 L Verde/Negro ', 10,276 , 'Hidrátate sin perder el estilo.', 'set.jpg', 'Tupperware Eco Twist.', 'Nuevo', 'Tiene sello hermético para que nada se derrame. Diseño ergonómico para un agarre seguro.', 3.5),
(6, 'Bolsa Grande de Construcción', 2, 199, 'Práctico juego para el desarrollo de la primera infancia.', 'bloks.jpg', 'Mega Bloks', 'Nuevo', '80 bloques Juguete de Construcción para niñas de 1 a 5 años', 4.5),
(6, 'Figura de acción mandaloriana.',2 ,856 , 'El Mandaloriano es usado en batalla, un formidable cazarrecompensas en una galaxia cada vez más peligrosa.', 'figura.jpg', 'STAR WARS', 'Nuevo', 'Dimensiones: 22.9 x 3.8 x 15.2 centimeters.', 5.0),
(6, 'Nerf Lanzador N-Strike Elite Firestrike.',5, 219, 'Orientación por luz.', 'nerf.jpg', 'Nerf', 'Nuevo', 'Fija el objetivo hasta por 4.5 metros de distancia.', 3.5),
(6, 'Disney Raya and The Last Dragon.',5 ,139 , 'Caja Sorpresa con Figura y 2 Accesorios.', 'raya.jpg', 'Disney', 'Nuevo', 'Los niños y niñas recibirán aleatoriamente 1 de 8 personajes inspirados en la película de Disney Raya and the Last Dragon', 4.5),
(7, 'Vendas de mano.',10 , 301, 'Elasticidad perfecta, vendas de boxeo V4 estilo mexicano.', 'vendas.jpg', 'Hayabusa', 'Nuevo', 'El sistema de cierre de velcro extra ancho con trabilla para el pulgar y la etiqueta "This Side Up" garantizan un ajuste seguro y personalizado cada vez que entrenas', 1.5),
(7, 'Diademas Deportivas', 5, 299, 'Bandas para Correr, Entrenamiento Cruzado, Yoga y Bicicleta.', 'diademas.jpg', 'Athletico', 'Nuevo', 'Paquete de valor de 5 piezas: cada color / estilo de una pieza, obtendrá 5 piezas de diadema para mujer en total', 3.5),
(7, 'BARRAS DE PROTEÍNA',25 ,2425 , 'Pack de 24.', 'barras.jpg', 'DEXTRO ENERGY', 'Nuevo', 'Con carbohidratos rápidamente disponibles con deliciosas proteínas crujientes y solo 50 g de grasa.', 1.5),
(8, 'Memoria de elefante 5',5 , 143, 'Juegos para niños de 7 y 8 años.', 'elefante.jpg','Josep Lluís Martínez Picanyol', 'Nuevo', 'Pasta blanda – Texto grande', 5.0),
(8, 'Consola PS4 ', 1,8829 , 'Megapack 15 con Fallout 76.', 'ps4.jpg', 'Sony Interactive Entertainment LLC', 'Nuevo', 'Clasificación de los juegos incluídos de B 12 Años a C Adultos +18 Años', 4.5),
(8, '70 Crucigramas para adultos',5, 137, '988 palabras a descubrir.', 'cruci.jpg', 'AmazingPrint', 'Nuevo', 'AmazingPrint', 1.5);

create table carrito
(
	IdUsuario int not null,
    IdProducto int not null,
    Cantidad int not null,
    foreign key(IdUsuario) references Usuario(IdUsuario) on delete cascade on update cascade,
    foreign key(IdProducto) references Producto(IdProducto) on delete cascade on update cascade,
    primary key(IdUsuario,IdProducto)
);

create table venta
(
	IdVenta int auto_increment not null,
    IdUsuario int not null,
    Fecha TimeStamp not null,
    Total double not null,
    Estado boolean not null,
    foreign key(IdUsuario) references Usuario(IdUsuario) on delete cascade on update cascade,
    primary key(IdVenta, IdUsuario)
);

create table compra
(
	IdVenta int not null,
    IdProducto int not null,
    Cantidad int not null,
    foreign key(IdVenta) references Venta(IdVenta) on delete cascade on update cascade,
    foreign key(IdProducto) references Producto(IdProducto) on delete cascade on update cascade,
    primary key(IdProducto, IdVenta)
);
