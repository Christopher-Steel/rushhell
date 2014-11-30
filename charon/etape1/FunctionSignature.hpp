#pragma once

template <typename T>
struct							FunctionSignature;

template <typename T>
struct							FunctionSignature <T(void)>
{
	typedef T					(*type)();
};

template <typename T, typename U>
struct							FunctionSignature <T(U)>
{
	typedef T					(*type)(U);
};

template <typename T, typename U, typename V>
struct							FunctionSignature <T(U, V)>
{
	typedef T					(*type)(U, V);
};

template <typename T, typename U, typename V, typename W>
struct							FunctionSignature <T(U, V, W)>
{
	typedef T					(*type)(U, V, W);
};

template <typename T, typename U, typename V, typename W, typename X>
struct							FunctionSignature <T(U, V, W, X)>
{
	typedef T					(*type)(U, V, W, X);
};