#include "stdafx.h"
#include "Player.h"
using namespace std;

ostream& operator<< (ostream& o, Player p) {
	if (p.meta != nullptr) {
		for (pair<string, string> kv : *(p.meta)) {
			o << kv.first << ": " << kv.second << endl;
		}
	}
	return o;
}

Player::Player(): name(""), meta(nullptr) {}
Player::~Player() {
	delete meta;
}