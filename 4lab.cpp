#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

using namespace std;

class Drone {
public:
    int x = 0, y = 0, z = 0;
    bool hasCargo = true;

    void Move(int dx, int dy, int dz) {
        x += dx; y += dy; z += dz;
        cout << "  ...moved to (X:" << x << ", Y:" << y << ", Z:" << z << ")\n";
    }
    void DropCargo() {
        if (hasCargo) {
            cout << "[DRONE ACTION: Cargo dropped!]\n"; hasCargo = false;
        } else {
            cout << "[DRONE ACTION: No cargo!]\n";
        }
    }
    void PrintCoordinates() {
        cout << "--------------------------------\n";
        cout << "Final Coordinates: (X: " << x << ", Y: " << y << ", Z: " << z << ")\n";
    }
};

class ICommand {
public:
    virtual ~ICommand() {}
    virtual void execute(Drone& drone) = 0;
};

class MoveUpCommand : public ICommand {
private: int meters;
public: MoveUpCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Up (U) by " << meters << "m\n";
        drone.Move(0, 0, meters);
    }
};
class MoveDownCommand : public ICommand {
private: int meters;
public: MoveDownCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Down (D) by " << meters << "m\n";
        drone.Move(0, 0, -meters);
    }
};
class MoveForwardCommand : public ICommand {
private: int meters;
public: MoveForwardCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Forward (F) by " << meters << "m\n";
        drone.Move(0, meters, 0);
    }
};
class MoveBackCommand : public ICommand {
private: int meters;
public: MoveBackCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Back (B) by " << meters << "m\n";
        drone.Move(0, -meters, 0);
    }
};
class MoveLeftCommand : public ICommand {
private: int meters;
public: MoveLeftCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Left (L) by " << meters << "m\n";
        drone.Move(-meters, 0, 0);
    }
};
class MoveRightCommand : public ICommand {
private: int meters;
public: MoveRightCommand(int m) : meters(m) {}
    void execute(Drone& drone) override {
        cout << "Executing: Right (R) by " << meters << "m\n";
        drone.Move(meters, 0, 0);
    }
};
class DropCargoCommand : public ICommand {
public:
    void execute(Drone& drone) override {
        cout << "Executing: Drop Cargo (DROP)\n";
        drone.DropCargo();
    }
};

class CommandParser {
public:
    virtual ~CommandParser() {}

    vector<unique_ptr<ICommand>> parseProgram(const string& programString) {
        vector<unique_ptr<ICommand>> program;
        stringstream ss(programString);
        string cmd;
        int value;

        while (ss >> cmd) {
            unique_ptr<ICommand> command = nullptr;

            if (cmd == "DROP") { 
                command = this->createCommand(cmd, 0); 
            } 
            else if (ss >> value) { 
                command = this->createCommand(cmd, value);
            }

            if (command) {
                program.push_back(std::move(command));
            } else {
                 cout << "WARNING: Unknown command '" << cmd << "'\n";
            }
        }
        return program;
    }

protected:
    virtual unique_ptr<ICommand> createCommand(const string& cmd, int value) = 0;
};

class StandardDroneParser : public CommandParser {
protected:
    unique_ptr<ICommand> createCommand(const string& cmd, int value) override {
        if (cmd == "U") return make_unique<MoveUpCommand>(value);
        if (cmd == "D") return make_unique<MoveDownCommand>(value);
        if (cmd == "F") return make_unique<MoveForwardCommand>(value);
        if (cmd == "B") return make_unique<MoveBackCommand>(value);
        if (cmd == "L") return make_unique<MoveLeftCommand>(value);
        if (cmd == "R") return make_unique<MoveRightCommand>(value);
        if (cmd == "DROP") return make_unique<DropCargoCommand>();
        
        return nullptr;
    }
};

int main() {
    string programString = "U 10 F 20 L 5 DROP R 10 B 5 D 10 DROP";
    Drone myDrone;

    unique_ptr<CommandParser> parser = make_unique<StandardDroneParser>();
    
    vector<unique_ptr<ICommand>> program = parser->parseProgram(programString);

    cout << "\n...Executing program...\n";
    for (const auto& command : program) {
        command->execute(myDrone);
    }
    cout << "...Program finished!\n";

    myDrone.PrintCoordinates();

    return 0;
}