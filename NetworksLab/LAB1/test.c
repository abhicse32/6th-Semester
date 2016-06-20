#include <unistd.h>
       #include <stdio.h>

       main()
       {
          char *cwd;
          if ((cwd = getcwd(NULL, 64)) == NULL) {
              perror("pwd");
              exit(2);
          }
          printf("%s\n", cwd);
          free(cwd); /* free memory allocated by getcwd() */
          return(0);
       }