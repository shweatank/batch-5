#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define MAX_POINTS 1000
#define SWIPE_THRESHOLD 50
#define CLOSE_THRESHOLD 30

typedef struct {
    int x;
    int y;
} Point;

Point points[MAX_POINTS];
int point_count = 0;

void reset_points() {
    point_count = 0;
}

double distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

void detect_gesture() {
    if (point_count < 2) return;

    Point start = points[0];
    Point end   = points[point_count - 1];

    int dx = end.x - start.x;
    int dy = end.y - start.y;

    // Swipe Detection
    if (abs(dx) > abs(dy)) {
        if (dx > SWIPE_THRESHOLD)
            printf("Swipe Right\n");
        else if (dx < -SWIPE_THRESHOLD)
            printf("Swipe Left\n");
    } else {
        if (dy > SWIPE_THRESHOLD)
            printf("Swipe Down\n");
        else if (dy < -SWIPE_THRESHOLD)
            printf("Swipe Up\n");
    }

    // Simple Square Detection (Closed shape)
    if (distance(start, end) < CLOSE_THRESHOLD) {
        printf("Closed Shape Detected (Possible Square)\n");
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Touch Simulation",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          800, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event event;
    int running = 1;
    int drawing = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                drawing = 1;
                reset_points();
            }

            if (event.type == SDL_MOUSEMOTION && drawing) {
                if (point_count < MAX_POINTS) {
                    points[point_count].x = event.motion.x;
                    points[point_count].y = event.motion.y;
                    point_count++;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                drawing = 0;
                detect_gesture();
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for (int i = 1; i < point_count; i++) {
            SDL_RenderDrawLine(renderer,
                points[i-1].x, points[i-1].y,
                points[i].x, points[i].y);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
