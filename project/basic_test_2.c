#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_POINTS 2000
#define CLOSE_THRESHOLD 30
#define ANGLE_THRESHOLD 25
#define MIN_SIDE 30

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
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double angle(Point a, Point b, Point c) {
    double ab = distance(a,b);
    double bc = distance(b,c);
    double ac = distance(a,c);

    return acos((ab*ab + bc*bc - ac*ac)/(2*ab*bc)) * 180/M_PI;
}

// Detect corners by checking angle changes
int detect_corners(Point corners[]) {
    int count = 0;

    for(int i=10; i<point_count-10; i++) {
        double ang = angle(points[i-10], points[i], points[i+10]);

        if(fabs(ang - 180) > ANGLE_THRESHOLD) {
            if(count < 10)
                corners[count++] = points[i];
        }
    }

    return count;
}

void detect_shape() {
    if(point_count < 20) return;

    if(distance(points[0], points[point_count-1]) > CLOSE_THRESHOLD) {
        printf("Open Shape (Maybe Alphabet)\n");
        return;
    }

    Point corners[10];
    int corner_count = detect_corners(corners);

    printf("Corners detected: %d\n", corner_count);

    if(corner_count == 3)
        printf("Triangle Detected\n");

    else if(corner_count == 4) {
        double d1 = distance(corners[0], corners[2]);
        double d2 = distance(corners[1], corners[3]);

        if(fabs(d1 - d2) < 40)
            printf("Square Detected\n");
        else
            printf("Rectangle / Rhombus Detected\n");
    }

    else if(corner_count == 5)
        printf("Pentagon Detected\n");

    else
        printf("Unknown Closed Shape\n");
}

// Very basic alphabet detection
void detect_alphabet() {
    if(point_count < 20) return;

    int min_x=10000,max_x=0,min_y=10000,max_y=0;

    for(int i=0;i<point_count;i++) {
        if(points[i].x < min_x) min_x = points[i].x;
        if(points[i].x > max_x) max_x = points[i].x;
        if(points[i].y < min_y) min_y = points[i].y;
        if(points[i].y > max_y) max_y = points[i].y;
    }

    int width = max_x - min_x;
    int height = max_y - min_y;

    if(height > width*1.5)
        printf("Possible Capital I\n");

    else if(width > height*1.5)
        printf("Possible Capital Z or -\n");

    else if(width > 50 && height > 50)
        printf("Possible Capital O\n");

    else
        printf("Unknown Alphabet Shape\n");
}

int main() {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Gesture Detection",
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
                reset_points();
            }

            if(event.type == SDL_MOUSEMOTION && drawing) {
                if(point_count < MAX_POINTS) {
                    points[point_count].x = event.motion.x;
                    points[point_count].y = event.motion.y;
                    point_count++;
                }
            }

            if(event.type == SDL_MOUSEBUTTONUP) {
                drawing=0;

                if(distance(points[0], points[point_count-1]) < CLOSE_THRESHOLD)
                    detect_shape();
                else
                    detect_alphabet();
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer,0,255,0,255);

        for(int i=1;i<point_count;i++) {
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
