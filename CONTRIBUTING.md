# Contributing to hwmonitor

First off, thank you for considering contributing to **hwmonitor**! It's people like you that make open source tools great.

This document provides a set of guidelines for contributing to the repository.

---

## 1. Code of Conduct

By participating in this project, you agree to maintain a respectful, inclusive, and harassment-free environment for everyone. Please be kind and professional in all interactions.

## 2. Getting Started

### Prerequisites
*   A Linux environment (the tool relies heavily on Linux-specific filesystems like `sysfs` and `procfs`).
*   `gcc` and `make` installed.
*   Familiarity with C11.

### Setup
1. Fork the repository on GitHub.
2. Clone your fork locally **with submodules**:
   ```bash
   git clone --recursive https://github.com/your-username/hwmonitor.git
   ```
3. Create a new branch for your feature or bug fix:
   ```bash
   git checkout -b feature/my-new-feature
   ```

## 3. Development Guidelines

**hwmonitor** is built with extreme performance, modularity, and memory safety in mind. Please adhere to the following when writing code:

### Architecture
*   **No Shell Commands:** Do not use `system()`, `popen()`, or execute external binaries (like `lspci` or `dmidecode`). All hardware discovery must be done by parsing standard Linux interfaces (`/sys/`, `/proc/`, etc.).
*   **Modularity:** Keep hardware logic separated. If you add a new hardware type (e.g., `disk.c`), ensure it has its own header and doesn't tightly couple with other hardware modules.
*   **Zero Dependencies:** We aim to keep dependencies to an absolute minimum. If a third-party library is strictly necessary, it should be integrated via a Git submodule, not as a system-wide dependency.

### Code Style (Linux Kernel Style)
This project strongly adheres to the Linux kernel coding style conventions:
*   **Indentation:** Use 2 spaces for indentation.
*   **Braces:** Single-statement `if`/`for`/`while` blocks **must not** use braces.
    ```c
    // Bad
    if (!ptr) {
      return -1;
    }

    // Good
    if (!ptr)
      return -1;
    ```
*   **Null Checks:** Use `!ptr` instead of `ptr == NULL`.
*   **Pointers:** Bind the asterisk to the type, not the variable (e.g., `char* str`, not `char *str`).

### Memory Safety
*   This project has a zero-leak policy.
*   Every allocated struct must have a corresponding deep-free function (e.g., `free_cpu()`, `free_battery()`).
*   Ensure all dynamically allocated strings inside structs are freed before the struct itself is freed.

## 4. Submitting Changes

1.  **Test your changes:** Run `make clean && make` and ensure the binary builds without errors or warnings.
2.  **Verify memory:** Use tools like `valgrind` to ensure your new features do not introduce memory leaks.
    ```bash
    valgrind ./build/hwmonitor.o --json
    ```
3.  **Commit:** Write clear, concise commit messages.
4.  **Push:** Push your branch to your fork.
5.  **Pull Request:** Open a Pull Request against the `master` branch. Provide a detailed description of what your PR does and why it is necessary.

We look forward to reviewing your contributions!
