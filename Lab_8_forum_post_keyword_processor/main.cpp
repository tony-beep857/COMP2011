#include "lab8.h"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

void initializeBoardPointers(ForumPost* postPtrArray[], ForumPost postArray[], int size) {
    for (int i = 0; i < size; ++i) {
        postPtrArray[i] = &postArray[i];
    }
}

void initializePostMetadata(ForumPost& post) {
    for (int i = 0; i < MAX_WORD_NUM; ++i) {
        post.words[i].text[0] = '\0';
        post.words[i].matched = false;
    }

    post.word_count = 0;
    post.keyword_hits = 0;
    post.has_match = false;
    strcpy(post.first_keyword, "none");
}

void removeTrailingCarriageReturn(char text[]) {
    int length = static_cast<int>(strlen(text));

    if (length > 0 && text[length - 1] == '\r') {
        text[length - 1] = '\0';
    }
}

bool loadForumData(
    ForumPost postArray[],
    int& postCount,
    char keywords[][MAX_WORD_LEN],
    int& keywordCount
) {
    if (!(cin >> postCount >> keywordCount)) {
        return false;
    }

    if (postCount < 0 || postCount > MAX_POST_NUM ||
        keywordCount < 0 || keywordCount > MAX_KEYWORD_NUM) {
        return false;
    }

    for (int i = 0; i < postCount; ++i) {
        cin >> postArray[i].post_id >> setw(MAX_AUTHOR_LEN) >> postArray[i].author;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(postArray[i].raw_text, MAX_RAW_LEN);
        removeTrailingCarriageReturn(postArray[i].raw_text);
        initializePostMetadata(postArray[i]);
    }

    for (int i = 0; i < keywordCount; ++i) {
        cin >> setw(MAX_WORD_LEN) >> keywords[i];
    }

    return true;
}

void printMenu() {
    cout << "===========================================" << endl;
    cout << "Choose what to do next:" << endl;
    cout << "1. Display parsed posts" << endl;
    cout << "2. Analyze keyword matches" << endl;
    cout << "3. Build weekly board order" << endl;
    cout << "4. Display weekly board" << endl;
    cout << "5. Quit" << endl;
}

void printTokens(const ForumPost& post, bool showMatchedMark) {
    cout << "tokens:";

    if (post.word_count > 0) {
        cout << " ";
    }

    for (int i = 0; i < post.word_count; ++i) {
        cout << post.words[i].text;

        if (showMatchedMark && post.words[i].matched) {
            cout << "*";
        }

        if (i + 1 < post.word_count) {
            cout << " ";
        }
    }

    cout << endl;
}

void displayParsedPosts(const ForumPost postArray[], int size) {
    cout << "Parsed posts:" << endl;

    for (int i = 0; i < size; ++i) {
        cout << "[" << postArray[i].post_id << "] " << postArray[i].author << endl;
        cout << "raw_text: " << postArray[i].raw_text << endl;
        cout << "word_count: " << postArray[i].word_count << endl;
        printTokens(postArray[i], false);
        cout << endl;
    }
}

void displayBoardOrder(ForumPost* postPtrArray[], int size) {
    cout << "Weekly board order:" << endl;

    for (int i = 0; i < size; ++i) {
        cout << postPtrArray[i]->post_id << " (hits: " << postPtrArray[i]->keyword_hits << ")" << endl;
    }

    cout << endl;
}

void displayWeeklyBoard(ForumPost* postPtrArray[], int size) {
    cout << "Weekly board:" << endl;

    for (int i = 0; i < size; ++i) {
        cout << "[" << postPtrArray[i]->post_id << "] " << postPtrArray[i]->author << endl;
        cout << "keyword_hits: " << postPtrArray[i]->keyword_hits << endl;
        cout << "has_match: " << (postPtrArray[i]->has_match ? "true" : "false") << endl;
        cout << "first_keyword: " << postPtrArray[i]->first_keyword << endl;
        printTokens(*postPtrArray[i], true);
        cout << endl;
    }
}

int main() {
    ForumPost postArray[MAX_POST_NUM];
    char keywords[MAX_KEYWORD_NUM][MAX_WORD_LEN];
    int postCount = 0;
    int keywordCount = 0;

    if (!loadForumData(postArray, postCount, keywords, keywordCount)) {
        return 0;
    }

    ForumPost* postPtrArray[MAX_POST_NUM];

    initializeBoardPointers(postPtrArray, postArray, postCount);
    tokenizePosts(postArray, postCount);

    int choice = 0;
    bool keywordAnalysisDone = false;

    while (true) {
        printMenu();

        if (!(cin >> choice)) {
            break;
        }

        if (choice == 5) {
            break;
        }

        switch (choice) {
            case 1:
                displayParsedPosts(postArray, postCount);
                break;
            case 2:
                cout << markKeywordMatches(postArray, postCount, keywords, keywordCount)
                     << " post(s) contain at least one matched token." << endl;
                cout << endl;
                keywordAnalysisDone = true;
                break;
            case 3:
                if (!keywordAnalysisDone) {
                    cout << "Please run option 2 before option 3." << endl;
                    cout << endl;
                    break;
                }
                reorderBoardOrder(postPtrArray, postCount);
                displayBoardOrder(postPtrArray, postCount);
                break;
            case 4:
                displayWeeklyBoard(postPtrArray, postCount);
                break;
            default:
                break;
        }
    }

    return 0;
}
