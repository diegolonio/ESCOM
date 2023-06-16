% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #5: Misioneros y caníbales.
%
%   - Complete el programa para resolver el problema de Misioneros y
%   caníbales, usando las indicaciones y los predicados de la
%   presentación #21.
%
%   - Su programa debe poder entregar la solución al problema, buscando
%   en tres órdenes diferentes: profundo, ancho y profundización
%   iterativa.
%
%   - La salida del programa debe tener la estructura que se muestra en
%   la lámina #37 de esa misma presentación.
%
%   Para obtener una lista con los estados de una solución utilizando
%   el algoritmo...
%
%   - Depth-First Search (DFS), utilice el predicado:
%
%       busqueda_dfs(<estado inicial>, <estado meta>, <ruta>).
%
%   - Breath-First Search (BFS), utilice el predicado:
%
%       busqueda_bfs(<estado inicial>, <estado meta>, <ruta>).
%
%   - Iterative Deepening Search (IDS), utilice el predicado:
%
%       busqueda_ids(<estado inicial>, <estado meta>, <ruta>).
%
%   Para mostrar una descripción legible para el humano de una solución
%   generada con cualquiera de los predicados descritos anteriormente,
%   utilice el predicado:
%
%                           despliega(<ruta>).
% =====================================================================

% =====================================================================
%   estado_meta/1.
%   estado_meta(+Estado).
%   Estado es el estado a alcanzar en la búsqueda de una solución con
%   cualquiera de los tres algoritmos disponibles en esta base de
%   conocimiento.
% =====================================================================
:- dynamic(estado_meta/1).

% =====================================================================
%   descripcion_movimiento/3.
%   descripcion_movimiento(+EstadoA, +EstadoB, -Descripción).
%   Verdadero si Descripción es una descripción en lenguaje natural del
%   movimiento aplicado a EstadoA para llegar a EstadoB.
% =====================================================================
descripcion_movimiento([MO1, CO1, MD1, CD1], [MO2, CO2, MD2, CD2], 'UN MISIONERO') :-
    (MO2 is MO1 - 1, CO2 is CO1, MD2 is MD1 + 1, CD2 is CD1);
    (MO2 is MO1 + 1, CO2 is CO1, MD2 is MD1 - 1, CD2 is CD1).
descripcion_movimiento([MO1, CO1, MD1, CD1], [MO2, CO2, MD2, CD2], 'DOS MISIONEROS') :-
    (MO2 is MO1 - 2, CO2 is CO1, MD2 is MD1 + 2, CD2 is CD1);
    (MO2 is MO1 + 2, CO2 is CO1, MD2 is MD1 - 2, CD2 is CD1).
descripcion_movimiento([MO1, CO1, MD1, CD1], [MO2, CO2, MD2, CD2], 'UN MISIONERO Y UN CANIBAL') :-
    (MO2 is MO1 - 1, CO2 is CO1 - 1, MD2 is MD1 + 1, CD2 is CD1 + 1);
    (MO2 is MO1 + 1, CO2 is CO1 + 1, MD2 is MD1 - 1, CD2 is CD1 - 1).
descripcion_movimiento([MO1, CO1, MD1, CD1], [MO2, CO2, MD2, CD2], 'UN CANIBAL') :-
    (MO2 is MO1, CO2 is CO1 - 1, MD2 is MD1, CD2 is CD1 + 1);
    (MO2 is MO1, CO2 is CO1 + 1, MD2 is MD1, CD2 is CD1 - 1).
descripcion_movimiento([MO1, CO1, MD1, CD1], [MO2, CO2, MD2, CD2], 'DOS CANIBALES') :-
    (MO2 is MO1, CO2 is CO1 - 2, MD2 is MD1, CD2 is CD1 + 2);
    (MO2 is MO1, CO2 is CO1 + 2, MD2 is MD1, CD2 is CD1 - 2).

% =====================================================================
%   estado_valido/1.
%   estado_valido(+Estado).
%   Verdadero si estado cumple con las restricciones del problema de
%   los misioneros y caníbales: en ambas orillas siempre debe haber
%   más misioneros que caníbales o ninguno.
% =====================================================================
estado_valido([MO, CO, MD, CD]) :-
    MO >= 0,
    CO >= 0,
    MD >= 0,
    CD >= 0,
    (MO >= CO; MO is 0),
    (MD >= CD; MD is 0).

% =====================================================================
%   movimiento/2.
%   movimiento(+EstadoA, -EstadoB).
%   Verdadero si EstadoB es producto de aplicar cualquiera de los cinco
%   movimientos posibles en el problema de los misioneros y caníbales
%   al estado EstadoA.
% =====================================================================
movimiento([MO1, CO1, MD1, CD1, Orilla1], [MO2, CO2, MD2, CD2, Orilla2]) :-
    % Un misionero
    ((MO2 is MO1 - 1, CO2 is CO1, MD2 is MD1 + 1, CD2 is CD1, Orilla1 = 'o', Orilla2 = 'd');
    (MO2 is MO1 + 1, CO2 is CO1, MD2 is MD1 - 1, CD2 is CD1, Orilla1 = 'd', Orilla2 = 'o');
    % Dos misioneros
    (MO2 is MO1 - 2, CO2 is CO1, MD2 is MD1 + 2, CD2 is CD1, Orilla1 = 'o', Orilla2 = 'd');
    (MO2 is MO1 + 2, CO2 is CO1, MD2 is MD1 - 2, CD2 is CD1, Orilla1 = 'd', Orilla2 = 'o');
    % Un misionero y un caníbal
    (MO2 is MO1 - 1, CO2 is CO1 - 1, MD2 is MD1 + 1, CD2 is CD1 + 1, Orilla1 = 'o', Orilla2 = 'd');
    (MO2 is MO1 + 1, CO2 is CO1 + 1, MD2 is MD1 - 1, CD2 is CD1 - 1, Orilla1 = 'd', Orilla2 = 'o');
    % Un caníbal
    (MO2 is MO1, CO2 is CO1 - 1, MD2 is MD1, CD2 is CD1 + 1, Orilla1 = 'o', Orilla2 = 'd');
    (MO2 is MO1, CO2 is CO1 + 1, MD2 is MD1, CD2 is CD1 - 1, Orilla1 = 'd', Orilla2 = 'o');
    % Dos caníbales
    (MO2 is MO1, CO2 is CO1 - 2, MD2 is MD1, CD2 is CD1 + 2, Orilla1 = 'o', Orilla2 = 'd');
    (MO2 is MO1, CO2 is CO1 + 2, MD2 is MD1, CD2 is CD1 - 2, Orilla1 = 'd', Orilla2 = 'o')),
    estado_valido([MO2, CO2, MD2, CD2]).

% =====================================================================
%   sucesores/2.
%   sucesores(+Ruta, +PosiblesRutasSucesoras).
%   Dada una ruta Ruta, obtiene todas sus posibles rutas sucesoras
%   PosiblesRutasSucesoras cuyo primer estado es sucesor del primer
%   estado de Ruta. Ejemplo:
%
%   Para la ruta [[3, 3, 0, 0, 'o']], la lista de rutas sucesoras
%   contendría las rutas:
%
%       - [2, 2, 1, 1, d], [3, 3, 0, 0, o]]
%       - [[3, 2, 0, 1, d], [3, 3, 0, 0, o]]
%       - [[3, 1, 0, 2, d], [3, 3, 0, 0, o]]]
% =====================================================================
sucesores([Estado|RestoAgenda], Sucesores) :-
    findall([Sucesor, Estado|RestoAgenda], (movimiento(Estado, Sucesor), \+ member(Sucesor, [Estado|RestoAgenda])), Sucesores).

% =====================================================================
%   dfs/2.
%   dfs(+Agenda, -RutaInvertida).
%   Implementación en lenguaje Prolog del recorrido de un árbol
%   utilizando el algoritmo de búsqueda a lo profundo. El recorrido
%   termina cuando el primer Estado de Agenda es el estado meta y es
%   agregado al principio de RutaInvertida.
% =====================================================================
dfs([[Estado|Trayecto]|_], [Estado|Trayecto]) :-
    estado_meta(Estado).
dfs([Candidato|Frontera], Ruta) :-
    sucesores(Candidato, Sucesores),
    append(Sucesores, Frontera, NuevaAgenda),
    dfs(NuevaAgenda, Ruta).

% =====================================================================
%   busqueda_dfs/3.
%   busqueda_dfs(+EstadoInicial, +EstadoMeta, -Ruta).
%   Obtiene la ruta Ruta entre los estados EstadoInicial y EstadoMeta
%   utilizando el algoritmo de búsqueda a lo profundo.
% =====================================================================
busqueda_dfs(A, B, Ruta) :-
    retractall(intentos(_)),
    assert(intentos(0)),
    retractall(estado_meta(_)),
    assert(estado_meta(B)),
    dfs([[A]], RutaInvertida),
    reverse(RutaInvertida, Ruta).

% =====================================================================
%   bfs/2.
%   bfs(+Agenda, -RutaInvertida).
%   Implementación en lenguaje Prolog del recorrido de un árbol
%   utilizando el algoritmo de búsqueda a lo ancho. El recorrido
%   termina cuando el primer Estado de Agenda es el estado meta y es
%   agregado al principio de RutaInvertida.
% =====================================================================
bfs([[Estado|Trayecto]|_], [Estado|Trayecto]) :-
    estado_meta(Estado).
bfs([Candidato|Frontera], Ruta) :-
    sucesores(Candidato, Sucesores),
    append(Frontera, Sucesores, NuevaAgenda),
    bfs(NuevaAgenda, Ruta).

% =====================================================================
%   busqueda_bfs/3.
%   busqueda_bfs(+EstadoInicial, +EstadoMeta, -Ruta).
%   Obtiene la ruta Ruta entre los estados EstadoInicial y EstadoMeta
%   utilizando el algoritmo de búsqueda a lo ancho.
% =====================================================================
busqueda_bfs(A, B, Ruta) :-
    retractall(estado_meta(_)),
    assert(estado_meta(B)),
    bfs([[A]], RutaInvertida),
    reverse(RutaInvertida, Ruta).

% =====================================================================
%   ids/3.
%   ids(+EstadoInicial, +EstadoMeta, -Ruta).
%   Implementación en lenguaje Prolog del algoritmo de búsqueda
%   iterativa a lo profundo para obtener la ruta Ruta entre los estados
%   EstadoInicial y EstadoMeta.
% =====================================================================
ids(Estado, Estado, [Estado]).
ids(EstadoInicial, EstadoFinal, [EstadoFinal|Trayecto]) :-
    ids(EstadoInicial, Z, Trayecto),
    movimiento(Z, EstadoFinal),
    \+ member(EstadoFinal, Trayecto).

% =====================================================================
%   busqueda_ids/3.
%   busqueda_ids(+EstadoInicial, +EstadoMeta, -Ruta).
%   Obtiene la ruta Ruta entre los estados EstadoInicial y EstadoMeta
%   utilizando el algoritmo de búsqueda iterativa a lo profundo.
% =====================================================================
busqueda_ids(A, B, Ruta) :-
    retractall(estado_meta(_)),
    assert(estado_meta(B)),
    ids(A, B, RutaInvertida),
    estado_meta(B),
    reverse(RutaInvertida, Ruta).

tabular(_, [_]).
tabular(NumeroRegistro, [[MOA, COA, MDA, CDA, OrillaA], [MOB, COB, MDB, CDB, OrillaB]|RestoTrayecto]) :-
    descripcion_movimiento([MOA, COA, MDA, CDA], [MOB, COB, MDB, CDB], Descripcion),
    format('(~d~3|) aplicando ~s~40| se llega a ((~d, ~d, ~s) (~d, ~d, ~s))~n', [NumeroRegistro, Descripcion, MOB, COB, OrillaB, MDB, CDB, OrillaA]),
    NumeroSiguienteRegistro is NumeroRegistro + 1,
    tabular(NumeroSiguienteRegistro, [[MOB, COB, MDB, CDB, OrillaB]|RestoTrayecto]).

% =====================================================================
%   despliega/1.
%   despliega(+Ruta).
%   Mostrar en consola una tabla descriptiva de la ruta Ruta a seguir
%   para llegar de un estado inicial a un estado meta.
% =====================================================================
despliega(Ruta) :-
    Ruta = [[MO, CO, MD, CD, Orilla]|Trayecto],
    length(Trayecto, NumeroPasos),
    format('Solución con ~d pasos: ~2n', [NumeroPasos]),
    (Orilla = 'o' -> format('Inicio en ((~d, ~d, ~s) (~d, ~d, ~s))~n', [MO, CO, Orilla, MD, CD, 'd']);
    format('Inicio en ((~d, ~d, ~s) (~d, ~d, ~s))~n', [MO, CO, Orilla, MD, CD, 'o'])),
    tabular(1, Ruta), nl.
