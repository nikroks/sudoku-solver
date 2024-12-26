#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <functional>

using namespace std;

struct assumption_struct {
    bool solved;
    int x; 
    int y;
    vector<int> possible_assumption;
};

class operations {
    public:
    std::vector<std::function<void()> > operations_list;

    void execute() {
        for(auto it = operations_list.begin() ; it != operations_list.end(); it++) {
            (*it)();
        }
    }
};

class block {
    public:
        int x_pos, y_pos, value;
        vector<int> possible_values;
        block* square_block_start;
        map<int, set<block* > > row_map;
        map<int, set<block* > > column_map;
        map<int, set<block* > > inner_square_block_map;
        bool assumed; 
    
    block(int x, int y) {
        x_pos = x;
        y_pos = y;
        value=0;
    }

    void operator= (int assignment_value) {
        value = assignment_value;

        while (possible_values.size() > 0) {
            possible_values.pop_back();
        }

        row_map.clear();
        column_map.clear();
        inner_square_block_map.clear();
    }
};

block block_array[9][9] = {{block(0, 0), block(0, 1), block(0, 2), block(0, 3), block(0, 4), block(0, 5), block(0, 6), block(0, 7), block(0, 8)}, 
                           {block(1, 0), block(1, 1), block(1, 2), block(1, 3), block(1, 4), block(1, 5), block(1, 6), block(1, 7), block(1, 8)}, 
                           {block(2, 0), block(2, 1), block(2, 2), block(2, 3), block(2, 4), block(2, 5), block(2, 6), block(2, 7), block(2, 8)}, 
                           {block(3, 0), block(3, 1), block(3, 2), block(3, 3), block(3, 4), block(3, 5), block(3, 6), block(3, 7), block(3, 8)}, 
                           {block(4, 0), block(4, 1), block(4, 2), block(4, 3), block(4, 4), block(4, 5), block(4, 6), block(4, 7), block(4, 8)}, 
                           {block(5, 0), block(5, 1), block(5, 2), block(5, 3), block(5, 4), block(5, 5), block(5, 6), block(5, 7), block(5, 8)}, 
                           {block(6, 0), block(6, 1), block(6, 2), block(6, 3), block(6, 4), block(6, 5), block(6, 6), block(6, 7), block(6, 8)}, 
                           {block(7, 0), block(7, 1), block(7, 2), block(7, 3), block(7, 4), block(7, 5), block(7, 6), block(7, 7), block(7, 8)}, 
                           {block(8, 0), block(8, 1), block(8, 2), block(8, 3), block(8, 4), block(8, 5), block(8, 6), block(8, 7), block(8, 8)}};

bool is_valid (int x, int y, int value) {
    block* block_ptr = &block_array[x][y];
    block* root = block_ptr->square_block_start;

    // Block value is uninitialized
    if(block_ptr->value != 0) {
        return false;
    }

    for (int i=0; i<9; i++) {

        // Check all elements of the column
        if((i != x) && (block_array[i][y].value == value)) {
            return false;
        }

        // Check all elements of the row
        if((i != y) && (block_array[x][i].value == value)) {
            return false;
        }
    }

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if((root->x_pos + i == x) && (root->y_pos + j == y)) {
                continue;
            }

            if(block_array[root->x_pos + i][root->y_pos + j].value == value) {
                return false;
            }
        }
    }

    return true;
}

void populate_block_data_structure () {

    block* root;

    for(int i=0; i<9; i++) { // Row iterator
        for(int j=0; j<9; j++) {  // Column iterator
            for (int k=1; k<=9; k++) {  // Value iterator
                if(is_valid(i, j, k)) {
                    block_array[i][j].possible_values.push_back(k);
                }
            }
        }
    }

    for(int i=0; i<9; i++) { // Row iterator
        for(int j=0; j<9; j++) {  // Column iterator
            for(auto it = block_array[i][j].possible_values.begin(); it != block_array[i][j].possible_values.end(); it++) {
                
                // Populate row map
                for (int k=0; k<9; k++) {
                    if(block_array[i][k].value != 0) {
                        // Value already assigned
                        continue;
                    }

                    auto it2 = find(block_array[i][k].possible_values.begin(), block_array[i][k].possible_values.end(), *it);
                    if(it2 != block_array[i][k].possible_values.end()){
                        block_array[i][k].row_map[*it].insert(&(block_array[i][j]));
                    }
                }

                // Populate column map
                for (int k=0; k<9; k++) {
                    if(block_array[k][j].value != 0) {
                        // Value already assigned
                        continue;
                    }

                    auto it2 = find(block_array[k][j].possible_values.begin(), block_array[k][j].possible_values.end(), *it);
                    if(it2 != block_array[k][j].possible_values.end()) {
                        block_array[k][j].column_map[*it].insert(&(block_array[i][j]));
                    }
                }

                root = block_array[i][j].square_block_start;
                // Populate inner square block
                for (int k=0; k<3; k++) {
                    for(int l=0; l<3; l++) {
                        if(block_array[root->x_pos + k][root->y_pos + l].value != 0) {
                            // Value already assigned
                            continue;
                        }

                        auto it2 = find(block_array[root->x_pos + k][root->y_pos + l].possible_values.begin(), block_array[root->x_pos + k][root->y_pos + l].possible_values.end(), *it);
                        if(it2 != block_array[root->x_pos + k][root->y_pos + l].possible_values.end()) {
                            block_array[root->x_pos + k][root->y_pos + l].inner_square_block_map[*it].insert(&(block_array[i][j]));
                        }
                    }
                }
            }
        }
    }

}

void initialise(char a[9][9]) {
    for (int i=0; i<9; i+=3) {
        for (int j=0; j<9; j+=3) {
            for (int k=0; k<3; k++) {
                for (int l=0; l<3; l++) {
                    block_array[i+k][j+l].square_block_start = &block_array[i][j];
                    if(a[i+k][j+l] != '-') {
                        block_array[i+k][j+l] = a[i+k][j+l] - '0';
                    }
                }
            }
        }
    }

    populate_block_data_structure();

    return;
}

operations handle_assignment_dependencies (int x, int y, int value) {

    operations ops;
    block* root = block_array[x][y].square_block_start;

    ops.operations_list.push_back([&](){block_array[x][y].value = 0;});
    block_array[x][y].value = value;

    // Clear possible_values vectors for (x, y)
    while(block_array[x][y].possible_values.size() > 0) {
        auto temp = block_array[x][y].possible_values[block_array[x][y].possible_values.size() - 1];
        ops.operations_list.push_back([&](){block_array[x][y].possible_values.push_back(temp);});
        block_array[x][y].possible_values.pop_back();
    }

    // Clear row_map for (x, y)
    auto temp_row_map = block_array[x][y].row_map;
    ops.operations_list.push_back([&](){block_array[x][y].row_map = temp_row_map;});
    block_array[x][y].row_map.clear();

    // Clear column_map for (x, y)
    auto temp_column_map = block_array[x][y].column_map;
    ops.operations_list.push_back([&](){block_array[x][y].column_map = temp_column_map;});
    block_array[x][y].column_map.clear();

    // Clear inner_square_block_map for (x, y)
    auto temp_inner_square_block_map = block_array[x][y].inner_square_block_map;
    ops.operations_list.push_back([&](){block_array[x][y].inner_square_block_map = temp_inner_square_block_map;});
    block_array[x][y].inner_square_block_map.clear();


    for (int i=0; i<9; i++) {

        // Iterator over (*, y)
        if((x != i) && (block_array[i][y].value == 0)) {
            auto it = find(block_array[i][y].possible_values.begin(), block_array[i][y].possible_values.end(), value);
            if(it != block_array[i][y].possible_values.end()) {

                ops.operations_list.push_back([&](){block_array[i][y].possible_values.push_back(*it);});
                block_array[i][y].possible_values.erase(it);
                
                auto temp_row_map = block_array[i][y].row_map[value];
                ops.operations_list.push_back([&](){block_array[i][y].row_map[value] = temp_row_map;});
                block_array[i][y].row_map.erase(value);
                
                auto temp_column_map = block_array[i][y].column_map[value];
                ops.operations_list.push_back([&](){block_array[i][y].column_map[value] = temp_column_map;});
                block_array[i][y].column_map.erase(value);
                
                auto temp_inner_square_block_map = block_array[i][y].inner_square_block_map[value];
                ops.operations_list.push_back([&](){block_array[i][y].inner_square_block_map[value] = temp_inner_square_block_map;});
                block_array[i][y].inner_square_block_map.erase(value);

                block* inner_root = block_array[i][y].square_block_start;

                for (int k=0; k< 3; k++) {
                    for(int l=0; l<3; l++) {
                        for (auto it=block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map.begin(); it != block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map.end(); it++) {
                            if(it->first == value) {
                                for(auto it2=it->second.begin(); it2 != it->second.end(); it2++) {
                                    if(((*it2)->x_pos == i) && ((*it2)->y_pos == y)) {
                                        ops.operations_list.push_back([=](){block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map[value].insert(*it2);});
                                        it->second.erase((*it2));
                                        break;
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
        }

        // Iterator over (x, *)
        if((y != i) && (block_array[x][i].value == 0)) {
            auto it = find(block_array[x][i].possible_values.begin(), block_array[x][i].possible_values.end(), value);
            if(it != block_array[x][i].possible_values.end()) {

                ops.operations_list.push_back([&](){block_array[x][i].possible_values.push_back(*it);});
                block_array[x][i].possible_values.erase(it);

                auto temp_row_map = block_array[x][i].row_map[value];
                ops.operations_list.push_back([&](){block_array[x][i].row_map[value] = temp_row_map;});
                block_array[x][i].row_map.erase(value);

                auto temp_column_map = block_array[x][i].column_map[value];
                ops.operations_list.push_back([&](){block_array[x][i].column_map[value] = temp_column_map;});
                block_array[x][i].column_map.erase(value);

                auto temp_inner_square_block_map = block_array[x][i].inner_square_block_map[value];
                ops.operations_list.push_back([&](){block_array[x][i].inner_square_block_map[value] = temp_inner_square_block_map;});
                block_array[x][i].inner_square_block_map.erase(value);

                block* inner_root = block_array[x][i].square_block_start;

                for (int k=0; k< 3; k++) {
                    for(int l=0; l<3; l++) {
                        for (auto it=block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map.begin(); it != block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map.end(); it++) {
                            if(it->first == value) {
                                for(auto it2=it->second.begin(); it2 != it->second.end(); it2++) {
                                    if(((*it2)->x_pos == x) && ((*it2)->y_pos == i)) {
                                        ops.operations_list.push_back([=](){block_array[inner_root->x_pos + k][inner_root->y_pos + l].inner_square_block_map[value].insert(*it2);});
                                        it->second.erase((*it2));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Iterator over all blocks in the 9-block square
    for (int i=0; i< 3; i++) {
        for (int j=0; j<3; j++) {

            if((root->x_pos + i == x) && (root->y_pos + j == y)) {
                continue;
            }

            if(block_array[root->x_pos + i][root->y_pos + j].value != 0) {
                continue;
            }

            auto it = find(block_array[root->x_pos + i][root->y_pos + j].possible_values.begin(), block_array[root->x_pos + i][root->y_pos + j].possible_values.end(), value);
            if(it != block_array[root->x_pos + i][root->y_pos + j].possible_values.end()) {

                ops.operations_list.push_back([&](){block_array[root->x_pos + i][root->y_pos + j].possible_values.push_back(*it);});
                block_array[root->x_pos + i][root->y_pos + j].row_map.erase(*it);

                auto temp_row_map = block_array[root->x_pos + i][root->y_pos + j].row_map[value];
                ops.operations_list.push_back([&](){block_array[root->x_pos + i][root->y_pos + j].row_map[value] = temp_row_map;});
                block_array[root->x_pos + i][root->y_pos + j].column_map.erase(*it);

                auto temp_column_map = block_array[root->x_pos + i][root->y_pos + j].column_map[value];
                ops.operations_list.push_back([&](){block_array[root->x_pos + i][root->y_pos + j].column_map[value] = temp_column_map;});
                block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map.erase(*it);

                auto temp_inner_square_block_map = block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map[value];
                ops.operations_list.push_back([&](){block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map[value] = temp_inner_square_block_map;});
                block_array[root->x_pos + i][root->y_pos + j].possible_values.erase(it);
            }

            for (auto it=block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map.begin(); it != block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map.end(); it++) {

                for(auto it2=it->second.begin(); it2 != it->second.end(); it2++) {
                    if(((*it2)->x_pos == x) && ((*it2)->y_pos == y)) {
                        ops.operations_list.push_back([=](){block_array[root->x_pos + i][root->y_pos + j].inner_square_block_map[value].insert(*it2);});
                        it->second.erase((*it2));
                        break;
                    }
                }
            }
        }
    }

    return ops;
}

void assign_possible_value () {

    int i=0, j=0;
    bool changed = true, rerun_loop = true;

    while(rerun_loop) {

        changed = false;
        rerun_loop = false;

        while (i < 9) {
            while (j < 9) {

                if(block_array[i][j].value == 0) {
                    if(block_array[i][j].possible_values.size() == 1) {
                        handle_assignment_dependencies(i, j, block_array[i][j].possible_values[0]);
                        changed = true;
                        goto reloop;
                    }

                    for(auto it=block_array[i][j].row_map.begin(); it != block_array[i][j].row_map.end(); it++) {
                        if(it->second.size() == 1) {
                            handle_assignment_dependencies(i, j, it->first);
                            changed=true;
                            goto reloop;
                        }
                    }

                    for(auto it=block_array[i][j].column_map.begin(); it != block_array[i][j].column_map.end(); it++) {
                        if(it->second.size() == 1) {
                            handle_assignment_dependencies(i, j, it->first);
                            changed=true;
                            goto reloop;
                        }
                    }

                    for(auto it=block_array[i][j].inner_square_block_map.begin(); it != block_array[i][j].inner_square_block_map.end(); it++) {
                        if(it->second.size() == 1) {
                            handle_assignment_dependencies(i, j, it->first);
                            changed=true;
                            goto reloop;
                        }
                    }
reloop:
                    if(changed)
                    {
                        i=0;
                        j=0;
                        changed = false;
                        rerun_loop = true;
                        continue;
                    }
                }

                j++;
            }

            i++;
            j=0;
        }
    }
    
}

void print_sudoku_table () {
    cout << endl << endl << endl;
    for (int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            if(block_array[i][j].value == 0) {
                cout << "- ";
            } else {
                cout << block_array[i][j].value << " ";
            }
        }
        cout << endl;
    }
}

bool identify_if_sudoku_solved(assumption_struct& result) {

    for (int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            if(block_array[i][j].value == 0) {
                result.solved = false;
                result.x = i;
                result.y = j;
                
                if(block_array[i][j].possible_values.size() > 0) {
                    result.possible_assumption = block_array[i][j].possible_values;
                }
                return false;
            }
        }
    }

    return true;
}

bool assume_value_to_solve_sudoku (int number_of_assumed_values) {
    bool solved;
    assumption_struct result;
    operations ops;

    assign_possible_value();

    if(! identify_if_sudoku_solved(result)) {

        if(result.possible_assumption.size() == 0) {
            // Assumption is wrong
            return false;
        } else {
            // Can Assume further

            for (int i=0; i<result.possible_assumption.size(); i++) {

                cout << "Assuming value " << result.possible_assumption[i] << " at (" << result.x << ", " << result.y << ")" << endl;

                ops = handle_assignment_dependencies(result.x, result.y, result.possible_assumption[i]);

                solved = assume_value_to_solve_sudoku(number_of_assumed_values + 1);

                if(solved) {
                    return true;
                } else {
                    // The assigned value through assumption is incorrect. Thus, all the operations need to be revereted.
                    cout << "Assumed value of " << result.possible_assumption[i] << " at (" << result.x << ", " << result.y << ") is incorrect" << endl;
                    ops.execute();
                }
            }

            return false;
        }
    } else {
        // Sudoku solved
        return true;
    }
}

int main() {

    // Give the input sudoku here.
    char input[9][9] = {{'-', '-', '-', '1', '-', '6', '-', '-', '7'},
                        {'3', '-', '4', '-', '-', '9', '-', '-', '-'},
                        {'1', '-', '-', '-', '5', '-', '8', '-', '-'},
                        {'8', '-', '-', '4', '-', '-', '6', '-', '9'},
                        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
                        {'6', '-', '1', '-', '-', '8', '-', '-', '4'},
                        {'-', '-', '2', '-', '1', '-', '-', '-', '8'},
                        {'-', '-', '-', '3', '-', '-', '5', '-', '6'},
                        {'5', '-', '-', '6', '-', '4', '-', '-', '-'}};
    initialise(input);

    assign_possible_value();

    print_sudoku_table();

    assumption_struct result;

    if (! identify_if_sudoku_solved(result)) {
        assume_value_to_solve_sudoku(0);
    }

    print_sudoku_table();
    
    return 0;
}