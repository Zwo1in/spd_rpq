#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

struct Task;
using Tasks = std::vector<Task>;

int cmax(Tasks);
void algorithm(Tasks&);

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
            for (int i = 0; i<size; ++i) {
                tasks_list[current].emplace_back(i, data);
            }
            current ++;
        }
    } while (current != 4);

    for (auto& tasks: tasks_list) {
        algorithm(tasks);
    }

    int sum{};
    for (auto& tasks: tasks_list) {
        sum += cmax(tasks);
    }
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}

struct Task {
    int id, R, P, Q;

    Task(): id(0), R(0), P(0), Q(0) {}

    Task(int id, std::ifstream& input): id(id) {
        input >> R >> P >> Q;
    }

    bool operator==(const Task& other) {
        return this->id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Task& t);
};

std::ostream& operator<<(std::ostream& os, const Task& t) {
    os << "Task { id: " << t.id << " R: " << t.R << " P: " << t.P << " Q: " << t.Q << " }";
    return os;
}


int cmax(Tasks tasks) {
    int m{}, c{};

    for (const auto& task: tasks) {
        m = std::max(m, task.R) + task.P;
        c = std::max(c, m + task.Q);
    }
    return c;
}

void algorithm(Tasks& tasks) {
    std::for_each(tasks.begin(), tasks.end(), [](auto it) { std::cout << it << std::endl; });
    std::cout << std::endl << std::endl;

    Tasks shortest_R(tasks.size()), shortest_Q(tasks.size()), ret;

    std::partial_sort_copy(tasks.begin(), tasks.end(), shortest_R.begin(), shortest_R.end(), [](const auto& t1, const auto& t2){
        return t1.R < t2.R;
    });

    std::partial_sort_copy(tasks.begin(), tasks.end(), shortest_Q.begin(), shortest_Q.end(), [](const auto& t1, const auto& t2){
        return t1.Q < t2.Q;
    });

    auto R_begin = shortest_R.begin();
    auto Q_begin = shortest_Q.rbegin();
    auto R_end   = shortest_R.end();
    while (R_begin != R_end) {
        auto insert = [&](auto iter) {
            if (std::find(ret.begin(), ret.end(), *iter) == ret.end()) {
                ret.push_back(*iter);
            }
        };
        insert(R_begin++);
        insert(Q_begin++);
    }
    tasks = ret;
    std::for_each(tasks.begin(), tasks.end(), [](auto it) { std::cout << it << std::endl; });
    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;
}
