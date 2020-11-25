#include <stdio.h>
int main()
{
	int f0, f1, f2, i;
	int m, r;
	f0 = 0;
	f1 = 1;
	m = 5;
	if (m < 1)
	{
		r = m;
	}
	else
	{
		i = 2;
		while (i <= m)
		{
			f2 = f0 + f1;
			f1 = f2;
			i++;
		}
		r = f2;
	}

	return 0;
}
