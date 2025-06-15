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
vector<Weapon> weapons = {
    {"Sword", 0, 0, 0},
    {"Bow", 0, 0, 0},
    {"Wand", 0, 0, 0},
    {"Hammer", 0, 0, 0}
};

vector<Rune> allRunes = {
    {"Titan Core", 1200, 150, 100, 3},
    {"Berserker", 800, 450, 100, 3},
    {"Iron Wall", 1000, 100, 250, 3},
    {"Balanced", 600, 150, 150, 2},
    {"Charger", 400, 250, 50, 2},
    {"Guard", 700, 100, 100, 2},
    {"Assassin", 300, 300, 30, 2},
    {"Sentinel", 750, 50, 180, 2},
    {"Hybrid", 550, 180, 80, 2},
    {"Swift", 200, 120, 30, 1},
    {"Bulwark", 300, 60, 60, 1},
    {"Ember", 150, 150, 30, 1},
    {"Core", 250, 80, 40, 1},
    {"Shield", 280, 40, 70, 1},
    {"Strike", 180, 130, 20, 1},
    {"Vigor", 220, 110, 30, 1},
    {"Sprint", 260, 90, 50, 1},
    {"Piercer", 160, 140, 25, 1},
};


int inputInt(int min, int max) {
    int val;
    while (true) {
        cin >> val;
        if (!cin.fail() && val >= min && val <= max) break;
        cout << "Invalid input. Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}


double maxPower = 0;
void knapsack(int idx, int val, vector<Rune> &curr, vector<Rune> &opt) {
    if (val > 7) return;
    double hp = 0, atk = 0, def = 0;
    for (auto &r : curr) hp += r.hp, atk += r.atk, def += r.def;
    double p = (atk / 2.0) + (hp / 10.0) + (def / 7.0);
    if (p > maxPower) {
        maxPower = p;
        opt = curr;
    }
    for (int i = idx; i < allRunes.size(); ++i) {
        curr.push_back(allRunes[i]);
        knapsack(i + 1, val + allRunes[i].value, curr, opt);
        curr.pop_back();
    }
}
