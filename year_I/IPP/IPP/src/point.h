
#ifndef MIDDLE_AGES_POINT_H
#define MIDDLE_AGES_POINT_H

/**
 * @file
 * File for representing point structure.
 */


/**
 * @struct defPoint
 * @brief Represents a point in program.
 */
struct defPoint {
    int x; /**< Column number. */
    int y; /**< Row number. */
};

/**
 * @typedef Point
 * @brief Pointer handle for defPoint.
 */
typedef struct defPoint *Point;

/**
 * Compares two points in lexicographical order.
 * Returns 1 if second argument is strictly greater,
 * -1 if is strictly lower and 0 if both arguments
 * are equal.
 * @param[in] p1 First point.
 * @param[in] p2 Second point.
 */

int comparePoints(Point p1, Point p2);

/**
 * Checks whether point is inside the board.
 * Also frees that point pointer.
 */
int validatePoint(Point p, int n);

/**
 * Returns distance between points in maximum metric.
 */
int distance(Point a, Point b);

/**
 * Creates a point from two coordinates.
 */
Point makePoint(int x, int y);

/**
 * Returns a copy of the point.
 */
Point copyPoint(Point p);

#endif //MIDDLE_AGES_POINT_H
