#include <stdio.h>


int main(int argc, const char* argv[])
{
    extern int main_stub_tcp_attack(int argc,const char* argv[]);
	return main_stub_tcp_attack(argc,argv);
}
