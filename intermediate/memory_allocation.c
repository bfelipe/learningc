#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  We discussed briefely about how OS provide a virtual
  memory for the usage of our process.
  We also briefely discussed how memory is layed out
  for every variable we declare in our program.
  Now let's discuss two regions of memory that is very important
  for the execution of our program.

  The stack is our first memory region to discuss.

  Every time our program runs when a fuction is
  called we pille a memory frame into a stack structure.
  Which is basically a form of arranging memory for the variables
  instantiated by our function.
  If new functions are called a new memory frame is pilled on top
  of the stack. Once the function returns that frame is dealocated 
  and that memory frame is free to use again.

  Example:
  
  Memory array
  lower addresses ... | null | null | null| null | ... high addresses

  int foo(int a) {
    int b;
    char c;
    return a;
  }

  lower addresses... | return address | int a | int b | char c | stack pointer | ... high addresses ()
  #################################### |------------memory frame---------------|######################

  Let's see a short example.
*/

void printStackPointerDiff(void) {
  static void *last_sp = NULL;
  void *current_sp;
  current_sp = __builtin_frame_address(0);
  long diff = (char*)last_sp - (char*)current_sp;
  if (last_sp == NULL){
    last_sp = current_sp;
    diff = 0;
  }
  printf("---------------------------------\n");
  printf("Stack pointer offset: %ld bytes\n", diff);
  printf("---------------------------------\n");
}

int pile_3(int x) {
  int a;
  char *b;
  printStackPointerDiff();
  return 0;
}

int pile_2(float x) {
  int a;
  double b;
  printStackPointerDiff();
  pile_3(a);
  return 0;
}

int pile_1(int x) {
  int a;
  float b;
  printStackPointerDiff();
  pile_2(b);
  return 0;
}
/*
  The printStackPointerDiff will show the amount of memory allocated for the current frame 
  (i.e., the memory used by the local variables, arguments, and function overhead of the
  function that just called printStackPointerDiff).
  As new functions start to stack over each other, you will see a increase in bytes
  for each allocated memory for variables, return pointer, and so on.
*/

int non_pile_1(int x) {
  int a;
  float b;
  printStackPointerDiff();
  return 0;
}

int non_pile_2(float x) {
  int a;
  double b;
  printStackPointerDiff();
  return 0;
}

/*
  The above example doesn't pile stack frames, you can notice very different results,
  like negative numbers. This is because the last_stack_pointer stored in our printStackPointerDiff
  were too high, or too far in the memory array, compared to the new stack pointer.

  In general, stack allocation is much faster compared to allocate on the heap.
  Variabled are allocated and dealocated as needed and functions are sequentially living next to each other
  allowing to fast access over data by the cpu. Also stack allocation is thread safe. Each thread has its own
  stack.

  However there is a downside by using stack allocation. Stacks are fast, but they have limited size.
  Which means, as long as you keep pushing frames over the stack (for example a very long recursion) and not
  deallocating them, or allocating large variables, you can easily end up with a stack-overflow.

  Stack size is limited by the OS, you can check the size it sets by using 

  ulimit -s

  It often set to 8mb long.
  You could also increase the size by using the same command, followed by a number in kilobytes.
  Could be useful if you can't use malloc to allocate memory on the heap.
*/

void stack_overflow(void) {
    int pool_size = 1024 * 1024 * 10;
    char text[pool_size]; // too large to allocate on the stack.
}

/*
  Now we know memory is allocated and deallocated in the stack by the life time of the function.
  Let's see an example of what happen with we return a pointer from a function
*/

typedef struct {
  int x, y;
} coordenates_t;

coordenates_t *unefined_behavior(int x, int y) {
  /*
    function will lead to an undefined behavior
    once the function returns c address no longer exists.
    Stacks are safe to use only within the memory frame (context)of the current function
  */ 
  // coordenates_t c = {x, y};
  // return &c;
}

// correct implementation
coordenates_t correct_implementation(int x, int y) {
  coordenates_t c = {x, y};
  return c;
}

/*
  Another region we can allocate memory is on the heap.

  Heap is a dynamic memory pool, which we can allocate and
  deallocate as needed.
  Often languages which uses garbage collection software
  manage the heap by counting the references to an object.
  And when that reference is 0 the space if deallocated and free
  to use.

  In C we allocate space in the heap with the help of malloc().
  It receive the number in bytes to allocate (int size * sizeof(type))
  and returns a generic pointer to the address on the heap as void * pointer, which
  we can cast to our desired type later.

  We store data that out live the function context, or data that is too big to store
  in the stack frame on the heap.
  Later when we don't need the data we should use free(pointer) function to release
  the memory.
*/

// previous stack overflow example, now with heap allocation
int * heap_allocation(int size) {
  /*
    malloc stands for memory allocation
    the function returns a pointer on the heap
    
    keep in mind this address may be uninitialized,
    which means that pointer contains whatever data
    which was previously stored at that location

    after used the pointer should be free, to avoid memory leak.

    an alternative function is called calloc, which behaves the same
    but it initializes the allocated memory with zeroes for future usage
  */ 
  int *int_array = (int *)malloc(size * sizeof(int)); // cast pointer to a int pointer
  if (int_array == NULL) {
    printf("fail to allocate memory\n");
    exit(1);
  }
  return int_array;
}

/*
  The example above allocate on the heap 10485760 bytes = around 10mb
  After calling the function we free the resource by giving the pointer as parameter
  and that memory space is again free to use.

  We can persive the function like this follow example:
  
  Heap Memory:

  ... | pointer address returned by malloc | 0 | 1 |2 | 3 | ... high addresses
  #######################|---------------allocated range -------------|#######

  Stack Memory:
  ...| return address | int size | int_arr * from heap | stack pointer | ... high addresses
  #######################|---------------stack frame -------------|########################

*/

// calloc example
int *scalar_array(int size, int multiplyer) {
  int idx = 0;
  int max = size * multiplyer;
  int *result = calloc(max, sizeof(int));
  if (result == NULL) {
    printf("fail to allocate memory");
    return NULL;
  }
  while (idx <= max) {
    result[idx] = idx * 2;
    idx += 1;
  };

  return result;
};

/*
  Let's say you want to change the amount of allocated memory
  on the heap. You can use realloc(), by giving the pointer, and a new
  amount of bytes to reallocate.
  It may increase of shrink in size.
  The function only works for resources that wasn't free yet.
  If increased, the new memory slots are undefined, as if it was created
  my malloc.
  The expansion process copy the allocated data from the given pointer to a new
  memory address, and free the old allocated memory.
  If there isn't enough memory to use, it returns null.
*/

int main() {
    pile_1(0);
    non_pile_1(0);
    non_pile_2(0);
    // stack_overflow();
    int *arr = heap_allocation(10485760);
    arr[0] = 1;
    printf("heap array: %zu bytes\n", sizeof(arr));
    /*
      once we free the resource our pointer becomes a dangling pointer
      it still exists, but points to a dealocated memory.
      it can become very dangerous, so once deallocated it should not
      be used.*/
    free(arr); // arr should not be used
    // printf("%d\n", arr[0]); segmentation fault
    
    /*
      Sometimes we may want to store data from the stdout into local buffer
      we can achieve this using snprintf().
      It may come handy using it for debbug.
      But it is also a good example of using such feature when dealing with
      stack allocation or heap allocation, as we have to give to the function
      the ammount of bytes we want to write, as well as destination pointer to
      be written.
    */
    const int BUFFER_SIZE = 12;
    char log_buffer[BUFFER_SIZE];
    const char *prefix = "hello";
    const char *suffix = "world";

  
    int bytes_written = snprintf(
        log_buffer, 
        sizeof(log_buffer), 
        "%s %s", 
        prefix,
        suffix
    );

    printf("--- snprintf Results ---\n");
    printf("Buffer Capacity: %d bytes\n", BUFFER_SIZE);
    printf("Bytes Attempted: %d bytes\n", bytes_written);
    printf("Resulting String: '%s'\n", log_buffer);

    int size = 10, multiplyer = 2;
    int max = size * multiplyer;
    int *scalar_arr = scalar_array(size, multiplyer);
    if (!scalar_arr) {
      exit(1);
    }
    for (int i = 0; i <= max; i++) {
      printf("%d - %d\n", i, scalar_arr[i]);
    }
    scalar_arr = realloc(scalar_arr, 10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
      printf("%d - %d\n", i, scalar_arr[i]);
    }
    /*
      Allocating and dealocating memory from the heap
      if a manual habit which should be developed by the programmer.
      If not freeing the resources, it can lead to memory leak, and other
      programs may not have that unused resources to use.
      The OS will free that memory once the process exits, but for 
      long running software it is not the desired expectation.
      Keep in mind, free() doesn't change the content of the allocated
      memory. It simply signal to the OS that that memory is free to use again.
    */
    free(scalar_arr);

    /*
      Understanding Endianness (Byte Ordering)
      Endianness describes the order in which the bytes
      of a multi-byte data type (like an int or float)
      are stored and retrieved from memory.

      👑 Big-Endian (Most Significant Byte First)
      Big-endian architecture stores the Most Significant
      Byte (MSB)—the byte that holds the highest value—at
      the lowest memory address.

      This ordering is intuitive because it matches how we
      read numbers and write in scientific notation
      (highest value digit first).

      Context: It is historically used in older mainframes
      and is the required Network Byte Order for
      communication protocols (like TCP/IP).

      Memory
                        least significat
      | 0 | 1 | 2 | 3 | 4 |
        most significant

      👶 Little-Endian (Least Significant Byte First)
      Little-endian architecture stores the Least
      Significant Byte (LSB)—the byte that holds the
      lowest value—at the lowest memory address.

      This is the dominant architecture today, used
      by most Intel x86/x64 processors
      (PCs and modern servers).
      
       Memory
                        most significat
      | 0 | 1 | 2 | 3 | 4 |
        least significant

      🌐 Importance
      Endianness is crucial for data portability and
      interoperability, especially in two domains:

      Network Applications: When systems with different
      endianness communicate, data must be converted to
      the common Big-Endian network byte order before transmission.

      Binary Files: Standards for binary file formats
      (e.g., image files) must specify their byte order
      to ensure files created on one machine are correctly read on another.

      In modern systems, the compiler and operating
      system manage the complexity of reading and writing
      data correctly according to the native endianness,
      shielding the programmer from the byte-swapping details
      unless they are dealing with networking or low-level
      file manipulation.
    */
    return 0;
}