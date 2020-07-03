#pragma once

#include <QLineEdit>
#include <QMap>
#define NUMBER_OF_INT_STATS 6
#define NUMBER_OF_FLOAT_STATS 3
#define NUMBER_OF_STATS 10
#define UNSOLVED_SCORE 2000

namespace mms {

enum StatsEnum{TOTAL_DISTANCE,
               TOTAL_TURNS,
               BEST_RUN_DISTANCE,
               BEST_RUN_TURNS,
               CURRENT_RUN_DISTANCE,
               CURRENT_RUN_TURNS, // integers
               TOTAL_EFFECTIVE_DISTANCE,
               BEST_RUN_EFFECTIVE_DISTANCE,
               CURRENT_RUN_EFFECTIVE_DISTANCE, // floats
               SCORE // has a text box but is not saved in an array
              };

class Stats
{
public:
    Stats();
    void resetAll(); // Reset all score stats
    void addDistance(int distance = 1); // Increase the distance and effective distance
    void addTurn(); // Increment the number of turns
    void bindText(StatsEnum stat, QLineEdit* uiText); // Indicate which QLineEdit to use for that stat
    void startRun(); // A run starts when the mouse exits the starting tile.
    void finishRun(); // A run finishes when the mouse enters the goal.
    void endUnfinishedRun(); // A run ends unfinished when the mouse returns to the start tile
    void penalizeForReset(); // Applies a penalty when the mouse resets to the start tile

private:
//    int intStatValues[NUMBER_OF_INT_STATS]; // total distance, turns
    QMap<StatsEnum, int> intStatValues;
//    float floatStatValues[NUMBER_OF_FLOAT_STATS]; // effective distance, score
    QMap<StatsEnum, float> floatStatValues;
    QLineEdit* textField[NUMBER_OF_STATS];
    bool startedRun;
    bool solved;
    float penalty;
    void updateScore();
    void increment(StatsEnum stat, int increase);
    void increment(StatsEnum stat, float increase);
    void setStat(StatsEnum stat, int value);
    void setStat(StatsEnum stat, float value);
    static float getEffectiveDistance(int distance);
    void reset(StatsEnum stat);
};

}
