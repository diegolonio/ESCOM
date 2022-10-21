% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   EXAMEN #1: Sistema metro CDMX.
%
%   - Programe en Prolog un "sistema de ayuda para usuarios del Sistema
%   de Trasporte Colectivo Metro CdMx" según las instrucciones de la
%   presentación anexa.
%
%   - Los predicados básicos a programar son: mejor_ruta/4, peor_ruta/4
%   y reporte_viaje/2.
%
%   - 3 puntos extra sobre la calificación para el predicado
%   reporte_viaje_simplificado/2 según se explicó en clase.
%
%   Para obtener la mejor ruta entre dos estaciones utilice el
%   predicado mejor_ruta/2.
%
%           mejor_ruta(<estación origen>, <estación destino>).
%
%   Para obtener la peor ruta entre dos estaciones utilice el predicado
%   peor_ruta/2.
%
%           peor_ruta(<estación origen>, <estación destino>).
%
%   Para imprimir en consola el reporte de viaje entre dos estaciones
%   utilice el predicado reporte_viaje/2.
%
%           reporte_viaje(<estación origen>, <estación destino>).
%
%   Para imprimir en consola la descripción en lenguaje natural del
%   viaje entre dos estaciones utilice el predicado
%   reporte_viaje_simplificado/2.
%
%   reporte_viaje_simplificado(<estación origen>, <estación destino>).
%
%   NOTA: La mejor ruta calculada entre un determinado origen y destino
%   es almacenada con la ayuda del predicado ruta/4 para optimizar los
%   tiempos de consulta entre consultas repetidas.
% =====================================================================

% =====================================================================
%   Consulta de la base de conocimiento que contiene el grafo que
%   modela a la red del metro de la CDMX.
% =====================================================================
:- consult('./metro-CDMX.prolog').

% =====================================================================
%   ruta/4.
%   ruta(+Origen, +Destino, -Ruta, -Tiempo).
%   Tiempo es el tiempo aproximado de viaje a través de la ruta Ruta
%   entre las estaciones Origen y Destino.
% =====================================================================
:- dynamic(ruta/4).

% =====================================================================
%   capitalizar/2.
%   capitalizar(+Palabra, -PalabraCapitalizada).
%   Palabra es una cadena o una átomo de una sola palabra en
%   minúsculas, PalabraCapitalizada es Palabra pero con la primera
%   letra en mayúscula.
% =====================================================================
capitalizar([], []).
capitalizar([Inicial|Resto], [InicialCapitalizada|Resto]):-
    code_type(InicialCapitalizada, to_upper(Inicial)).

% =====================================================================
%   formato_titulo/2.
%   formato_titulo(+ListaPalabras, +ListaConEspacios).
%   ListaPalabras contiene una serie de cadenas con caracteres
%   alfanuméricos, ListaConEspacios contiene los elementos de
%   ListaPalabras con el caracter espacio intercalado entre cada
%   elemento.
% =====================================================================
formato_titulo([Palabra], [Palabra]).
formato_titulo([Palabra|RestoOrigen], [Palabra, ' '|RestoDestino]) :-
    formato_titulo(RestoOrigen, RestoDestino).

% =====================================================================
%   formato_titulo/3.
%   formato_titulo(+Referencia, +ListaPalabras, -ListaCapitalizada).
%   Convierte una lista de cadenas alfanuméricas en minúsculas a un
%   formato de título, es decir, cada palabra capitalizada (excepto
%   preposiciones) y con espacios intercalados entre cada palabra; el
%   resultado es una lista devuelta en ListaCapitalizada.
%   Es estrictamente necesario que en la llamada inicial al predicado,
%   Referencia y ListaPlabras sean la misma lista.
% =====================================================================
formato_titulo(_, [Palabra], [PalabraCapitalizada]) :-
    atom_codes(Palabra, Codigos),
    capitalizar(Codigos, CodigosCapitalizados),
    atom_codes(PalabraCapitalizada, CodigosCapitalizados).
formato_titulo(ListaAtomos, [Palabra|RestoOrigen], [Palabra, ' '|RestoDestino]) :-
    member(Palabra, [de, del, la, los, el]),
    \+ nth1(1, ListaAtomos, Palabra),
    formato_titulo(ListaAtomos, RestoOrigen, RestoDestino), !.
formato_titulo(ListaAtomos, [Palabra|RestoOrigen], [PalabraCapitalizada, ' '|RestoDestino]) :-
    atom_codes(Palabra, Codigos),
    capitalizar(Codigos, CodigosCapitalizados),
    atom_codes(PalabraCapitalizada, CodigosCapitalizados),
    formato_titulo(ListaAtomos, RestoOrigen, RestoDestino).

% =====================================================================
%   atomo_a_texto/3.
%   atomo_a_texto(+Opción, +Átomo, -Texto).
%   Convierte un átomo en una cadena de texto. Opción puede ser el
%   átomo n o el átomo c: con la opción n, Texto contiene las palabras
%   de Átomo en minúsculas y con espacios; con la opción c, Texto
%   contiene las palabras de Átomo en formato de título.
% =====================================================================
atomo_a_texto(Opcion, Atomo, Texto) :-
    atom_string(Atomo, Tmp1),
    split_string(Tmp1, '_', '', Tmp2),
    maplist(atom_string, Tmp3, Tmp2),
    (Opcion = n -> formato_titulo(Tmp3, Tmp4); formato_titulo(Tmp3, Tmp3, Tmp4)), !,
    maplist(atom_string, Tmp4, Tmp5),
    atomic_list_concat(Tmp5, Texto).

% =====================================================================
%   recorrido/4.
%   recorrido(+Origen, +Destino, +Memoria, +Ruta).
%   Busca un camino en un grafo no dirigido que representa la red del
%   metro de la CDMX con comienzo en la estación (vértice) Origen y
%   final en la estación (vértice) Destino. Memoria es una lista
%   auxiliar en la que se almacenan las estaciones (vértices) que ya
%   han sido visitadas. Ruta es la lista ordenada de las estaciones que
%   conforman el camino de la estación Origen a la estación Destino.
% =====================================================================
recorrido(A, B, _, [Linea-A, Linea-B]) :-
    A \== B,
    (sigue(A, B, Linea); sigue(B, A, Linea)).
recorrido(A, B, Memoria, [Linea-A|RestoRuta]) :-
    A \== B,
    (sigue(A, Z, Linea); sigue(Z, A, Linea)),
    Z \== B,
    \+ member(Z, [A|Memoria]),
    recorrido(Z, B, [A|Memoria], RestoRuta).

% =====================================================================
%   determinar_direccion/3.
%   determinar_direccion(+Origen, +Destino, -Dirección).
%   Dadas una estación Origen y una estación Destino de una misma línea
%   del metro, determina la estación límite Dirección de dicha línea
%   según la dirección del viaje de la estación Origen a la estación
%   Destino.
% =====================================================================
determinar_direccion(A, B, Direccion) :-
    sigue(A, B, Linea),
    trayecto(Linea, _, Direccion), !.
determinar_direccion(A, B, Direccion) :-
    \+ sigue(A, B, _),
    sigue(B, A, Linea),
    trayecto(Linea, Direccion, _).

% =====================================================================
%   grado_estacion/2.
%   grado_estacion(+Estación, -Grado).
%   Determina el grado Grado de Estación, es decir, el número de
%   caminos que inciden en dicha estación.
% =====================================================================
grado_estacion(Estacion, Grado) :-
    findall(true, (sigue(Estacion, _, _); sigue(_, Estacion, _)), Incidencias),
    length(Incidencias, Grado).

% =====================================================================
%   transbordo/3.
%   transbordo(+EstaciónA, +EstaciónB, +EstaciónC).
%   Dadas tres estaciones EstaciónA, EstaciónB y EstaciónC; determina
%   si existe un cambio de línea entre las estaciones EstaciónA y
%   EstaciónC al pasar por la estación EstaciónB.
% =====================================================================
transbordo(A, B, C) :-
    \+ (sigue(A, B, Linea), sigue(B, C, Linea)),
    \+ (sigue(C, B, Linea), sigue(B, A, Linea)).

% =====================================================================
%   conteo_transbordos/2.
%   conteo_transbordos(+Ruta, -Conteo).
%   Calcula el número de transbordos que se hacen al efectuar el viaje
%   a través de la ruta Ruta. El resultado es devuelto en Conteo.
% =====================================================================
conteo_transbordos([_, _], 0) :- !.
conteo_transbordos([_-A, L2-B, L3-C|RestoRuta], ConteoTotal) :-
    \+ transbordo(A, B, C),
    conteo_transbordos([L2-B, L3-C|RestoRuta], ConteoTotal), !.
conteo_transbordos([_-A, L2-B, L3-C|RestoRuta], ConteoTotal) :-
    transbordo(A, B, C),
    conteo_transbordos([L2-B, L3-C|RestoRuta], ConteoParcial),
    ConteoTotal is 1 + ConteoParcial.

% =====================================================================
%   sumatoria_grados_estaciones/2.
%   sumatoria_grados_estaciones(+Ruta, -Sumatoria).
%   Sumatoria es la sumatoria del grado de cada una de las estaciones
%   de la ruta Ruta.
% =====================================================================
sumatoria_grados_estaciones([_, _-Estacion], SumatoriaTotal) :-
    grado_estacion(Estacion, SumatoriaTotal), !.
sumatoria_grados_estaciones([_, Línea-Estacion|RestoRuta], Sumatoria) :-
    grado_estacion(Estacion, GradoEstacion),
    sumatoria_grados_estaciones([Línea-Estacion|RestoRuta], SumatoriaParcial),
    Sumatoria is GradoEstacion + SumatoriaParcial.

% =====================================================================
%   tiempo_total_viaje/2.
%   tiempo_total_viaje(+Ruta, +TiempoTotalViaje).
%   TiempoTotalViaje es el tiempo total aproximado que toma realizar
%   un viaje a través de la ruta Ruta.
% =====================================================================
tiempo_total_viaje(Ruta, TiempoTotalViaje) :-
    valor_parametro(tiempo_tramo, TiempoTramo),
    valor_parametro(tiempo_transbordo, TiempoTransbordo),
    valor_parametro(tiempo_inicial, TiempoInicial),
    valor_parametro(tiempo_final, TiempoFinal),
    sumatoria_grados_estaciones(Ruta, SumatoriaGrados),
    conteo_transbordos(Ruta, ConteoTransbordos),
    TiempoTotalViaje is TiempoTramo * SumatoriaGrados + ConteoTransbordos * TiempoTransbordo + TiempoInicial + TiempoFinal.

% =====================================================================
%   viaje/4.
%   viaje(+Origen, +Destino, -Ruta, -Tiempo).
%   Ruta es la lista de estaciones a través de las cuales es posible
%   llegar de la estación Origen a la estación Destino. Tiempo es el
%   tiempo que toma realizar dicho viaje.
% =====================================================================
viaje(A, B, Ruta, Tiempo) :-
    recorrido(A, B, [], Ruta),
    tiempo_total_viaje(Ruta, Tiempo).

% =====================================================================
%   ruta_corta/3.
%   ruta_corta(+Rutas, -Ruta, -TiempoMínimo).
%   Ruta es la ruta con el menor tiempo TiempoMínimo de entre todas las
%   rutas Rutas.
% =====================================================================
ruta_corta(Rutas, Ruta, TiempoMinimo) :-
    pairs_values(Rutas, Tiempos),
    min_list(Tiempos, TiempoMinimo),
    member(Ruta-TiempoMinimo, Rutas).

% =====================================================================
%   mejor_ruta/4.
%   mejor_ruta(+Origen, +Destino, -Ruta, -Tiempo).
%   Ruta es la ruta con el mejor tiempo (el más corto) Tiempo entre las
%   estaciones Origen y Destino.
%   Si la ruta se encuentra almacenada, la consulta; en el caso
%   contrario, la calcula y la almacena.
% =====================================================================
mejor_ruta(A, B, MejorRuta, MejorTiempo) :-
    ruta(A, B, MejorRuta, MejorTiempo), !.
mejor_ruta(A, B, MejorRuta, MejorTiempo) :-
    findall(Ruta-Tiempo, viaje(A, B, Ruta, Tiempo), Rutas),
    ruta_corta(Rutas, MejorRuta, MejorTiempo),
    assert(ruta(A, B, MejorRuta, MejorTiempo)).

% =====================================================================
%   ruta_larga/3.
%   ruta_larga(+Rutas, -Ruta, -TiempoMáximo).
%   Ruta es la ruta con el mayor tiempo TiempoMáximo de entre todas las
%   rutas Rutas.
% =====================================================================
ruta_larga(Rutas, Ruta, TiempoMaximo) :-
    pairs_values(Rutas, Tiempos),
    max_list(Tiempos, TiempoMaximo),
    member(Ruta-TiempoMaximo, Rutas).

% =====================================================================
%   peor_ruta/4.
%   peor_ruta(+Origen, +Destino, -Ruta, -Tiempo).
%   Ruta es la ruta con el peor tiempo (el más largo) Tiempo entre las
%   estaciones Origen y Destino.
% =====================================================================
peor_ruta(A, B, PeorRuta, PeorTiempo) :-
    findall(Ruta-Tiempo, viaje(A, B, Ruta, Tiempo), Rutas),
    ruta_larga(Rutas, PeorRuta, PeorTiempo).

% =====================================================================
%   mostrar_tiempo_viaje/1.
%   mostrar_tiempo_viaje(+Tiempo).
%   Dado el tiempo de viaje en minutos Tiempo, imprime en consola dicho
%   tiempo convertido a horas y minutos (si los hay).
% =====================================================================
mostrar_tiempo_viaje(Tiempo) :-
    Horas is Tiempo // 60,
    Auxiliar is (Tiempo / 60 - Tiempo // 60) * 60,
    Minutos is round(Auxiliar),
    format('Tiempo total de viaje: ~d minutos', [Tiempo]),
    (Horas > 0 -> (
        write(' = '),
        (Horas > 1 -> format('~d horas', [Horas]); format('~d hora', [Horas])),
        (Minutos > 0 -> (Minutos > 1 -> format(' y ~d minutos', [Minutos]); write(' y 1 minuto')); true)
    ); true),
    write(' aprox.'), nl, nl.

% =====================================================================
%   mostrar_inicio/1.
%   mostrar_inicio(+Ruta).
%   Imprime en consola el tiempo que tardaría un usuario en entrar a la
%   primera estación de la ruta Ruta.
% =====================================================================
mostrar_inicio([_-PrimeraEstacion|_]) :-
    valor_parametro(tiempo_inicial, TiempoInicial),
    atomo_a_texto(c, PrimeraEstacion, CadenaPrimeraEstacion),
    format('Inicio:~10|~s, ~d minutos', [CadenaPrimeraEstacion, TiempoInicial]), nl, nl.

% =====================================================================
%   mostrar_tramos/2.
%   mostrar_tramos(+NúmeroTramo, +Ruta).
%   Imprime en consola el tiempo de viaje entre dos estaciones
%   consecutivas de la ruta Ruta; cuando sea posible realizar un
%   transbordo, si el usuario debe efectuarlo o no; y el grado de cada
%   estación.
% =====================================================================
mostrar_tramos(NumeroTramo, [_-A, _-B]) :-
    valor_parametro(tiempo_tramo, TiempoTramo),
    grado_estacion(B, GradoEstacion),
    TiempoArista is TiempoTramo * GradoEstacion,
    atomo_a_texto(c, A, CadenaA),
    atomo_a_texto(c, B, CadenaB),
    format('Tramo ~d:~11|~s, ~s, ~d minutos', [NumeroTramo, CadenaA, CadenaB, TiempoArista]), nl,
    format('~11|destino con grado ~d.', [GradoEstacion]), nl, nl, !.
mostrar_tramos(NumeroTramo, [_-A, L2-B, L3-C|RestoRuta]) :-
    valor_parametro(tiempo_tramo, TiempoTramo),
    grado_estacion(B, GradoEstacion),
    transbordo(A, B, C),
    valor_parametro(tiempo_transbordo, TiempoTransbordo),
    TiempoArista is TiempoTramo * GradoEstacion + TiempoTransbordo,
    atomo_a_texto(c, A, CadenaA),
    atomo_a_texto(c, B, CadenaB),
    format('Tramo ~d:~11|~s, ~s, ~d minutos', [NumeroTramo, CadenaA, CadenaB, TiempoArista]), nl,
    format('~11|destino con grado ~d, transbordo.', [GradoEstacion]), nl, nl,
    NumeroSiguienteTramo is NumeroTramo + 1,
    mostrar_tramos(NumeroSiguienteTramo, [L2-B, L3-C|RestoRuta]), !.
mostrar_tramos(NumeroTramo, [_-A, L2-B, L3-C|RestoRuta]) :-
    valor_parametro(tiempo_tramo, TiempoTramo),
    grado_estacion(B, GradoEstacion),
    TiempoArista is TiempoTramo * GradoEstacion,
    atomo_a_texto(c, A, CadenaA),
    atomo_a_texto(c, B, CadenaB),
    format('Tramo ~d:~11|~a, ~a, ~d minutos', [NumeroTramo, CadenaA, CadenaB, TiempoArista]), nl,
    format('~11|destino con grado ~d', [GradoEstacion]),
    (GradoEstacion > 2 -> write(', sin transbordo'); write('.')), nl, nl,
    NumeroSiguienteTramo is NumeroTramo + 1,
    mostrar_tramos(NumeroSiguienteTramo, [L2-B, L3-C|RestoRuta]).

% =====================================================================
%   mostrar_final/1.
%   mostrar_final(+Ruta).
%   Imprime en consola el tiempo que tardaría un usuario en salir de la
%   última estación de la ruta Ruta.
% =====================================================================
mostrar_final(Ruta) :-
    last(Ruta, _-UltimaEstacion),
    valor_parametro(tiempo_final, TiempoFinal),
    atomo_a_texto(c, UltimaEstacion, CadenaUltimaEstacion),
    format('Final:~11|~s, ~d minutos', [CadenaUltimaEstacion, TiempoFinal]), nl.

% =====================================================================
%   reporte/2.
%   reporte(+Ruta, +Tiempo).
%   Imprime en consola los detalles del viaje a través de la ruta Ruta
%   con la ayuda de los predicados mostrar_tiempo_viaje/1,
%   mostrar_inicio/1, mostrar_tramos/2 y mostrar_final/1.
% =====================================================================
reporte(Ruta, Tiempo) :-
    mostrar_tiempo_viaje(Tiempo),
    mostrar_inicio(Ruta),
    mostrar_tramos(1, Ruta),
    mostrar_final(Ruta).

% =====================================================================
%   reporte_viaje/2.
%   reporte_viaje(+Origen, +Destino).
%   Imprime en consola los detalles de la mejor ruta entre las
%   estaciones Origen y Destino con la ayuda del predicado reporte/2.
%   Si la ruta se encuentra almacenada, la consulta; en el caso
%   contrario, calcula la ruta y la almacena.
% =====================================================================
reporte_viaje(A, B) :-
    ruta(A, B, Ruta, Tiempo),
    reporte(Ruta, Tiempo), !.
reporte_viaje(A, B) :-
    mejor_ruta(A, B, Ruta, Tiempo),
    assert(ruta(A, B, Ruta, Tiempo)),
    reporte(Ruta, Tiempo).

% =====================================================================
%   comienzo/5.
%   comienzo(+Línea, +Dirección, +Origen, +Destino, -Lista).
%   Lista es una oración dividida en palabras que describe en lenguaje
%   natural el comienzo de la ruta de la estación Origen a la estación
%   Destino. Como detalles adicionales, se incluye la línea Línea, el
%   color Color y la dirección Dirección del primer tramo.
% =====================================================================
comienzo(Linea, Color, Direccion, Origen, Destino, Lista) :-
    Lista = [A1, ' a', A2, ' ', Origen, ' de la línea de color ', Color, ' (', Linea, '), ', A3, A4, ' hasta', A5, ' ', Destino, '.'],
    random_member(A1, ['Una vez que te hayas subido', 'Luego de haberte subido', 'Cuando te subas']),
    random_member(A2, ['', ' la estación']),
    random_member(A3, ['viajarás', 'vas a viajar', 'recorrerás la línea']),
    random_member(A4, ['', [' con dirección', ' ', Direccion]]),
    random_member(A5, ['', ' la estación']).

% =====================================================================
%   continuacion/5.
%   continuacion(+Línea, +Color, +Dirección, +Origen, +Destino, -Lista).
%   Lista es una oración dividida en palabras que describe en lenguaje
%   natural un determinado tramo intermedio de la ruta de la estación
%   Origen a la estación Destino. Como detalles adicionales, se incluye
%   la línea Línea, el color Color y la dirección Dirección del tramo.
% =====================================================================
continuacion(Linea, Color, Direccion, Origen, Destino, Lista) :-
    Lista = [A1, ',', A2, ' vas a transbordar a la línea de color ', Color, ' (', Linea, '), y ', A3, A4, ' hasta', A5, ' ', Destino, '.'],
    random_member(A1, ['Luego', 'Después', 'Entonces', 'Seguidamente', 'A continuación']),
    random_member(A2, ['', [' en ', Origen]]),
    random_member(A3, ['viajarás', 'vas a viajar', 'recorrerás la línea']),
    random_member(A4, ['', [' con dirección ', Direccion]]),
    random_member(A5, ['', ' la estación']).

% =====================================================================
%   conclusion/5.
%   conclusion(+Línea, +Color, +Dirección, +Origen, +Destino, -Lista).
%   Lista es una oración dividida en palabras que describe en lenguaje
%   natural el final de la ruta de la estación Origen a la estación
%   Destino. Como detalles adicionales, se incluye la línea Línea, el
%   color Color y la dirección Dirección del último tramo.
% =====================================================================
conclusion(Linea, Color, Direccion, Origen, Destino, Lista) :-
    Lista = [A1, ',', A2, ' vas a transbordar a la línea de color ', Color, ' (', Linea, ') y ', A3, A4, A5, ' ', Destino, '. ¡Saliendo de la estación habrás llegado a tu destino!'],
    random_member(A1, ['Finalmente', 'Por último', 'Ya para terminar', 'Para finalizar', 'Para terminar']),
    random_member(A2, ['', [' en ', Origen]]),
    random_member(A3, ['viajarás', 'vas a viajar', 'recorrerás el resto de la línea']),
    random_member(A4, [' hasta llegar a', [' con dirección ', Direccion, ' hasta']]),
    random_member(A5, ['', ' la estación']).

% =====================================================================
%   oracion/6.
%   oracion(+Opción, +Línea, +Auxiliar, +Origen, +Destino, -Oración).
%   Oración es la cadena con la descripción en lenguaje natural del
%   tramo entre las estaciones consecutivas ÁtomoOrigen y ÁtomoDestino
%   de la línea ÁtomoLínea según la opción Opción.
%   Opción puede tomar tres posibles valores:
%
%   1.- Describe el tramo como el comienzo del viaje.
%   2.- Describe el tramo como la continuación del viaje.
%   3.- Describe el tramo como el final del viaje.
%
%   ÁtomoAuxiliar permite determinar la dirección del viaje en el
%   tramo.
% =====================================================================
oracion(Opcion, AtomoLinea, AtomoAuxiliar, AtomoOrigen, AtomoDestino, Oracion) :-
    color(AtomoLinea, ÁtomoColor),
    determinar_direccion(AtomoOrigen, AtomoAuxiliar, ÁtomoDirección),
    atomo_a_texto(n, AtomoLinea, Linea),
    atomo_a_texto(n, ÁtomoColor, Color),
    atomo_a_texto(c, ÁtomoDirección, Direccion),
    atomo_a_texto(c, AtomoOrigen, Origen),
    atomo_a_texto(c, AtomoDestino, Destino),
    ((Opcion = 1, comienzo(Linea, Color, Direccion, Origen, Destino, Tmp1));
    (Opcion = 2, continuacion(Linea, Color, Direccion, Origen, Destino, Tmp1));
    (Opcion = 3, conclusion(Linea, Color, Direccion, Origen, Destino, Tmp1))), !,
    flatten(Tmp1, Tmp2),
    atomic_list_concat(Tmp2, Oracion).

% =====================================================================
%   describir_ruta/2.
%   describir_ruta(+Referencia, +Ruta).
%   Imprime en consola la descripción en lenguaje natural de cada uno
%   de los tramos de la ruta Ruta.
%   Referencia es la ruta completa que permite determinar el tipo de
%   descripción de cada tramo del viaje.
% =====================================================================
describir_ruta(RutaCompleta, [Linea-Segmento]) :-
    length(RutaCompleta, 1),
    Segmento = [Origen, Auxiliar|_],
    last(Segmento, Destino),
    oracion(1, Linea, Auxiliar, Origen, Destino, Oracion),
    write(Oracion), write(' ¡Saliendo de la estación habrás llegado a tu destino!'), !.
describir_ruta(RutaCompleta, [Linea-Segmento]) :-
    \+ length(RutaCompleta, 1),
    Segmento = [Origen, Auxiliar|_],
    last(Segmento, Destino),
    oracion(3, Linea, Auxiliar, Origen, Destino, Oracion),
    write(Oracion), write(' '), !.
describir_ruta(RutaCompleta, Ruta) :-
    Ruta = [Linea-Segmento|RestoRuta],
    nth1(1, RutaCompleta, Linea-Segmento),
    RestoRuta = [_-[Destino|_]|_],
    ((Segmento = [Origen, Auxiliar|_], oracion(1, Linea, Auxiliar, Origen, Destino, Oracion));
    (Segmento = [Origen|_], oracion(1, Linea, Destino, Origen, Destino, Oracion))), !,
    write(Oracion), write(' '),
    describir_ruta(RutaCompleta, RestoRuta), !.
describir_ruta(RutaCompleta, Ruta) :-
    Ruta = [Linea-Segmento|RestoRuta],
    RestoRuta = [_-[Destino|_]|_],
    ((Segmento = [Origen, Auxiliar|_], oracion(2, Linea, Auxiliar, Origen, Destino, Oracion));
    (Segmento = [Origen|_], oracion(2, Linea, Destino, Origen, Destino, Oracion))), !,
    write(Oracion), write(' '),
    describir_ruta(RutaCompleta, RestoRuta).

% =====================================================================
%   reporte_viaje_simplificado/2.
%   reporte_viaje_simplificado(+Origen, +Destino).
%   Imprime en consola la descripción en lenguaje natural del viaje a
%   través de la ruta entre las estaciones Origen y Destino.
%   Si la ruta se encuentra almacenada, la consulta; en el caso
%   contrario, la calcula y la almacena.
% =====================================================================
reporte_viaje_simplificado(A, B) :-
    ruta(A, B, Ruta, _),
    group_pairs_by_key(Ruta, RutaLineas),
    describir_ruta(RutaLineas, RutaLineas), !.
reporte_viaje_simplificado(A, B) :-
    mejor_ruta(A, B, Ruta, Tiempo),
    assert(ruta(A, B, Ruta, Tiempo)),
    group_pairs_by_key(Ruta, RutaLineas),
    describir_ruta(RutaLineas, RutaLineas).
