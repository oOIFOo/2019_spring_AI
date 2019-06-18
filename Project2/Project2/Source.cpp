#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
class data_set {
public:
	float attr[4];
	int classes;
};
class node {
public:
	node *large;
	node *less;
	int test_attr;
	float threshold;
	int classes;
};
class test_data_set {
public:
	float attr[4];
	int classes;
	int record[20];
};

data_set data_in[205], training_data[200];
test_data_set validation_data[20];
int attr_list[4];

void init() {
	for (int i = 0; i < 205; i++) {
		for (int j = 0; j < 4; j++) {
			data_in[i].attr[j] = 0;
			training_data[i].attr[j] = 0;
		}
		data_in[i].classes = 0;
		training_data[i].classes = 0;
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 4; j++) validation_data[i].attr[j] = 0;
		validation_data[i].classes = 0;
	}
	for (int i = 0; i < 4; i++) attr_list[i] = 0;
}

int read_data(int num, int attr_num) {
	fstream file;
	string name;
	int i = 0;
	char buf[200];
	switch (num) 
	{
	case 0:name = "cross200.txt"; break;
	case 1:name = "ellipse100.txt"; break;
	case 2:name = "iris.data"; break;
	default:
		break;
	}

	file.open(name, ios::in);
	while (1) {
		file >> data_in[i].attr[0];
		file >> data_in[i].attr[1];
		file >> data_in[i].classes;
		i++;
		if (file.eof() != 0) break; 					  
	}
	for (int j = 0; j < attr_num; j++) attr_list[j] = 1;

	file.close();
	return i - 1;
}

void random_select(int data_num, int train_data_num) {
	int tmp, set[50];
	srand((unsigned)time(NULL));

	for (int i = 0; i < train_data_num;i++) {
		tmp = rand() % data_num;
		for (int k = 0; k < 4; k++) validation_data[i].attr[k] = data_in[tmp].attr[k];
		validation_data[i].classes = data_in[tmp].classes;
		set[i] = tmp;
	}

	tmp = 0;
	for (int i = 0; i < data_num; i++) {
		for (int j = 0; j < train_data_num; j++)
			if (i == set[j]) continue;

		training_data[tmp] = data_in[i];
		tmp++;
	}
}

void swap(data_set &a, data_set &b) {
	data_set temp = a;
	a = b;
	b = temp;
}

void sort(int start, int end, int attr_num) {
	for(int i = start; i < end;i++)
		for (int j = start; j < end - i - 1; j++)
			if (training_data[j].attr[attr_num] > training_data[j + 1].attr[attr_num])	
				swap(training_data[j], training_data[j + 1]);
}

float com_impurity(int start_point, int split_point, int end_point) {
	float a, b, Ga, Gb, G, na, nb, total;

	total = end_point - start_point;
	na = (split_point - start_point)/total; nb = (end_point - split_point) / total;
	a = 0; b = 0;
	for (int i = 0; i < split_point; i++) {
		if (training_data[i].classes == 1) a++;
		else b++;
	}
	Ga = 1 - pow((a / (a + b)), 2) - pow((b / (a + b)), 2);

	a = 0; b = 0;
	for (int i = split_point; i < end_point; i++) {
		if (training_data[i].classes == 1) a++;
		else b++;
	}
	Gb = 1 - pow((a / (a + b)), 2) - pow((b / (a + b)), 2);

	G = na*Ga + nb*Gb;
	return G;
}

int max(int a, int b, int c) {
	if (a >= b && a >= c) return a;
	if (b >= a && b >= c) return b;
	if (c >= a && c >= b) return c;
}

node* train(int start, int end) {
	int attr_num = 0;
	float impurity, min_impurity = 10;
	int split_point = 0, split_attr = 0;
	int one, two, three;
	node *new_node = new node;
	//cout << "ss" << endl;
	one = 0; two = 0; three = 0;						//check end condition
	for (attr_num = 0; attr_num < 4; attr_num++) {
		if (attr_list[attr_num] != 0) break;
		else if (attr_num == 3) {
			for (int i = 0; i < end; i++) {
				if (training_data[i].classes == 1) one++;
				else if (training_data[i].classes == 2) two++;
				else if (training_data[i].classes == 3) three++;
			}
			if (max(one, two, three) == one) new_node->classes = 1;
			else if (max(one, two, three) == two) new_node->classes = 2;
			else if (max(one, two, three) == three) new_node->classes = 3;

			return new_node;
		}
	}

	//cout << attr_num << " " << attr_list[attr_num] << endl;

	/*attr_num = rand() % 2;									//Extremely random forest
	if (attr_list[attr_num] == 0) attr_num = 1 - attr_num;*/
	for (; attr_num < 4; attr_num++) {
		if (attr_list[attr_num] != 0) {
			sort(start, end, attr_num);							//split
			for (int j = start; j < end; j++) {
				impurity = com_impurity(start, j, end);
				if (min_impurity > impurity) {
					min_impurity = impurity;
					split_attr = attr_num;
					split_point = j;
				}
			}
		}
	}

	attr_list[split_attr] = 0;							//recursive
	new_node -> threshold = (training_data[split_point].attr[split_attr] + training_data[split_point + 1].attr[split_attr]) / 2;
	new_node -> test_attr = split_attr;
	new_node -> classes = 0;
	new_node->less = new node; new_node->large = new node;
	new_node -> less = train(start, split_point);
	new_node -> large = train(split_point, end);
	attr_list[split_attr] = 1;

	return new_node;
}

void free_tree(node *tree) {
	if (tree->classes == 0) {
		free_tree(tree->large);
		free_tree(tree->less);
	}
	delete tree;
	return;
}

void test(node *tree, int times, int train_data_num) {
	int correct = 0;
	for (int i = 0; i < train_data_num ; i++) {
		node *next = tree;
		while (next->classes == 0) {
			
			if (validation_data[i].attr[next->test_attr] <= next->threshold) next = next->less;
			else next = next->large;
		}
		validation_data[i].record[times] = next->classes;
	}
	free_tree(tree);
}

void print(int train_data_num) {
	int one = 0, two = 0, three = 0;
	float result = 0, correct = 0;
	for (int i = 0; i < train_data_num; i++) {
		for (int j = 0; j < 10; j++) {
			if (validation_data[i].record[j] == 1) one++;
			else if (validation_data[i].record[j] == 2) two++;
			else if (validation_data[i].record[j] == 3) three++;
		}

		if (max(one, two, three) == one) result = 1;
		else if (max(one, two, three) == two) result = 2;
		else if (max(one, two, three) == three) result = 3;

		if (validation_data[i].classes == result) correct++;

		one = 0; two = 0; three = 0;
	}

	cout << "correct_rate = " << (correct / train_data_num)*100 << "%" << endl;
	return;
}

int main(void) {
	int data_num, file_num, total_attr_num, train_data_num;
	float correct_rate = 0;
	node *tree;

	file_num = 0;
	train_data_num = 20;                                //train_data_num

	if(file_num == 0 || file_num == 1) total_attr_num = 2;
	else total_attr_num = 3;

	init();
	data_num = read_data(file_num, total_attr_num);
	
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 10; i++) {                  //forest number
			random_select(data_num, train_data_num);
			tree = train(0, data_num - train_data_num);
			test(tree, i, train_data_num);
		}

		print(train_data_num);
		system("pause");
	}

	system("pause");
	return 0;
}