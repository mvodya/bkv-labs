%  Software engineering
%  Lab 7

% Протестированно в:
% SWI-Prolog (64 bit / version 8.0.2)

% Люди, у которых нет детей (и не получается определить по определению мать/отец)
man(peter).
man(george).
% Правило: все отцы - мужчины
man(X) :- father(X, _).

woman(anna2).
woman(maria3).
woman(charlotte).
% Правило: все матери - женщины
woman(X) :- mother(X, _).

% Установление связей отцовства
father(claude, eliz).
father(rich6, eliz2).
father(filipp, charles).
father(andrew, filipp).
father(rich1, andrew).
father(louis, anna).
father(rich5, rich6).
father(charles, henry).
father(charles, william).
father(william, charlotte).
father(william, george).
father(henry, peter).

% Установление связей материнства
mother(cecilie, eliz).
mother(eliz2, charles).
mother(eliz2, anna2).
mother(eliz2, maria2).
mother(maria2, maria3).
mother(anna, filipp).     
mother(olga, andrew).
mother(maria, rich6).
mother(eliz, eliz2).
mother(diana, henry).
mother(diana, william).
mother(catherine, charlotte).
mother(catherine, george).

%% ОСНОВНЫЕ ПРАВИЛА %%

% Родители
parent(X, Y) :- father(X, Y) ; mother(X, Y).

% Предки
prec(X, Y) :- father(X, Y).
prec(X, Y) :- mother(X, Y).
prec(X, Y) :- parent(X, Z), prec(Z, Y).

% Потомки
suc(X, Y) :- father(Y, X); mother(Y, X).
suc(X, Y) :- father(Z, X), suc(Z, Y).
suc(X, Y) :- mother(Z, X), suc(Z, Y).

% Сыновья
listson(X, Y) :- man(X), parent(Y, X).
son(X, Y) :- setof(_, listson(X, Y), _).

% Дочери
listdaughter(X, Y) :- woman(X), parent(Y, X).
daughter(X, Y) :- setof(_, listdaughter(X, Y), _).

% Внуки
listgrandson(X, Y) :- man(X), parent(Y, Z), parent(Z, X).
grandson(X, Y) :- setof(_, listgrandson(X, Y), _).
listgranddaughter(X, Y) :- woman(X), parent(Y, Z), parent(Z, X).
granddaughter(X, Y) :- setof(_, listgranddaughter(X, Y), _).

% Братья
listbrother(X, Y) :- man(X), parent(Z, X), parent(Z, Y), X \== Y.
brother(X, Y) :- setof(_, listbrother(X, Y), _).

% Дяди
uncle(X, Y) :- setof(_, (parent(Z, Y), listbrother(X, Z)), _).

% Сестры
listsister(X, Y) :- woman(X), parent(Z, X), parent(Z, Y), X \== Y.
sister(X, Y) :- setof(_, listsister(X, Y), _).

% Тети
aunt(X, Y) :- setof(_, (parent(Z, Y), listsister(X, Z)), _).

% Деды
grandfather(X, Y) :- setof(_, listgrandfather(X,Y), _).
listgrandfather(X, Y) :-  man(X), parent(Z, Y), parent(X, Z).

% Бабушки
grandmother(X, Y) :- setof(_, listgrandmother(X, Y), _).
listgrandmother(X, Y) :- woman(X), parent(Z, Y), parent(X, Z).

% Кузины
cousin(X, Y) :- setof(_, listcousin(X, Y), _).
listcousin(X, Y) :- uncle(Z, Y), parent(Z, X).
listcousin(X, Y) :- aunt(Z, Y), parent(Z, X).