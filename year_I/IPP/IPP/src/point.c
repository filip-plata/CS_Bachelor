#include <stdlib.h>
#include "point.h"


int comparePoints(struct defPoint *a, struct defPoint *b) {
    if (b->x > a->x) {
        return 1;
    }
    if (b->x < a->x) {
        return -1;
    }
    if (b->y > a->y) {
        return 1;
    }
    if (b->y < a->y) {
        return -1;
    }
    return 0;
}

int distance(Point a, Point b) {
    int distX = abs(a->x - b->x);
    int distY = abs(a->y - b->y);

    return (distX > distY) ? distX : distY;
}

Point makePoint(int x, int y) {
    Point result = (Point) malloc(sizeof(struct defPoint));
    result->x = x;
    result->y = y;
    return result;
}

int validatePoint(Point p, int n) {
    int result = 0;

    if (1<= p->x && p->x <= n && 1<= p->y && p->y <= n) {
        result = 1;
    }

    free(p);
    return result;
}

Point copyPoint(Point p) {
    return makePoint(p->x, p->y);
}
