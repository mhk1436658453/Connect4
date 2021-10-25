#include "constant.h"
#include <string>
using namespace constants;

// every AI is given board state 'b' as an input to return a column number

// random AI that choose a random column
int random_ai(char b[b_h][b_w], char chess) { 

	int r = rand() % 7 + 1;
	while(b[0][r - 1] != empty_chess){		// make sure the it does not drop a chess into a column that is already full
		r = rand() % 7 + 1;
	}
	return r; 
};

// check possible connections, it returns the number of possible connections on the board
// n_exist is the number of chess that existed on the board
// for example X_X_ is a possible connection with n_exist = 2, X_XX is a possible connection with n_exist = 3
int checkPosCon(char b[b_h][b_w], char chess, int n_exist) {

	int possibleConnections{ 0 };	// possible 4 connections, to be returned at the end of function
	
	// horizontal check
	for (int y = 0; y < b_h; y++) {
		for (int x = 0; x <= b_w - 4; x++) {
			int empty{ 0 };
			int exist{ 0 };
			for (int i = 0; i < 4; i++) {
				char temp = b[y][x + i];
				if (temp == chess) { exist++; }
				if (temp == empty_chess) { empty++; }
			}
			if (((exist + empty) == 4) && (exist == n_exist)) { possibleConnections++; }
		}

	}
	
	// vertical check
	// Same code as above, but changed axis
	for (int x = 0; x < b_w; x++) {
		for (int y = 0; y <= b_h - 4; y++) {
			int empty{ 0 };
			int exist{ 0 };
			for (int i = 0; i < 4; i++) {
				char temp = b[y + i][x];
				if (temp == chess) { exist++; }
				if (temp == empty_chess) { empty++; }
			}
			if (((exist + empty) == 4) && (exist == n_exist)) { possibleConnections++; }
		}

	}
	
	// diagonal check
	// going down and right
	for (int x = 0; x <= b_w - 4; x++) {
		for (int y = 0; y <= b_h - 4; y++) {
			int empty{ 0 };
			int exist{ 0 };
			for (int i = 0; i < 4; i++) {
				char temp = b[y + i][x + i];
				if (temp == chess) { exist++; }
				if (temp == empty_chess) { empty++; }
			}
			if (((exist + empty) == 4) && (exist == n_exist)) { possibleConnections++; }
		}

	}
	// going up and right
	for (int x = 0; x <= b_w - 4; x++) {
		for (int y = b_h - 4; y < b_h; y++) {
			int empty{ 0 };
			int exist{ 0 };
			for (int i = 0; i < 4; i++) {
				char temp = b[y - i][x + i];
				if (temp == chess) { exist++; }
				if (temp == empty_chess) { empty++; }
			}
			if (((exist + empty) == 4) && (exist == n_exist)) { possibleConnections++; }
		}

	}




	return possibleConnections;
};

// use 'threat' as an evaluation function
// this function reutrn threat levels of the player
// here threat is a summation of possible connections on the board
int board_value(char b[b_h][b_w], char chess) {
	// define 4 connect as a very large value
	// the rest use a smaller coef
	int threats = 0;
	threats += checkPosCon(b, chess, 1) + 10*checkPosCon(b, chess, 2) + 100* checkPosCon(b, chess, 3) + 10000 * checkPosCon(b, chess, 4);
	return threats;
};

void printintvec(vector<int> const input)
{
	for (int i = 0; i < input.size(); i++) {
		cout << input.at(i) << ' ';
	}
}

class tree {
public:
	vector <int> move_seq;			// contain the sequence of move
	int action_value;				
	tree* parent_ptr;
	vector <tree*> children_ptr;
	char state[b_h][b_w];			// contains the board state
	tree()
		:	action_value{ 0 },
			parent_ptr{ NULL }, 

			state{ NULL }
	{}
	// for debugging pruning
	
	//~tree() { 
	//	cout << "node ";
	//	printintvec(move_seq);
	//	cout << " is detroyed" << "\n";
	//}
	
};

void expand(tree* t, int depth, int br_factor) {
	for (int move_col = 0; move_col < br_factor; move_col++) {
		tree* new_node = new tree;
		new_node->move_seq = t->move_seq;
		new_node->move_seq.push_back(move_col);
		new_node->parent_ptr = t;
		t->children_ptr.push_back(new_node);
		if (depth > 1) { expand(new_node, depth -1, br_factor); };
	}
};

void pruneTree(tree* t) {
	bool have_child = !(t->children_ptr.empty());
	if (have_child) {
		for (int i = 0; i < t->children_ptr.size(); i++) {
			pruneTree(t->children_ptr[i]);
		}
	}
	delete t;
};


void newBoardState(char (&b)[b_h][b_w], int col, char chess) {
	// check the first empty space in the column
	for (int i = b_h - 1; i >= 0; i--) {
		if (b[i][col] == empty_chess) {
			b[i][col] = chess;
			break;
		}
	}
}

// prune tree that contains invalid moveset state
// also set board state for each node
void pruneInvalid(tree* t, char myChess, char enemyChess, int curr_depth = 0) {
	bool have_child = !(t->children_ptr.empty());
	if (have_child) {
		for (int i = 0; i < t->children_ptr.size(); i++) {
			int nextMove = t->children_ptr[i]->move_seq.back();
			bool moveIsValid = (t->state[0][nextMove] == empty_chess);
			if (moveIsValid) {
				memcpy(t->children_ptr[i]->state, t->state, sizeof(char) * b_h * b_w);
				char nextChess;
				char nextNextChess;
				if (curr_depth % 2 == 0) {
					nextChess = myChess ;
					nextNextChess = enemyChess ;
				}
				else {
					nextChess = enemyChess;
					nextNextChess = myChess;
				}

				newBoardState(t->children_ptr[i]->state, nextMove, nextChess);
				pruneInvalid(t->children_ptr[i], myChess, nextNextChess, curr_depth + 1);
			}else { 
				pruneTree(t->children_ptr[i]); 
				t->children_ptr.erase(t->children_ptr.begin()+i);
			}

		}
	}
}

int minimax(tree* t, int depth, char myChess, char enemyChess) {
	bool have_child = !(t->children_ptr.empty());
	if (have_child)	{
		bool maximizePlayer = depth % 2 == 0;
		if (maximizePlayer) {
			t->action_value = -INT_MIN;
			//get
			for (int i = 0; i < t->children_ptr.size(); i++) {
				int val = minimax(t->children_ptr[i], depth - 1, myChess, enemyChess);
				if (val >= t->action_value) { t->action_value = val; }
			}

		}
		else { // minimize player
			t->action_value = INT_MAX;
			for (int i = 0; i < t->children_ptr.size(); i++) {
				int val = minimax(t->children_ptr[i], depth - 1, myChess, enemyChess);
				if (val <= t->action_value) { t->action_value = val; }
			}

		}
	}else {
		// get action value
		t->action_value = board_value(t->state, myChess) - board_value(t->state, enemyChess);
	}


	return t->action_value;
}

int minimax_ai(char b[b_h][b_w], char myChess, char enemyChess) {
	char b_state[b_h][b_w];		// new variable, i want to make sure we dont't change b
	memcpy(b_state, b, sizeof(b));
	tree* root = new tree;
	memcpy(root->state, b, sizeof(char) * b_h * b_w);
	root->parent_ptr = NULL;
	int depth{ 4 };			// tree depth
	int width{ 7 };			// width of each level
	// expand tree
	expand(root, depth, width);
	// prune tree that include invalid moves and set board state at each ndoe
	pruneInvalid(root, myChess, enemyChess);
	// minimax algorithm
	minimax(root, depth, myChess, enemyChess);

	int bestMove{};
	// find the node with max value in the next layer
	for (int i = 0; i < root->children_ptr.size(); i++) {
		int moveVal = root->children_ptr[i]->action_value;
		if (moveVal >= root->action_value) {
			bestMove = root->children_ptr[i]->move_seq.back();
		}
		
	}

	return bestMove;
};
