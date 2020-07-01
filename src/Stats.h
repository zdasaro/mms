#pragma once

#include <QLineEdit>
#define NUMBER_OF_INT_STATS 6
#define NUMBER_OF_FLOAT_STATS 3
#define NUMBER_OF_STATS 10
#define UNSOLVED_SCORE 1000

namespace mms {

enum StatsEnum{TOTAL_DISTANCE, TOTAL_TURNS, BEST_RUN_DISTANCE, BEST_RUN_TURNS, CURRENT_RUN_DISTANCE, CURRENT_RUN_TURNS, // integers
               TOTAL_EFFECTIVE_DISTANCE, BEST_RUN_EFFECTIVE_DISTANCE, CURRENT_RUN_EFFECTIVE_DISTANCE, // floats
               SCORE // has a text box but is not saved in an array
              };

class Stats
{
public:
    Stats();
    void reset(StatsEnum stat);
    void resetAll();
    void addDistance(int distance = 1);
    void addTurn();
    void bindText(StatsEnum stat, QLineEdit* uiText);
    void startRun();
    void finishRun(bool reachedGoal);
    void penalizeForReset();

private:
    int intStatValues[NUMBER_OF_INT_STATS]; // total distance, turns
    float floatStatValues[NUMBER_OF_FLOAT_STATS]; // effective distance, score
    QLineEdit* textField[NUMBER_OF_STATS] = {nullptr};
    bool startedRun;
    bool solved;
    void updateScore();
    void increment(StatsEnum stat, int increase);
    void increment(StatsEnum stat, float increase);
    void setStat(StatsEnum stat, int value);
    void setStat(StatsEnum stat, float value);
    static float getEffectiveDistance(int distance);
    float penalty = 0;
};

}
