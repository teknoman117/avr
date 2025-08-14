#pragma once

#define CONSOLE(stmt) do { \
        lockConsole(); \
        stmt; \
        unlockConsole(); \
    } while (0)

#define CONSOLE_RET(stmt, r) do { \
        lockConsole(); \
        r = stmt; \
        unlockConsole(); \
    } while (0)

extern void setupConsole(void);

extern void lockConsole(void);
extern void unlockConsole(void);