#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <atomic>
#include <stdlib.h>

double f(double x) {
    return sin(1/x);
}

void integrate(std::atomic<double>& result, double a, double b, int steps) {
    double h = (b - a) / steps;
    double local_result = 0.5 * (f(a) + f(b));

    for (int i = 1; i < steps; i++)
        local_result += f(a + i*h);

    local_result *= h;
    result.fetch_add(local_result);
}

int main(int argc, char *argv[]) {
    double a = 0.1;  
    if (argc > 1)
        a = atof(argv[1]);

    double b = 1.0;
    if (argc > 2)
        b = atof(argv[2]);

    int n = std::thread::hardware_concurrency();  // number of CPUs
    int steps = 1000000000;
    if (argc > 3)
        steps = atoi(argv[3]);
    if (argc > 4)
        n = atoi(argv[4]);
    int thread_steps = steps/n;

    std::vector<std::thread> threads(n);
    std::atomic<double> result(0.0);

    double seg_len = (b-a)/n/(n+1)*2;
    double start = a;

    for (int i = 0; i < n; ++i) {
        double loc_len = (i+1)*seg_len;

        threads[i] = std::thread(integrate, std::ref(result), start, start+loc_len, thread_steps);
        start += loc_len;
     }

    for (auto& thread : threads) 
        thread.join();

    std::cout << "Result: " << result << std::endl;

    return 0;
}

