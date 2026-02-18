#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define MAX_POINTS 3000

#define CLOSE_THRESHOLD 40
#define SWIPE_THRESHOLD 80

typedef struct {
    float x;
    float y;
} Point;

Point points[MAX_POINTS];
int point_count = 0;

float distance(Point a, Point b) {
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

void detect_swipe() {

    Point start = points[0];
    Point end   = points[point_count-1];

    float dx = end.x - start.x;
    float dy = end.y - start.y;

    if(fabs(dx) > fabs(dy)) {
        if(dx > SWIPE_THRESHOLD)
            printf("Swipe Right\n");
        else if(dx < -SWIPE_THRESHOLD)
            printf("Swipe Left\n");
    } else {
        if(dy > SWIPE_THRESHOLD)
            printf("Swipe Down\n");
        else if(dy < -SWIPE_THRESHOLD)
            printf("Swipe Up\n");
    }
}

void detect_shape() {

    if(point_count < 30)
        return;

    /* Check closed */
    if(distance(points[0], points[point_count-1]) > CLOSE_THRESHOLD) {
        printf("Open Shape → ");
        detect_swipe();
        return;
    }

    float min_x=10000, max_x=0, min_y=10000, max_y=0;

    for(int i=0;i<point_count;i++) {
        if(points[i].x < min_x) min_x = points[i].x;
        if(points[i].x > max_x) max_x = points[i].x;
        if(points[i].y < min_y) min_y = points[i].y;
        if(points[i].y > max_y) max_y = points[i].y;
    }

    float width  = max_x - min_x;
    float height = max_y - min_y;

    float aspect_ratio = width / height;

    /* Calculate approximate area using bounding box */
    float bbox_area = width * height;

    /* Approximate drawn area by counting points inside */
    float path_area = 0;
    for(int i=1;i<point_count;i++) {
        path_area += fabs(points[i].x * points[i-1].y -
                          points[i-1].x * points[i].y);
    }
    path_area = path_area / 2;

    float area_ratio = path_area / bbox_area;

    printf("Aspect ratio: %.2f  Area ratio: %.2f\n",
           aspect_ratio, area_ratio);

    /* Triangle */
    if(area_ratio < 0.6) {
        printf("Triangle Detected\n");
        return;
    }

    /* Square */
    if(aspect_ratio > 0.8 && aspect_ratio < 1.2) {
        printf("Square Detected\n");
        return;
    }

    /* Rectangle */
    if(aspect_ratio >= 1.2 || aspect_ratio <= 0.8) {
        printf("Rectangle Detected\n");
        return;
    }

    printf("Shape Not Recognized\n");
}

int main() {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Stable Working Shape Detector",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,600,0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

    SDL_Event event;
    int running=1, drawing=0;

    while(running) {

        while(SDL_PollEvent(&event)) {

            if(event.type == SDL_QUIT)
                running=0;

            if(event.type == SDL_MOUSEBUTTONDOWN) {
                drawing=1;
                point_count=0;
            }

            if(event.type == SDL_MOUSEMOTION && drawing) {
                if(point_count < MAX_POINTS)
                    points[point_count++] =
                        (Point){event.motion.x, event.motion.y};
            }

            if(event.type == SDL_MOUSEBUTTONUP) {
                drawing=0;
                detect_shape();
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer,0,255,0,255);

        for(int i=1;i<point_count;i++)
            SDL_RenderDrawLine(renderer,
                points[i-1].x, points[i-1].y,
                points[i].x, points[i].y);

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}
