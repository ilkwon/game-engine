#include <iostream>
#include "ExampleBase.h"
#include "Example02.h"
#include "Example03.h"
int main() {

	//ExampleBase* example = new ExampleBase();
	//example->Run();

	//Example02 example02{};
	//example02.Run();

	Example03 example03{};
	example03.Run();

	return 0;
}