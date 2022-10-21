% =====================================================================
%   Diego Armando Apolonio Villegas
%
%   TAREA #3: Problemas tipo cebra.
%
%   - Resuelva en Prolog los tres problemas recortados de tipo cebraque
%   se describen en las láminas #43 y #44 de la presentación #15
%   Razonamiento por unificación.
%
%   - Entregue cada problema en un archivo diferente (suba 3 archivos
%   como solución a esta tarea).
%
%   - Puede, si así lo desea, resolver en SWISH y entregar sólo la liga
%   a los archivos originales.
%
%   VERSIÓN #3:
%       4 casas, 4 atributos y 9 pistas.
%
%       1. Hay dos casas entre la del bolichista y la del nadador.
%       2. Hay una casa entre la del irlandés y la del que juega
%       voleyball.
%       3. La segunda casa es negra.
%       4. Hay una casa entre la del dueño de caballos y la casa roja.
%       5. Un escocés vive junto al dueño de tortugas.
%       6. Hay dos casas entre la del dueño de caballos y la casa del
%       dueño de mariposas.
%       7. El bolichista vive en algún lugar posterior a la casa del
%       tenista.
%       8. Hay una casa entre la del que juega voleyball y la casa
%       blanca.
%       9. Un ruso vive en la primera casa.
%
%   En esta versión, una casa es modelada con el predicado casa/4:
%
%           casa(<color>, <nacionalidad>, <deporte>, <animal>)
%
%   Para visualizar las soluciones parciales al problema tipo cebra de
%   esta versión, utilice el predicado visualiza_vecindario/1.
%
%               visualiza_vecindario(<lista de casas>).
% =====================================================================

% =====================================================================
%   dos_casas_entre/3.
%   dos_casas_entre(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) con una
%   subsecuencia en la que aparece Casa1 seguida de dos casas
%   arbitrarias, seguidas de Casa2.
% =====================================================================
dos_casas_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa1, _, _, Casa2|_], Vecindario).
dos_casas_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa2, _, _, Casa1|_], Vecindario).

% =====================================================================
%   una_casa_entre/3.
%   una_casa_entre(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) con una
%   subsecuencia en la que se encuentra Casa1 seguida de una casa
%   arbitraria, seguida de Casa2.
% =====================================================================
una_casa_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa1, _, Casa2|_], Vecindario).
una_casa_entre(Casa1, Casa2, Vecindario) :- append(_, [Casa2, _, Casa1|_], Vecindario).

% =====================================================================
%   junto/3.
%   junto(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) en la que
%   Casa1 y Casa2 se encuentran una junto a la otra.
% =====================================================================
junto(Casa1, Casa2, Vecindario) :- append(_, [Casa1, Casa2|_], Vecindario).
junto(Casa1, Casa2, Vecindario) :- append(_, [Casa2, Casa1|_], Vecindario).

% =====================================================================
%   posterior/3.
%   posterior(+Casa1, +Casa2, -Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) en la que
%   Casa2 se encuentra en alguna posición posterior a Casa1.
% =====================================================================
posterior(Casa2, Casa1, Vecindario) :-
    append(_, [Casa1|Resto], Vecindario),
    append(_, [Casa2|_], Resto).

% =====================================================================
%   visualiza_vecindario/1.
%   visualiza_vecindario(-Vecindario).
%   Vecindario es una sucesión de casas (lista de elementos) cumple con
%   las clausulas consecuentes de visualiza_vecindario/1.
% =====================================================================
visualiza_vecindario(V) :-
    V = [_, _, _, _],
    dos_casas_entre(casa(_, _, boliche, _), casa(_, _, natación, _), V),
    una_casa_entre(casa(_, irlanda, _, _), casa(_, _, voleyball, _), V),
    V = [_, casa(negro, _, _, _), _, _],
    una_casa_entre(casa(_, _, X, caballo), casa(rojo, _, _, _), V),
    junto(casa(_, escocia, _, _), casa(_, _, _, tortuga), V),
    dos_casas_entre(casa(_, _, X, caballo), casa(_, _, _, mariposa), V),
    posterior(casa(_, _, boliche, _), casa(_, _, tenis, _), V),
    una_casa_entre(casa(_, _, voleyball, _), casa(blanco, _, _, _), V),
    V = [casa(_, rusia, _, _), _, _, _].
