#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")

#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#define N 16

using namespace std;
vector<vector<int>> grid;
bool sudoku_solvable = true;
vector<int> numbers= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,16};
vector<int> blank = {0,0,0,0,0,0,0,0,0};

vector<vector<int>> valid_row_elements;
vector<vector<int>> valid_coloumn_elements;
vector<vector<int>> valid_block_elements;

stack<vector<int>, vector<vector<int>> > stack_of_moves;

int ToInt(char c){
	if (c <58){
		return int(c)-48;
	}
	else{
		return int(c)-65+10;
	}
}

char ToChar(int i){
	if (i<10){
		return i+'0';
	}
	else{
		return i-10+'A';
	}
}

void print_vector(vector<vector<int>> vector){
	for(int i = 0; i<vector.size(); i++){
		for(int j = 0; j<vector[i].size(); j++){
			cout << vector[i][j];
			if(j!=vector[i].size()-1){
			cout << ' ';
			}
		}
		cout << endl;
	}
}

void print_grid(){
	char c;
	for (int I = 0; I < N; I++) {
        for (int J = 0; J < N; J++){
            c = ToChar(grid[I][J]);
            cout << c;
            if(J != 15){
            	cout << ' ';
            }
        }
        cout << endl;
	}
}

int get_block_number(int row, int col){
	if(row < 4){
		if(col <4){
			return 0;
		}
		else if(col < 8){
			return 1;
		}
		else if(col < 12){
			return 2;
		}
		else{
			return 3;
		}
	}
	else if(row < 8){
		if(col <4){
			return 4;
		}
		else if(col < 8){
			return 5;
		}
		else if(col < 12){
			return 6;
		}
		else{
			return 7;
		}	
	}
	else if(row < 12){
		if(col <4){
			return 8;
		}
		else if(col < 8){
			return 9;
		}
		else if(col < 12){
			return 10;
		}
		else{
			return 11;
		}	
	}
	else{
		if(col <4){
			return 12;
		}
		else if(col < 8){
			return 13;
		}
		else if(col < 12){
			return 14;
		}
		else{
			return 15;
		}
	}
}

void add_back(int I, int J, int n){ //parameters inclde coordinates and the NUMBER
	if(n!=0){
		if(valid_row_elements[I][n-1]==0){
			valid_row_elements[I][n-1]=n;
			valid_row_elements[I][N]=valid_row_elements[I][N]+1;
		}
		if(valid_coloumn_elements[J][n-1]==0){
			valid_coloumn_elements[J][n-1]=n;
			valid_coloumn_elements[J][N]=valid_coloumn_elements[J][N]+1;
		}
		int block_num = get_block_number(I,J);
		if(valid_block_elements[block_num][n-1]==0){
			valid_block_elements[block_num][n-1]=n;
			valid_block_elements[block_num][N]=valid_block_elements[block_num][N]+1; //add 1 to the counter for each row 
		}
	}
}

void used_value(int I, int J, int n){ //parameters inclde coordinates and the NUMBER
	if(n!=0){
		if(valid_row_elements[I][n-1]!=0){
			valid_row_elements[I][n-1]=0;
			valid_row_elements[I][N]=valid_row_elements[I][N]-1;
		}
		if(valid_coloumn_elements[J][n-1]!=0){
			valid_coloumn_elements[J][n-1]=0;
			valid_coloumn_elements[J][N]=valid_coloumn_elements[J][N]-1;
		}
		int block_num = get_block_number(I,J);
		if(valid_block_elements[block_num][n-1]!=0){
			valid_block_elements[block_num][n-1]=0;
			valid_block_elements[block_num][N]=valid_block_elements[block_num][N]-1; //subtract 1 from the counter for each row 
		}
	}
}

bool num_is_valid(int i, int j, int num){//checks if a number is valid based on its position
	if(num == 0){
		return false;
	}
	if(valid_row_elements[i][num-1]!=0){
		if(valid_coloumn_elements[j][num-1]!=0){
			if(valid_block_elements[get_block_number(i,j)][num-1]!=0){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
	
}

void fill_in_matrices(){
	for(int n = 0; n<N; n++){
		valid_row_elements.push_back(numbers);
		valid_coloumn_elements.push_back(numbers);
		valid_block_elements.push_back(numbers);
			
	}
	char c;
	int n;
    for (int I = 0; I < N; I++) {
		vector<int> v1;
        for (int J = 0; J < N; J++) { 
			cin >> c;
			n = ToInt(c);
			v1.push_back(n);
			used_value(I, J, n);
        }
        grid.push_back(v1);
    }	
}

vector<int> get_best_loop(int i,int j){
	vector<int> best_loop;
	if(valid_row_elements[i][N]>valid_coloumn_elements[j][N]){
		best_loop = valid_coloumn_elements[j];
	}
	else{
		best_loop = valid_row_elements[i];	
	}
	int block_number = get_block_number(i,j);
	if(best_loop[N]>valid_block_elements[block_number][N]){
		best_loop = valid_block_elements[block_number];
	}
	return best_loop;
}

void possible_hidden_singles(int i, int j){
	for(int loop_var = 0; loop_var<N; loop_var++){
		if(grid[i][loop_var]==0){
			vector<int> values;
			vector<int> best_loop = get_best_loop(i,loop_var);
			for(int n=0; n<N; n++){
				if(num_is_valid(i,loop_var,best_loop[n])==true){
					values.push_back(best_loop[n]);
					
				}
				if(values.size()>1){
					break;	
				}
			}
			if(values.size()==1){
				grid[i][loop_var] = values[0];
				used_value(i,loop_var,values[0]);
				possible_hidden_singles(i,loop_var);
				stack_of_moves.push({i,loop_var,values[0],1});
			}
		}
		if(grid[loop_var][j]==0){
			vector<int> values;
			vector<int> best_loop = get_best_loop(loop_var, j);
			for(int n=0; n<N; n++){
				if(num_is_valid(loop_var, j ,best_loop[n])==true){
					values.push_back(best_loop[n]);
					
				}
				if(values.size()>1){
					break;	
				}
			}
			if(values.size()==1){
				grid[loop_var][j] = values[0];
				used_value(loop_var,j,values[0]);
				possible_hidden_singles(loop_var,j);
				stack_of_moves.push({loop_var, j, values[0],1});
			}
		}
	}
	int startBlock_row = i - i%4;
	int startBlock_coloumn = j - j%4;
	for(int r = 0; r<4; r++){
		for(int c = 0; c<4; c++ ){
			if(grid[r+startBlock_row][c+startBlock_coloumn]==0){
				vector<int> values;
				vector<int> best_loop = get_best_loop(r+startBlock_row, c+startBlock_coloumn);
				for(int n=0; n<N; n++){
					if(num_is_valid(r+startBlock_row, c+startBlock_coloumn ,best_loop[n])==true){
						values.push_back(best_loop[n]);	
					}
					if(values.size()>1){
						break;	
					}
				}
				if(values.size()==1){
					grid[r+startBlock_row][c+startBlock_coloumn] = values[0];
					used_value(r+startBlock_row,c+startBlock_coloumn,values[0]);
					possible_hidden_singles(r+startBlock_row,c+startBlock_coloumn);
					stack_of_moves.push({r+startBlock_row,c+startBlock_coloumn, values[0],1});
				}
			}
		}
	}
}

void hidden_singles_recursion(int i,int j){

	for(int loop_var = 0; loop_var<N; loop_var++){
		if(grid[i][loop_var]==0){
			vector<int> values;
			vector<int> best_loop = get_best_loop(i,loop_var);
			for(int n=0; n<N; n++){
				if(num_is_valid(i,loop_var,best_loop[n])==true){
					values.push_back(best_loop[n]);
					
				}
				if(values.size()>1){
					break;	
				}
			}
			if(values.size()==1){
				grid[i][loop_var] = values[0];
				used_value(i,loop_var,values[0]);
				hidden_singles_recursion(i,loop_var);
			}
		}
	}
	for(int loop_var = 0; loop_var<N; loop_var++){
		if(grid[loop_var][j]==0){
			vector<int> values;
			vector<int> best_loop = get_best_loop(loop_var, j);
			for(int n=0; n<N; n++){
				if(num_is_valid(loop_var, j ,best_loop[n])==true){
					values.push_back(best_loop[n]);
					
				}
				if(values.size()>1){
					break;	
				}
			}
			if(values.size()==1){
				grid[loop_var][j] = values[0];
				used_value(loop_var,j,values[0]);
				hidden_singles_recursion(loop_var,j);
			}
		}
	}
	int startBlock_row = i - i%4;
	int startBlock_coloumn = j - j%4;
	for(int r = 0; r<4; r++){
		for(int c = 0; c<4; c++ ){
			if(grid[r+startBlock_row][c+startBlock_coloumn]==0){
				vector<int> values;
				vector<int> best_loop = get_best_loop(r+startBlock_row, c+startBlock_coloumn);
				for(int n=0; n<N; n++){
					if(num_is_valid(r+startBlock_row, c+startBlock_coloumn ,best_loop[n])==true){
						values.push_back(best_loop[n]);	
					}
					if(values.size()>1){
						break;	
					}
				}
				if(values.size()==1){
					grid[r+startBlock_row][c+startBlock_coloumn] = values[0];
					used_value(r+startBlock_row,c+startBlock_coloumn,values[0]);
					hidden_singles_recursion(r+startBlock_row,c+startBlock_coloumn);
				}
			}
		}
	}
	
}

void fill_hidden_singles(){
	for(int i = 0; i<N; i++){
		for(int j = 0; j<N; j++){
			if(grid[i][j]==0){
				vector<int> values;
				vector<int> best_loop = get_best_loop(i,j);
				for(int n=0; n<N; n++){
					if(num_is_valid(i,j,best_loop[n])==true){
						values.push_back(best_loop[n]);
						
					}
					if(values.size()>1){
						break;	
					}
				}
				if(values.size()==1){
					grid[i][j] = values[0];
					used_value(i,j,values[0]);
					hidden_singles_recursion(i,j);
				}
			}
		}
	}
}

void SOLVE(){
	int n=0;
	int i=0;
	int j=0;
	bool valid_character_found;
	while(i<N){
		while(j<N && sudoku_solvable == true){
			if(grid[i][j]==0){
				valid_character_found = false;
				vector<int> best_loop = get_best_loop(i,j);
				while(n<N){
					if(num_is_valid(i,j,best_loop[n])==true){
						grid[i][j]=best_loop[n];
						used_value(i,j,best_loop[n]);
						stack_of_moves.push({i,j,best_loop[n],0}); 
						valid_character_found=true;
						possible_hidden_singles(i,j);	
						break;
					}
					n++;
				}
				n=0;
				if(valid_character_found == false){
					if(stack_of_moves.empty()!=true){
						
						vector<int> temp;
						while(stack_of_moves.top()[3]==1){
							
							temp = stack_of_moves.top();
							grid[temp[0]][temp[1]]=0;
							add_back(temp[0],temp[1], temp[2]);
							stack_of_moves.pop();
							
						}
						
						temp = stack_of_moves.top();
						grid[temp[0]][temp[1]]=0;
						stack_of_moves.pop();
						add_back(temp[0], temp[1], temp[2]);
						
						j = temp[1]-1;
						i = temp[0];
						if(temp[1]==0){
							if(i>0){
								i = temp[0]-1;
								j = 14;
							}
						}
						n = temp[2];
					}
					else{
						sudoku_solvable = false;
					}
				}
			}
			j++;
		}
		j = 0;
		i++;
	}
}

int main(){
    ios_base::sync_with_stdio(false); //for fast inputs
    cin.tie(NULL);
    cout.tie(NULL);
	
	fill_in_matrices();
	fill_hidden_singles();
	SOLVE();
	if(sudoku_solvable==false){
		cout << "No Solution" << endl;
	}
	else{
		print_grid();	
	}
	
}
/*
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0

0 0 0 0 7 0 0 0 0 0 0 0 0 E 0 2
7 0 B 0 0 0 0 0 A 0 2 D 0 F 0 5
1 0 G E 3 B 2 0 5 0 0 0 0 0 0 D
0 2 0 5 0 0 0 0 0 0 7 0 0 8 B 0
2 0 0 0 0 0 G 0 0 6 0 0 0 0 0 0
0 B 1 C E 0 0 0 8 0 5 7 0 A 0 0
F 0 0 G 0 0 0 0 1 0 9 2 0 0 6 4
0 0 5 0 2 0 0 7 0 0 0 4 8 1 0 B
0 0 4 0 0 0 0 8 0 A 0 B C 0 0 1
0 0 8 6 0 E 0 3 D 0 0 0 0 9 0 0
0 0 D F B G 5 4 0 0 3 0 0 0 0 A
0 G 0 9 C 0 0 0 0 0 0 6 B 0 8 0
0 0 0 D 6 0 0 0 0 1 F G 0 0 7 0
E 0 0 A 0 C B 0 9 0 0 0 0 5 0 0
0 0 3 0 5 0 0 0 0 B 0 0 D 0 E 0
0 0 0 0 0 1 0 G 3 E C 0 A 0 0 0

0 6 0 0 0 0 0 8 B 0 0 F E 0 0 G
F B 0 0 0 G E 0 0 0 C 0 0 6 0 0
D 0 9 C 0 0 0 0 3 G E 0 F B A 0
2 0 G 0 B 0 F A 1 0 0 0 0 0 0 0
0 F B A 0 0 G 2 D 8 9 C 0 0 0 0 
C D 0 0 4 1 5 6 2 3 0 0 0 0 B A
5 0 6 1 C 0 9 0 F B A 7 G 0 0 3
0 2 0 0 0 A 0 B 6 0 5 0 0 D 0 9
A 7 F B G 0 0 0 C D 0 0 0 0 0 6
9 0 0 0 0 0 1 0 0 2 0 G A 0 0 B
1 0 4 6 9 D 0 0 7 0 B 0 3 G 0 0
G E 0 0 7 0 A F 4 6 1 0 0 0 D 8
B A 0 F 0 0 0 G 9 C D 0 0 1 5 4 
0 0 C 0 1 4 6 0 G 0 0 0 B A 0 0
0 0 5 0 8 C D 0 A 0 0 B 2 0 0 E
3 G 0 0 A 0 0 7 0 0 6 0 0 0 C 0














*/
