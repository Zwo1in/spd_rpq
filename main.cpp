#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

struct Task;
using Tasks = std::vector<Task>;

int cmax(Tasks);

int main() {
    std::ifstream data;
    data.open("data", std::ios::in);
    if (!data) {
        std::cout << "Couldn't find file 'data' in program's dir" << std::endl;
        return -1;
    }

    Tasks tasks_list[4];
    int current = 0;
    int size;
    std::string tmp;
    do {
        data >> tmp;
        if (tmp.find("data.") != std::string::npos) {
            data >> size;
            tasks_list[current].reserve(size);
            while (size --) {
                tasks_list[current].emplace_back(data);
            }
            current ++;
        }
    } while (current != 4);

    int sum{};
    for (auto& tasks: tasks_list) {
        sum += cmax(tasks);
    }
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}

struct Task {
    int R, P, Q;

    Task(int r, int p, int q) {
        R = r;
        P = p;
        Q = q;
    }

    Task(std::ifstream& input) {
        input >> R >> P >> Q;
    }
};

int cmax(Tasks tasks) {
    int m{}, c{};

    for (auto task: tasks) {
        m = std::max(m, task.R) + task.P;
        c = std::max(c, m + task.Q);
    }
    return c;
}
