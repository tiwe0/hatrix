//
// Created by 张政 on 2024/12/17.
//

#ifndef HATRIX_SHADOWCASTING_HPP
#define HATRIX_SHADOWCASTING_HPP

#define BIG_NUMBER 25565

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>

// Utility function to round ties up (equivalent to round_ties_up in Python)
int round_ties_up(float n) {
    return static_cast<int>(std::floor(n + 0.5f));
}

// Utility function to round ties down (equivalent to round_ties_down in Python)
int round_ties_down(float n) {
    return static_cast<int>(std::ceil(n - 0.5f));
}

// Enum to represent the four cardinal directions
enum Cardinal { North = 0, East = 1, South = 2, West = 3 };

// Function to transform a tile based on quadrant and cardinal direction
std::pair<int, int> transform(Cardinal cardinal, std::pair<int, int> origin, std::pair<int, int> tile) {
    int row = tile.first, col = tile.second;
    int ox = origin.first, oy = origin.second;

    if (cardinal == North) {
        return std::make_pair(ox + col, oy - row);
    } else if (cardinal == South) {
        return std::make_pair(ox + col, oy + row);
    } else if (cardinal == East) {
        return std::make_pair(ox + row, oy + col);
    } else if (cardinal == West) {
        return std::make_pair(ox - row, oy + col);
    }
    return std::make_pair(ox, oy);  // Default, should not reach here
}

// Function to compute slope
float slope(const std::pair<int, int>& tile) {
    int row_depth = tile.first;
    int col = tile.second;
    return (2 * col - 1) / (2.0f * row_depth);  // Using float for slope calculation
}

// Row struct to represent each row's depth and slopes
struct Row {
    int depth;
    float start_slope, end_slope;

    Row(int depth, float start_slope, float end_slope)
            : depth(depth), start_slope(start_slope), end_slope(end_slope) {}
};

// Check if a tile is symmetric
bool is_symmetric(const Row& row, const std::pair<int, int>& tile) {
    int col = tile.second;
    return (col >= row.depth * row.start_slope && col <= row.depth * row.end_slope);
}

// Function to generate tiles for a given row and maximum distance
std::vector<std::pair<int, int>> tiles(const Row& row, float max_distance) {
    int min_col = round_ties_up(row.depth * row.start_slope);
    int max_col = round_ties_down(row.depth * row.end_slope);
    std::vector<std::pair<int, int>> result;
    for (int col = min_col; col <= max_col; ++col) {
        if (std::sqrt(row.depth * row.depth + col * col) > max_distance) {
            continue;  // Skip tiles that are too far
        }
        result.push_back(std::make_pair(row.depth, col));
    }
    return result;
}

// Function to get the next row in the scan (pure function version)
Row next_row(const Row& row) {
    return Row(row.depth + 1, row.start_slope, row.end_slope);
}

// Function to perform iterative scan for the FOV calculation
void scan_iterative(const Row& row, const std::pair<int, int>& origin,
                    const std::function<bool(int, int)>& is_wall,
                    const std::function<void(int, int)>& reveal,
                    const std::function<bool(int, int)>& is_floor,
                    float max_distance) {
    std::vector<Row> rows = {row};

    while (!rows.empty()) {
        Row current_row = rows.back();  // Equivalent to row = rows.pop()
        rows.pop_back();  // Remove the last element

        std::pair<int, int> prev_tile = {BIG_NUMBER, BIG_NUMBER};  // Initialize with an invalid tile
        for (const auto& tile : tiles(current_row, max_distance)) {
            if (is_wall(tile.first, tile.second) || is_symmetric(current_row, tile)) {
                reveal(tile.first, tile.second);
            }

            if (is_wall(prev_tile.first, prev_tile.second) && is_floor(tile.first, tile.second)) {
                current_row.start_slope = slope(tile);
            }

            if (is_floor(prev_tile.first, prev_tile.second) && is_wall(tile.first, tile.second)) {
                Row next = next_row(current_row);
                next.end_slope = slope(tile);
                rows.push_back(next);  // Add the next row to the stack
            }

            prev_tile = tile;
        }

        if (current_row.depth <= max_distance && is_floor(prev_tile.first, prev_tile.second)) {
            rows.push_back(next_row(current_row));  // Add the next row if last tile was floor
        }
    }
}

// Function to compute field of view (pure function version)
void utils_compute_fov(std::pair<int, int> origin,
                 const std::function<bool(int, int)>& is_blocking,
                 const std::function<void(int, int)>& mark_visible,
                 float max_distance) {

    mark_visible(origin.first, origin.second);  // Mark the origin as visible

    for (int i = 0; i < 4; ++i) {
        Cardinal quadrant = static_cast<Cardinal>(i);

        // Reveal function
        auto reveal = [&quadrant, &origin, &mark_visible](int row, int col) {
            auto transformed = transform(quadrant, origin, std::make_pair(row, col));
            mark_visible(transformed.first, transformed.second);
        };

        // Is wall function
        auto is_wall = [&quadrant, &origin, &is_blocking](int row, int col) -> bool {
            if (row == BIG_NUMBER || col == BIG_NUMBER){
                return false;
            };
            auto transformed = transform(quadrant, origin, std::make_pair(row, col));
            return is_blocking(transformed.first, transformed.second);
        };

        // Is floor function
        auto is_floor = [&quadrant, &origin, &is_blocking](int row, int col) -> bool {
            if (row == BIG_NUMBER || col == BIG_NUMBER){
                return false;
            };
            auto transformed = transform(quadrant, origin, std::make_pair(row, col));
            return !is_blocking(transformed.first, transformed.second);
        };

        Row first_row(1, -1.0f, 1.0f);  // Use float for slope
        scan_iterative(first_row, origin, is_wall, reveal, is_floor, max_distance);
    }
}

#endif //HATRIX_SHADOWCASTING_HPP
