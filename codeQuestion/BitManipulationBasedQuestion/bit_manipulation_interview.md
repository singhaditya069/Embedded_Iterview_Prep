# Bit Manipulation Interview Questions

This guide covers bit manipulation problems commonly asked in coding interviews, organized from easy to advanced.

## Easy Level

### 1. Check if a number is even or odd
Determine if a number is even or odd without using modulo operator.

### 2. Check if a number is power of 2
Check if a given number is a power of 2.

### 3. Count set bits (Hamming weight)
Count the number of 1s in the binary representation of a number.

### 4. Swap two numbers without temporary variable
Swap two integers without using a third variable.

## Medium Level

### 5. Find the only non-repeating element
In an array where every element appears twice except one, find the single element.

### 6. Find two non-repeating elements
In an array where every element appears twice except two elements, find those two.

### 7. Reverse bits of a number
Reverse the bits of a 32-bit unsigned integer.

### 8. Find missing number
Find the missing number in an array of 1 to n.

## Advanced Level

### 9. Bit manipulation for subset generation
Generate all subsets of a set using bit manipulation.

### 10. Count number of 1s in binary representation (efficient)
Count set bits efficiently.

### 11. Find position of rightmost set bit
Find the position of the rightmost set bit.

### 12. Check if two numbers have opposite signs
Check if two integers have opposite signs.

### 13. Add two numbers without using + operator
Add two numbers using bit manipulation.

### 14. Multiply by 7 using bit manipulation
Multiply a number by 7 using only bit operations.

## Key Concepts to Remember

1. **Bitwise Operators:**
   - `&` (AND): Sets bit if both are 1
   - `|` (OR): Sets bit if either is 1
   - `^` (XOR): Sets bit if bits are different
   - `~` (NOT): Flips all bits
   - `<<` (Left shift): Multiplies by 2
   - `>>` (Right shift): Divides by 2

2. **Common Tricks:**
   - `n & (n-1)`: Clears rightmost set bit
   - `n & -n`: Isolates rightmost set bit
   - `n ^ (n >> 1)`: Gray code conversion
   - `(n + (n >> 1))`: Count set bits in O(log n) time

3. **Time Complexities:**
   - Counting bits: O(log n) for efficient methods
   - Most operations: O(1) or O(log n)

Practice these problems and understand the underlying bit operations to excel in interviews!