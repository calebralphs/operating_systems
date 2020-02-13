Caleb Ralphs
CS 3013
Project 1

Program Usage
    Each phase of the program can be run by the following calls:
        make all
        make clean
        ./boring
        ./custom
        ./ multi [args]
    You can see the boring_output.txt, custom_output.txt, and multi_output.txt for the test case outputs.

Boring Commander
    I developed boring command once through soley for the purpose of part 1 of the assignment, and it worked fine.
    But I then went back and generalized the execCommand() function to work for arbitrary command calls, parsing
    the arguments correctly. All of the functions for the boring commander can be found in boring.h, since I used
    the functions both for the boring commander, boring.c, and also included the header file in the custom 
    commander. I tested the boring commander by just running the executable and looking at the output to see that
    it "matched" that shown in the assignment.

Custom Commander
    I leveraged the generalized execCommand() function from the boring commander for this part as well. Since in
    had that function handling arbitrary input command lines (with the arguments), I just implemented a function
    to handle parsing the input text file, called handleFile(). The handleFile() function is located in custom.h
    because I leveraged it also in the multi commander. I tested the custom commander by using a multitude of 
    commands: cd, ls, pwd, whoami; all with variations of arguemnts to make sure the lines were parsed and 
    handled correctly.

Multi Commander
    For the multi commander, I first check if there are additional command line arguments that indicate which
    lines from the text file should be backgrounded. If there are no additional arugments, then I just call the
    handleFile() function which was defined in the custom.h file. In the case that there are lines that need to
    be backgrounded, then I proceed with the handleBackground() function. The algorithm that I devloped was to
    iterate through the lines of the text file, forking on every line. Then if the fork is a child, then I start
    the timer, and grab the info on the rusage, forking again to create a grandchild. Then for that fork, if it
    is the grandchild, then execute the command. If it is the child, i.e., the parent of the grandchild, then
    I call wait(NULL) and get the statistics for the process, then exit. Now, we've handled the case for the
    first fork being the child, and now need to handle the parent process. For the parent process, I check if
    the process is supposed to be backgrounded. If the line number is one that should be backgrounded, then I
    store its pid in a pre-allocated list of integers, but only if it isn't the first backgrounded process.
    This means that if I have multiple processes that need to be backgrounded, I only store the pids of the
    subsequent backgrounded lines, not the first one, e.g., if I am backgrounding lines 1, 3, and 4, then I only
    store the pids for the line 3 and 4 processes. Now back to the case that the line is not one that needs to
    be backgrounded. In this case, I have a regular wait for that child pid, to make sure that we do not continue
    execution until that child, the grandchild, has finished. Then, after we have iterated through all of the
    lines of the text file and the while loop finished, I use a wait(NULL) to handle the first backgrounded
    process. Then all of the subsequent backgrounded processes' pids are iterated through and waited for, thus,
    handling the case where they took longer to finish than the rest of the processes. To test the funcitonality
    I backgrounded multiple sleep calls with varying times, making sure that they were waited for before the
    program finished and exited. I tried with only backgrounding one line and also with backgrounded multiple
    lines, handling all the cases that could be tried. I also made sure that my program worked when there were
    no lines that needed o be backgrounded.
