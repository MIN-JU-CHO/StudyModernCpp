# 익명 네임스페이스
## 기본 특징

- Internal Linkage를 가짐.
    - 다른 파일에서 동일 이름 선언 가능.
- 네임스페이스의 이름이 고유한 이름으로 대체됨. (컴파일러)
    
    ```cpp
    namespace {
    		int i;
    }
    ```
    
    - `::(unique)::i` 로 처리
- 특정 파일 내에서만 유효한 로컬 네임 스페이스
    - `static` 전역 변수/함수와 유사

### 사용 목적

특정 파일 내에서만 사용하게끔 정의하여, **이름 충돌 방지**

**전역** 범위 변수를 **캡슐화** → 의도치 않은 외부 접근 방지

## 코드 분석

```cpp
namespace
{
    int i; // defines ::(unique)::i
}
 
void f()
{
    i++;   // increments ::(unique)::i
}
 
namespace A
{
    namespace
    {
        int i;        // A::(unique)::i
        int j;        // A::(unique)::j
    }
 
		// 위 익명 네임스페이스는 namespace A에 종속되므로, 이 범위 밖에서는 A::i, A::j 명시 필요
    void g() { i++; } // A::(unique)::i++
}
 
using namespace A; // introduces all names from A into global namespace
 
void h()
{
    i++;    // ERROR: ::(unique)::i and ::A::(unique)::i are both in scope
    A::i++; // ok, increments ::A::(unique)::i
    j++;    // ok, increments ::A::(unique)::j
}
```

## 익명 네임스페이스와  `static` 의 차이

- `static` 키워드:
    - 전역 변수나 함수에 사용하면 Internal Linkage를 가짐
    - 하지만 **네임스페이스를 구분하지 않고**, 단순히 전역 변수/함수의 범위를 해당 번역 단위로 제한
- 익명 네임스페이스:
    - 네임스페이스를 정의하고, 그 안에 선언된 모든 이름에 Internal Linkage를 부여
    - 아래처럼 구조적으로 코드를 더 깔끔하게 관리

```cpp
namespace {
    int counter = 0;

    void incrementCounter() {
        counter++;
    }
}

namespace {
		bool flag = false;
		
		void switchFlag() {
				flag = !flag;
		}
}

void doSomething() {
    incrementCounter();
    std::cout << "Counter: " << counter << "\n"; // Counter: 1
    
    switchFlag();
    std::cout << "Flag: " << flag << "\n"; // Flag: 1
    
    switchFlag();
    std::cout << "Flag: " << flag << "\n"; // Flag: 0
}
```