#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

// Register struct representing a RISC-V register
struct Register {
    bool canUse = true; // Initially set to true
};

// Instruction struct representing an instruction with opcode, description, stage, and index
struct Instruction {
    string opcode;       // 32-bit machine code in string form
    string instruction;  // Instruction in words (e.g., add x5, x6, x7)
    string stage = "not_processed_yet"; // Initial stage
};

// Output struct for formatted result
struct Output {
    string instruction; // Instruction in words
    string stages;      // Pipeline stages output format
};

// Processor struct containing up to 5 instructions
struct Processor {
    vector<pair<Instruction, int>> instructions; // Tracks each instruction with index
};

// Function to read instructions from CSV file
vector<Instruction> readInstructionsFromCSV(const string& filename) {
    vector<Instruction> instructions;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string index, address, opcode, instr;

        // Extract values based on space-separated format
        ss >> index >> address >> opcode;
        getline(ss, instr); // Remaining part is the instruction in words

        // Trim leading spaces from instr
        instr = instr.substr(instr.find_first_not_of(" "));

        instructions.push_back({opcode, instr});
    }

    return instructions;
}

// Function to simulate pipeline stages with opcode logic
void simulatePipeline(vector<Instruction>& instructions, vector<Output>& outputLog) {
    queue<pair<Instruction, int>> pipeline;
    bool canAddNewInstruction = true; // Flag for controlling new instruction addition
    int instructionIndex = 0; // Tracks position in the instruction list

    int cycle = 0;

    while (instructionIndex < instructions.size() || !pipeline.empty()) {
        string cycleOutput = "";

        // Add new instruction to pipeline if space available and no stall
        if (canAddNewInstruction && pipeline.size() < 5 && instructionIndex < instructions.size()) {
            instructions[instructionIndex].stage = "IF";
            pipeline.push({instructions[instructionIndex], instructionIndex});
            // add cycle times ;in ouput log at that index.
            instructionIndex++; // Advance index instead of erasing from vector
        }

        // Process each instruction from back to front
        int size = pipeline.size();
        for (int i = size - 1; i >= 0; --i) {
            auto instrPair = pipeline.front();
            pipeline.pop();
            auto& instr = instrPair.first;

            string opcode = instr.opcode.substr(25, 7); // Extract opcode bits
            if (opcode == "0110011") {
                // R-type logic placeholder
            } else if (opcode == "0010011") {
                // I-type logic placeholder
            } else if (opcode == "0100011") {
                // S-type logic placeholder
            }

            if (instr.stage == "IF") instr.stage = "ID";
            else if (instr.stage == "ID") instr.stage = "EX";
            else if (instr.stage == "EX") instr.stage = "MEM";
            else if (instr.stage == "MEM") instr.stage = "WB";

            cycleOutput += (i == size - 1 ? instr.instruction + ";" : " ;") + instr.stage;
            pipeline.push(instrPair);  // Move processed instruction to back of queue
        }

        outputLog.push_back({pipeline.front().first.instruction, cycleOutput});
        while (!pipeline.empty() && pipeline.front().first.stage == "WB") {
            pipeline.pop();
        }

        cycle++;
    }

    // Display output log
    for (const auto& entry : outputLog) {
        cout << entry.instruction << " " << entry.stages << endl;
    }
}

int main() {
    // Create a list of 32 registers
    vector<Register> registers(32);

    // Read instructions from CSV file
    vector<Instruction> instructionList = readInstructionsFromCSV("instructions.csv");

    // Create output list and populate with initial instructions
    vector<Output> outputLog;
    for (const auto& instr : instructionList) {
        outputLog.push_back({instr.instruction, ""});
    }

    // Simulate pipeline stages
    simulatePipeline(instructionList, outputLog);

    return 0;
}
