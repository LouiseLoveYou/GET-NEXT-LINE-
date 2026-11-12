*This project has been created as part of the 42 curriculum by Kisella.*

# 🌙 get_next_line

## 📝 Description

get_next_line is an individual project from the 42 school curriculum. The goal is to write a function, `get_next_line()`, that returns **one line at a time** from a file descriptor.

This project introduces **static variables** in C, and develops a better understanding of file descriptors, `read()`, dynamic memory management and memory leaks.

```c
char *get_next_line(int fd);
```

Repeated calls to `get_next_line()` let you read the content of a file descriptor line by line, until the end:

- 📖 It returns the line that was read, **including the terminating `\n`**, except for the last line if the file does not end with a `\n`.
- 🚫 It returns `NULL` when there is nothing left to read, or when an error occurs.
- 🧵 It works on regular files, on the standard input and on pipes.

Only `read()`, `malloc()` and `free()` are allowed. `libft`, `lseek()` and global variables are forbidden.

The bonus part (several file descriptors at the same time with a single static variable) is not implemented.

## ⚙️ Instructions

### 🏗️ Compilation

The size of the buffer given to `read()` is defined at compilation with the `-D BUFFER_SIZE=n` option:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Any positive value works (`1`, `42`, `9999`, `10000000`...). With a `BUFFER_SIZE` lower than or equal to `0`, `get_next_line()` returns `NULL`.

### 🚀 Usage

Include the header and call the function in a loop until it returns `NULL`. Every returned line is allocated: **the caller must free it**.

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

## 🗂️ Project structure

- 📄 `get_next_line.h` — Header with the prototypes.
- 📄 `get_next_line.c` — `get_next_line()` and its three internal helpers `read_to_stock()`, `extract_line()` and `updated_stock()`.
- 📄 `get_next_line_utils.c` — String helpers: `ft_strlen`, `ft_strchr`, `ft_strjoin`.

## 🧠 Technical considerations

- 💻 Written in C.
- 📏 Follows the 42 Norm.
- 🧪 Compiled with `-Wall -Wextra -Werror`.
- 📥 Uses `read()` to get the data, `malloc()` and `free()` to manage memory.
- 🧷 Uses one `static` variable to keep the unread data between two calls.
- 🚫 No global variable, no `libft`, no `lseek()`.

## 🔍 Algorithm and justification

### Chosen algorithm

`get_next_line()` keeps what has been read but not returned yet in a `static` string called `stock`. Each call does three steps:

1. **Fill the stock** — `read_to_stock()` calls `read()` with `BUFFER_SIZE` bytes and appends them to `stock` with `ft_strjoin()`. It stops as soon as `stock` contains a `\n`, or when `read()` returns `0` (end of file).
2. **Extract the line** — `extract_line()` allocates a new string and copies `stock` up to the first `\n` (included), or up to the end if there is none.
3. **Update the stock** — `updated_stock()` allocates a new string containing only what follows the first `\n`, then frees the old `stock`. If nothing remains, it frees `stock` and returns `NULL`.

The function then returns the extracted line. The remainder stays in the static variable, ready for the next call.

**Error handling.**
- If `fd` is negative or `BUFFER_SIZE` is not positive, `NULL` is returned immediately.
- If `read()` fails (`-1`) or a `malloc()` fails, everything allocated is freed and `NULL` is returned. Because the static variable receives that `NULL`, the next call starts from a clean state and never reuses a freed pointer.
- When the end of the file is reached, the last line is returned, then `stock` is freed: the following call returns `NULL` and nothing is left allocated.

**Justification — why a static string?**
`read()` often reads more than one line (a large `BUFFER_SIZE`) and sometimes less (a small one, a pipe). The extra data must survive between two calls, and global variables are forbidden. A `static` variable has exactly the lifetime needed: created once, it keeps its value from one call to the next and is only visible inside `get_next_line()`.

**Justification — why stop reading at the first `\n`?**
The subject asks to read as little as possible and never to read the whole file first. With the `\n` check in the reading loop, a call only reads what is needed to complete one line. This is also what makes the function work on a terminal or an open pipe, where `read()` would block if called again once the line is complete.

**Justification — why three small helpers?**
Each function has a single job (read, cut, shrink), which keeps every function short enough for the Norm (25 lines), makes each one easy to test on its own, and makes the cleanup of memory explicit: `read_to_stock()` frees `stock` when it fails, `updated_stock()` frees the old `stock` once the remainder is copied, and the caller frees the returned line.

**Justification — why does it work for any `BUFFER_SIZE`?**
- `BUFFER_SIZE = 1`: the loop reads one byte at a time, so it is slow but correct.
- `BUFFER_SIZE = 9999` or `10000000`: the buffer is allocated with `malloc()` (not on the stack, which would overflow for large values), and the surplus is kept in `stock`.
- A line longer than `BUFFER_SIZE`: the loop simply runs several times and `stock` grows with each `ft_strjoin()`.

**Trade-off.** Every `ft_strjoin()` and every `updated_stock()` allocates and copies the data again. This is simple and safe, but a very long line read with a tiny `BUFFER_SIZE` costs more copies than a more advanced approach (a list of chunks, for example).

### Chosen data structures

- A **static `char *`** (`stock`): the only state kept between calls. It holds the data read but not yet returned.
- A **temporary buffer** of `BUFFER_SIZE + 1` bytes, allocated with `malloc()` for each call and freed before returning.
- **Heap-allocated strings** for the returned line and for the new stock.
- **Integer indexes** (`size_t`) to walk through the strings.

**Justification — why these structures?**
A single string is enough to represent "what remains to be returned", and it is easy to verify: at any time, the only memory that stays allocated between two calls is `stock`. No list or table is needed for the mandatory part, so there is no extra bookkeeping that could leak.

## 🧪 Testing

The function was tested with `BUFFER_SIZE` values of `1`, `2`, `42`, `1000`, `9999` and `10000000`, on:

- an empty file, a file with a single `\n`, a file with only empty lines;
- a file with and without a final `\n`;
- very long lines (10 000 characters) and lines of `BUFFER_SIZE - 1`, `BUFFER_SIZE` and `BUFFER_SIZE + 1` characters;
- the standard input and pipes;
- an invalid file descriptor (`-1`, closed, not open) and a directory;
- failing `read()` and `malloc()` calls.

Memory was checked with `valgrind --leak-check=full` and AddressSanitizer (no error, no leak), and the code was checked with `norminette`. An unofficial tester script was also used.

## 📚 Resources

- 📖 [read(2) — Linux Manual](https://man7.org/linux/man-pages/man2/read.2.html)
- 📘 [malloc(3) — Linux Manual](https://man7.org/linux/man-pages/man3/malloc.3.html)
- 📗 [Static variables in C — cppreference (storage duration)](https://en.cppreference.com/w/c/language/storage_duration)
- 🧰 [Valgrind Documentation](https://valgrind.org/docs/manual/manual.html)
- 🐧 [Linux man-pages](https://man7.org/linux/man-pages/)

### 🤖 AI usage

AI (Claude) was used as a learning support during the project, not to write the core of it. It was used for:

- **Concepts:** explaining what a `static` variable is, why `free()` does not reset a pointer, what a use-after-free and a double free are, and how `read()` behaves with different buffer sizes.
- **Code review:** pointing out mistakes in the functions I wrote (stray `;` after an `if`, wrong operators, missing `free`, missing terminating `'\0'`, Norm issues) and giving hints. I wrote and corrected the code myself. The shortened version of `updated_stock()` was suggested by the AI to fit the 25-line limit, and I then studied it and integrated it.
- **Testing:** suggesting edge cases and the checks to run (valgrind, `BUFFER_SIZE` values, invalid file descriptors).
- **README:** drafting and structuring this file from my code and the subject.

All code was understood, tested and verified by me.

## 👩‍💻 Author

**Louise** 💜

🎓 42 Student  
🔑 Login: `Kisella`

---

*Made with ☕, C and a little bit of lo-fi.* 🌙