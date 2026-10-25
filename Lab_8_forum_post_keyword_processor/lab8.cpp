#include "lab8.h"
#include <cstring>

// ===== TODO START: Task 1 =====
void tokenizePosts(ForumPost* postArray, int size) {
    for (ForumPost* p = postArray; p < postArray + size; ++p) {
        char* src = p->raw_text;
        int word_idx = 0;

        while (*src != '\0') { // is not the end of sentence
            int char_idx = 0;
            while (*src != ' ' && *src != '\0') {
                p->words[word_idx].text[char_idx] = *src;
                ++char_idx;
                ++src;
            }
            p->words[word_idx].text[char_idx] = '\0';
            p->words[word_idx].matched = false;

            ++word_idx; // jump to the next word

            if (*src == ' ') { //jump the space
                ++src;
            }
        }
        p->word_count = word_idx;
    }
}
// ===== TODO END: Task 1 =====

// ===== TODO START: Task 2 =====
int markKeywordMatches(
    ForumPost postArray[],
    int size,
    const char keywords[][MAX_WORD_LEN],
    int keywordCount)
{
    // TODO:
    // 1. Process posts in storage order and reset keyword_hits, has_match,
    //    and first_keyword for each post.
    // 2. Check each token against the keyword list using substring matching.
    // 3. Update matched, keyword_hits, has_match, and first_keyword in place.
    // 4. Return the number of posts that contain at least one matched token.
    int matchCount = 0;

    for (int i = 0; i < size; ++i)
    {
        ForumPost &post = postArray[i];

        // reset everything as required
        post.keyword_hits = 0;
        post.has_match = false;
        strcpy(post.first_keyword, "none");

        for (int w = 0; w < post.word_count; ++w)
        {
            WordToken &token = post.words[w];
            token.matched = false;

            bool token_matched = false;

            // check keywords in the exact order given
            for (int k = 0; k < keywordCount; ++k)
            {
                if (strstr(token.text, keywords[k]) != NULL) //keyword has found
                { // contiguous substring
                    token.matched = true;
                    token_matched = true;

                    // first matched token in this post?
                    if (!post.has_match)
                    {
                        post.has_match = true;
                        strcpy(post.first_keyword, keywords[k]);
                    }
                    break; // if matched, you don't have to search anymore
                }
            }

            if (token_matched)
            {
                post.keyword_hits++;
            }
        }

        if (post.has_match)
        {
            matchCount++;
        }
    }

    (void)postArray;
    (void)size;
    (void)keywords;
    (void)keywordCount;

    return matchCount;
}
// ===== TODO END: Task 2 =====

// ===== TODO START: Task 3 =====
void reorderBoardOrder(ForumPost *postPtrArray[], int size)
{
    // TODO:
    // 1. Reorder only postPtrArray.
    // 2. Sort by keyword_hits (descending).
    // 3. Break ties by word_count, total character count, and author.
    // 4. This ordering must be stable: if all comparison fields are tied,
    //    keep the original relative order of those pointers.
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - 1 - i; ++j) {
            ForumPost* a = postPtrArray[j];
            ForumPost* b = postPtrArray[j + 1];

            // compare a and b - should a come BEFORE b?
            bool a_before_b = true;  // Default: keep original order (stable)

            if (a->keyword_hits != b->keyword_hits) {
                a_before_b = (a->keyword_hits > b->keyword_hits);           // 1. hits desc
            }
            else if (a->word_count != b->word_count) {
                a_before_b = (a->word_count < b->word_count);               // 2. word_count asc
            }
            else {
                // 3. total character count (sum of word lengths, no spaces)
                int len_a = 0, len_b = 0;
                for (int w = 0; w < a->word_count; ++w)
                    len_a += strlen(a->words[w].text);
                for (int w = 0; w < b->word_count; ++w)
                    len_b += strlen(b->words[w].text);

                if (len_a != len_b) {
                    a_before_b = (len_a < len_b);
                }
                else {
                    // 4. author lex smaller
                    int cmp = strcmp(a->author, b->author);
                    if (cmp != 0) {
                        a_before_b = (cmp < 0);
                    }
                    // 5. all tied → keep original relative order (do nothing)
                }
            }

            if (!a_before_b) {
                // swap the pointers (only the display order changes)
                postPtrArray[j]     = b;
                postPtrArray[j + 1] = a;
            }
        }
    }

    (void)postPtrArray;
    (void)size;
}
// ===== TODO END: Task 3 =====
