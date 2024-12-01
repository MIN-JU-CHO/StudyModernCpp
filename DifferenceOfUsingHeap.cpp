// This is for checking Difference Of Using Heap between C / C++

#include <cstdio>
#include <iostream>
class Cat
{
public:
		Cat()
		{std::cout << "meow\n";}
		~Cat()
		{std::cout << "bye\n";}
};
int main(void)
{
		// C style heap Cat Array
		Cat* catap = (Cat*) malloc(sizeof(Cat)*5);  // (X) Cannot call constructor
		free(catap);                                // (X) Cannot call destructor
		
		// C++ style heap Cat Array
		Cat* catap = new Cat[5];                    // Call constructor 5 times
		delete [] catap;                            // Call destructor 5 times
		
		// ** Safer C++ style heap **
		// delete 안해도 되는 방법 -> 스마트 포인터, 배열은 STL vector 등 RAII 패턴 이용
		std::unique_ptr<Cat> catp = std::make_unique<Cat>();
		std::vector<Cat> cats(5);
}