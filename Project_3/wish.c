#include <stdio.h>
#include <string.h>

void start_process(void) {

}



int main(int argc, char** argv) {
    if (argc > 2) {
        fprintf(stderr, "usage: ./wish [file]");
        exit(1);
    }
    
    return 0;
}