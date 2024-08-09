#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/mem.h>
#include <kernel/atag.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <stdarg.h>

#define ALPHABET_SIZE 26

// Simplified memory allocation functions
void* simple_heap = (void*)0x10000; // Example start of heap
size_t heap_size = 0x10000; // Example heap size

void* simplified_kmalloc(uint32_t size) {
    if (size > heap_size) {
        puts("Out of memory\n");
        return NULL;
    }
    void* allocated_memory = simple_heap;
    simple_heap += size;
    heap_size -= size;
    return allocated_memory;
}

void simplified_kfree(void* ptr) {
    (void)ptr;
}

// LinkedList Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Head of the LinkedList
Node* head = NULL;

// Function to create a new node
Node* createnode(int data) {
    Node* new_node = (Node*)simplified_kmalloc(sizeof(Node));
    if (new_node == NULL) {
        puts("Failed to allocate memory for new node\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Function to add a node to the LinkedList
void addnode(int data) {
    Node* new_node = createnode(data);
    if (new_node == NULL) {
        return;
    }
    if (head == NULL) {
        head = new_node;
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Function to display the LinkedList
void displaynode() {
    Node* temp = head;
    printf("Linked list contents: ");
    if(temp == NULL) {
        printf("List is empty.\n");
    } else{
        while (temp->next != NULL) {
            printf("%d -> ", temp->data);
            temp = temp->next;
        }
        printf("%d\n", temp->data);
    }
}

// Function to clear the LinkedList
void clearlist() {
    Node* temp = head;
    while (temp != NULL) {
        Node* next = temp->next;
        simplified_kfree(temp);
        temp = next;
    }
    head = NULL;
}

// Function to compare two strings
int custom_strcmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Function to convert a string to an integer
int custom_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    
    // Skip leading whitespaces if any
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
        str++;
    }

    // Check if the first character is a negative sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Iterate through each character of the string
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }

    // Return the result multiplied by the sign
    return result * sign;
}

// Renamed function to avoid conflict with existing declaration
void custom_itoa(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Process each digit
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    // If number is negative, append '-'
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Function to clear a buffer
void clear_buffer(char *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = '\0';
    }
}

// Caesar Cipher Encryption Function
void encrypt(char *input, char *output, int shift) {
    int i = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            output[i] = (c - 'a' + shift) % ALPHABET_SIZE + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            output[i] = (c - 'A' + shift) % ALPHABET_SIZE + 'A';
        } else {
            output[i] = c; // Non-alphabet characters are unchanged
        }
    }
    output[i] = '\0'; // Null-terminate the output string
}

// Caesar Cipher Decryption Function
void decrypt(char *input, char *output, int shift) {
    int i = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            output[i] = (c - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            output[i] = (c - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'A';
        } else {
            output[i] = c; // Non-alphabet characters are unchanged
        }
    }
    output[i] = '\0'; // Null-terminate the output string
}

// Simplified printf function
void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *str_arg;
    int int_arg;
    char buffer[256];

    while (*format) {
        if (*format == '%' && *(format + 1) != '\0') {
            format++;
            if (*format == 's') {
                str_arg = va_arg(args, char *);
                puts(str_arg);
            } else if (*format == 'd') {
                int_arg = va_arg(args, int);
                custom_itoa(int_arg, buffer);
                puts(buffer);
            }
        } else {
            putc(*format);
        }
        format++;
    }

    va_end(args);
}

// Function to display the help message
void display_help() {
    puts("Available commands:\n");
    puts("help - Display this help message\n");
    puts("sum - Sum two integers\n");
    puts("encrypt - Encrypt a sentence using Caesar cipher\n");
    puts("decrypt - Decrypt a sentence using Caesar cipher\n");
    puts("addnode - Add a node to the linked list\n");
    puts("displaylist - Display the linked list\n");
    puts("clearlist - Clear the linked list\n");
    puts("exit - Exit the kernel\n");
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    char buf[256];    // Buffer for user input
    char encrypted[256]; // Buffer for encrypted text
    char decrypted[256]; // Buffer for decrypted text
    int running = 1;   // Condition variable for the loop

    uart_init();
    puts("Initializing Memory Module\n");
    mem_init((atag_t *)atags);

    puts("Hello, Kernel World!\n");

    // Command processing loop
    puts("Message: CMPS240 Project Summer 2024!\n");
    while (running) {
        printf(">");
        clear_buffer(buf, sizeof(buf));
        gets(buf, sizeof(buf));

        if (!custom_strcmp(buf, "help")) {
            display_help();
        } else if (!custom_strcmp(buf, "sum")) {
            int a, b;
            printf("Enter first integer: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            a = custom_atoi(buf);

            printf("Enter second integer: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            b = custom_atoi(buf);

            printf("Sum: %d\n", a + b);
        } else if (!custom_strcmp(buf, "encrypt")) {
            int shift;
            printf("Enter Caesar cipher shift value: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            shift = custom_atoi(buf);

            printf("Enter sentence to encrypt: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            encrypt(buf, encrypted, shift);

            printf("Encrypted sentence: %s\n", encrypted);
        } else if (!custom_strcmp(buf, "decrypt")) {
            int shift;
            printf("Enter Caesar cipher shift value: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            shift = custom_atoi(buf);

            printf("Enter sentence to decrypt: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            decrypt(buf, decrypted, shift);

            printf("Decrypted sentence: %s\n", decrypted);
        } else if (!custom_strcmp(buf, "addnode")) {
            int data;
            printf("Enter node data: ");
            clear_buffer(buf, sizeof(buf));
            gets(buf, sizeof(buf));
            data = custom_atoi(buf);

            addnode(data);
            puts("Node added successfully.\n");
        } else if (!custom_strcmp(buf, "displaylist")) {
            displaynode();
        } else if (!custom_strcmp(buf, "clearlist")) {
            clearlist();
            puts("Linked list cleared.\n");
        } else if (!custom_strcmp(buf, "exit")) {
            puts("Exiting kernel...\n");
            running = 0;
        } else {
            puts("Unknown command. Type 'help' for a list of available commands.\n");
        }
    }
}