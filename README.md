# Minishell

Minishell is a simplified Unix shell written in C. It mimics the behavior of Bash, handling user input, parsing, executing commands, managing redirections and pipelines, and implementing key built-in functions.

---

## 🚀 Features

- Prompt with input reading loop
- Command parsing with quotes and escape handling
- Pipes (`|`)
- Redirections (`<`, `>`, `>>`, `<<`)
- Environment variable expansion (`$VAR`)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- Built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

---

## 🧠 Project Goals

- Learn how shells work internally
- Understand process creation and management (`fork`, `execve`, `wait`)
- Master Unix file descriptors and redirection
- Handle edge cases like quotes, syntax errors, heredocs, and pipes
- Implement a clean and modular parser/executor

---

## 🛠️ How to Build

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
## 📦 How to Use

./minishell

Then type a command like:

echo Hello | cat -e > output.txt
## ⚠️ Requirements (from the 42 Project)

    No use of system(), popen(), or similar shortcuts

    Only allowed to use a limited set of functions (e.g., malloc, free, read, write, open, close, fork, execve, wait, etc.)

    Must manage memory properly (no leaks)

    Signals must behave like in Bash (e.g., Ctrl+C should interrupt command)
