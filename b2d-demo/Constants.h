#pragma once

//RENDERING VALUES
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

//TILE VALUES
const int TILE_SIZE = 32;
const int TILES_TOTAL_WIDTH = 200;
const int TILES_TOTAL_HEIGHT = 40;
const int COLOR_SETTINGS_SIZE = 100;

//MAP VALUES
const int MAP_SIZE_WIDTH = TILE_SIZE * TILES_TOTAL_WIDTH;
const int MAP_SIZE_HEIGHT = TILE_SIZE * TILES_TOTAL_HEIGHT;

const int HeightY = MAP_SIZE_HEIGHT - WINDOW_HEIGHT;

const float DEG = 57.29577f;

float dt;
float timeStep = 1.0f / 60.0f;