# Introducing gvn, the ultimate C shell

This is a shell I wrote on my free time, with a little help from [https://brennan.io/2015/01/16/write-a-shell-in-c/](https://brennan.io/2015/01/16/write-a-shell-in-c/).

# Capabilities

Implements basic UNIX commands. Only works on Windows due to my usage of the Windows API. UNIX commands like cat, echo, ls, cp, pwd, and clear have been implemented. We have also implemented a flag system that is able to use different commands with options. For
example, we have:

```
echo -n "Hello, World!"
```

This will use the -n flag to print the line without the newline at the end.

Looking at implementing commands like grep and regex search in the future, as well as customization with config files. Also looking at implementing the redirection operator, which gives immense flexibility to the user in terms of text transformation.

Recently implemented the basic workings of a lexical analyser which is able to detect operators, identifiers, integers, delimeters, and reserved keywords! Planning on using this to perform more in depth syntactical analysis, and implement operators like redirection(>) and pipe(|)!

I've also implemented the ability to spawn processes within the shell. You may run any executable, or program within the shell. The shell spawns a child process where the program runs. This is currently only available for Windows, as I have made use of the Windows API. UNIX implementation is not difficult, it would simply require use of the POSIX threads library.
