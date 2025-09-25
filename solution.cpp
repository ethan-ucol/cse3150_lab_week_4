#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    // TODO: implement print_board here
	for (const auto& row : board){
        for (size_t j = 0; j < row.size(); ++j){
            cout << row[j];
        if (j < row.size() - 1) cout << " ";
        }
        cout << "/n";
    }
    cout << endl;

    write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly

void spawn_tile(vector<vector<int>>& board) {
	std::vector <std::pair<int, int>> empty_cells;
    for (int r=0;r<4;++r){
        for (int c=0;c<4;++c){
            if (board[r][c]==0)
                empty_cells.push_back({r, c});

        }
    // TODO: Feed this into chat GPT and have it correct the function for you
    // with proper prompting
}
    //If there are no empty cells, do nothing
    if (empty_cells.empty()) return;
	
	// Random Number Generator	
	std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist_cell(0, empty_cells.size() - 1);
    std::uniform_real_distribution<double> dist_value(0.0, 1.0);

    int idx = dist_cell(rng);
    int r = empty_cells[idx].first;
    int c = empty_cells[idx].second;

    // 90% chance 2, 10% chance 4
    board[r][c] = (dist_value(rng) < 0.9) ? 2 : 4;
}


// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end

std::vector<int> compress_row(const std::vector<int>& row) {
    std::vector<int> compressed;
    
    std::copy_if(row.begin(), row.end(), std::back_inserter(compressed), [](int x){return x != 0;});
    
    compressed.resize(4,0);
    
    return compressed;
}

// TODO: Merge a row (assumes the row is already compressed)

std::vector<int> merge_row(std::vector<int> row) {
    for (int i = 0; i + 1 < row.size(); ++i){
        if (row[i] == row[i+1]){
            row[i] *= 2;
            row[i+1] = 0;
        }
    }
    return row;
}

// TODO: use copy_if and iterators

bool move_left(vector<vector<int>>& board) {
    
    bool moved = false;
    
    for (int i = 0; i < board.size(); ++i){
        vector<int> old_row = board[i];
        
        vector<int> new_row = compress_row(board[i]);

        new_row = merge_row(new_row);

        board[i] = new_row;
    
        if (old_row != new_row){
            moved = true;
        }

    }
    return moved;
}

// TODO: use reverse iterators

bool move_right(vector<vector<int>>& board) {
    bool moved = false;
    
    for (int i = 0; i < board.size(); ++i){
        vector<int> old_row = board[i];

        std::reverse(board[i].begin(), board[i].end());
    
        vector<int> new_row = compress_row(board[i]);

        new_row = merge_row(board[i]);

        std::reverse(board[i].begin(), board[i].end());

        if (old_row != new_row){
            moved = true;
        }
    }    
    
    return moved;
}


// TODO: use column traversal

bool move_up(vector<vector<int>>& board){
    bool moved = false;
    
    for (int j = 0; j < board[0].size(); ++j){
        vector<int> col;
    
        for (int i = 0; i < board.size(); ++i){
            col.push_back(board[i][j]);
        }
	
	    vector<int> old_col = col;
    
	    col = compress_row(col);
	    col = merge_row(col);	

        for (int i = 0; i < board.size(); ++i){
            board[i][j] = col[i];
        }   

        if (old_col != col){
            moved = true;
        }
    }

    return moved;
}

// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){
    bool moved = false;

    for (int j = 0; j < board[0].size(); ++j){
        vector<int> col;

        for (int i = 0; i < board.size(); ++i){
            col.push_back(board[i][j]);
        }

        vector<int> old_col = col;
        std::reverse(col.begin(), col.end());
        
        col = compress_row(col);
        col = merge_row(col);

        std::reverse(col.begin(), col.end());
        
        for (int i = 0; i < board.size(); ++i){
            board[i][j] = col[i];
        }

        if (col != old_col){
            moved = true;
        }

    }
    
    return moved;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
    // with proper prompting
            // TODO: get the history and print the board and continue
            if (!history.empty()){
                board = history.top();
                history.pop();
            }
            print_board(board, false);
            continue;
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            history.push(prev);
            spawn_tile(board);
        }
    }
    return 0;
}
