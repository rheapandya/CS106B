/* HEADER:
 * Rhea Pandya
 * Michael Young
 * I implemented the floodedRegionsIn function in this file RisingTides.cpp which is a tool that models flooding due to sea level rise.
 */

#include "RisingTides.h"

using namespace std;


/* floodedRegionsIn is a function that takes in terrain which is a grid of doubles (a list of locations of water sources),
 * sources which is a vector containing GridLocations (what square the water originates from),
 * and height, a double that indicates the height of the water at the source(s).
 * This function uses the breadth-first search algorithm to determine which regions are under water.
 * Water flows up, down, left, and right and will submerge any region whose height is less than or equal to the height of the water (passed in as a parameter).
 * This function returns a grid of booleans where true means flooded and false means not flooded.
 */
Grid<bool> floodedRegionsIn(const Grid<double>& terrain,
                            const Vector<GridLocation>& sources,
                            double height) {
    // new grid to flood
    Grid<bool> newTerrain(terrain.numRows(), terrain.numCols());

    // creates an empty queue to put flooded squares;
    Queue<GridLocation> floodedSquares;

    // GridLocation to hold location of the adjacent square
    GridLocation newLocation;

    // a grid of directions to cycle through for adjacent squares
    Grid <int> directions = {
        {0, -1}, // left
        {0, 1},  // right
        {-1, 0}, // up
        {1, 0}   // down
    };

    // for each water source at or below the water level, flood that square, and add that square to the queue

    for (GridLocation source: sources){
        newTerrain.set(source, true);
        floodedSquares.enqueue(source);
    }

    // while the queue isnt empty dequeue a position from the front of the queue
    while(!floodedSquares.isEmpty()){
        GridLocation currentSquare = floodedSquares.dequeue();

        // cycle through each square adjacent to the position in a cardinal direction
        for (int i = 0; i < 4; i++) {
            newLocation.row = currentSquare.row + directions[i][0];
            newLocation.col = currentSquare.col + directions[i][1];
            // check if this newLocation is in bounds, not filled, and at or below the water level
            if (terrain.inBounds(newLocation) &&
                !newTerrain.get(newLocation.row, newLocation.col) &&
                terrain.get(newLocation.row, newLocation.col) <= height) {
                // flood the square and add it to the queue
                newTerrain.set(newLocation, true);
                floodedSquares.enqueue(newLocation);
            }
        }
    }

    return newTerrain;
}



/***** Test Cases Below This Point *****/
#include "GUI/SimpleTest.h"
PROVIDED_TEST("Nothing gets wet if there are no water sources.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        // empty
    };

    /* There are no water sources, so nothing should be underwater. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Everything gets wet if all locations are below the water level.") {
    Grid<double> world = {
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Everything should flood; there are no barriers to stop the water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true,  true },
        {  true,  true,  true },
        {  true,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't cross a vertical levee.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 0, 2, 0 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Only locations to the left of the barrier should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true, false, false },
        {  true, false, false },
        {  true, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't cross a diagonal levee.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* Water only flows in the four cardinal directions, so it can't
     * pass through the barrier. Only the top should be flooded.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can't flow diagonally.") {
    Grid<double> world = {
        { 0, 2, 0 },
        { 2, 0, 2 },
        { 0, 2, 0 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* Water should be trapped in the center, since it can't move
     * diagonally.
     */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false, false, false },
        { false,  true, false },
        { false, false, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can flow in all cardinal directions.") {
    Grid<double> world = {
        { 2, 0, 2 },
        { 0, 0, 0 },
        { 2, 0, 2 }
    };

    Vector<GridLocation> sources = {
        { 1, 1 }
    };

    /* The water in this case should flow up, down, left, and right. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        { false,  true, false },
        {  true,  true,  true },
        { false,  true, false }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Water can flow from multiple sources.") {
    Grid<double> world = {
        { 0, 0, 2 },
        { 0, 2, 0 },
        { 2, 0, 0 }
    };

    Vector<GridLocation> sources = {
        { 0, 0 },
        { 2, 2 }
    };

    /* Everything except the levee should be under water. */
    Grid<bool> water = floodedRegionsIn(world, sources, 1.0);
    Grid<bool> expected = {
        {  true,  true, false },
        {  true, false,  true },
        { false,  true,  true }
    };

    EXPECT_EQUAL(water, expected);
}

PROVIDED_TEST("Handles asymmetric worlds and non-square grids") {
    Grid<double> world = {
        { 3, 1, 4, 1 },
        { 5, 9, 2, 6 },
        { 5, 3, 5, 8 }
    };

    /* Initial test - water shouldn't leak out from the 2 if the height is 3.5. */
    Vector<GridLocation> sources = {
        { 1, 2 }
    };

    Grid<bool> expected = {
        { false, false, false, false },
        { false, false,  true, false },
        { false, false, false, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 3.5), expected);

    /* Now, increase the water height to 4.5. */
    expected = {
        {  true,  true,  true,  true },
        { false, false,  true, false },
        { false, false, false, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 4.5), expected);

    /* Now, increase the water height to 5.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true, false,  true, false },
        {  true,  true,  true, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 5.5), expected);

    /* Now, increase the water height to 6.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true, false,  true,  true },
        {  true,  true,  true, false },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 6.5), expected);

    /* Now, increase the water height to 9.5. */
    expected = {
        {  true,  true,  true,  true },
        {  true,  true,  true,  true },
        {  true,  true,  true,  true },
    };

    EXPECT_EQUAL(floodedRegionsIn(world, sources, 9.5), expected);
}

PROVIDED_TEST("Stress test: Handles a large, empty world quickly.") {
    Grid<double> world(100, 100); // Large world, everything defaults to 0 height.
    Vector<GridLocation> sources = {
        { 0, 0 }
    };

    /* This may take a long time to complete if the solution is inefficient. Look
     * for things like
     *
     * 1. passing around large objects by *value* rather than by *reference*,
     * 2. revisiting the same squares multiple times (e.g. flooding the same
     *    cell many times due to not checking if something is flooded),
     *
     * etc.
     *
     * We computed the 0.1s cutoff by taking our reference solution, running it
     * on a middle-of-the-line computer, and multiplying its runtime by 10.
     */
    Grid<bool> water;
    EXPECT_COMPLETES_IN(0.1,
        water = floodedRegionsIn(world, sources, 1.0);
    );
    EXPECT_EQUAL(water.numRows(), world.numRows());
    EXPECT_EQUAL(water.numCols(), world.numCols());

    /* Everything should be flooded. */
    for (int row = 0; row < world.numRows(); row++) {
        for (int col = 0; col < world.numCols(); col++) {
            EXPECT_EQUAL(water[row][col], true);
        }
    }
}
