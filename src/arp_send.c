#include <stdio.h>


int main(int argc, const char* argv[])
{
    extern int main_stub_arp_send(int argc,const char* argv[]);
	return main_stub_arp_send(argc,argv);
}
