#include"types.h"
#include"stat.h"
#include"user.h"

int a=1;
void basictest(void){
	printf(1,"before fork %d\n",get_n_free_pages());
	int pid=fork();//decrease because of lazy alloc
	printf(1,"after fork %d\n",get_n_free_pages());
	if(pid!=0){
		wait();
		if(fork()==0){
			printf(1,"second after fork %d\n",get_n_free_pages());//maintain because of cow
			a=5;
			printf(1,"change second after fork %d\n",get_n_free_pages());//decrease of cow
			exit();
		}else{
			wait();
			printf(1,"final %d\n",get_n_free_pages());//restore
			sleep(5);
		}
	}
	return ;
}
void test1(void)
{
	printf(1,"2before fork %d\n",get_n_free_pages());
	int pid=fork();
	if(pid==0){
		printf(1,"2before change after fork %d\n",get_n_free_pages());
		a=2;
		printf(1,"2after change after fork %d\n",get_n_free_pages());
		exit();
	}else{
		wait();
		printf(1,"2final %d\n",get_n_free_pages());
	}
	return ;
}
int main(void)
{
	basictest();
//	test1();
	exit();
}
