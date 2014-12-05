#include <iostream>
#include <cstdarg>

void TestVariableList(const char* content, ...)
{
	va_list variable_list;
	va_start(variable_list, content);

	va_end(variable_list);
}

int main()
{
	TestVariableList("%d", 123);

	std::cin.get();
	return 0;
}