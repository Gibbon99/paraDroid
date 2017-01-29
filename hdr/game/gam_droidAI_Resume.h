#pragma once

// Checks to see if the droid needs to return to patrolling
int ai_isNotPatrolling(int whichDroid);

// Need to find the nearest WP to move to
int ai_onResumeDest(int whichDroid);

// Work out the AStar path to the destination
int ai_reachedResumeDest(int whichDroid);

// Arrived at the destination
int ai_foundResumeDest(int whichDroid);