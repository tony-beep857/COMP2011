# Lab 8: Forum Post Keyword Processor & Reordering System

## Overview

`lab8.cpp` implements a post-processing and text analytics engine for forum content in C++. The system parses raw text into discrete tokens (words), searches posts for matching keywords using substring matching, tracks keyword occurrence statistics, and reorders post pointers using custom multi-criteria sorting algorithms.

---

## Technical Tasks & Features

### Task 1: String Tokenization (`tokenizePosts`)
* Iterates through an array of `ForumPost` instances using pointer arithmetic (`ForumPost* p = postArray`).
* Parses the raw string (`p->raw_text`) cell by cell, separating words delimited by spaces (`' '`).
* Dynamically constructs word tokens inside `p->words[word_idx].text` and appends proper C-string null terminators (`'\0'`).
* Sets initial token properties (`matched = false`) and updates total `word_count`.

### Task 2: Keyword Searching & Matching (`markKeywordMatches`)
* Resets post statistics prior to processing: sets `keyword_hits = 0`, `has_match = false`, and `first_keyword = "none"`.
* Scans each word token against a array of search keywords using `strstr` for substring matching.
* Marks matched tokens (`token.matched = true`), updates total hits, and captures the first matched keyword in sequence.
* Returns the total count of posts containing at least one matching keyword.

### Task 3: Board Display Reordering (`reorderBoardOrder`)
Sorts an array of post pointers (`ForumPost* postPtrArray[]`) without modifying the original array stored in memory. It applies a **stable Bubble Sort algorithm** using the following hierarchy:

1. **Keyword Hits (Descending):** Posts with more matching keywords appear first.
2. **Word Count (Ascending):** Shorter posts (fewer words) take priority on tie.
3. **Total Character Count (Ascending):** Combined length of all tokens (`strlen`) breaks ties.
4. **Author Name (Lexicographical):** Alphabetical ordering based on `strcmp(a->author, b->author)`.
5. **Original Storage Order:** Maintains stability if all parameters are identical.

---

## Data Structures (`lab8.h`)

The system works with three primary entities:

```cpp
struct WordToken {
    char text[MAX_WORD_LEN];  // C-string word content
    bool matched;             // True if word contains a target keyword
};

struct ForumPost {
    char author[MAX_NAME_LEN];
    char raw_text[MAX_TEXT_LEN];
    WordToken words[MAX_WORDS];
    int word_count;
    
    // Analytics & Search Metadata
    int keyword_hits;
    bool has_match;
    char first_keyword[MAX_WORD_LEN];
};
```

---

## Compilation & Execution

### Compilation

Compile `lab8.cpp` together with your driver program (`main.cpp`) using standard C++ compiler flags:

```bash
g++ -std=c++11 main.cpp lab8.cpp -o forum_processor
```

### Running the Executable

Execute the compiled program from your terminal:

```bash
./forum_processor
```

---

## Summary of Core API Functions

| Function Signature | Description |
| :--- | :--- |
| `void tokenizePosts(ForumPost* postArray, int size)` | Splits raw post text into structured `WordToken` items. |
| `int markKeywordMatches(ForumPost postArray[], int size, const char keywords[][MAX_WORD_LEN], int keywordCount)` | Scans tokens for keywords, updates post stats, and returns matching post counts. |
| `void reorderBoardOrder(ForumPost* postPtrArray[], int size)` | Sorts post pointers stably based on hits, length, and author metadata. |