#include <stdio.h>
#include <stdlib.h>
#include "rest_module.h"
#include "rest_vmad.h"


/**@todo get ride of that global variable as soon as the dynamic library is in place*/
restModule *mymodule;

void restModuleLoad ()
{
	//module(s) loading
	
	//module(s) init
	//bind
	restBind(mymodule);
	//init
	mymodule->init(mymodule);
}




void restModuleUnload ()
{
	//quit
	mymodule->quit(mymodule);
}
