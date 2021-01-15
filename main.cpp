#include <cstring>
#include <iostream>
#include <vector>

#define NULL 0
#define SEARCHING 1
#define LEFT_WIN 2
#define RIGHT_WIN 3
#define REPEAT 4

struct Board {
	static const int base = 10;
	int a, b, c, d;
	Board(int _a, int _b, int _c, int _d) {
		a = _a % base, b = _b % base, c = _c % base, d = _d % base;
		if (!a && b) {
			int t = a;
			a = b, b = t;
		}
		if (!c && d) {
			int t = c;
			c = d, d = t;
		}
	}
	int hash() const {
		return a * base * base * base + b * base * base + c * base + d;
	}
	int who_win() {
		if (!a && !b)
			return LEFT_WIN;
		if (!c && !d)
			return RIGHT_WIN;
		return NULL;
	}
	Board add(int type, bool round) {
		switch (type) {
		case 0:
			if (a == 0 || c == 0)
				return Board(0, 0, 0, 0);
			return round ? Board(a + c, b, c, d) : Board(a, b, c + a, d);
		case 1:
			if (round && (a == 0 || d == 0) || !round && (c == 0 || b == 0))
				return Board(0, 0, 0, 0);
			return round ? Board(a + d, b, c, d) : Board(a, b, c + b, d);
		case 2:
			if (round && (b == 0 || c == 0) || !round && (d == 0 || a == 0))
				return Board(0, 0, 0, 0);
			return round ? Board(a, b + c, c, d) : Board(a, b, c, d + a);
		case 3:
			if (b == 0 || d == 0)
				return Board(0, 0, 0, 0);
			return round ? Board(a, b + d, c, d) : Board(a, b, c, d + b);
		}
	}
};

int status[Board::base*Board::base*Board::base*Board::base];

int dfs(Board board, int round) {
	if (status[board.hash()] == SEARCHING || status[board.hash()] == REPEAT)
		return REPEAT;
	if (status[board.hash()] == LEFT_WIN)
		return LEFT_WIN;
	if (status[board.hash()] == RIGHT_WIN)
		return RIGHT_WIN;
	if (status[board.hash()] == NULL)
		return NULL;
	int winner = board.who_win();
	if (winner == LEFT_WIN)
		return status[board.hash()] = LEFT_WIN;
	if (winner == RIGHT_WIN)
		return status[board.hash()] = RIGHT_WIN;
	status[board.hash()] = SEARCHING;
	std::vector<int> res;
	for (int i = 0; i < 4; ++i) {
		Board b = board.add(i, round % 2);
		if (b.hash() == 0)
			continue;
		res.push_back(dfs(b, round+1));
	}
	bool flag = true;
	int sign = REPEAT;
	for (auto i : res)
		if (i != sign)
			flag = false;
	if(flag)
		return status[board.hash()] = REPEAT; // 怎么走都会陷入重复
	sign = LEFT_WIN;
	for (auto i : res)
		if (i == sign)
			flag = true;
	if (round %2 == 1 && flag)
		return status[board.hash()] = LEFT_WIN; //先手有必胜策略
	flag = true;
	for (auto i : res)
		if (i != sign)
			flag = false;
	if (round % 2 == 0 && flag)
		return status[board.hash()] = LEFT_WIN; //后手怎么走都会让先手赢
	//以下同理，是后手必胜情况
	sign = RIGHT_WIN;
	for (auto i : res)
		if (i == sign)
			flag = true;
	if (round % 2 == 0 && flag)
		return status[board.hash()] = RIGHT_WIN;
	flag = true;
	for (auto i : res)
		if (i != sign)
			flag = false;
	if (round % 2 == 1 && flag)
		return status[board.hash()] = RIGHT_WIN;
	//在这一方只有两个选择，要么让对方赢，要么陷入重复，显然最优策略是后者
	return status[board.hash()] = REPEAT;
}
int main() {
	memset(status, -1, sizeof(status));
	std::cout << dfs(Board(1, 1, 1, 1), 1);
	return 0;
}