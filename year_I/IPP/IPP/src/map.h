/**
 * @brief
 * Map utility.
 * @file
 * Provides basic functionality of a map,
 * like insertion, removal and retrieval.
 *
 * Mapping is from points to pieces, but
 * could be easily adapted to be from
 * void* to void*.
 */

#ifndef MIDDLE_AGES_MAP_H
#define MIDDLE_AGES_MAP_H

#include "piece.h"
#include "point.h"

/**
 * @typedef struct def_map *Map
 * @brief Pointer handle for a map structure.
 */
typedef struct def_map *Map;

/**
 * Map constructor.
 * @param[in] compare Compare function. Should return
 *                    1, 0, -1 and provide total ordering
 *                    over set of keys.
 * @param destructor Destructor of elements stored in a map.
 */
Map newMap(int (*compare)(Point, Point), void (*destructor)(Piece));

/**
 * Return piece from a given point.
 * Returns NULL, if key is not present.
 * @param[in] map
 * @param[in] key Key to be searched.
 */
Piece getValue(Map map, Point key);

/**
 * Removes value from key from a map.
 * If key is not present, no action is taken.
 * Returned value is not freed from memory, to
 * avoid excessive memory allocation.
 * @param[in] map
 * @param[in] key Key to be removed
 */
void removeValue(Map map, Point key);

/**
 * Insert into map binding from key to value.
 * If key is already present, passed key pointer
 * is freed and no further action is taken.
 */
void insertValue(Map map, Point key, Piece value);

/**
 * Deletes map from memory completely.
 */
void destroyMap(Map);


#endif //MIDDLE_AGES_MAP_H
