#include <stdio.h>


int main(int argc, const char* argv[])
{
    extern int main_stub_arp_recv(int argc,const char* argv[]);
	return main_stub_arp_recv(argc,argv);
}
