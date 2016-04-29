#include <stdio.h>


int main(int argc, const char* argv[])
{
    extern int main_sniffer_stub(int argc,const char* argv[]);
	return main_sniffer_stub(argc,argv);
}
