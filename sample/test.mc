int a = 0;

struct s
{
	int a;
	short b;
	char c;
};


int func(int a, struct s* s)
{
	a = s->a;

	return 1;
}