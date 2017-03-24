#include <stdio.h>

typedef void  (*fp)(void);
fp freturn_fun(){	
	int a = 8;
	void fun(){
		printf("Hello, world! [%d]\n", a);
		
	}	
	return fun;
}


int main(){
	freturn_fun()();
}
