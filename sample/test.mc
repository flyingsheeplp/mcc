int a = 0;

struct s
{
	int a;
	short b;
	char c;
};

char* string = "hello world!\n";

char t = 'E';

a.b = ...;

int func(int a, struct s* s)
{
	if(a>0){
		a = s->a - s->c;
	}else{
		a = s->a;
	}

	return 1;
}