#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <windows.h>
using namespace std;

class Item {
public:
    string name;
    int hp, atk, def, value;
    Item(string n = "", int h = 0, int a = 0, int d = 0, int v = 0)
        : name(n), hp(h), atk(a), def(d), value(v) {}
};

class Weapon : public Item {
public:
    Weapon() : Item("", 0, 0, 0, 0) {}
    Weapon(string n, int h, int a, int d) : Item(n, h, a, d, 0) {}
    string skill() const {
        if (name == "Sword") return "deal 2x atk";
        if (name == "Bow") return "deal 1.5x atk (reduced)";
        if (name == "Wand") return "deal 1.6x atk - 40% resist";
        if (name == "Hammer") return "deal 1.75x atk + heal 10% hp";
        return "";
    }
};

class Rune : public Item {
public:
    Rune() : Item("", 0, 0, 0, 0) {}
    Rune(string n, int h, int a, int d, int v) : Item(n, h, a, d, v) {}
};

class Player {
public:
    string name;
    int baseHp, baseAtk, baseDef;
    int hp, atk, def;
    int maxHp, sp = 3;
    int score = 0;
    Weapon weapon;
    vector<Rune> runes;

    void updateStats() {
        baseHp = 0;
        baseAtk = 0;
        baseDef = 0;
        for (auto &r : runes) {
            baseHp += r.hp;
            baseAtk += r.atk;
            baseDef += r.def;
        }
        hp = baseHp;
        atk = baseAtk;
        def = baseDef;
        maxHp = baseHp;
    }
    void scaleStats(int round) {
        if (round > 1) {
            int factor = round - 1;
            atk = baseAtk + baseAtk * factor;
            def = baseDef + baseDef * factor;
            hp = maxHp = baseHp + baseHp * factor;
        }
    }
    double power() {
        return (atk / 2.0) + (hp / 10.0) + (def / 7.0);
    }
};

class Enemy {
public:
    int hp = 1500, atk = 250, def = 150;
    int maxHp = 1500;
    int state = 0;

    void scale(int round) {
        int factor = (round - 1);
        int factorAtk = (round - 1) / 2;
        hp = maxHp = 1500 * pow(2, factor);
        atk = 250 * pow(2, factorAtk);
        def = 150 * pow(2, factor);
    }
};

struct BattleState {
    int playerHp, playerSp, enemyHp, enemyState;
};

int main(){

}
