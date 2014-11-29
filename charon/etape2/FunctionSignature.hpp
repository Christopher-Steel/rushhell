#pragma once


/*
** ETAPE I.1
** Voil� �a tiens en une ligne, j'ai demand� � payan parce que j'avais des doutes et il a fait pareil.
** Je sais pas pourquoi dans le sujet il parle de sp�cialiser le template, je l'ai fait juste en dessous au cas o� mais bon...
*/

#include <iostream>

template <typename T>
struct		FunctionSignature
{
  typedef T	*type;
};

/*
** Apr�s tests, les surcharges ci dessous ne changent pas le comportement du code
** Je pensais au d�but que ca empecherait d'utiliser le template avec des fonctions
** de plus de 4 arguments puisque c'est ce qu'ils semblent demander mais il n'en
** est rien.
*/
/*
template <typename T, typename U>
struct		FunctionSignature <T(U)>
{
  typedef T	(*type)(U);
};

template <typename T, typename U, typename V>
struct		FunctionSignature <T(U, V)>
{
  typedef T	(*type)(U, V);
};

template <typename T, typename U, typename V, typename W>
struct		FunctionSignature <T(U, V, W)>
{
  typedef T	(*type)(U, V, W);
};

template <typename T, typename U, typename V, typename W, typename X>
struct		FunctionSignature <T(U, V, W, X)>
{
  typedef T	(*type)(U, V, W, X);
};
*/
