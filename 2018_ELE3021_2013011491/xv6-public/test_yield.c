#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
	int a=fork();
	if(a==0){
		while(1){
		printf(0,"child\n");
		yield();
		}
	}else{
		while(1){
		printf(0,"parent\n");
		yield();}
	}
}
