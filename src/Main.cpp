#include <stdio.h>
#include "Core/Application.h"

int main(int argc, char* argv[])
{
    printf("\n\n========= APPLICATION STARTS =========\n\n");
    for (int i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
        if (i == argc - 1) { printf("\n"); }
    }

    // Create application and start the game loop @ run()
    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();
    
    printf("\n========= EXITING APPLICATION =========\n");
    
    return 0;
}
