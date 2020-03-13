#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <limits>
#include <array>

struct Task;
using Tasks = std::vector<Task>;

using Solver = std::pair<bool, int>;

int cmax(Tasks);
void algorithm(Tasks&, Solver);
void get_id(Tasks);

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

    Solver solver{true, 0};
    const float grades[]{3.5, 4, 4.5, 5, 5.5};
    const int results[]{103378, 100926, 100381, 100010, 100000};
    std::array<Solver, 5> bests;
    current = 0;
    while (current < 5) {
        Tasks tasks_list_tmp[4] = tasks_list;
        for (auto& tasks: tasks_list_tmp) {
            algorithm(tasks, solver);
        }

        int sum{};

        for (auto& tasks: tasks_list_tmp) {
            sum += cmax(tasks);
        }

        if (sum < results[current]) {
            std::cout << "Grade: " << grades[current] << " Req: " << results[current] << " Got Cmax = " << sum << " with solver: <presorting " << solver.first << ", repeatitions " << solver.second << ">\n";
            bests[current] = solver;
            current ++;
        }

        if (solver.first)
            solver.second++;

        solver.first = solver.first == false;
    }

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

void double_ended_sort(Tasks& tasks) {
    Tasks shortest_R(tasks.size()),
          shortest_Q(tasks.size());
    std::deque<Task> ret;

    std::partial_sort_copy(tasks.begin(), tasks.end(), shortest_R.begin(), shortest_R.end(),
        [](const auto& t1, const auto& t2){
            return t1.R < t2.R;
        });
    std::partial_sort_copy(tasks.begin(), tasks.end(), shortest_Q.begin(), shortest_Q.end(),
        [](const auto& t1, const auto& t2){
            return t1.Q < t2.Q;
        });

    auto Q_iter = shortest_Q.begin();
    auto R_iter = shortest_R.begin();
    auto Q_end  = shortest_Q.end();
    auto ret_middle = ret.begin();
    while (Q_iter != Q_end) {
        if (std::find(ret.begin(), ret.end(), *Q_iter) == ret.end()) {
            ret_middle = ret.insert(ret_middle, *Q_iter);
        }
        if (std::find(ret.begin(), ret.end(), *R_iter) == ret.end()) {
            ret_middle = ret.insert(++ret_middle, *R_iter);
        }
        Q_iter++;
        R_iter++;
    }
    tasks = {ret.begin(), ret.end()};
}


void get_id(Tasks tasks) { 
	for(auto task: tasks) {
		std::cout << task.id+1 << " ";
	}
}

void best_swaps(Tasks& tasks) {
    auto optimal = std::make_pair(&tasks[0], std::numeric_limits<int>::max());

    for (int i=0; i < tasks.size(); ++i) {
        auto current = std::find_if(tasks.begin(), tasks.end(),
            [i](const auto& task) {
                return task.id == i;
            });

        for (auto& t_better: tasks) {
            if (*current == t_better) {
                continue;
            }
            std::swap(*current, t_better);

            if (cmax(tasks) < optimal.second) {
                optimal = std::make_pair(&t_better, cmax(tasks));
            }
            std::swap(t_better, *current);
        }
        std::iter_swap(optimal.first, current);
        optimal.second = std::numeric_limits<int>::max();
    }
}

void algorithm(Tasks& tasks, Solver solver) {
    // Display before
    //std::for_each(tasks.begin(), tasks.end(), [](auto it) { std::cout << it << std::endl; });
    //std::cout << std::endl << std::endl;

    // Algorithm
    if (solver.first)
        double_ended_sort(tasks);
    for (int i=0; i<solver.second; i++) 
		best_swaps(tasks);

    // Display after
    //std::for_each(tasks.begin(), tasks.end(), [](auto it) { std::cout << it << std::endl; });
    //std::cout << std::endl << std::endl;
}
