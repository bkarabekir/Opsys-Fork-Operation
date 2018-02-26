/**
 * Created by burakkarabekir on 26/02/18.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define TREE_HEIGHT 3
//**function prototypes**//
int readFile(int pid);

void processChild();

int processParent(int leftPid, int rightPid);

void tree(int height);

int readFile(int pid) { // read file content as integer with given pid
    wait(NULL);
    char fname[32];
    int tmp;
    sprintf(fname, "%d.txt", pid);// sprintf(str, "Value of Pi = %f", M_PI);
    FILE *fp = fopen(fname, "r");
    fscanf(fp, "%d", &tmp);//fscanf puts the int(int file) in int r values address.
    fclose(fp);
    return tmp;
}

void processChild() {
    srand(getpid());
    int rnd = rand() % 512;
    char fname[32];
    sprintf(fname, "%d.txt", getpid());//creation of a text file by using pid.
    printf("Random-Number :%d by generated %d\n", rnd, getpid());
    fflush(stdout);
    printf("\nFilename :%s\n", fname);
    FILE *fp = fopen(fname, "w");
    fprintf(fp, "%d", rnd);//write random number into the text file.
    fclose(fp);
}

int processParent(int leftPid, int rightPid) {
    int leftValue = readFile(leftPid);
    int rightValue = readFile(rightPid);
    int sum = leftValue + rightValue;
    char fname[20];
    sprintf(fname, "%d.txt", getpid());//file naming
    printf("\nFilename :%s\n", fname);
    FILE *fp = fopen(fname, "w");
    fprintf(fp, "%d", sum);//writing sum of the files.
    printf("Parent`s sum: %d from %d\n", sum, getpid());
    fclose(fp);
    return sum;
}

void tree(int height) {
    //printf("--height:%d\n\n\n ",height);
    if (height <= 0) {
        processChild();
        return;
    }
    int pid0 = fork(); // new process
    if (pid0 >= 0) {
        if (pid0 > 0) { // parent
            int pid1 = fork();
            if (pid1 > 0) {//parent
                printf("\n--Parent Processes--\n");
                printf("parent: %d\nleft-child: %d\nright-child: %d\nheight: %d\n",
                       getpid(), pid0, pid1, height);
                wait(NULL); // after this point all childeren is finished
                //read file sum them up
                int total = processParent(pid0, pid1);
                if (height == TREE_HEIGHT)
                    printf("Sum of All: %d\n", total);
            }
            if (pid1 == 0) { //right child
                tree(--height);
            }
        }
        if (pid0 == 0) {//access parent
            tree(--height);
        } else { /* error fork return -1 */
            perror("\nfork"); /* error message */
            exit(0);
        }
    }
}

int main() {
    tree(TREE_HEIGHT);
}