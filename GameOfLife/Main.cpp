#include <stdio.h>
#include "Application.h"

int main(int argc, char* argv[])
{
    printf("\n\n========= APPLICATION STARTS =========\n\n");
    for (int i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
        if (i == argc - 1) { printf("\n"); }
    }

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();
    
    printf("\n\n========= EXITING APPLICATION =========\n\n");
    
    return 0;
}
