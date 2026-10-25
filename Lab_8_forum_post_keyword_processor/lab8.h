#ifndef LAB8_H
#define LAB8_H

// These MAX_* constants are capacity limits for the provided fixed-size arrays.
const int MAX_POST_NUM = 8;
const int MAX_AUTHOR_LEN = 20;
const int MAX_RAW_LEN = 80;
const int MAX_WORD_NUM = 8;
const int MAX_WORD_LEN = 16;
const int MAX_KEYWORD_NUM = 6;

struct WordToken {
    char text[MAX_WORD_LEN];
    bool matched;
};

struct ForumPost {
    int post_id;
    char author[MAX_AUTHOR_LEN];
    char raw_text[MAX_RAW_LEN];
    WordToken words[MAX_WORD_NUM];
    int word_count;
    int keyword_hits;
    bool has_match;
    char first_keyword[MAX_WORD_LEN];
};

void tokenizePosts(ForumPost* postArray, int size);
int markKeywordMatches(
    ForumPost postArray[],
    int size,
    const char keywords[][MAX_WORD_LEN],
    int keywordCount
);
void reorderBoardOrder(ForumPost* postPtrArray[], int size);

#endif
