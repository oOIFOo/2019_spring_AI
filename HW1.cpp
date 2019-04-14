#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class puzzle_info {
public:
	int start_x;
	int start_y;
	int length;
	int direction;
};

class node {
public:
	int puzzle_index;
	string word;
};

class binary_constraint {
public:
	int puzzle1;
	int puzzle2;
	int index1;
	int index2;
	int ban_index;
	char ban_list[5];
};

puzzle_info p_info[50];
node stack[10000];
string word_info[3000];
binary_constraint bin[20];
int puzzle_count;
int bin_count;
int board[100][100];
int index[21];
int top, x, y;
int visited_node;

void init() {
	visited_node = 0;
	top = -1;
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			board[i][j] = 0;

	for (int i = 0; i < 20; i++) {
		bin[i].ban_index = 0;
		for (int j = 0; j < 5; j++)
			bin[i].ban_list[j] = 0;
	}
}

int read_puzzle(int num) {
	fstream file;
	int i;
	char buf[200];
	file.open("puzzle.txt", ios::in);
	for (int i = 0; i < num;i++) file.getline(buf, sizeof(buf));
    
	for (i = 0; buf[i] != 0; i = i + 10) {
		p_info[i / 10].start_x = atoi(&buf[i]);
		p_info[i / 10].start_y = atoi(&buf[i+2]);
		p_info[i / 10].length  = atoi(&buf[i+4]);
		if(buf[i+6] == 'A') p_info[i / 10].direction = 0;
		else p_info[i / 10].direction = 1;
	}

	file.close();

	for (i = 0; p_info[i].length != 0; i++) {}

	return i;
}

void read_word() {
	fstream file;
	string buf;
	file.open("English words 3000.txt", ios::in);
	for (int i = 0; i < 3000; i++) {
		getline(file, buf);
		word_info[i] = buf;
	}
}

void count_sort() {
	string word_info_sorted[3000];
	int index_tmp[21];
	for (int i = 0; i <= 20; i++) index[i] = 0;

	for (int i = 0; i < 3000; i++) {
		switch (word_info[i].size())
		{
		case 1 : index[ 1]++; break;
		case 2 : index[ 2]++; break;
		case 3 : index[ 3]++; break;
		case 4 : index[ 4]++; break;
		case 5 : index[ 5]++; break;
		case 6 : index[ 6]++; break;
		case 7 : index[ 7]++; break;
		case 8 : index[ 8]++; break;
		case 9 : index[ 9]++; break;
		case 10: index[10]++; break;
		case 11: index[11]++; break;
		case 12: index[12]++; break;
		case 13: index[13]++; break;
		case 14: index[14]++; break;
		case 15: index[15]++; break;
		case 16: index[16]++; break;
		case 17: index[17]++; break;
		case 18: index[18]++; break;
		case 19: index[19]++; break;
		case 20: index[20]++; break;
		default:
			break;
		}
	}
	for (int i = 2; i <= 20; i++) {
		index[i] = index[i] + index[i - 1];
	}
	for (int i = 20; i >= 0; i--) {
		index[i] = index[i - 1];
	}

	index[1] = 0;

	for (int i = 1; i <= 20; i++) {
		index_tmp[i] = index[i];
	}


	for (int i = 0; i < 3000; i++) {
		word_info_sorted[index_tmp[word_info[i].size()]] = word_info[i];
		index_tmp[word_info[i].size()]++;
	}

	for (int i = 0; i < 3000; i++) word_info[i] = word_info_sorted[i];
}

int com_binary_constraint() {
	int x, y, tmp, count;
	count = 0;

	for (int i = 0; p_info[i].length != 0; i++) {
		x = p_info[i].start_x;
		y = p_info[i].start_y;

		if (p_info[i].direction == 0) {
			for (int j = 0; j < p_info[i].length; j++) {
				if (board[x + j][y] == 0) {
					board[x + j][y] = i * 100 + j;
				}
				else {
					tmp = board[x + j][y];										 //tmp / 100 = puzzle_index, tmp % 100 = word_index
					for (int w = 0; w < count; w++) {
						if ((bin[count].puzzle1 == i) && (bin[count].puzzle2 == (tmp / 100))) continue;
					}
					bin[count].puzzle1 = tmp / 100;
					bin[count].index1 = tmp % 100;
					bin[count].puzzle2 = i;
					bin[count].index2 = j;
					count++;
				}
			}
		}
		else {
			for (int j = 0; j < p_info[i].length; j++) {
				if (board[x][y + j] == 0) {
					board[x][y + j] = i * 100 + j;
				}
				else {
					tmp = board[x][y + j];
					for (int w = 0; w < count; w++) {
						if (bin[count].puzzle1 == i && bin[count].puzzle2 == tmp / 100) continue;
					}

					bin[count].puzzle1 = tmp / 100;
					bin[count].index1 = tmp % 100;
					bin[count].puzzle2 = i;
					bin[count].index2 = j;
					count++;
				}
			}
		}
	}

	init();
	return count;
}

string select_word(int word_num) {
	string word = word_info[word_num];
	return word;
}

int check_board(int puzzle_num, int word_num) {
	puzzle_info puzzle = p_info[puzzle_num];
	string word = select_word(word_num);

	x = puzzle.start_x;
	y = puzzle.start_y;

	if (puzzle.direction == 0) {
		for (int i = 0; i < puzzle.length; i++) {
			if ((board[x + i][y] == 0) || (board[x + i][y] >= puzzle_num * 1000) || (board[x + i][y] % 1000 == word[i])) {}
			else return 0;
		}
	}
	else {
		for (int i = 0; i < puzzle.length; i++) {
			if ((board[x][y + i] == 0) || (board[x][y + i] >= puzzle_num * 1000) || (board[x][y + i] % 1000 == word[i])) {}
			else return 0;

		}
	}
	return 1;
}

void update_board(int puzzle_num, string word) {
	puzzle_info puzzle = p_info[puzzle_num];

	x = puzzle.start_x;
	y = puzzle.start_y;
	//cout << word << endl;
	if (puzzle.direction == 0) {
		for (int i = 0; i < puzzle.length; i++) {
			if(board[x + i][y] % 1000 != word[i]) board[x + i][y] = puzzle_num * 1000 + word[i];
		}
	}
	else {
		for (int i = 0; i < puzzle.length; i++) {
			if (board[x][y + i] % 1000 != word[i]) board[x][y + i] = puzzle_num * 1000 + word[i];
		}
	}
}

void forward_info(node current_node) {
	int puzzle_num = current_node.puzzle_index;
	string word = current_node.word;

	for (int i = 0; i < bin_count; i++) {
		cout << i << ' ' << bin[i].ban_index << ' ' << bin[i].index1 << endl;
		if (bin[i].puzzle1 == puzzle_num) {
			for (int j = 0; j < bin[i].ban_index; j++)
				if (bin[i].ban_list[j] == word[bin[i].index1])
					return;

			bin[i].ban_list[bin[i].ban_index] = word[bin[i].index1];
			bin[i].ban_index++;
		}
		else if (bin[i].puzzle2 == puzzle_num) {
			for (int j = 0; j < bin[i].ban_index; j++)
				if (bin[i].ban_list[j] == word[bin[i].index1])
					return;

			bin[i].ban_list[bin[i].ban_index] = word[bin[i].index2];
			bin[i].ban_index++;
		}
	}
}

int check_forward(int puzzle_num, int word_num) {
	puzzle_info puzzle = p_info[puzzle_num];
	string word = select_word(word_num);

	for (int i = 0; i < bin_count; i++) {
		if (bin[i].puzzle1 == puzzle_num) {
			for (int j = 0; j < bin[i].ban_index; j++) {
				if (word[bin[i].index1] == bin[i].ban_list[j])
					return 0;
			}
		}
		else if (bin[i].puzzle2 == puzzle_num) {
			for (int j = 0; j < bin[i].ban_index; j++) {
				if (word[bin[i].index2] == bin[i].ban_list[j])
					return 0;
			}
		}
	}
	return 1;
}

node pop() {
	node current_node;
	current_node = stack[top];
	top--;

	//forward_info(current_node);
	update_board(current_node.puzzle_index, current_node.word);
	return current_node;
}

void push(node current_node, int word_num) {
	node tmp = current_node;
	string word = select_word(word_num);

	top++;
	stack[top].puzzle_index = current_node.puzzle_index + 1;
	stack[top].word = word;
}

void push_child(node current_node) {
	puzzle_info tmp = p_info[current_node.puzzle_index + 1];
	if (tmp.length != 0) {
		for (int i = index[tmp.length]; i < index[tmp.length + 1]; i++) {   //push all childs
			if (check_board(current_node.puzzle_index + 1, i)) {
				push(current_node, i);
			}
		}
	}
}

void print_solution() {
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 25; i++) {
			if (board[i][j] != 0) cout << char(board[i][j] % 1000);
			else cout << ' ';
		}
		cout << endl;
	}
}

int match() {
	int sol = 0;
	node current_node;

	current_node.puzzle_index = -1;
	push_child(current_node);

	while (1) {
		current_node = pop();
		if (current_node.puzzle_index == puzzle_count - 1) {
			print_solution();
			sol++;
			break;
		}
		else if (top <= 0) {
			break;
		}
		else {
			push_child(current_node);
		}
		visited_node++;
		//cout << current_node.puzzle_index << ' ' << top << endl;
	}
	return sol;
}

int main(void){
	int sol;
	puzzle_count = read_puzzle(4);
	read_word();
	count_sort();
	init();
	bin_count = com_binary_constraint() + 1;
	
	sol = match();
	if (sol) {
		cout << "solution found!!" << endl << endl;
		cout << "visited node:" << visited_node << endl;
		//cout << sol << endl;
	}
	else {
		cout << "no solution found!!" << endl;
	}

	system("pause");
	return 0;
}
