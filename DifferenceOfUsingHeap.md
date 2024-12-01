# 힙 메모리 사용 C/C++

**사용자 지정 클래스**의 인스턴스를 만들 때, **C 스타일의 힙 예약은 불가능**하다. 생성자와 소멸자가 호출되지 않는다.

```cpp
#include <stdlib.h>
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
		Cat* catap = (Cat*) malloc(sizeof(Cat)*5); // Cannot call constructor
		free(catap); // Cannot call destructor
		
		// C++ style heap Cat Array
		Cat* catap = new Cat[5]; // Call constructor 5 times
		delete [] catap; // Call destructor 5 times
		
		// Safer C++ style heap
		// delete 안해도 되는 방법 -> 스마트 포인터, 배열은 STL vector 등 RAII 패턴 이용
		std::unique_ptr<Cat> catp = std::make_unique<Cat>();
		std::vector<Cat> cats(5);
}
```

벡터는 C# 배열처럼 스택에 원소 첫 시작 주소를 저장하고, 힙에 원소 데이터를 저장한다. 다만 C# 배열과의 차이점은 **다른 힙 메모리를 잡아 동적 배열처럼 사용**할 수 있다.

<aside>
💡

- C/C++ 배열의 크기가 반드시 상수여야 하는 이유, C#/Java 배열의 크기는 런타임에 결정되어도 되는 이유 (그러나 둘 다 **정적 배열**로, 변동은 불가능)
    - C/C++ 배열은 원소 데이터를 스택에 저장한다.
        - **스택에 배열 원소가 직접 저장**되기 때문에, 컴파일 타임에 크기를 알아야 함.
        - **스택 메모리는 고정 크기 블록**으로 동작하므로, 컴파일러는 프로그램 실행 전에 각 함수의 스택 요구량 계산 필요.
        - 따라서 컴파일 타임에 알 수 있는 상수만 크기로 지정 가능.
    - C# 배열은 원소 데이터를 힙에 저장하고, 첫 시작 원소의 주소를 스택에 저장한다.
        - 따라서 스택에 저장하는 **주소의 크기**(어차피 8Byte <- 64bit기준)는 컴파일 타임에 알 수 있고,
        - 배열의 크기를 런타임에서 지정해도 된다. (변수를 사용하더라도 당시의 수로 처음 지정되면 그 크기로 고정된다. 정적 배열이므로.)
        - 힙을 사용한다고 해서 원소의 크기를 무조건 변동시킬 수 있는 것이 아니다. 힙에서 고정 사이즈만큼 예약해놓고, **예측 가능한 범위에서 연속적으로 메모리에 저장됨**을 잘 활용할 수 있는 자료구조다.
        - 힙을 사용하는 것의 장점은 반드시 컴파일 타임에 사이즈를 알지 않아도 된다는 점.
</aside>