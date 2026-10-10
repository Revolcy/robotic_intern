#include <bits/stdc++.h>

using namespace std;

struct position {
    int x, y;

    bool operator==(const position& other) const {
        return x == other.x && y == other.y;
    }
};

class battery {
private:
    int level;

public:
    battery(int initial = 100) : level(initial) {}

    void consume(int amount) {
        level -= amount;
        if (level < 0) level = 0;
    }

    void recharge() {
        level = 100;
    }

    int getLevel() const {
        return level;
    }

    bool isDying(int minLevel, int maxLevel) const {
        return level >= minLevel && level <= maxLevel;
    }
};

class robot {
protected:
    string name;
    position pos;
    class battery battery;
    int score;

public:
    robot(string name, int x, int y)
        : name(name), pos({x, y}), battery(100), score(0) {}

    virtual ~robot() {}

    position getPos() const { return pos; }
    string getName() const { return name; }
    int getScore() const { return score; }
    int getBattery() const { return battery.getLevel(); }
    void addScore() { score += 10; }

    void consumeBattery(int amount = 5) {
        battery.consume(amount);
    }

    void rechargeBattery() {
        battery.recharge();
    }
};

class PlayerRobot : public robot {
public:
    PlayerRobot(string name, int x, int y) : robot(name, x, y) {}

    void moveWASD(char input, int gridSize) {
        if ((input == 'w' || input == 'W') && pos.y > 0) pos.y--;
        else if ((input == 's' || input == 'S') && pos.y < gridSize - 1) pos.y++;
        else if ((input == 'a' || input == 'A') && pos.x > 0) pos.x--;
        else if ((input == 'd' || input == 'D') && pos.x < gridSize - 1) pos.x++;

        consumeBattery(5);
    }
};

// AI Robot (Otonom pakai FSM)
class AIRobot : public robot {
private:
    string state;

public:
    AIRobot(string name, int x, int y) : robot(name, x, y), state("Kerja") {}

    string getState() const { return state; }

    void notice(position charger, position target) {
        if (battery.isDying(1, 30)) {
            state = "Santai";
        } else if (battery.getLevel() == 100) {
            state = "Kerja";
        }

        position destination = (state == "Kerja") ? target : charger;

        if (pos.x < destination.x) {
            pos.x++;
        } else if (pos.x > destination.x) {
            pos.x--;
        }

        else if (pos.y < destination.y) {
            pos.y++;
        } else if (pos.y > destination.y) {
            pos.y--;
        }

        consumeBattery(5);

        if (pos.x == charger.x && pos.y == charger.y) {
            rechargeBattery();
            cout << name << " lagi nyantai\n";
        }
    }
};

void renderBoard(int size, position playerPos, position aiPos, position chargerPos, position targetPos) {
    cout << "\n===============================\n";
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            position current = {x, y};
            if (current == playerPos) cout << "[P] ";
            else if (current == aiPos) cout << "[A] ";
            else if (current == targetPos) cout << "[*] ";
            else if (current == chargerPos) cout << "[C] ";
            else cout << " .  ";
        }
        cout << "\n";
    }
    cout << "===============================\n";
    cout << "Keterangan: [P] Player | [A] AI | [*] Target | [C] Charger\n";
}

int main() {
    srand(time(0));
    const int GRID_SIZE = 5;

    position charger = {0, 0};
    position target = {4, 4};

    PlayerRobot player("Player1", 0, 4);
    AIRobot ai("AMBATRON", 4, 0);

    cout << "=== GAME: PLAYER VS AMBATRON ===\n";

    while (player.getBattery() > 0 && ai.getBattery() > 0) {
        renderBoard(GRID_SIZE, player.getPos(), ai.getPos(), charger, target);


        cout << player.getName() << " -> Baterai: " << player.getBattery() << "% | Skor: " << player.getScore() << "\n";
        cout << ai.getName() << " [" << ai.getState() << "] -> Baterai: " << ai.getBattery() << "% | Skor: " << ai.getScore() << "\n\n";

        char moveInput;
        cout << "Masukkan Arah (W/A/S/D): ";
        cin >> moveInput;
        player.moveWASD(moveInput, GRID_SIZE);

        if (player.getPos() == charger) {
            player.rechargeBattery();
            cout << "Santai Dulu Ga Sih\n";
        }

        ai.notice(charger, target);

        if (player.getPos() == target) {
            cout << "\n PLAYER BERHASIL MENGAMBIL TARGET!\n";
            player.addScore();
            target = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        } else if (ai.getPos() == target) {
            cout << "\n AMBATRON BERHASIL MENGAMBIL TARGET!\n";
            ai.addScore();
            target = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        }
    }

    // Akhir Permainan
    cout << "\n=== GAME OVER ===\n";
    cout << "Skor Akhir:\n";
    cout << "Player: " << player.getScore() << "\n";
    cout << "AMBATRON: " << ai.getScore() << "\n\n";

    if (player.getScore() > ai.getScore()) {
        cout << "You Mogged AMBATRON! \n";
    } else if (player.getScore() < ai.getScore()) {
        cout << "💀 Bro got Backshotted by AMBATRON\n";
    } else {
        cout << "Draw \n";
    }

    return 0;
}