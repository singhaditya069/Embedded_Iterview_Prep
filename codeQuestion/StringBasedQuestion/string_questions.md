# String-Based Coding Questions: Easy to Advanced

## Easy Level

### 1. **Reverse a String**
- Given a string, reverse it and return the reversed string.
- **Example:** `"hello"` → `"olleh"`
- **Concepts:** String manipulation, loops

### 2. **Check Palindrome**
- Determine if a string is a palindrome (reads same forwards and backwards).
- **Example:** `"racecar"` → `true`, `"hello"` → `false`
- **Concepts:** Two pointers, string comparison

### 3. **Check Anagram**
- Two strings are anagrams if one can be rearranged to form the other.
- **Example:** `"listen"` and `"silent"` → `true`
- **Concepts:** Sorting, hashing, character count

### 4. **Count Character Occurrences**
- Count the frequency of each character in a string.
- **Example:** `"hello"` → `{h:1, e:1, l:2, o:1}`
- **Concepts:** Hash map, loops

### 5. **Find First Non-Repeating Character**
- Return the first character that appears only once in the string.
- **Example:** `"leetcode"` → `"l"`
- **Concepts:** Hash map, string traversal

### 6. **Remove Duplicates**
- Remove all duplicate characters from a string.
- **Example:** `"hello"` → `"helo"`
- **Concepts:** Hash set, string building

### 7. **Check Valid Parentheses**
- Verify if parentheses are balanced in a string.
- **Example:** `"(())"` → `true`, `"([)]"` → `false`
- **Concepts:** Stack, string validation

### 8. **Simple String Compression**
- Compress strings by replacing consecutive characters with count.
- **Example:** `"aabbbcccc"` → `"a2b3c4"`
- **Concepts:** String building, loops, character grouping

### 9. **Reverse Words in String**
- Reverse the order of words in a string.
- **Example:** `"Hello World"` → `"World Hello"`
- **Concepts:** String splitting, array reversal

### 10. **Find Longest Common Prefix**
- Find the longest common prefix among array of strings.
- **Example:** `["flower","flow","flight"]` → `"fl"`
- **Concepts:** String comparison, loops

---

## Intermediate Level

### 11. **Longest Palindromic Substring**
- Find the longest substring that is a palindrome.
- **Example:** `"babad"` → `"bab"` or `"aba"`
- **Concepts:** Expand around center, dynamic programming

### 12. **Longest Substring Without Repeating Characters**
- Find the length of the longest substring without repeating characters.
- **Example:** `"abcabcbb"` → `3` (substring "abc")
- **Concepts:** Sliding window, hash map

### 13. **Group Anagrams**
- Group strings that are anagrams together.
- **Example:** `["eat","tea","ate","bat"]` → `[["eat","tea","ate"],["bat"]]`
- **Concepts:** Hash map, sorting

### 14. **Valid Anagram**
- Check if two strings are anagrams of each other.
- **Example:** `"anagram"` and `"nagaram"` → `true`
- **Concepts:** Sorting, character frequency

### 15. **Generate Permutations of String**
- Generate all permutations of a given string.
- **Example:** `"ABC"` → `["ABC","ACB","BAC","BCA","CAB","CBA"]`
- **Concepts:** Backtracking, recursion

### 16. **String Rotation Check**
- Check if one string is a rotation of another.
- **Example:** `"waterbottle"` and `"erbottlewat"` → `true`
- **Concepts:** String matching, pattern matching

### 17. **Word Break**
- Determine if a string can be segmented into dictionary words.
- **Example:** `"leetcode"` with dict `["leet","code"]` → `true`
- **Concepts:** Dynamic programming, hash set

### 18. **Implement strStr()**
- Find the index of the first occurrence of a substring.
- **Example:** `"hello"` and `"ll"` → `2`
- **Concepts:** String matching, KMP algorithm

### 19. **Valid Palindrome II (Allow Remove One Char)**
- Check if string is palindrome after removing at most one character.
- **Example:** `"abca"` → `true` (remove 'b' or 'c')
- **Concepts:** Two pointers, string comparison

### 20. **Regularization/Format Strings**
- Format strings according to specific rules (remove spaces, convert case, etc.).
- **Example:** `"hello world"` → `"helloworld"` (lowercase, no spaces)
- **Concepts:** String manipulation, loops

---

## Advanced Level

### 21. **Longest Palindromic Substring (Optimized)**
- Find longest palindromic substring using Manacher's algorithm.
- **Time Complexity:** O(n)
- **Concepts:** Manacher's algorithm, dynamic programming

### 22. **Wildcard Matching**
- Match a string against a pattern with `?` and `*` wildcards.
- **Example:** `"aa"` matches `"*"` → `true`, `"cb"` matches `"?a"` → `false`
- **Concepts:** Dynamic programming, greedy approach

### 23. **Regular Expression Matching**
- Implement regex matching with `.` and `*` support.
- **Example:** `"ab"` matches `".*"` → `true`
- **Concepts:** Dynamic programming, state machine

### 24. **Minimum Window Substring**
- Find the minimum window in a string containing all characters of another string.
- **Example:** `"ADOBECODEBANC"` and `"ABC"` → `"BANC"`
- **Concepts:** Sliding window, hash map

### 25. **Word Ladder**
- Find the shortest path from start word to end word using word transformations.
- **Example:** `"hit"` → `"cog"` with dict → `["hit","hot","dot","dog","log","cog"]` (length 5)
- **Concepts:** BFS, graph traversal, hashing

### 26. **Scramble String**
- Check if one string is a scrambled version of another.
- **Example:** Two strings interleaved recursively
- **Concepts:** Recursion, memoization, dynamic programming

### 27. **Edit Distance (Levenshtein Distance)**
- Find minimum edits (insert, delete, replace) to transform one string to another.
- **Example:** `"horse"` to `"ros"` → `3`
- **Concepts:** Dynamic programming, 2D DP

### 28. **Distinct Subsequences**
- Count the number of distinct subsequences of a string that match a substring.
- **Example:** `"rabbbit"` contains `"rab"` → `3`
- **Concepts:** Dynamic programming, string analysis

### 29. **Valid Number**
- Validate if a string represents a valid number (integer, decimal, scientific notation).
- **Example:** `"2"`, `"-0.1"`, `"2e10"`, `"1e9"` are valid, `"e"` is invalid
- **Concepts:** State machine, string parsing

### 30. **Multiply Strings**
- Multiply two string numbers without converting to integers.
- **Example:** `"123"` × `"456"` → `"56088"`
- **Concepts:** String manipulation, arithmetic operations

### 31. **Zigzag Conversion**
- Convert string to zigzag pattern and read line by line.
- **Example:** `"PAYPALISHIRING"` with numRows=3 → `"PAHNAPLSIIGYIR"`
- **Concepts:** String indexing, pattern recognition

### 32. **Basic Calculator (String Expression)**
- Evaluate a mathematical expression as a string.
- **Example:** `"2+3*4"` → `14`, `"(1+2)*3"` → `9`
- **Concepts:** Stack, parsing, operator precedence

### 33. **Serialize and Deserialize String**
- Serialize a complex data structure to string and back.
- **Concepts:** Encoding, decoding, design patterns

### 34. **Text Justification**
- Format text to a given width with full justification.
- **Example:** Words justified to width 16
- **Concepts:** String manipulation, greedy algorithms

### 35. **Rabin-Karp String Search**
- Implement string matching using rolling hash.
- **Time Complexity:** O(n + m) average case
- **Concepts:** Hashing, pattern matching algorithms

### 36. **KMP Pattern Matching**
- Find all occurrences of pattern in text using KMP algorithm.
- **Time Complexity:** O(n + m)
- **Concepts:** KMP algorithm, failure function

### 37. **Suffix Array and LCP Array**
- Build suffix arrays for efficient string searching.
- **Concepts:** Advanced string data structures

### 38. **Palindrome Pairs**
- Find all pairs of indices where concatenated strings form palindromes.
- **Example:** `["abcd","dcba","lls","s","sssll"]` → `[[0,1],[1,0],[3,2],[2,4]]`
- **Concepts:** Hash map, trie, string matching

### 39. **Substring with Concatenation of All Words**
- Find all substrings that concatenate all words from a given list.
- **Example:** `"barfoothefoobarman"` with `["foo","bar"]` → `[0,9]`
- **Concepts:** Sliding window, hash map

### 40. **String Compression and Decompression**
- Implement custom compression/decompression algorithms.
- **Example:** Run-length encoding or Huffman coding
- **Concepts:** Algorithms, data compression

