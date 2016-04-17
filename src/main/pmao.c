
#include "util.h"
#include "conf.h"
#include "log.h"
/*
 * pmao.c
 *
 *  Created on: 2012-6-5
 *      Author: pmsserver
 */


int cmdline_init(int argc, const char* argv[])
{


	return 0;
}


/**
 * pmao initialize function!
 */
void pmao_init(int argc, const char* argv[])
{
	//command line init!
	cmdline_init(argc,argv);

	//configuration init!
	conf_init();

	//other module's init comes here!
	// ......

}

/**
 * pmao destroy when exit!
 */
void pmao_destroy()
{
	//before quit, destroy any other resoure here!
	LOG_INFO("pmaod is going to be destroyed! ");

	//destroy other resoure here !
	//...
}
