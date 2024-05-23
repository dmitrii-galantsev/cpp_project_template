#include "my_fork.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <future>
#include <iostream>

int call_dangerous_func() {
    // Simulate some work
    std::cout << "Function is running in the child process" << std::endl;

    // Simulate failure
    std::abort();
    return -1;
}

void call_func_fork() {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        int result = call_dangerous_func();
        exit(result == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
    } else {
        // Parent process
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            if (exitStatus == EXIT_SUCCESS) {
                std::cout << "Function succeeded in the child process" << std::endl;
            } else {
                std::cout << "Function failed in the child process with exit status: " << exitStatus
                          << std::endl;
            }
        } else if (WIFSIGNALED(status)) {
            int termSignal = WTERMSIG(status);
            std::cout << "Child process terminated by signal: " << termSignal << std::endl;
        } else {
            std::cout << "Child process ended abnormally" << std::endl;
        }
    }
}

void call_func_thread() {
    std::packaged_task<int()> task(call_dangerous_func);
    std::future<int> result = task.get_future();
    std::thread t(std::move(task));

    try {
        int status = result.get();  // This will throw if functionToCall throws
        std::cout << "Function succeeded with status: " << status << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Function failed with exception: " << e.what() << std::endl;
    }

    t.join();  // Ensure the thread has finished execution
}

int my_fork() {
    std::cout << "*** call_func_fork\n";
    call_func_fork();
    // std::cout << "*** call_func_thread\n";
    // call_func_thread(); // This would fail! because threads don't catch aborts
    return 0;
}
