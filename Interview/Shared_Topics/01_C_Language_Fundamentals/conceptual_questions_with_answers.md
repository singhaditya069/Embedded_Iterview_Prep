# C Language Fundamentals - Questions & Answers

## Data Types and Memory

### Q1: Primitive data types (char, int, float, double)
**Answer:**
- **char** (1 byte): Single character, range -128 to 127 (signed) or 0-255 (unsigned)
  - Used for: individual characters, small integer values
  - Example: `char c = 'A';` or `unsigned char b = 255;`

- **int** (typically 4 bytes): Integer values
  - Range: -2^31 to 2^31-1 (on 32-bit systems)
  - Example: `int count = 100;`
  - Variants: `short int` (2 bytes), `long int` (4-8 bytes)

- **float** (4 bytes): Single-precision floating point
  - Range: ~3.4e-38 to 3.4e+38
  - Precision: ~6-7 decimal digits
  - Example: `float temp = 23.5f;`

- **double** (8 bytes): Double-precision floating point
  - Range: ~1.7e-308 to 1.7e+308
  - Precision: ~15-17 decimal digits
  - Better for accurate calculations: `double pi = 3.14159265359;`

**Practical Note**: Use `double` for most calculations (better precision). Use `float` only when memory/bandwidth is critical.

---

### Q2: sizeof operator and memory layout
**Answer:**
- **sizeof** returns the size in bytes of a data type or variable
  ```c
  int i;
  printf("%zu bytes\n", sizeof(i));        // Typically 4 bytes
  printf("%zu bytes\n", sizeof(int));      // 4 bytes
  printf("%zu bytes\n", sizeof(float));    // 4 bytes
  printf("%zu bytes\n", sizeof(double));   // 8 bytes
  ```

- **Memory Layout Example**:
  ```
  Stack (higher addresses)
  ├─ Local variables
  ├─ Function parameters
  └─ Return addresses
  
  Heap (lower addresses, grows down)
  ├─ Dynamically allocated memory
  
  Data Segment
  ├─ Global initialized variables
  ├─ Static variables
  
  BSS Segment
  ├─ Global uninitialized variables
  ├─ Static uninitialized variables
  
  Text Segment (Read-only)
  └─ Program code
  ```

- **Practical Usage**: Use `sizeof` to ensure portable code across different architectures

---

### Q3: Signed vs. Unsigned types
**Answer:**
- **Signed Types**: Can represent both positive and negative values
  - Uses one bit for sign (MSB)
  - Range: -2^(n-1) to 2^(n-1)-1 for n-bit integer
  - Example: `signed char` (-128 to 127)

- **Unsigned Types**: Only positive values (and zero)
  - All bits used for magnitude
  - Range: 0 to 2^n - 1 for n-bit integer
  - Example: `unsigned char` (0 to 255)

- **Comparison**:
  ```c
  signed char s = -1;        // -1
  unsigned char u = -1;      // 255 (wraparound)
  
  if (s == u) { }  // TRUE! – dangerous
  if (s < 0) { }   // TRUE
  if (u < 0) { }   // FALSE
  ```

- **Best Practices**:
  - Use `unsigned` for: bit-flags, counts, addresses
  - Use `signed` for: calculations, temperatures, sensor values
  - Be careful when mixing signed/unsigned (implicit conversions)

---

### Q4: Type casting and promotion rules
**Answer:**
- **Implicit Type Conversion (Type Promotion)**:
  ```c
  char c = 5;
  int i = 10;
  float f = c + i;  // c promoted to int, then result to float
  
  // Promotion hierarchy: char → short → int → long → float → double
  ```

- **Explicit Type Casting**:
  ```c
  int i = 10;
  float f = (float)i;          // Explicit cast
  char c = (char)i;            // Truncates to lowest byte
  int truncated = (int)3.9;    // Loses decimal part
  ```

- **Dangerous Conversions to Avoid**:
  ```c
  int *ptr = (int *)malloc(sizeof(int));
  char *wrong = (char *)ptr;   // OK in C, but mixing types
  
  // Better: use void * and explicit cast when needed
  void *generic = malloc(sizeof(int));
  int *correct = (int *)generic;
  ```

- **Practical Embedded Example**:
  ```c
  uint32_t large = 0x12345678;
  uint8_t small = (uint8_t)large;  // small = 0x78
  ```

---

## Pointers

### Q1: Pointer declaration and usage
**Answer:**
- **Declaration**:
  ```c
  int i = 10;
  int *ptr;              // Pointer to int
  int *ptr = &i;         // Initialize with address of i
  
  int **ptr2ptr;         // Pointer to pointer to int
  int *arr_of_ptrs[10];  // Array of 10 pointers to int
  int (*ptr_to_arr)[10]; // Pointer to array of 10 integers
  ```

- **Basic Operations**:
  ```c
  int x = 5;
  int *p = &x;
  
  printf("%d\n", *p);    // Dereference: prints 5
  printf("%p\n", p);     // Address
  printf("%d\n", p[0]);  // Same as *p
  
  int *q = p;            // Copy pointer
  ```

- **Practical Usage in Embedded**:
  ```c
  // Register access
  volatile uint32_t *timer = (volatile uint32_t *)0x40000000;
  *timer = 1000;  // Write to timer register
  uint32_t count = *timer;  // Read timer value
  ```

---

### Q2: Pointer arithmetic
**Answer:**
- **Scaling Based on Type**:
  ```c
  int arr[] = {10, 20, 30, 40, 50};
  int *p = arr;          // Points to arr[0]
  
  p++;                   // Moves by sizeof(int) bytes (typically 4)
  printf("%d\n", *p);    // 20
  
  p += 2;                // Moves by 2 * sizeof(int) = 8 bytes
  printf("%d\n", *p);    // 40
  
  char *c = "hello";
  c++;                   // Moves by 1 byte
  printf("%c\n", *c);    // 'e'
  ```

- **Pointer Subtraction**:
  ```c
  int *p1 = &arr[3];
  int *p2 = &arr[0];
  int diff = p1 - p2;    // Difference in elements: 3
  ```

- **Common Mistake**:
  ```c
  int *p = (int *)0x1000;
  p += 1;                // Now points to 0x1004 (if sizeof(int) = 4)
  // NOT 0x1001 as a C++ programmer might expect!
  ```

---

### Q3: Void pointer and casting
**Answer:**
- **Generic Pointer**:
  ```c
  void *generic;
  
  int i = 42;
  generic = &i;          // OK - any pointer converts to void *
  
  char c = 'A';
  generic = &c;          // OK - can reassign
  
  int *p = (int *)generic;  // Explicit cast required to use
  ```

- **Use Cases**:
  ```c
  // Generic memory allocation
  void *mem = malloc(256);  // malloc returns void *
  int *ints = (int *)mem;
  char *chars = (char *)mem;
  
  // Generic callbacks
  void callback(void *arg) {  // Can receive any type
      int *data = (int *)arg;
      printf("%d\n", *data);
  }
  
  int value = 100;
  callback(&value);
  ```

- **Important**: Void pointer cannot be dereferenced without casting

---

### Q4: Function pointers and callbacks
**Answer:**
- **Declaration and Usage**:
  ```c
  // Function pointer declaration
  int (*func_ptr)(int, int);  // Points to function returning int, taking 2 ints
  
  // Function implementation
  int add(int a, int b) {
      return a + b;
  }
  
  // Assignment
  func_ptr = add;      // Or &add
  int result = func_ptr(3, 5);  // Call through pointer
  ```

- **Typedef for Clarity**:
  ```c
  typedef int (*arithmetic_func)(int, int);
  
  arithmetic_func operation = add;
  result = operation(10, 20);
  ```

- **Callback Pattern** (Common in embedded):
  ```c
  typedef void (*gpio_callback)(void);
  
  void gpio_enable_interrupt(uint8_t pin, gpio_callback handler) {
      // When interrupt occurs, call handler()
      handler();
  }
  
  void button_pressed(void) {
      printf("Button pressed!\n");
  }
  
  gpio_enable_interrupt(10, button_pressed);
  ```

---

### Q5: Pointer pitfalls: dangling pointers, null pointers
**Answer:**
- **Dangling Pointer** (Pointer to freed memory):
  ```c
  int *ptr = (int *)malloc(sizeof(int));
  free(ptr);
  *ptr = 5;              // DANGEROUS! Use-after-free
  ptr = NULL;            // Good practice after free
  ```

- **Null Pointer** (Invalid address):
  ```c
  int *ptr = NULL;
  if (ptr) {             // Check before dereference
      printf("%d\n", *ptr);
  }
  
  // Dereferencing NULL = crash
  *ptr = 10;             // CRASH!
  ```

- **Other Pitfalls**:
  ```c
  // Returning pointer to local variable
  int *bad_function(void) {
      int local = 5;
      return &local;     // WRONG! local destroyed after return
  }
  
  // Correct approach
  int *good_function(void) {
      int *heap = malloc(sizeof(int));
      *heap = 5;
      return heap;       // Caller must free
  }
  ```

- **Best Practices**:
  - Always initialize pointers: `int *p = NULL;`
  - Check before dereferencing: `if (ptr) { *ptr = ...; }`
  - Set to NULL after free: `free(ptr); ptr = NULL;`
  - Use static analysis tools (cppcheck, splint)

---

## Arrays and Strings

### Q1: Array declaration and initialization
**Answer:**
- **Declaration**:
  ```c
  int arr[5];                    // Uninitialized (contains garbage)
  int arr[5] = {0};              // All zeros
  int arr[5] = {1, 2, 3};        // {1, 2, 3, 0, 0}
  int arr[] = {10, 20, 30};      // Size inferred: 3
  ```

- **Storage Location**:
  ```c
  static int static_arr[10];     // Global scope, BSS segment (zeros)
  const int const_arr[] = {1,2}; // Read-only, text segment
  
  void function(void) {
      int local_arr[10];         // Stack, uninitialized
  }
  ```

- **Array in Memory**:
  ```c
  int arr[] = {10, 20, 30};
  // Memory layout (little-endian, 4 bytes per int):
  // arr[0]: 0x0A 0x00 0x00 0x00
  // arr[1]: 0x14 0x00 0x00 0x00
  // arr[2]: 0x1E 0x00 0x00 0x00
  ```

---

### Q2: Multi-dimensional arrays
**Answer:**
- **2D Array Declaration**:
  ```c
  int matrix[3][4];              // 3 rows, 4 columns
  int matrix[3][4] = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12}
  };
  ```

- **Memory Layout** (Row-major order):
  ```
  matrix[0][0] matrix[0][1] matrix[0][2] matrix[0][3]
  matrix[1][0] matrix[1][1] matrix[1][2] matrix[1][3]
  matrix[2][0] matrix[2][1] matrix[2][2] matrix[2][3]
  
  All stored contiguously in memory
  ```

- **Accessing Elements**:
  ```c
  matrix[1][2] = 7;              // Direct indexing
  *(*(matrix + 1) + 2) = 7;      // Pointer arithmetic
  ```

- **3D Arrays**:
  ```c
  int cube[2][3][4];             // 2 x 3 x 4
  cube[0][1][2] = 5;
  ```

---

### Q3: Array to pointer decay
**Answer:**
- **Automatic Conversion**:
  ```c
  int arr[5] = {1, 2, 3, 4, 5};
  int *ptr = arr;                // arr decays to pointer to first element
  
  // Equivalent to:
  int *ptr = &arr[0];
  ```

- **Function Parameter Declaration**:
  ```c
  void print_array(int arr[10]) {     // Looks like array parameter
      // But actually receives pointer!
  }
  
  // Same as:
  void print_array(int *arr) {        // This is what it really is
  }
  
  // Cannot use sizeof to get array size inside function
  void process(int arr[]) {
      // sizeof(arr) = sizeof(int *) = 4 or 8 bytes
      // NOT the array size!
  }
  ```

- **Consequences**:
  ```c
  void func(int arr[5]) {
      printf("%zu\n", sizeof(arr));   // Prints 4 or 8 (pointer size)
      // Must pass size separately:
  }
  
  int size = 5;
  func(arr);  // func must know size from context
  ```

---

### Q4: String manipulation and null-termination
**Answer:**
- **String Basics**:
  ```c
  char str1[] = "hello";          // Creates 6 bytes {h,e,l,l,o,\0}
  // NULL TERMINATOR is crucial!
  
  while (*str1 != '\0') {         // Common pattern
      printf("%c", *str1++);
  }
  ```

- **Null-Termination Importance**:
  ```c
  char str[5];
  strcpy(str, "hello");           // Needs 6 bytes but we only have 5
  // Buffer overflow! Corrupts memory
  
  char safe_str[6];
  strncpy(safe_str, "hello", 5);  // Safe version
  safe_str[5] = '\0';             // Ensure null terminator
  ```

- **String Safety**:
  ```c
  // Dangerous functions (no bounds checking)
  strcpy(dest, src);              // Can overflow
  strcat(str1, str2);             // Can overflow
  
  // Safe alternatives
  strncpy(dest, src, sizeof(dest)-1);
  strncat(str1, str2, MAX_LEN);
  snprintf(buffer, sizeof(buffer), "%s", str);
  ```

---

### Q5: String library functions (strcpy, strlen)
**Answer:**
- **strlen** - Get string length:
  ```c
  char str[] = "hello";
  size_t len = strlen(str);       // Returns 5 (not counting \0)
  
  // Manual implementation
  size_t my_strlen(const char *s) {
      size_t len = 0;
      while (s[len] != '\0') {
          len++;
      }
      return len;
  }
  ```

- **strcpy** - Copy string (UNSAFE):
  ```c
  char source[] = "hello";
  char dest[10];
  strcpy(dest, source);           // Unsafe! No bounds checking
  // If source > 10, buffer overflow occurs
  ```

- **strncpy** - Safe copy:
  ```c
  strncpy(dest, source, sizeof(dest) - 1);
  dest[sizeof(dest) - 1] = '\0';  // Ensure null termination
  ```

- **Other Functions**:
  ```c
  strcmp(str1, str2);             // Compare strings (0 if equal)
  strcat(dest, src);              // Concatenate (unsafe)
  strncat(dest, src, n);          // Safe concatenation
  strchr(str, 'a');               // Find character
  strstr(haystack, needle);       // Find substring
  ```

- **Best Practice**: Always use the 'n' (bounded) versions or snprintf

---

## Structs and Unions

### Q1: Struct layout and packing
**Answer:**
- **Basic Declaration**:
  ```c
  struct Point {
      int x;
      int y;
      char label;
  };
  
  // Size depends on alignment
  printf("%zu\n", sizeof(struct Point));  // Typically 12 bytes
  ```

- **Memory Layout (with padding)**:
  ```
  struct Point {
      int x;      // Offset 0-3 (4 bytes)
      int y;      // Offset 4-7 (4 bytes)
      char label; // Offset 8 (1 byte) + 3 bytes padding
  }; // Total: 12 bytes
  ```

- **Struct Initialization**:
  ```c
  struct Point p1 = {10, 20, 'A'};
  struct Point p2 = {.x = 5, .y = 15, .label = 'B'};
  ```

- **Passing to Functions**:
  ```c
  void move_point(struct Point *p, int dx, int dy) {
      p->x += dx;
      p->y += dy;
  }
  
  move_point(&p1, 2, 3);
  ```

---

### Q2: Padding and alignment
**Answer:**
- **Why Padding Exists**:
  ```
  Modern CPUs access memory more efficiently when data is aligned.
  Misaligned access may be slower or even crash.
  ```

- **Alignment Rules**:
  - `char`: 1-byte alignment
  - `short`: 2-byte alignment
  - `int`: 4-byte alignment (on 32-bit systems)
  - `double`: 8-byte alignment
  - **Struct**: Aligned to largest member

- **Example of Padding**:
  ```c
  struct Bad {
      char a;         // Offset 0
      // 3 bytes padding
      int b;          // Offset 4
      char c;         // Offset 8
      // 7 bytes padding
  };
  printf("%zu\n", sizeof(struct Bad));  // 16 bytes (not 6)
  
  // Better layout:
  struct Good {
      int b;          // Offset 0
      char a;         // Offset 4
      char c;         // Offset 5
      // 2 bytes padding
  };
  printf("%zu\n", sizeof(struct Good)); // 8 bytes (not 6)
  ```

- **Forcing Packing** (Embedded/Protocol):
  ```c
  #pragma pack(1)
  struct Packed {
      char a;         // 1 byte
      int b;          // 4 bytes, no padding
      char c;         // 1 byte
  };
  #pragma pack()
  printf("%zu\n", sizeof(struct Packed));  // 6 bytes
  
  // GCC alternative:
  struct Packed {
      char a;
      int b;
      char c;
  } __attribute__((packed));
  ```

---

### Q3: Unions and overlapping memory
**Answer:**
- **Union Basics**:
  ```c
  union Data {
      int i;              // 4 bytes
      float f;            // 4 bytes
      char c;             // 1 byte
  };
  
  printf("%zu\n", sizeof(union Data));  // 4 bytes (max member)
  
  // All share same memory!
  union Data d;
  d.i = 0x12345678;
  printf("%f\n", d.f);    // Bit pattern interpreted as float
  ```

- **Use Cases**:
  ```c
  // Converting between representations
  union Converter {
      uint32_t bits;
      float value;
  };
  
  union Converter c;
  c.value = 3.14;
  printf("0x%X\n", c.bits);  // See bit pattern of float
  ```

- **Hardware Registers** (Common in embedded):
  ```c
  union StatusReg {
      uint32_t raw;
      struct {
          uint8_t error : 1;
          uint8_t ready : 1;
          uint8_t busy : 1;
          uint8_t reserved : 5;
      } fields;
  };
  
  union StatusReg status;
  status.raw = read_register();
  if (status.fields.error) { /* handle error */ }
  ```

---

### Q4: Bit-fields in structs
**Answer:**
- **Declaration**:
  ```c
  struct Flags {
      unsigned int flag1 : 1;    // 1 bit
      unsigned int flag2 : 1;    // 1 bit
      unsigned int value : 30;   // 30 bits
  };
  
  printf("%zu\n", sizeof(struct Flags));  // Typically 4 bytes
  ```

- **Usage**:
  ```c
  struct Flags f = {0};
  f.flag1 = 1;                // Set flag1
  f.value = 12345;            // Set value
  
  if (f.flag1) {              // Check flag1
      printf("Flag is set\n");
  }
  ```

- **Register-Mapped Device**:
  ```c
  struct UART_Control {
      unsigned int enable : 1;
      unsigned int baud_rate : 4;   // 4 bits for baud selection
      unsigned int data_bits : 2;   // 8 or 9 bits
      unsigned int stop_bits : 1;
      unsigned int parity : 2;
      unsigned int reserved : 22;
  };
  
  volatile struct UART_Control *uart = (volatile struct UART_Control *)0x40004000;
  uart->enable = 1;
  uart->baud_rate = 8;  // 115200 baud selection
  ```

- **Caution**: Bit-field behavior can be implementation-specific (endianness, packing)

---

## Functions

### Q1: Function declaration vs. definition
**Answer:**
- **Declaration** (Also called prototype):
  ```c
  int add(int a, int b);              // Promises function exists
  void print_status(const char *msg); // Declare but not define
  ```

- **Definition** (Actual implementation):
  ```c
  int add(int a, int b) {             // Definition includes body
      return a + b;
  }
  ```

- **Forward Declarations** (Important in C):
  ```c
  // Without forward declaration, compiler doesn't know function exists yet
  f_call();  // ERROR if f() is defined later
  
  // With forward declaration:
  void f_call(void);  // Declaration
  
  int main(void) {
      f_call();       // OK - compiler knows it exists
  }
  
  void f_call(void) {  // Definition
      printf("Hello\n");
  }
  ```

- **Header Files**:
  ```c
  // math.h (declarations)
  int add(int a, int b);
  int subtract(int a, int b);
  
  // math.c (definitions)
  int add(int a, int b) { return a + b; }
  int subtract(int a, int b) { return a - b; }
  
  // main.c
  #include "math.h"
  int main(void) { add(3, 5); }
  ```

---

### Q2: Parameter passing (by value, by pointer)
**Answer:**
- **By Value** (Copy made):
  ```c
  void increment(int x) {
      x++;                 // Only local copy incremented
  }
  
  int i = 5;
  increment(i);
  printf("%d\n", i);       // Still 5
  ```

- **By Pointer** (Reference to original):
  ```c
  void increment_ptr(int *x) {
      (*x)++;              // Original modified
  }
  
  int i = 5;
  increment_ptr(&i);
  printf("%d\n", i);       // Now 6
  ```

- **Array Parameters** (Always by reference):
  ```c
  void fill_array(int arr[10]) {   // Really int *arr
      arr[0] = 100;                // Modifies original array
  }
  
  int nums[10] = {0};
  fill_array(nums);                // No need for &
  printf("%d\n", nums[0]);         // 100
  ```

- **Struct Parameters**:
  ```c
  // By value (copy made, inefficient for large structs)
  void process_struct(struct Large s) {
      // s is copy
  }
  
  // By pointer (efficient, original modified)
  void process_struct(struct Large *s) {
      s->field = 10;
  }
  ```

---

### Q3: Return values and void functions
**Answer:**
- **Returning Values**:
  ```c
  int square(int x) {
      return x * x;  // Must return int
  }
  
  int result = square(5);  // result = 25
  ```

- **Multiple Values via Pointers**:
  ```c
  void divide_with_remainder(int dividend, int divisor, 
                            int *quotient, int *remainder) {
      *quotient = dividend / divisor;
      *remainder = dividend % divisor;
  }
  
  int q, r;
  divide_with_remainder(17, 5, &q, &r);
  printf("%d remainder %d\n", q, r);  // 3 remainder 2
  ```

- **Void Functions** (No return):
  ```c
  void print_message(const char *msg) {
      printf("%s\n", msg);
      return;          // Optional, returns void
  }
  ```

- **Returning Pointers** (Be careful!):
  ```c
  // WRONG - returns pointer to local variable
  int *bad_function(void) {
      int local = 5;
      return &local;   // Undefined behavior after return
  }
  
  // CORRECT - allocate on heap
  int *good_function(void) {
      int *ptr = malloc(sizeof(int));
      *ptr = 5;
      return ptr;      // Caller must free
  }
  ```

---

### Q4: Function scope and linkage
**Answer:**
- **Internal Linkage** (file scope):
  ```c
  // In file1.c
  static int helper(int x) {   // Internal to this file only
      return x * 2;
  }
  
  // In file2.c
  helper(5);  // ERROR - not visible outside file1.c
  ```

- **External Linkage** (global scope):
  ```c
  // In file1.c
  int global_var = 10;  // External - visible to other files
  
  void global_func(void) {  // External - visible to other files
      printf("%d\n", global_var);
  }
  
  // In file2.c
  extern int global_var;  // Declaration (not definition)
  extern void global_func(void);
  
  global_func();  // OK - called from different file
  ```

- **Static vs Extern**:
  ```c
  // config.c
  static int secret = 42;         // Internal to this file
  extern int public_value = 100;  // External (extern is optional for definitions)
  
  // main.c
  extern int public_value;  // Access from other file
  int result = public_value;  // OK
  // int s = secret;  // ERROR - not visible
  ```

---

## Storage Classes

### Q1: auto, register, static, extern
**Answer:**
- **auto** (Default for local variables):
  ```c
  void func(void) {
      auto int x = 5;     // Explicit auto (rarely used, default)
      int y = 10;         // Implicitly auto
      // x and y created on stack, destroyed when function exits
  }
  ```

- **register** (Hint to use CPU register):
  ```c
  void fast_loop(void) {
      register int i;     // Compiler will try to keep in CPU register
      for (i = 0; i < 1000; i++) {
          // Loop counter in register = faster
      }
  }
  // Note: Modern compilers ignore this, do their own optimization
  ```

- **static** (Multiple uses!):
  ```c
  // 1. File scope (internal linkage)
  static int file_var = 0;  // Only visible in this file
  
  // 2. Local variable with static lifetime
  void counter(void) {
      static int count = 0;  // Initialized once, persists across calls
      count++;
      printf("%d\n", count);  // 1, 2, 3, ...
  }
  
  // 3. Static function (file scope)
  static void helper_func(void) {
      // Only visible in this file
  }
  ```

- **extern** (External linkage):
  ```c
  // In header file
  extern int global_count;   // Declaration
  extern void init_system(void);
  
  // In source file
  int global_count = 0;      // Definition
  void init_system(void) {
      global_count = 1;
  }
  
  // In another file
  #include "header.h"
  printf("%d\n", global_count);  // OK - extern symbol
  ```

---

### Q2: Global vs. local variables
**Answer:**
- **Global Variables** (Entire program scope):
  ```c
  int global_var = 100;  // Accessible from all files (if extern)
  
  void func1(void) {
      printf("%d\n", global_var);  // Can access
  }
  
  void func2(void) {
      global_var = 200;             // Can modify
  }
  ```

- **Local Variables** (Function scope):
  ```c
  void func(void) {
      int local_var = 5;  // Only visible inside func
      // goes out of scope when function ends
  }
  
  printf("%d\n", local_var);  // ERROR - undefined variable
  ```

- **Block Scope** (Since C99):
  ```c
  for (int i = 0; i < 10; i++) {  // i has block scope
      printf("%d\n", i);
  }
  printf("%d\n", i);  // ERROR - i out of scope (C99+)
  ```

- **Shadowing** (Inner scope hides outer):
  ```c
  int x = 10;  // Global x
  
  void func(void) {
      int x = 20;  // Local x shadows global
      printf("%d\n", x);      // 20
      printf("%d\n", ::x);    // C doesn't have :: scope resolution
      // To access global: use different name or extern
  }
  ```

- **Embedded Systems Consideration**:
  ```c
  // Globals slow down access in some embedded systems
  // Local variables are faster (on stack/register)
  // But globals useful for shared state across functions
  ```

---

### Q3: Static functions for file scope
**Answer:**
- **Purpose**: Create functions visible only in current file
  ```c
  // config.c
  static void read_config(void) {  // Private to config.c
      // implementation
  }
  
  void init_config(void) {         // Public (external)
      read_config();               // Can call private function
  }
  
  // main.c
  extern void init_config(void);
  init_config();                   // OK
  read_config();                   // ERROR - not visible
  ```

- **Advantages**:
  1. Name collision prevention - can use common names like `helper()` in multiple files
  2. Clear public API - only exported functions visible
  3. Encapsulation in C (C doesn't have classes)

- **Example Module Design**:
  ```c
  // led.h (public interface)
  void led_init(void);
  void led_on(void);
  void led_off(void);
  
  // led.c (implementation)
  static LED_struct leds[10];       // Private state
  
  static void configure_gpio(int pin) {  // Private helper
      // setup code
  }
  
  void led_init(void) {             // Public function
      configure_gpio(LED_PIN);      // Can use private helper
  }
  
  void led_on(void) {
      // implementation
  }
  ```

---

### Q4: Lifetime vs. visibility
**Answer:**
- **Lifetime**: How long variable exists in memory
- **Visibility**: Which parts of code can see the variable

- **Matrix of Combinations**:
  ```c
  // Long lifetime, limited visibility
  static int file_var = 0;   // Exists entire program, visible only in this file
  
  // Long lifetime, wide visibility
  int global_var = 0;        // Exists entire program, visible everywhere (extern)
  
  // Short lifetime, limited visibility
  {
      int block_var = 0;     // Exists while in block, visible only in block
  }
  
  // Short lifetime, wide visibility (rare)
  static void func(void) {
      static int counter = 0;  // Exists entire program, visible only in func
  }
  ```

- **Example: Static Local Variable**:
  ```c
  void counter(void) {
      static int count = 0;  // Lifetime: entire program
                             // Visibility: only within this function
      count++;
      printf("%d\n", count);
  }
  
  counter();  // Prints 1
  counter();  // Prints 2 (count persists!)
  counter();  // Prints 3
  ```

---

## Preprocessor

### Q1: #define and macros
**Answer:**
- **Simple Text Replacement**:
  ```c
  #define MAX_BUFFER 256
  #define PI 3.14159
  
  char buffer[MAX_BUFFER];  // Expands to char buffer[256];
  float area = r * r * PI;  // Expands to r * r * 3.14159;
  ```

- **Parameterized Macros**:
  ```c
  #define ADD(a, b) ((a) + (b))      // Note: parentheses critical
  
  int x = ADD(3, 4);                 // Expands to int x = ((3) + (4));
  int y = ADD(2 + 3, 4 * 5);         // Expands correctly
  ```

- **Dangerous Macro**:
  ```c
  #define BAD_ADD(a, b) a + b
  int x = BAD_ADD(2 + 3, 4 * 5);     
  // Expands to: int x = 2 + 3 + 4 * 5;  = 25 (not 35!)
  
  #define GOOD_ADD(a, b) ((a) + (b))
  int x = GOOD_ADD(2 + 3, 4 * 5);
  // Expands to: int x = ((2 + 3) + (4 * 5)); = 27 (correct!)
  ```

- **Common Embedded Macros**:
  ```c
  #define BIT_SET(reg, bit)    ((reg) |= (1 << (bit)))
  #define BIT_CLEAR(reg, bit)  ((reg) &= ~(1 << (bit)))
  #define BIT_TOGGLE(reg, bit) ((reg) ^= (1 << (bit)))
  #define BIT_CHECK(reg, bit)  (((reg) >> (bit)) & 1)
  
  uint32_t status = 0x0F;
  BIT_SET(status, 4);    // status = 0x1F
  BIT_CLEAR(status, 2);  // Clear bit 2
  ```

---

### Q2: #include for dependencies
**Answer:**
- **System Headers** (Angle brackets):
  ```c
  #include <stdio.h>       // Searches standard library directories
  #include <stdlib.h>
  #include <string.h>
  #include <stdint.h>      // For uint32_t etc
  ```

- **Local Headers** (Quotes):
  ```c
  #include "config.h"      // Searches current directory first
  #include "drivers/uart.h"
  ```

- **Include Guard** (Prevent multiple inclusion):
  ```c
  // config.h
  #ifndef CONFIG_H
  #define CONFIG_H
  
  #define VERSION 1
  #define MAX_CONNECTIONS 10
  
  #endif  // CONFIG_H
  ```

- **Pragma Once** (Modern alternative):
  ```c
  // config.h
  #pragma once
  
  #define VERSION 1
  ```

- **Nested Includes**:
  ```c
  // main.c
  #include "config.h"      // might include other headers
  #include "uart.h"        // might include "config.h" again
                           // Guard prevents double-inclusion
  ```

---

### Q3: Conditional compilation (#ifdef, #ifndef)
**Answer:**
- **Basic Conditionals**:
  ```c
  #ifdef DEBUG
  #define LOG(msg) printf("[DEBUG] %s\n", msg)
  #else
  #define LOG(msg)  // Empty in release
  #endif
  
  // Compile with: gcc -DDEBUG main.c (to enable DEBUG)
  ```

- **Feature Selection**:
  ```c
  #ifdef ENABLE_UART
  void uart_init(void) { /* enable UART */ }
  #else
  void uart_init(void) { }  // Stub
  #endif
  
  // Compile with different flags for different variants
  ```

- **Nested Conditions**:
  ```c
  #ifdef PLATFORM_LINUX
      #include <sys/types.h>
      typedef long time_t;
  #elif defined(PLATFORM_WINDOWS)
      #include <windows.h>
      typedef __int64 time_t;
  #else
      #error "Unknown platform"
  #endif
  ```

- **Practical Embedded Example**:
  ```c
  #ifdef BOARD_STM32F4
      #include "stm32f4xx.h"
      #define GPIO_PORT GPIOA
  #elif defined(BOARD_STM32L1)
      #include "stm32l1xx.h"
      #define GPIO_PORT GPIOB
  #else
      #error "Unsupported board"
  #endif
  ```

---

### Q4: Predefined macros (__LINE__, __FILE__)
**Answer:**
- **Common Predefined Macros**:
  ```c
  __FILE__    // String containing filename
  __LINE__    // Integer containing line number
  __DATE__    // Compilation date
  __TIME__    // Compilation time
  __STDC__    // Defined if compiler is C standard compliant
  ```

- **Debug Logging**:
  ```c
  void debug_log(const char *file, int line, const char *msg) {
      printf("[%s:%d] %s\n", file, line, msg);
  }
  
  #define DEBUG_LOG(msg) debug_log(__FILE__, __LINE__, msg)
  
  // Usage:
  DEBUG_LOG("System initializing");
  // Expands to: debug_log("main.c", 42, "System initializing")
  ```

- **Assert Macro**:
  ```c
  #define ASSERT(condition) \
      if (!(condition)) { \
          printf("ASSERTION FAILED: %s at %s:%d\n", \
                 #condition, __FILE__, __LINE__); \
          while(1);  /* Hang in debug */ \
      }
  
  ASSERT(ptr != NULL);  // Will show the exact condition that failed
  ```

- **Error Handling**:
  ```c
  #define ERROR_CHECK(condition, msg) \
      if (!(condition)) { \
          fprintf(stderr, "ERROR [%s:%d] %s\n", __FILE__, __LINE__, msg); \
          return -1; \
      }
  
  ERROR_CHECK(file != NULL, "File pointer is NULL");
  ```

---

## Memory Layout

### Q1: Stack and heap allocation
**Answer:**
- **Stack Allocation** (Automatic, fast):
  ```c
  void function(void) {
      int local_var;      // Allocated on stack
      char buffer[256];   // 256 bytes allocated on stack
      // Automatically freed when function returns
  }
  
  // Size known at compile time
  // Very fast (just move stack pointer)
  // Limited size (typically KB)
  ```

- **Heap Allocation** (Dynamic, manual):
  ```c
  int *ptr = malloc(sizeof(int) * 100);  // Allocate on heap
  if (ptr == NULL) {
      printf("Allocation failed\n");
      return;
  }
  ptr[0] = 42;
  free(ptr);  // Must free manually!
  ptr = NULL; // Good practice
  
  // Size can be determined at runtime
  // Slower than stack (fragmentation possible)
  // Limited by RAM size
  ```

- **Stack vs Heap Comparison**:
  ```
  Stack:
  - Automatic allocation/deallocation
  - Smaller size (10KB - 10MB)
  - Fast access
  - Variables go out of scope
  
  Heap:
  - Manual allocation/deallocation
  - Larger size (rest of RAM)
  - Slower access
  - Persists until explicitly freed
  ```

- **Stack Overflow Example**:
  ```c
  void recursive_disaster(int depth) {
      char large_buffer[10000];  // 10KB each call
      if (depth > 0) {
          recursive_disaster(depth - 1);  // Stack grows, grows...
      }
  }
  
  recursive_disaster(1000);  // CRASH - stack overflow!
  ```

- **Memory Leak Example**:
  ```c
  void memory_leak(void) {
      int *ptr = malloc(1024);
      printf("Processing...\n");
      return;  // LEAK! Forgot free(ptr)
      // Memory lost forever
  }
  ```

---

### Q2: Segment layout: text, rodata, data, bss
**Answer:**
- **Text Segment** (Executable code, read-only):
  ```c
  // This function's machine code goes in text segment
  int add(int a, int b) {
      return a + b;
  }
  ```

- **Read-Only Data (rodata)**:
  ```c
  const char *message = "Hello";  // Points to rodata
  const int VERSION = 100;        // In rodata
  
  // Cannot be modified (read-only memory)
  ```

- **Initialized Data (data)**:
  ```c
  int global_with_value = 42;     // In data segment (initialized)
  char buffer[256] = {0};         // In data segment (initialized)
  ```

- **Uninitialized Data (bss)**:
  ```c
  int global_uninitialized;       // In bss (zero-initialized)
  static char large_buffer[100000];  // In bss (zero-initialized)
  // Note: bss doesn't increase executable size
  ```

- **Memory Layout Diagram**:
  ```
  Address
  High        ┌─────────────────────┐
              │ Stack (grows down)  │  Local variables
              │                     │
              ├─────────────────────┤
              │ Heap (grows up)     │  malloc, dynamic
              │                     │
              ├─────────────────────┤
              │ BSS Segment         │  uninitialized globals
              ├─────────────────────┤
              │ Data Segment        │  initialized globals
              ├─────────────────────┤
              │ Text (Code)         │  Program code
  Low         └─────────────────────┘
  ```

- **Practical Embedded**:
  ```c
  // In embedded, knowing segment layout is crucial for:
  // 1. Placing bootloader in flash memory start
  // 2. Putting stack in RAM end
  // 3. Putting heap before stack
  // 4. Ensuring BSS is initialized at startup
  ```

---

### Q3: Stack frame and function calls
**Answer:**
- **Call Stack Setup**:
  ```c
  int multiply(int a, int b) {    // Stack frame for multiply
      int result = a * b;         // Local variable
      return result;
  }
  
  int main(void) {                // Stack frame for main
      int x = multiply(3, 4);     // Call multiply
      return x;                   // multiply frame destroyed
  }
  ```

- **Stack Frame Structure**:
  ```
  multiply frame:
  ├─ Return address (where to continue in main)
  ├─ Old base pointer (previous frame)
  ├─ Parameter: b = 4
  ├─ Parameter: a = 3
  ├─ Local variable: result
  
  Space for "result" freed when function returns
  ```

- **Stack Overflow Protection**:
  ```c
  // Limited stack - don't declare huge arrays on stack
  
  // BAD:
  void process(void) {
      char buffer[1000000];  // 1MB on stack - OVERFLOW!
  }
  
  // GOOD:
  uintptr_t heap_buffer = malloc(1000000);  // Use heap
  ```

- **Return Value Mechanism**:
  ```c
  int get_status(void) {
      return 42;  // Often returned in CPU register (x86: EAX)
  }
  
  int status = get_status();
  // Value handed off via register or stack
  ```

---

### Q4: Heap fragmentation
**Answer:**
- **Fragmentation Issue**:
  ```c
  // Initially: 1000 bytes free
  int *p1 = malloc(100);  // 900 free
  int *p2 = malloc(100);  // 800 free
  int *p3 = malloc(100);  // 700 free
  
  free(p2);  // Hole in middle, 800 free
  
  // Now trying to allocate 500 bytes
  int *p4 = malloc(500);  // Works in p2's spot
  
  // But if we need 900 bytes contiguous:
  int *p5 = malloc(900);  // FAILS even though 900 bytes free!
  ```

- **Memory Layout After Free**:
  ```
  Allocated:  [p1][used][p3][free p2 spot][p4]
  
  Fragmentation = many small free blocks that can't satisfy large requests
  ```

- **Avoiding Fragmentation**:
  ```c
  // Strategy 1: Allocate early, free late
  void *buffers[10];
  for (int i = 0; i < 10; i++) {
      buffers[i] = malloc(1024);
  }
  // ... use buffers ...
  for (int i = 0; i < 10; i++) {
      free(buffers[i]);  // Free in order
  }
  
  // Strategy 2: Use memory pools (embedded)
  uint8_t pool[10 * 1024];  // Pre-allocated
  // Allocate from pool instead of malloc
  
  // Strategy 3: LIFO (Last-In-First-Out) allocation
  // Allocate in order, free in reverse
  ```

- **Embedded Solution - Memory Pools**:
  ```c
  struct MemoryPool {
      uint8_t buffer[KB(10)];
      size_t used;
  };
  
  struct MemoryPool pool = {0, 0};
  
  void *pool_alloc(size_t size) {
      if (pool.used + size > sizeof(pool.buffer)) {
          return NULL;  // Out of memory
      }
      void *ptr = &pool.buffer[pool.used];
      pool.used += size;
      return ptr;
  }
  
  void pool_reset(void) {
      pool.used = 0;  // Deallocate all at once
  }
  ```

---

## Real-world Scenarios with Detailed Answers

### Scenario 1: Memory Corruption

**Context**: Embedded system exhibits random crashes due to memory corruption.

**Q1: How would you identify the culprit pointer?**

**Answer**:
1. **Enable Compiler Warnings**:
   ```c
   gcc -Wall -Wextra -Wpedantic code.c
   // Catches many issues: uninitialized variables, type mismatches
   ```

2. **Add Debug Assertions**:
   ```c
   #define ASSERT(cond) \
       if (!(cond)) { \
           printf("ASSERT: %s:%d\n", __FILE__, __LINE__); \
           abort(); \
       }
   
   ASSERT(ptr != NULL);
   ASSERT((uintptr_t)ptr % sizeof(int) == 0);  // Alignment check
   ```

3. **Use Valgrind (on development system)**:
   ```bash
   valgrind --leak-check=full ./myprogram
   // Shows memory errors, leaks, invalid accesses
   ```

4. **Add Memory Guards**:
   ```c
   typedef struct {
       uint32_t guard1;
       uint8_t data[256];
       uint32_t guard2;
   } GuardedMemory;
   
   GuardedMemory mem;
   
   void check_guards(void) {
       ASSERT(mem.guard1 == 0xDEADBEEF);
       ASSERT(mem.guard2 == 0xCAFEBABE);
   }
   ```

---

**Q2: What debugging techniques would you use?**

**Answer**:
1. **Serial Port Logging**:
   ```c
   void debug_print(const char *fmt, ...) {
       #ifdef DEBUG
       char buffer[256];
       va_list args;
       va_start(args, fmt);
       vsnprintf(buffer, sizeof(buffer), fmt, args);
       va_end(args);
       uart_send_string(buffer);
       #endif
   }
   
   debug_print("Value at addr 0x%p = 0x%x\n", ptr, *ptr);
   ```

2. **JTAG Debugging**:
   ```bash
   # Connect debugger (GDB)
   arm-none-eabi-gdb firmware.elf
   (gdb) target remote localhost:3333
   (gdb) load
   (gdb) break main
   (gdb) continue
   (gdb) print ptr
   (gdb) print *ptr
   (gdb) watch ptr     # Break when ptr changes
   ```

3. **Heap Checker**:
   ```c
   typedef struct HeapHeader {
       size_t size;
       uint32_t magic;
   } HeapHeader;
   
   #define HEAP_MAGIC 0xDEADBEEF
   
   void *safe_malloc(size_t size) {
       HeapHeader *h = (HeapHeader *)malloc(sizeof(HeapHeader) + size);
       h->size = size;
       h->magic = HEAP_MAGIC;
       return (void *)( h + 1);
   }
   ```

---

**Q3: How would you implement guards?**

**Answer**:
```c
// Comprehensive guard system
typedef struct {
    uint32_t before_magic;
    uint8_t *data;
    size_t size;
    uint32_t after_magic;
} ProtectedBuffer;

#define GUARD_MAGIC_BEFORE 0xABCDABCD
#define GUARD_MAGIC_AFTER  0xDCDCDCDC

ProtectedBuffer *create_protected(size_t size) {
    ProtectedBuffer *pb = malloc(sizeof(ProtectedBuffer));
    pb->before_magic = GUARD_MAGIC_BEFORE;
    pb->data = malloc(size + 2 * sizeof(uint32_t));
    
    // Place guards around data
    uint32_t *guard_before = (uint32_t *)pb->data;
    *guard_before = GUARD_MAGIC_BEFORE;
    
    uint32_t *guard_after = (uint32_t *)(pb->data + size + sizeof(uint32_t));
    *guard_after = GUARD_MAGIC_AFTER;
    
    pb->size = size;
    pb->after_magic = GUARD_MAGIC_AFTER;
    
    return pb;
}

void check_protected(ProtectedBuffer *pb) {
    ASSERT(pb->before_magic == GUARD_MAGIC_BEFORE);
    ASSERT(pb->after_magic == GUARD_MAGIC_AFTER);
    
    uint32_t *g_before = (uint32_t *)pb->data;
    ASSERT(*g_before == GUARD_MAGIC_BEFORE);
    
    uint32_t *g_after = (uint32_t *)(pb->data + pb->size + sizeof(uint32_t));
    ASSERT(*g_after == GUARD_MAGIC_AFTER);
}
```

---

### Scenario 2: Performance-critical C Code

**Q1: What optimizations would you consider?**

**Answer**:
1. **Use Faster Data Types**:
   ```c
   // Avoid floating point in tight loops
   // BAD: Uses FPU
   for (int i = 0; i < 1000; i++) {
       float result = sin(i * 3.14159 / 1000.0);  // Slow!
   }
   
   // GOOD: Integer math or lookup table
   const uint16_t sin_table[360] = { /* precomputed */ };
   for (int i = 0; i < 360; i++) {
       uint16_t result = sin_table[i];  // Fast!
   }
   ```

2. **Reduce Function Calls**:
   ```c
   // BAD: Function call overhead
   int get_value(void) { return global_var; }
   for (int i = 0; i < 1000000; i++) {
       sum += get_value();
   }
   
   // GOOD: Inline or direct access
   int val = global_var;
   for (int i = 0; i < 1000000; i++) {
       sum += val;
   }
   ```

3. **Cache-Friendly Code**:
   ```c
   // BAD: Cache misses
   int matrix[1000][1000];
   for (int col = 0; col < 1000; col++) {
       for (int row = 0; row < 1000; row++) {
           sum += matrix[row][col];  // Column-major = cache misses
       }
   }
   
   // GOOD: Row-major access pattern
   for (int row = 0; row < 1000; row++) {
       for (int col = 0; col < 1000; col++) {
           sum += matrix[row][col];  // Cache-friendly
       }
   }
   ```

---

**Q2: How do inline functions help?**

**Answer**:
```c
// Instead of function call overhead
static inline int multiply(int a, int b) {
    return a * b;  // Compiler inserts code directly
}

int result = multiply(3, 4);
// Becomes: int result = 3 * 4; (no function call)
// Faster, but code size increases
```

---

**Q3: What compiler flags would you use?**

**Answer**:
```bash
# Optimization levels
gcc -O0 -g program.c        # No optimization, debug info
gcc -O1 program.c           # Basic optimizations
gcc -O2 program.c           # More aggressive
gcc -O3 program.c           # Maximum optimization
gcc -Os program.c           # Optimize for size

# Specific flags
gcc -march=armv7-a program.c  # Target architecture
gcc -mcpu=cortex-a8 program.c # Specific CPU
gcc -mfpu=neon program.c      # Enable NEON

# Profile-guided optimization
gcc -fprofile-generate -O2 program.c -o program
./program < typical_input     # Run with typical data
gcc -fprofile-use -O2 program.c -o program_optimized
```

---

This comprehensive Q&A covers all Clanguage fundamentals. Would you like me to continue with the other roles' topics (RTOS, Hardware, Kernel, etc.)?