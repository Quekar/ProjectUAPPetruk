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
bool isRuneUsed(const vector<Rune>& runes, const Rune& r) {
    for (const auto& rune : runes) {
        if (rune.name == r.name) return true;
    }
    return false;
}

void build(Player &p) {
    system("cls");
    cout << "===== Type Your Name =====\n";
    cout << "Name: ";
    getline(cin >> ws, p.name);
    
    vector<Rune> opt, curr;
    maxPower = 0;
    knapsack(0, 0, curr, opt);

    int val = 0;
    while (true) {
        system("cls");
        cout << "Choose Your Equipment\n";
        cout << "1. Weapon : " << (p.weapon.name.empty() ? "empty" : p.weapon.name);
        if (!p.weapon.name.empty()) cout << " [" << p.weapon.skill() << "]";
        cout << "\n";
        for (int i = 0; i < 4; ++i) {
            cout << i + 2 << ". Rune " << i + 1 << ": ";
            if (i < p.runes.size()) cout << p.runes[i].name << " [" << p.runes[i].hp << " hp, " << p.runes[i].atk << " atk, " << p.runes[i].def << " def, " << p.runes[i].value << "]" << "\n";
            else cout << "empty\n";
        }
        cout << "Rune Value (" << val << "/7)\n";
        p.updateStats();
        cout << "hp : " << p.baseHp << "   atk : " << p.baseAtk << "   def : " << p.baseDef << "\n";
        cout << "6. Start                   Power = " << (int)p.power() << "/" << (int)maxPower << "\n";

        int choice = inputInt(1, 6);
        if (choice == 1) {
            system("cls");
            cout << "Choose Your Weapon:\n";
            for (int i = 0; i < weapons.size(); ++i)
                cout << i + 1 << ". " << weapons[i].name << " [" << weapons[i].skill() << "]" << "\n";
            int w = inputInt(1, weapons.size());
            p.weapon = weapons[w - 1];
        } else if (choice >= 2 && choice <= 5) {
            system("cls");
            cout << "Choose a Rune:\n";
            for (int i = 0; i < allRunes.size(); ++i)
                cout << i + 1 << ". " << allRunes[i].name << " [" << allRunes[i].hp  << " hp, "<< allRunes[i].atk << " atk, " << allRunes[i].def << " def, " << allRunes[i].value << "]\n";
            int r = inputInt(1, allRunes.size());
            Rune selected = allRunes[r - 1];
            if (!isRuneUsed(p.runes, selected)) {
                int newVal = val;
                if (choice - 2 < p.runes.size()) newVal -= p.runes[choice - 2].value;
                if (newVal + selected.value <= 7) {
                    if (choice - 2 < p.runes.size()) {
                        val -= p.runes[choice - 2].value;
                        p.runes[choice - 2] = selected;
                        val += selected.value;
                    } else {
                        p.runes.push_back(selected);
                        val += selected.value;
                    }
                }
            }
        } else if (choice == 6 && !p.weapon.name.empty()) {
            break;
        }
    }

    p.updateStats();
    double percent = (p.power() / maxPower) * 100.0;
    cout << "Your build is " << (int)percent << "% optimal\n";
    cout << "Press enter to continue...";
    cin.ignore(); cin.get();
}

void saveScore(const string &name, int score) {
    ofstream out("leaderboard.txt", ios::app);
    out << name << " " << score << endl;
    out.close();
}

void merge(vector<pair<string, int>> &arr, int l, int m, int r) {
    vector<pair<string, int>> left(arr.begin() + l, arr.begin() + m + 1);
    vector<pair<string, int>> right(arr.begin() + m + 1, arr.begin() + r + 1);
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size()) {
        if (left[i].second > right[j].second)
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void mergeSort(vector<pair<string, int>> &arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void showLeaderboard() {
    system("cls");
    ifstream in("leaderboard.txt");
    vector<pair<string, int>> scores;
    string name;
    int score;
    while (in >> name >> score) scores.emplace_back(name, score);
    in.close();

    if (!scores.empty())
        mergeSort(scores, 0, scores.size() - 1);

    cout << "===== LEADERBOARD ====\n";
    for (auto &s : scores)
        cout << s.first << " : " << s.second << endl;
    cout << "Press enter to return...";
    cin.ignore(); cin.get();
}

void pause(){
    Sleep(3000);
    system("cls");
}

void battle(Player &p) {
    queue<string> actionLog;
    int round = 1;
    system("cls");
    while (true) {
        Enemy e;
        e.scale(round);
        p.scaleStats(round);
        p.sp = 3;
        stack<BattleState> history;
        int undoCount = 2;

        while (p.hp > 0 && e.hp > 0) {
            system("cls");
            cout << "=== Round " << round << " ===\n";
            cout << "=== Enemy hp: " << e.hp << "  atk: " << e.atk << "  def: " << e.def << " === \n";
            cout << "=== Player hp: " << p.hp << "  atk: " << p.atk << "  def: " << p.def << " ===";
            cout << "=== Sp: " << p.sp << " === Undo Remaining: " << undoCount << " ===\n";
            cout << "[1] Basic Attack\n[2] Skill Attack\n[3] Heal (30% HP)\n[4] Undo\n[5] Retreat\n[6] View Action Log\n";

            // Save the current state before any action
            history.push({p.hp, p.sp, e.hp, e.state});

            int act = inputInt(1, 6);

            if (act == 1) {
                int dmg = max(0, p.atk - e.def / 10);
                e.hp -= dmg;
                if (p.sp < 5) p.sp++;
                cout << "\nYou used Basic Attack and dealt " << dmg << " damage!\n";
                actionLog.push("Round " + to_string(round) + ": Player used Basic Attack and dealt " + to_string(dmg) + " damage.");
                Sleep(2000);
            } else if (act == 2 && p.sp > 0) {
                int dmg = 0;
                if (p.weapon.name == "Sword") dmg = 2 * p.atk;
                else if (p.weapon.name == "Bow") dmg = 2 * (p.atk * 75 / 100);
                else if (p.weapon.name == "Wand") dmg = (int)(1.6 * p.atk * (1 - 0.4));
                else if (p.weapon.name == "Hammer") {
                    dmg = (int)(1.75 * p.atk);
                    int heal = p.maxHp * 10 / 100;
                    p.hp = min(p.maxHp, p.hp + heal);
                    cout << "\nHammer heals you for " << heal << " HP!\n";
                    actionLog.push("Round " + to_string(round) + ": Hammer healed player for " + to_string(heal) + " HP.");
                    Sleep(2000);
                }
                dmg -= e.def / 10;
                e.hp -= max(0, dmg);
                p.sp--;
                cout << "\nYou used Skill Attack and dealt " << dmg << " damage!\n";
                actionLog.push("Round " + to_string(round) + ": Player used Skill Attack (" + p.weapon.name + ") and dealt " + to_string(dmg) + " damage.");
                Sleep(2000);
            } else if (act == 3 && p.sp > 0) {
                int heal = p.maxHp * 30 / 100;
                p.hp = min(p.maxHp, p.hp + heal);
                p.sp--;
                cout << "\nYou used Heal and restored " << heal << " HP!\n";
                actionLog.push("Round " + to_string(round) + ": Player healed " + to_string(heal) + " HP.");
                Sleep(2000);
            } else if (act == 4 && undoCount > 0 && history.size() > 1) {
                // Pop the current state since it's current, then restore previous state
                history.pop();
                if (!history.empty()) {
                    BattleState s = history.top();
                    p.hp = s.playerHp;
                    p.sp = s.playerSp;
                    e.hp = s.enemyHp;
                    e.state = s.enemyState;
                    undoCount--;
                    cout << "\nUndo successful! Restored previous state.\n";
                    Sleep(2000);
                    continue; // Skip rest of loop to re-display state
                }
            } else if (act == 5) {
                cout << "\nYou retreated from battle.\n";
                cout << "What a shame.\n";
                cout << "Loading to Main Menu";
                saveScore(p.name + "(Retreat)", p.score);
                Sleep(2000);
                return;
            } else if (act == 6) {
                system("cls");
                cout << "===== Action Log =====\n";
                queue<string> copy = actionLog;
                while (!copy.empty()) {
                    cout << copy.front() << endl;
                    copy.pop();
                }
                cout << "\nPress enter to continue...";
                cin.ignore(); cin.get();
                continue;
            } else {
                cout << "\nOut of SP! You didn't do anything\n";
                actionLog.push("Round " + to_string(round) + ": Player out of SP and didn't do anything");
                Sleep(2000);
            }

            // Enemy's turn to attack
            int edmg = e.atk - p.def / 10;
            if (e.state == 2) {
                edmg *= 2;
                cout << "Enemy used a charged attack and dealt " << edmg << " damage!\n";
                actionLog.push("Round " + to_string(round) + ": Enemy used a charged Attack and dealt " + to_string(edmg) + " damage.");
                e.state++;
                pause();
            } else if (e.state == 3) {
                int eheal = e.maxHp / 10;
                e.hp = min(e.maxHp, e.hp + eheal);
                cout << "Enemy healed for " << eheal << " HP!\n";
                actionLog.push("Round " + to_string(round) + ": Enemy healed " + to_string(eheal) + " HP.");
                e.state = 0;
                pause();
                continue;
            } else {
                cout << "Enemy used Basic Attack and dealt " << edmg << " damage!\n";
                actionLog.push("Round " + to_string(round) + ": Enemy used Basic Attack and dealt " + to_string(edmg) + " damage.");
                p.hp -= max(0, edmg);
                e.state++;
                pause();
            }
        }
        if (p.hp <= 0) {
            cout << "\nYou were defeated in battle.\n";
            saveScore(p.name, p.score);
            cout << "Press enter to continue...";
            cin.ignore(); cin.get();
            return;
        } else {
            cout << "You won this round!\n";
            cout << "Going to the next round...\n";
            pause();
            p.score += round;
            round++;
        }
    }
}

void mainMenu() {
    while (true) {
        system("cls");
        cout << "===================================\n";
        cout << "============= Welcome =============\n";
        cout << "===================================\n";
        cout << "[1] Main\n[2] Leaderboard\n[3] Exit\n";
        
        int choice = inputInt(1, 3);
        if (choice == 1) {
            Player p;
            build(p);
            battle(p);
        } else if (choice == 2) {
            showLeaderboard();
        } else break;
    }
}

int main() {
    cout << "\033[33m";
    mainMenu();
    cout << "\033]0m";
    return 0;
}
