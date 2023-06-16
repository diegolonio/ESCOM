% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #7: Agente jugador de Gato 4x4
%
%   - Construya en Prolog un agente jugador de Gato 4x4 siguiendo TODAS
%   las instrucciones que están en las láminas 31 a 39 de la
%   presentación #25 (Estrategia de juego).
%
%   - El predicado  principal  debe  parametrizar el nivel máximo de
%   produndización en el árbol de búsqueda y resolver usando tanto el
%   algoritmo Mini Max (Nega Max) como podas Alpha-Beta.
%
%   Para iniciar un nuevo juego utilice el predicado jugar/1:
%
%                       jugar(<profundidad>).
%
%   El predicado jugar/1 recibe como argumento el nivel máximo de
%   profundización del árbol de búsqueda para el algoritmo Mini Max.
%
%   Para configurar el simbolo que utilizará cualquiera de los dos
%   jugadores, utilice el predicado modificar_símbolo/2:
%
%           modificar_símbolo(<jugador>, <nuevo_símbolo>).
% =====================================================================
:- dynamic(símbolo/2).
:- retractall(símbolo(_, _)).

% =====================================================================
%   símbolo/2.
%   símbolo(+Jugador, -Símbolo).
%   Verdadero si Símbolo es el símbolo de Jugador.
% =====================================================================
símbolo(1, x).
símbolo(2, o).

% =====================================================================
%   siguiente_turno/2.
%   siguiente_turno(+JugadorActual, -SiguienteJugador).
%   SiguienteJugador es el jugador del turno que sucede a
%   JugadorActual.
% =====================================================================
siguiente_turno(1,2).
siguiente_turno(2,1).

% =====================================================================
%   obtener_fila/2.
%   obtener_fila(+Letra, -NúmeroFila).
%   Realiza un mapeo de las letras [A, B, C, D] a los números
%   [1, 2, 3, 4] que serán los números de fila NúmeroFila del tablero.
% =====================================================================
obtener_fila('A', 1).
obtener_fila('B', 2).
obtener_fila('C', 3).
obtener_fila('D', 4).

% =====================================================================
%   modificar_símbolo/2.
%   modificar_símbolo(+Jugador, +NuevoSímbolo).
%   Permite reemplazar el símbolo con el que se identificará al jugador
%   Jugador en el tablero por NuevoSímbolo.
% =====================================================================
modificar_símbolo(Jugador, Símbolo) :-
    retract(símbolo(Jugador, _)),
    assert(símbolo(Jugador, Símbolo)).

% =====================================================================
%   sustituir/4.
%   sustituir(+Índice, +Lista, +Elemento, -NuevaLista).
%   Elemento sustituye al elemento en el índice Índice de Lista, la
%   nueva lista es devuelta en NuevaLista.
% =====================================================================
sustituir(Índice, Lista, NuevoElemento, NuevaLista) :-
    nth1(Índice, Lista, _, Resultado),
    nth1(Índice, NuevaLista, NuevoElemento, Resultado).

% =====================================================================
%   empate(+Tablero).
%   Verdadero si no existen celdas vacías (-) en Tablero.
% =====================================================================
empate([A,B,C,D]) :-
    \+ member(-,A),
    \+ member(-,B),
    \+ member(-,C),
    \+ member(-,D).

% =====================================================================
%   jugada_posible/2.
%   jugada_posible(+Estado, -NuevoEstado).
%   Verdadero sí NuevoEstado es descendiente de Estado tras haber
%   hecho una jugada el jugador en turno.
% =====================================================================
jugada_posible((Tablero, Jugador), (NuevoTablero, SiguienteJugador)) :-
    append(FilaSuperior, [Fila|FilaInferior], Tablero),
    append(ColumnaIzquierda, [-|ColumnaDerecha], Fila),
    símbolo(Jugador, Símbolo),
    append(ColumnaIzquierda, [Símbolo|ColumnaDerecha], NuevaFila),
    append(FilaSuperior, [NuevaFila|FilaInferior], NuevoTablero),
    siguiente_turno(Jugador, SiguienteJugador).

% =====================================================================
%   obtener_jugadas_posibles/2.
%   obtener_jugadas_posibles(+Estado, -JugadasPosibles).
%   JugadasPosibles es una lista con todos los posibles estados que
%   descienden de Estado tras haber realizado todas las jugadas
%   posibles (una para cada nuevo estado) el jugador en turno.
% =====================================================================
obtener_jugadas_posibles(Estado, JugadasPosibles) :-
    findall((NuevoTablero, SiguienteJugador), jugada_posible(Estado, (NuevoTablero, SiguienteJugador)), JugadasPosibles).

% =====================================================================
%   jugada_ganadora/2.
%   jugada_ganadora(+Símbolo, +Tablero).
%   Verdadero si existe una fila, columna o diagonal del tablero con
%   todas sus celdas del mismo símbolo Símbolo.
% =====================================================================
jugada_ganadora(Símbolo, Tablero) :-
    append(_, [[Símbolo, Símbolo, Símbolo, Símbolo]|_], Tablero);
    (Tablero = [A, B, C, D],
    append(DerechaA, [Símbolo|_], A),
    append(DerechaB, [Símbolo|_], B),
    append(DerechaC, [Símbolo|_], C),
    append(DerechaD, [Símbolo|_], D),
    length(DerechaA, Longitud),
    length(DerechaB, Longitud),
    length(DerechaC, Longitud),
    length(DerechaD, Longitud));
    Tablero = [[Símbolo, _, _, _], [_, Símbolo, _, _], [_, _, Símbolo, _], [_, _, _, Símbolo]];
    Tablero = [[_, _, _, Símbolo], [_, _, Símbolo, _], [_, Símbolo, _, _], [Símbolo, _, _, _]].

% =====================================================================
%   posibilidades_ganadoras/2.
%   posibilidades_ganadoras(+Símbolo, +Tablero).
%   Verdadero si existe alguna fila, columna o diagonal del tablero
%   cuyas celdas estén vacías o tengan el símbolo Símbolo. 
% =====================================================================
posibilidades_ganadoras(Símbolo, Tablero) :-
    (append(_, [[Celda1, Celda2, Celda3, Celda4]|_], Tablero);
    (Tablero = [A, B, C, D],
    append(Derecha1, [Celda1|_], A),
    append(Derecha2, [Celda2|_], B),
    append(Derecha3, [Celda3|_], C),
    append(Derecha4, [Celda4|_], D),
    length(Derecha1, Longitud),
    length(Derecha2, Longitud),
    length(Derecha3, Longitud),
    length(Derecha4, Longitud));
    Tablero = [[Celda1, _, _, _], [_, Celda2, _, _], [_, _, Celda3, _], [_, _, _, Celda4]];
    Tablero = [[_, _, _, Celda1], [_, _, Celda2, _], [_, Celda3, _, _], [Celda4, _, _, _]]),
    (Celda1 = Símbolo; Celda1 = (-)),
    (Celda2 = Símbolo; Celda2 = (-)),
    (Celda3 = Símbolo; Celda3 = (-)),
    (Celda4 = Símbolo; Celda4 = (-)).

% =====================================================================
%   evaluación_heurística/2.
%   evaluación_heurística(+Estado, -Evaluación).
%   Implementación declarativa de la función de evaluación heurística:
%
%                           f(e) = G - P
%
%   Donde Evaluación es f(e) y Estado es el estado que se va a evaluar.
% =====================================================================
evaluación_heurística((Tablero, _), Evaluación) :-
    símbolo(2, SímboloAgente),
    símbolo(1, SímboloHumano),
    aggregate_all(count, posibilidades_ganadoras(SímboloAgente, Tablero), G),
    aggregate_all(count, posibilidades_ganadoras(SímboloHumano, Tablero), P),
    Evaluación is G - P.

% =====================================================================
%   minimax/4.
%   minimax(+Estado, +Profundiad, -SiguienteJugada, -Evaluación).
%   Implementación declarativa del algoritmo de Mini Max con nivel de
%   profundización máximo.
% =====================================================================
minimax((Tablero, Jugador), Profundidad, SiguienteJugada, Evaluación) :-
    Profundidad > 0,
    obtener_jugadas_posibles((Tablero, Jugador), Jugadas),
    M is Profundidad - 1,
    obtener_mejor_jugada(Jugadas, M, SiguienteJugada, Evaluación), !.
minimax(Estado, _, _, Evaluación) :-
    evaluación_heurística(Estado, Evaluación).

% =====================================================================
%   obtener_mejor_jugada/4.
%   obtener_mejor_jugada(+Jugadas, +Profundidad, -MejorJugada, -MejorEvaluación).
%   Dada una lista de estados Jugadas, se evalúa cada uno con el
%   algoritmo MiniMax con profundiad de búsqueda límite de Profundidad.
%   El estado con mejor evaluación es devuelto en MejorJugada y su
%   evaluación en MejorEvaluación.
% =====================================================================
obtener_mejor_jugada([Jugada], Profundidad, Jugada, Evaluación) :-
    minimax(Jugada, Profundidad, _, Evaluación), !.
obtener_mejor_jugada([JugadaA|RestoJugadas], Profundidad, MejorJugada, MejorEvaluación) :-
    minimax(JugadaA, Profundidad, _, EvaluaciónA),
    obtener_mejor_jugada(RestoJugadas, Profundidad, JugadaB, EvaluaciónB),
    asignar_valor(JugadaA, EvaluaciónA, JugadaB, EvaluaciónB, MejorJugada, MejorEvaluación).

% =====================================================================
%   asignar_valor/6.
%   asignar_valor(+EstadoA, +EvaluaciónA, +EstadoB, +EvaluaciónB, -Estado, -Evaluación).
%   Verdadero si Estado y Evaluación corresponden al estado con la
%   mínima evaluación (caso del jugador humano, mini), o la máxima
%   evaluación (caso del agente jugador, max).
% =====================================================================
asignar_valor((Tablero, 1), EvaluaciónA, _, EvaluaciónB, (Tablero, 1), EvaluaciónA) :-
    EvaluaciónA >= EvaluaciónB, !.
asignar_valor((Tablero, 2), EvaluaciónA, _, EvaluaciónB, (Tablero, 2), EvaluaciónA) :-
    EvaluaciónA =< EvaluaciónB, !.
asignar_valor(_, _, JugadaB, EvaluaciónB, JugadaB, EvaluaciónB).

% =====================================================================
%   celda_válida/2.
%   celda_válida(+Tablero, [+Fila, +Columna]).
%   Verdadero si la celda en la intersección de Fila y Columna de
%   Tablero corresponde a una celda vacía (-).
% =====================================================================
celda_válida(Tablero, [Y, X]) :-
    nth1(Y, Tablero, Fila),
    nth1(X, Fila, Celda),
    Celda = (-).

% =====================================================================
%   jugada_humano/3.
%   jugada_humano(+Estado, [+Fila, +Columna], -NuevoEstado).
%   Llena la celda en la intersección de Fila y Columna del tablero de
%   Estado con el símbolo del jugador en turno y devuelve el nuevo
%   estado en NuevoEstado.
% =====================================================================
jugada_humano((Tablero, 1), [Y, X], (NuevoTablero, 2)) :-
    nth1(Y, Tablero, Fila),
    sustituir(X, Fila, x, NuevaFila),
    sustituir(Y, Tablero, NuevaFila, NuevoTablero).

% =====================================================================
%   mostrar_tablero/1.
%   mostrar_tablero(+Tablero).
%   Despliega en consola el tablero del Gato 4x4 en una forma amigable
%   para el jugador humano.
% =====================================================================
mostrar_tablero([A,B,C,D]) :-
    format('~t    ~w ~t ~t ~w ~t ~t ~w ~t ~t ~w ~t ~n', [1, 2, 3, 4]),
    format('~t  - ~t-~t - ~t-~t - ~t-~t - ~t-~t - ~n', []),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['A'|A]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n',[]),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['B'|B]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n', []),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['C'|C]),
    format('~t  + ~t-~t + ~t-~t + ~t-~t + ~t-~t + ~n',[]),
    format('~w |~t ~w ~t|~t ~w ~t|~t ~w ~t|~t ~w ~t| ~n', ['D'|D]),
    format('~t  - ~t-~t - ~t-~t - ~t-~t - ~t-~t - ~n', []).

% =====================================================================
%   jugar/2.
%   jugar((+Tablero, +Humano), +Profundidad).
%   Permite al jugador Humano realizar una jugada en el tablero Tablero
%   para llamar posteriormente a jugar al agente artificial con una
%   profundidad de búsqueda de Profundidad niveles.
% =====================================================================
jugar((Tablero, 1), Profundidad) :-
    write('¡Es tu turno!'), nl, nl,
    write('Ingresa la posición de tu jugada ["Fila", Columna]: '),
    read([Letra, X]),
    obtener_fila(Letra, Y),
    (\+ celda_válida(Tablero, [Y, X]) ->
    (nl, write('¡Ups! Jugada no válida, por favor vuelve a intentarlo.'), nl, nl, jugar((Tablero, 1), Profundidad));
    (jugada_humano((Tablero, 1), [Y, X], (NuevoTablero, 2)),
    nl, mostrar_tablero(NuevoTablero), nl,
    símbolo(1, SímboloHumano),
    (jugada_ganadora(SímboloHumano, NuevoTablero) -> write('¡Felicidades, ganaste!');
    (empate(NuevoTablero) -> write('¡Empataste con la máquina!');
    (write('Ingresa cualquier letra para continuar'), read(_), jugar((NuevoTablero, 2), Profundidad)))))).

% =====================================================================
%   jugar/2.
%   jugar((+Tablero, +Máquina), +Profundidad).
%   Permite al agente artificial calcular su siguiente jugada con la
%   ayuda del algoritmo Mini Max.
% =====================================================================
jugar((Tablero, 2), Profundidad) :-
    nl, write('¡Ahora va la máquina!'), nl,
    minimax((Tablero, 2), Profundidad, (NuevoTablero, SiguienteJugador), _),
    nl, mostrar_tablero(NuevoTablero), nl,
    símbolo(2, SímboloAgente),
    (jugada_ganadora(SímboloAgente, NuevoTablero) -> write('¡Ay, no! Ganó la máquina. :C');
    (empate(NuevoTablero) -> write('¡Empataste con la máquina!');
    (jugar((NuevoTablero, SiguienteJugador), Profundidad)))).

% =====================================================================
%   jugar/2.
%   jugar(+Profundidad).
%   Inicia una partida de Gato 4x4 en la que el agente artificial
%   tendrá una profundidad límite en su árbol de búsqueda de
%   Profundidad niveles.
% =====================================================================
jugar(Profundidad) :-
    TableroInicial = [[-, -, -, -], [-, -, -, -], [-, -, -, -], [-, -, -, -]],
    nl, mostrar_tablero(TableroInicial), nl,
    jugar((TableroInicial, 1), Profundidad).
