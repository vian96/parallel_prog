#include <omp.h>
#include <stdio.h>

void print_thread_info() {
    int level = omp_get_level();
    printf("Level %d out of %d: thread %d out of %d, parent: %d, parent's parent: %d\n", 
            level, omp_get_active_level(), 
            omp_get_thread_num(), omp_get_num_threads(), 
            omp_get_ancestor_thread_num(level-1),
            omp_get_ancestor_thread_num(level-2));
}

int main() {
    omp_set_nested(1); 
    #pragma omp parallel num_threads(2)
    {
        print_thread_info();

        #pragma omp parallel num_threads(2)
        {
            print_thread_info();

            #pragma omp parallel num_threads(2)
            {
                print_thread_info();
            }
        }
        print_thread_info();
    }

    return 0;
}

