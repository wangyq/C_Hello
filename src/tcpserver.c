#include <stdio.h>


int main(int argc, const char* argv[])
{
    extern int main_tcpserver_stub(int argc,const char* argv[]);
	return main_tcpserver_stub(argc,argv);
}
