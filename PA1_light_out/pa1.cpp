// === Region: Project Declaration ===
//
//  COMP2011 Spring 2026
//  PA1: Lights Out
//
//  Your name:
//  Your ITSC email:           @connect.ust.hk
//
//  Note: Generative AI is NOT allowed in completing your lab exercises or programming assignments
//  Reference: https://course.cse.ust.hk/comp2011/web/code.html
//
//  Declaration:
//  I declare that I am not involved in plagiarism
//  I understand that both parties (i.e., students providing the codes and students copying the codes) will receive 0 marks.
//
//  Project TA: Peter CHUNG (cspeter@cse.ust.hk)
//
//  For code-level questions, please send a direct email to the above TA.
//  Asking questions with code blocks in a public discussion forum (e.g., Piazza) may cause plagiarism issues
//  Usually, you will get the quickest response via a direct email.
//
// =====================================

#include <iostream>
#include <fstream>
using namespace std;

const int MAX_FILE_PATH = 100;
const int MAX_BOARD_SIZE = 10;
const int MAX_MOVES = 100;

// Note: You don't need to modify the following function prototypes
// They provide some explanations of the given function and the TODO functions
// Please implement your code in TODO[X]: Your Code region

/**
 * Prints the board to the console.
 * Note that this is a given function.
 * @param game_board The game board
 * @param size The size of the game board
 */
void print_board(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size);

/**
 * TODO 1: Initializes a game board from a specified file path
 * @param file_path The path to the file with information of the board
 * @param game_board The 2D array to store the board in
 * @return -1 if the file cannot be opened; 0 if the given size is larger than MAX_BOARD_SIZE; the size of the board otherwise.
 */
int initialize_board(const char file_path[MAX_FILE_PATH], int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);

/**
 * TODO 2: Checks if the game board has been solved.
 * @param game_board The game board
 * @param size The size of the board
 * @return true if the board is solved; false otherwise
 */
bool check_win(int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size);

/**
 * TODO 3: Compares if two boards are equal.
 * @param game_board1 The first game board
 * @param game_board2 The second game board
 * @param size The size of the board
 * @return A boolean indicating whether the two game boards are identical
 */
bool check_board_equal(int game_board1[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int game_board2[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size);

/**
 * TODO 4: Flicks a light in the game board.
 * @param game_board The game board
 * @param size The size of the board
 * @param row The row to flick
 * @param col The column to flick
 * @param num_moves The number of moves played, which should be updated
 * @param row_inputs The array storing the row value of moves
 * @param col_inputs The array storing the column value of moves
 * @param undo A boolean indicating whether this flick is an undo move
 */
void flick(int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int row, int col, int &num_moves, int row_inputs[MAX_MOVES], int col_inputs[MAX_MOVES], bool undo);

/**
 * TODO 5: Adds a board state to the history array.
 * @param game_board The game board
 * @param size The size of the board
 * @param idx_to_add The index of game_history to add to
 * @param game_history The array of game history
 */
void add_to_history(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int idx_to_add, int game_history[MAX_MOVES + 1][MAX_BOARD_SIZE][MAX_BOARD_SIZE]);

/**
 * TODO 6: Prints a hint to the console for the player.
 * The hint should be the switch where, after clicking, the number of ON switches (with value 1) will be decreased the most.
 * The board MUST be reviewed top-to-bottom and left-to-right, i.e., you should check (0, 0), then (0, 1), ..., then (1, 0), ..., then (2, 0), ...
 * If there is a draw, return the FIRST switch.
 * "No hints available" should be printed if no switches would lead to a decrease of the number of ON switches.
 * @param game_board The game board
 * @param size The size of the board
 */
void print_hint(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size);

/**
 * TODO 7: Checks if the set of moves played has no repeating flicks
 * @param num_moves The number of moves played
 * @param row_inputs The array storing the row value of moves
 * @param col_inputs The array storing the column value of moves
 * @return true if the set of moves has no repeating flicks; false otherwise
 */
bool is_no_repeating_flicks(int num_moves, const int row_inputs[MAX_MOVES], const int col_inputs[MAX_MOVES]);

/**
 * TODO 8: Checks if the current board has appeared before, i.e. if a board state has been repeated.
 * @param game_board The game board
 * @param size The size of the board
 * @param game_history The array of game history
 * @param num_moves The number of moves played
 * @return
 */
bool duplicate_state(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, const int game_history[MAX_MOVES + 1][MAX_BOARD_SIZE][MAX_BOARD_SIZE], int num_moves);

/**
 * TODO 9: Fills a frequency map of the moves the player did.
 * @param freq_map The 2D array to store the freq to. You should NOT assume that this array has been initialized.
 * @param size The size of the board
 * @param num_moves The number of moves played
 * @param row_inputs The array storing the row value of moves
 * @param col_inputs The array storing the column value of moves
 */
void make_freq_map(int freq_map[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int num_moves, const int row_inputs[MAX_MOVES], const int col_inputs[MAX_MOVES]);

// Given function
void print_board(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size)
{
    for (int i = 0; i < 2 * size - 1; i++)
        cout << "=";
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << game_board[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < 2 * size - 1; i++)
        cout << "=";
    cout << endl;
}

// === Start your coding work here ===

// Implement your code in TODO[X]: Your Code region
// For non-void return type functions, remember to return proper values based on the return type

// TODO 1: Your Code
int initialize_board(const char file_path[MAX_FILE_PATH], int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE])
{
    ifstream ifs(file_path);
    if (!ifs) return -1; // return -1 if file cannot be opened
    int size; ifs >> size;
    if (size > MAX_BOARD_SIZE) return 0;
    for (int row = 0; row < size; row++){
        char line[MAX_FILE_PATH]; 
        ifs >> line;
        for (int col = 0; col < size; col++){
            game_board[row][col] = line[col]-'0';
        }
    }
    ifs.close();
    return size;
}

// TODO 2: Your Code
bool check_win(int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size)
{   for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            if (game_board[row][col] == 1) return false;
        }
    }
    return true;
}

// TODO 3: Your Code
bool check_board_equal(const int game_board1[MAX_BOARD_SIZE][MAX_BOARD_SIZE], const int game_board2[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size)
{   for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            if (game_board1[row][col] != game_board2[row][col]) return false;
        }
    }
    return true;
}

// TODO 4: Your Code
void flick(int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int row, int col, int &num_moves, int row_inputs[MAX_MOVES], int col_inputs[MAX_MOVES], bool undo)
{   // horizontal flick
    for (int c = col-1; c <= col+1; c++){
        if (c<0 || c> size-1) continue;
        game_board[row][c] = (game_board[row][c] == 0)? 1: 0;
    }
    // certical flick
    for (int r = row-1; r <= row+1; r++){
        if (r<0 || r> size-1) continue;
        if (r == row) continue; //anvoid flicking the centre back
        game_board[r][col] = (game_board[r][col] == 0)? 1: 0;
    }
    if (!undo){
        row_inputs[num_moves] = row;
        col_inputs[num_moves] = col;
        num_moves++;
    }
    if (undo) num_moves--;
}

// TODO 5: Your Code
void add_to_history(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int idx_to_add, int game_history[MAX_MOVES + 1][MAX_BOARD_SIZE][MAX_BOARD_SIZE])
{   for (int r = 0; r < size; r++){
        for (int c= 0; c < size; c++){
            game_history[idx_to_add][r][c] = game_board[r][c];
        }
    }
}

// TODO 6: Your Code
void print_hint(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size)
{
    int row = -1, col = -1;
    // Do not modify the row and col variable names above
    // These 2 variables will be used before exiting this function
    // Write your code for this function below
    
    // Count current lights on
    int current_light_on = 0;
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (game_board[r][c] == 1) current_light_on++;
        }
    }
    
    int dummy_num_moves = 0; 
    int dummy_row_inputs[MAX_MOVES] = {}; 
    int dummy_col_inputs[MAX_MOVES] = {};
    
    int min_light_on = current_light_on;  // Track the minimum
    int best_row = -1, best_col = -1;      // Track which cell achieves it
    
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            // Create a fresh copy
            int temp_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    temp_board[i][j] = game_board[i][j];
                }
            }
            
            // Flick at (r,c)
            flick(temp_board, size, r, c, dummy_num_moves, dummy_row_inputs, dummy_col_inputs, true);
            
            // Count lights after flick
            int temp_light_on = 0;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (temp_board[i][j] == 1) temp_light_on++;
                }
            }
            
            // Update if this gives FEWER lights than our current minimum
            if (temp_light_on < min_light_on) {
                min_light_on = temp_light_on;
                best_row = r;
                best_col = c;
            }
        }
    }
    
    // Only provide hint if it actually reduces lights
    if (min_light_on < current_light_on) {
        row = best_row;
        col = best_col;
    }
    
    // Write your code for this function above
    // Do not modify the if-else clause below
    if (row == -1 || col == -1) // If `row` and `col` are not updated, that means no hints are available
        cout << "No hints available!" << endl;
    else
        cout << "Hint: You can try flicking the switch at (" << row << ", " << col << ")!" << endl;
}

// TODO 7: Your Code
bool is_no_repeating_flicks(int num_moves, const int row_inputs[MAX_MOVES], const int col_inputs[MAX_MOVES])
{   int idx1, idx2;
    for (idx1 = 0; idx1 < num_moves; idx1++){
        for(idx2 = idx1+1; idx2 +1 < num_moves; idx2++){
            if (row_inputs[idx1] == row_inputs[idx2] && col_inputs[idx1]==col_inputs[idx2]) return false;
        }
    }
    return true;
}

// TODO 8: Your Code
bool duplicate_state(const int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, const int game_history[MAX_MOVES + 1][MAX_BOARD_SIZE][MAX_BOARD_SIZE], int num_moves)
{   bool duplicate;
    for (int moves = 0; moves < num_moves; moves++){
        duplicate = true;
        for (int r = 0; r < size; r++){
            for (int c = 0; c < size; c++){
                if (game_history[moves][r][c] != game_board[r][c]) duplicate = false;
            }
        }
        if (duplicate) return true;
    }
    return false;
}

// TODO 9: Your Code
void make_freq_map(int freq_map[MAX_BOARD_SIZE][MAX_BOARD_SIZE], int size, int num_moves, const int row_inputs[MAX_MOVES], const int col_inputs[MAX_MOVES])
{   // freq_map is not initialized
    for (int r = 0; r < size; r++){
        for (int c=0; c < size; c++){
            freq_map[r][c] = 0;
        }
    }

    for (int i = 0; i < num_moves; i++){
        freq_map[row_inputs[i]][col_inputs[i]]++;
    }

}

// === End your coding work here ===

/**
 * The main function.
 * Do not modify the main function!!!
 * @return The exit code
 */
int main()
{
    cout << "Welcome to the Lights Out Game!" << endl;

    char file_path[MAX_FILE_PATH];
    cout << "Enter the path of the board file: ";
    cin >> file_path;

    int game_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int game_history[MAX_MOVES+1][MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int choice = -1, row = -1, col = -1;
    bool win = false;
    int num_moves = 0;
    int row_inputs[MAX_MOVES], col_inputs[MAX_MOVES];

    int size = initialize_board(file_path, game_board);
    if (size == -1)
    {
        cout << "Error: Can not open " << file_path << endl;
        return -1;
    }
    if (size == 0)
    {
        cout << "The size of the board is larger than the maximum (" << MAX_BOARD_SIZE << ")! Exiting!" << endl;
        return -1;
    }

    print_board(game_board, size);
    add_to_history(game_board, size, 0, game_history);

    while (!win)
    {
        while (choice == -1)
        {
            cout << "Enter 0 to exit, 1 to flick a light switch, 2 to undo your previous move and 3 for a hint: ";
            cin >> choice;
            if (choice < 0 || choice > 3)
            {
                cout << "Invalid choice!" << endl;
                choice = -1;
            }
        }

        if (!choice)
        {
            cout << "Exiting!" << endl;
            return 0;
        }
        if (num_moves == MAX_MOVES)
        {
            cout << "Max number of moves reached! Exiting!" << endl;
            break;
        }

        if (choice == 1)
        {
            cout << "Enter row and column number of the light to flick, separated by a space (e.g. \"0 2\" for row 0, column 2): ";
            cin >> row >> col;
            if (row < 0 || row >= size || col < 0 || col >= size)
            {
                cout << "Invalid row or column!" << endl;
                row = col = -1;
                continue;
            }
            cout << "Flicking light at (" << row << ", " << col << ")..." << endl;
            flick(game_board, size, row, col, num_moves, row_inputs, col_inputs, false);
            add_to_history(game_board, size, num_moves, game_history);

            print_board(game_board, size);
            if (duplicate_state(game_board, size, game_history, num_moves))
            {
                cout << "This board state has appeared before!" << endl;
            }

            win = check_win(game_board, size);
        }
        else if (choice == 2)
        {
            if (num_moves != 0)
            {
                int r = row_inputs[num_moves - 1], c = col_inputs[num_moves - 1];
                cout << "Undoing flick at (" << r << ", " << c << ")..." << endl;
                flick(game_board, size, r, c, num_moves, row_inputs, col_inputs, true);
                print_board(game_board, size);
                win = check_win(game_board, size);
            }
            else
            {
                cout << "No moves to undo!" << endl;
            }
        }
        else
        {
            print_hint(game_board, size);
        }
        choice = -1;
    }

    int freq_map[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    cout << "Heatmap:" << endl;
    make_freq_map(freq_map, size, num_moves, row_inputs, col_inputs);
    print_board(freq_map, size);
    if (win)
    {
        cout << "You win! Your moves have " << (is_no_repeating_flicks(num_moves, row_inputs, col_inputs) ? "no " : "some ") << "repeating flicks!" << endl;
    }

    return 0;
}
