#include<stdlib.h>
#include<stdio.h>

int g_var1;
int g_var2 = 20;

int main(int argc, char **argv)
{
	static int 	s_var1;
	static int	s_var2 = 20;
	char		*str="Hello";
	char		*ptr;
	ptr = malloc(100);
	
	printf("[cmd args]: argc address: %p\n", argv);
	printf("\n");

	printf("[Stack]: str address: %p\n", &str);
	printf("[Stack]: ptr address: %p\n", &ptr);
	printf("\n");

	printf("[Heap]: malloc address: %p\n", ptr);
	printf("\n");
	
	printf("[bss]: s_var1 address: %p value: %d\n",&s_var1, g_var1);
	printf("[bss]: g_var1 address: %p value: %d\n",&g_var1, g_var1);

	printf("[data]: g_var2 address: %p value: %d\n", &g_var2, g_var2);
	printf("[data]: s_var2 address: %p value: %d\n", &s_var2, s_var2);

	printf("[rodata]: \"%s\" address: %p\n", str, str);
	printf("\n");

	printf("[text]: main() address: %p\n", main);
	printf("\n");

	return 0;

}

