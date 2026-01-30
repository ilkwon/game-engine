#include <iostream>
#include "ExampleBase.h"
#include "Example02.h"

int main() {

	//ExampleBase* example = new ExampleBase();
	//example->Run();

	Example02 example02{};
	example02.Run();

	return 0;
}