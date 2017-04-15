struct x{
    int a;
    char b;
    char* c;
};

char* test(struct x* src)
{
    int a = 10;
    if(a==10)
	a = 20;
    else
	a = 30;
    {
        char* b = 'c';
        test(b);
    }
    return a;
}
