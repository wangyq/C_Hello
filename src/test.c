#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "conf.h"
#include "log.h"
#include "tree.h"

void test_aa()
{
    //char buf[1024];

    //char* pChar = buf + 10;

//	char msg[128];
//
//	printf("CurDir = %s\n", g_var.pCurDir);
//	printf("ConfigFile = %s\n", g_var.pConfFile);
//	printf("section[app]-name[hello] 's value = %s \n", conf_getstr("app","hello",msg,127, "null msg"));
//	printf("section[app]-name[routerid] 's value = %d \n", conf_getint("app","routerid",0));
//
//	printf("cur time = %s \n", util_get_curtime(msg,127));
//
//	LOG_ERROR("error message! num = %d ,school=%s", 550,"peking");
//	LOG_WARN("warn message!");
}

/**
 * pmaod's main entry point!
 *
 */
int main(int argc, const char* argv[])
{

	//init!
	LOG_INFO("myhello start... ");

	test_aa();



	return 0;
}
