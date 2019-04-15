#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <modules/ForwardingInformationBase.hpp>

void fib_constructor_test(void)
{
	chdir("test");
	/**
	  * Because this is pretty hard to test because of the lack of public function
      * It will be inspected by gdb
      */
	ForwardingInformationBase* fib = new ForwardingInformationBase("routingtable/rt1");
	delete fib;
}
