#include <iostream>


#include "glad/glad.h"
#include "GLFW/glfw3.h"

//#include "ExampleBase.h"
//#include "Example02.h"
//#include "Example03.h"
#include "Example04.h"
int main() {

	//ExampleBase* example = new ExampleBase();
	//example->Run();

	//Example02 example02{};
	//example02.Run();

	//Example03 example03{};
	//example03.Run();

	Example04 example04{};
	example04.RunWidthParam(UvType::Fit, GL_REPEAT, GL_LINEAR);

	return 0;
}