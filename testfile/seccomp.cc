#define _BSD_SOURCE // readlink
#include <dlfcn.h>
#include <stdlib.h> // exit
#include <string.h> // strstr, memset
#include <link.h>   // ElfW
#include <errno.h>  // EPERM
#include <unistd.h> // readlink
#include <seccomp.h>
#include <stdio.h>
int syscalls_whitelist[] = {SCMP_SYS(read), SCMP_SYS(write), 
                            SCMP_SYS(fstat), SCMP_SYS(mmap), 
                            SCMP_SYS(mprotect), SCMP_SYS(munmap), 
                            SCMP_SYS(brk), SCMP_SYS(access), 
                            SCMP_SYS(exit_group)};
typedef int (*main_t)(int, char **, char **);

#ifndef __unbounded
# define __unbounded
#endif

static int (*main_orig)(int, char **, char **);

int main_hook(int argc, char **argv, char **envp)
{
    printf("Hooker\n");
    int ret = main_orig(argc, argv, envp);
    printf("--- After main ----\n");
    printf("main() returned %d\n", ret);
    return 0;
}

/*
 *  * Wrapper for __libc_start_main() that replaces the real main
 *   * function with our hooked version.
 *    */
int __libc_start_main(
    int (*main)(int, char **, char **),
    int argc,
    char **argv,
    int (*init)(int, char **, char **),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end)
{
    printf("libc_\n");
    /* Save the real main function address */
    main_orig = main;

    /* Find the real __libc_start_main()... */
    typeof(&__libc_start_main) orig =  (typeof(&__libc_start_main)) dlsym(RTLD_NEXT, "__libc_start_main");

    /* ... and call it with our custom main function */
    return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}