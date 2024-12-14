# Linkage 4가지

## 1. External Linkage

- 다른 translation unit에서 재선언할 수 있음.
    - 의미: 여러 곳에서 재선언(redeclare)되더라도 **동일한 엔티티**로 간주되어 **공유**
    - `extern` 키워드를 명시적으로 사용하거나
        
        ```cpp
        // file2.cpp
        extern void myFunction(); // 재선언
        ```
        
    - 기본적으로 외부 연결성을 가질 때 성립 (ex: static 안 쓴 전역 변수 등)
        
        ```cpp
        // file1.cpp
        void myFunction() {
            // Do something
        }
        ```
        

### `export`의 역할

- **모듈의 이름이나 엔티티를 외부로 공개**
- 공개된 이름(함수, 변수, 클래스 등)은 **다른 파일(translation unit)에서 접근 가능**
- `export` 없이 선언된 이름은 모듈 내부에서만 사용 가능

### 모듈 외부까지 External Linkage

- (C++20 이후) 재선언된 이름이 다른 새 모듈에 부착 가능.
    
    ```cpp
    // file1.cpp (lib_A 모듈)
    export module lib_A;
    
    export extern int sharedVar = 0; // sharedVar는 외부 연결성. 모듈 lib_A에서 선언됨.
    ```
    
    ```cpp
    // file2.cpp (lib_B 모듈)
    export module lib_B;
    
    import lib_A; // lib_A를 가져와 사용
    export extern int sharedVar; // sharedVar를 다시 선언. 이번엔 lib_B에 연결됨.
    ```
    
    ```cpp
    // main.cpp
    import lib_B; // lib_B만 import
    
    int main() {
        printf("%d\n", sharedVar); // lib_B에서 lib_A의 sharedVar를 사용 가능
    }
    ```
    
    - **sharedVar**는 다른 2개의 translation unit에서 선언되었지만, **다른 모듈(lib_A, lib_B)에서 사용 가능**
    - 외부 연결성을 가지므로 **프로그램 전체에서 공유**

## 2. Module Linkage (C++20 이후)

- 특정 모듈에만 국한됨
- 해당 모듈에서만 재선언 가능
- **다른 모듈과 절대 공유되지 않음 → `import` 하는 곳도 역시 사용 불가능**

```cpp
// lib_A.ixx
export module lib_A;

module int sharedVar; // module linkage: lib_A 모듈 내에서만 공유
```

### 모듈 내에서만 External Linkage → Module Linkage

```cpp
// lib_A.ixx
export module lib_A;

int moduleVar = 42; // module linkage: lib_A 모듈 내에서만 공유
```

- 즉, Module Linkage 안에서 **export 안하고 External Linkage 사용하는 경우** **결국 Module Linkage**가 된다. 결국 위의 코드와 동일한 의미.

```cpp
// lib_A.ixx
export module lib_A;

extern int internalSharedVar; // extern만 사용: lib_A 내부에서만 공유 가능
```

- extern 없어도 저 scope에서는 extern 이 된다(static 명시하지 않은 이상). 따라서 동일 의미.

### External Linkage와 Module Linkage를 전부 이해했다면…

```cpp
export module lib_A;
 
namespace ns // ns is not attached to lib_A.
{
    export extern "C++" int f(); // f is not attached to lib_A.
           extern "C++" int g(); // g is not attached to lib_A.
    export              int h(); // h is attached to lib_A.
}
// ns::h must be defined in lib_A, but ns::f and ns::g can be defined elsewhere
```

- 모듈에 Attach 된다는 의미
    - attach 된 이름은 모듈의 ODR(One Definition Rule) 규칙에 따라야 함. 모듈과 강하게 결합.
- ODR(One Definition Rule)
    - 모듈에 attach된 이름은 해당 **모듈 내부**에서 **단 1번만 정의**되어야 함.
- `extern` 디폴트: `extern “C++”`
    - C++ 네이밍 규칙, namespace 스코프가 영향을 주고, **모듈과의 ODR 결합 방해**.
- **`f`**:
    - 정의: 모듈 내부 또는 외부 어디에서든 **한 번만 정의** 가능.
        - `extern "C++"` 때문에 모듈에 attach되지 않으므로, 모듈 내부에 정의될 필요는 없음.
    - 사용: `lib_A`를 `import`한 모든 곳에서 사용 가능.
- **`g`**:
    - 정의: 모듈 내부 또는 외부 어디에서든 **한 번만 정의** 가능.
    - 사용: **모듈 내부에서만 사용 가능**.
        - 주의: `export`가 없으므로 `import`로 사용 불가능
- **`h`**:
    - 정의: ODR 규칙에 따라 **모듈 내부**에서 정의(`ns` 네임스페이스 안에서, 혹은 `ns::h`)되어야 하며, 다른 번역 단위에서 재정의(overload 포함) 불가.
    - 사용: `lib_A`를 `import`한 모든 곳에서 사용 가능.

| 이름 | 정의 위치 조건 | 사용 가능 범위 |
| --- | --- | --- |
| `f` | 모듈 내부/외부 어디서든 가능 | **모듈 외부에서도 사용 가능** |
| `g` | 모듈 내부/외부 어디서든 가능 | **모듈 내부에서만 사용 가능** |
| `h` | 모듈 내부의 `ns` 안에서 정의
모듈 내부 `ns::h` 로 정의 | **모듈 외부에서도 사용 가능** |

## 3. Internal Linkage

**`namespace` 범위**에서 선언된 다음 이름은 모두 내부 링크를 갖는다.

- 전역 변수, 변수 템플릿(C++14부터), 함수 또는 함수 템플릿이 `static`으로 선언 됨
    
    ```cpp
    namespace ns {
    		static int a = 0;
    }
    // ns::a 사용 가능
    ```
    
    >💡`static`: 
    >1. [declarations of namespace members with static storage duration and **internal linkage**](https://en.cppreference.com/w/cpp/language/storage_duration)
    >2. [definitions of block scope variables with static storage duration and initialized once](https://en.cppreference.com/w/cpp/language/storage_duration#Static_block_variables)
    >3. [declarations of class members not bound to specific instances](https://en.cppreference.com/w/cpp/language/static)<br>
    >**(1) 예시: namespace `static` 멤버 변수는 `::` 통해서 접근 가능. ⇒ 이 경우를 말한다.**<br>
    >(2) 예시: 함수 내부에 선언된 `static` 변수는 한 번만 초기화 됨<br>
    >(3) 예시: class 안에서 정의한 `static`은 인스턴스와 상관 없이 `::` 통해서만 접근 가능.
    
    
- 비휘발성 `const`-qualified 유형의 **비템플릿 변수** (C++14 이후)
    - 단, 아래는 **예외**적으로 Internal Linkage가 아니다.
        - 인라인 (C++17 이후)
        - [모듈 인터페이스 단위](https://en.cppreference.com/w/cpp/language/modules#Module_declarations) ( [private 모듈 조각](https://en.cppreference.com/w/cpp/language/modules#Private_module_fragment) 외부 , 있는 경우) 또는 [모듈 파티션](https://en.cppreference.com/w/cpp/language/modules#Module_partitions) 의 범위 내에서 선언됨. (C++20 이후)
        - `extern`으로 선언되었거나
        - `extern`으로 선언 되어서 internal linkage가 없었던 경우
    - 예시
        
        ```cpp
        const int constVar = 42; // Internal Linkage
        extern const int externConstVar = 100; // External Linkage
        inline const int inlineConstVar = 200; // External Linkage
        ```
        
- [익명 union](https://en.cppreference.com/w/cpp/language/union)의 멤버 변수들
    - 예시
        
        ```cpp
        int main() {
            union {
                int a;
            };
            a = 1;
        }
        ```
        
- [익명 네임스페이스](https://github.com/MIN-JU-CHO/StudyModernCpp/blob/main/UnnamedNamespace.md)
- 익명 네임스페이스 내의 네임스페이스 (`extern`을 명시하더라도 결국 익명에 감싸지니까)

## 4. No Linkage

**블록 범위**에서 선언된 다음 이름은 연결이 없음.

- 명시적으로 extern으로 선언되지 않은 변수 (static 수정자와 관계없이)
    - 전역 변수 제외. 함수 안의 변수 케이스만 가능
        
        ```cpp
        void func() {
            int x = 42; // no linkage
        }
        ```
        
- 로컬 클래스와 그 멤버 함수 (일반적인 지역 변수)
- typedef, enumerations, enumerators 등 블록 범위에서 선언된 다른 이름.

external , module, (C++20부터) 또는 internal linkage로 지정되지 않은 이름은 선언된 범위와 관계없이 링크가 없음.