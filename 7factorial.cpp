#ifdef ACSE_USE_TEMPLATES
template<typename T>
T fact(T c)
{
	T factorial = 1;
	for (T i = 1; i <= c; i++)
	{
		factorial *= i;
	}
	return factorial;
}
template int fact<int>(int c);
template double fact<double>(double c);
#else
double fact(double c)
{
	double factorial = 1;
	for (double i = 1; i <= c; i++)
	{
		factorial *= i;
	}
	return factorial;
}

int fact(int c)
{
	int factorial = 1;
	for (int i = 1; i <= c; i++)
	{
		factorial *= i;
	}
	return factorial;
}
#endif
