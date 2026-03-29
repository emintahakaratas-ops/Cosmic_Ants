#include <bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <filesystem>
using namespace std;

namespace fs = filesystem;

void log(string msg, string level = "INFO") {
    cout << "[" << level << "] " << msg << endl;
}

bool checkFile(string filepath) {
    return fs::exists(filepath);
}

bool compile(string source, string output) {
    log("Derle: " + source + " -> " + output, "COMPILE");
    remove(output.c_str());
    
    string cmd = string("g++.exe ") + source + " -o " + output;
    int result = system(cmd.c_str());
    if (result != 0) {
        log("Derleme başarısız!", "ERROR");
        return false;
    }
    log("Derleme başarılı", "OK");
    return true;
}

bool runProgram(string executable, int timeout_sec = 300) {
    log("Çalıştırılıyor: " + executable, "RUN");
    string cmd = executable;
    int result = system(cmd.c_str());
    if (result != 0) {
        log("Program başarısız (exit: " + to_string(result) + ")", "ERROR");
        return false;
    }
    log("Program başarılı", "OK");
    return true;
}

int main() {
    cout << "\n============================================================\n";
    cout << "PIPELINE BASLATILIYOR\n";
    cout << "============================================================\n\n";

    bool error = false;

    // Step 1: Height to Slope
    cout << "[Step 1/3] Height to Slope Conversion\n";
    cout << "============================================================\n";
    if (!checkFile("input_heights.txt")) {
        log("input_heights.txt bulunamadı", "ERROR");
        error = true;
    } else {
        if (compile("heightmap_to_slopemap.cpp", "heightmap_to_slopemap.exe")) {
            if (runProgram("heightmap_to_slopemap.exe")) {
                if (checkFile("slopemap.txt")) {
                    log("Step 1 başarılı - slopemap.txt oluşturuldu", "OK");
                } else {
                    log("slopemap.txt oluşturulamadı", "ERROR");
                    error = true;
                }
            } else {
                error = true;
            }
        } else {
            error = true;
        }
    }
    cout << "\n";

    // Step 2: Slope to Cost
    cout << "[Step 2/3] Slope to Cost Conversion\n";
    cout << "============================================================\n";
    if (!error && checkFile("slopemap.txt")) {
        if (compile("slopemap_to_costmap.cpp", "slopemap_to_costmap.exe")) {
            if (runProgram("slopemap_to_costmap.exe")) {
                if (checkFile("costmap.txt")) {
                    log("Step 2 başarılı - costmap.txt oluşturuldu", "OK");
                } else {
                    log("costmap.txt oluşturulamadı", "ERROR");
                    error = true;
                }
            } else {
                error = true;
            }
        } else {
            error = true;
        }
    } else if (!error) {
        log("slopemap.txt bulunamadı", "ERROR");
        error = true;
    }
    cout << "\n";

    // Step 3: A* Pathfinding
    cout << "[Step 3/3] A* Pathfinding\n";
    cout << "============================================================\n";
    if (!error && checkFile("costmap.txt")) {
        if (compile("main_code.cpp", "main_code.exe")) {
            if (runProgram("main_code.exe")) {
                if (checkFile("rota.txt")) {
                    log("Step 3 başarılı - rota.txt oluşturuldu", "OK");
                } else {
                    log("rota.txt oluşturulamadı", "ERROR");
                    error = true;
                }
            } else {
                error = true;
            }
        } else {
            error = true;
        }
    } else if (!error) {
        log("costmap.txt bulunamadı", "ERROR");
        error = true;
    }
    cout << "\n";

    // Summary
    cout << "============================================================\n";
    if (!error) {
        cout << "[SUCCESS] TÜM ADIMLAR BAŞARILI!\n";
        cout << "Rota dosyası hazır: rota.txt\n";
        cout << "============================================================\n";
        return 0;
    } else {
        cout << "[ERROR] BAZZ ADIMLAR BAŞARIŞIZ\n";
        cout << "Kontrol edin ve yeniden çalıştırın\n";
        cout << "============================================================\n";
        return 1;
    }
}
