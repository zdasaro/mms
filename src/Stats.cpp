#include "Stats.h"
#include "AssertMacros.h"

namespace mms{

Stats::Stats() {
}

void Stats::reset(StatsEnum stat) {
    if (stat < NUMBER_OF_INT_STATS) {
        setStat(stat, 0);
    }
    else {
        setStat(stat, float(0));
    }
    updateScore();
}

void Stats::resetAll() {
    startedRun = false;
    solved = false;
    for (int i = 0; i < NUMBER_OF_INT_STATS; i++) {
        intStatValues[i] = 0;
        textField[i]->setText("0");
    }
    for (int i = 0; i < NUMBER_OF_FLOAT_STATS; i++) {
        floatStatValues[i] = 0.0;
        textField[i + NUMBER_OF_INT_STATS]->setText("0");
    }
    // leave placeholder values for Best Run, such that the maximum score will be displayed
    // cannot use setStat here because the value is not displayed
    intStatValues[BEST_RUN_TURNS] = UNSOLVED_SCORE;
    textField[BEST_RUN_TURNS]->setText("");
    intStatValues[BEST_RUN_DISTANCE] = UNSOLVED_SCORE;
    textField[BEST_RUN_DISTANCE]->setText("");
    floatStatValues[BEST_RUN_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS] = UNSOLVED_SCORE;
    textField[BEST_RUN_EFFECTIVE_DISTANCE]->setText("");
    updateScore();
}

void Stats::addDistance(int distance) {
    float effectiveDistance = getEffectiveDistance(distance);
    increment(TOTAL_DISTANCE, distance);
    increment(TOTAL_EFFECTIVE_DISTANCE, effectiveDistance);
    if (startedRun) {
        increment(CURRENT_RUN_DISTANCE, distance);
        increment(CURRENT_RUN_EFFECTIVE_DISTANCE, effectiveDistance);
    }
    updateScore();
}

void Stats::addTurn() {
    increment(TOTAL_TURNS, 1);
    if (startedRun) {
        increment(CURRENT_RUN_TURNS, 1);
    }
    updateScore();
}

void Stats::increment(StatsEnum stat, int increase) {
    setStat(stat, intStatValues[stat] + increase);
}

void Stats::increment(StatsEnum stat, float increase) {
    setStat(stat, floatStatValues[stat - NUMBER_OF_INT_STATS] + increase);
}

void Stats::setStat(StatsEnum stat, int value) {
    ASSERT_LT(stat, NUMBER_OF_INT_STATS);
    intStatValues[stat] = value;
    textField[stat]->setText(QString::number(intStatValues[stat]));
}

void Stats::setStat(StatsEnum stat, float value) {
    ASSERT_LE(NUMBER_OF_INT_STATS, stat);
    ASSERT_LT(stat, NUMBER_OF_INT_STATS + NUMBER_OF_FLOAT_STATS);
    floatStatValues[stat - NUMBER_OF_INT_STATS] = value;
    textField[stat]->setText(QString::number(floatStatValues[stat - NUMBER_OF_INT_STATS]));
}

void Stats::bindText(StatsEnum stat, QLineEdit* uiText) {
    textField[stat] = uiText;
}

void Stats::updateScore() {
    // if solved, score = best_run_turns + best_run_effective_distance + 0.1*(total_turns + total_effective_distance)
    float score;
    if (solved) {
        score = floatStatValues[BEST_RUN_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS] + intStatValues[BEST_RUN_TURNS]
                + 0.1* (floatStatValues[TOTAL_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS] + intStatValues[TOTAL_TURNS]);
    }
    else {
        score = 2*UNSOLVED_SCORE;
    }

    textField[SCORE]->setText(QString::number(score));
}

float Stats::getEffectiveDistance(int distance) {
    return distance > 2 ? distance / 2.0 + 1 : distance;
}

void Stats::startRun() {
    reset(CURRENT_RUN_TURNS);
    reset(CURRENT_RUN_DISTANCE);
    reset(CURRENT_RUN_EFFECTIVE_DISTANCE);
    // add penalty to next run if necessary
    increment(CURRENT_RUN_EFFECTIVE_DISTANCE, penalty);
    increment(TOTAL_EFFECTIVE_DISTANCE, penalty);
    penalty = 0;
    startedRun = true;
}

void Stats::finishRun(bool reachedGoal) {
    startedRun = false;
    if (reachedGoal) {
        solved = true;
        float currentScore = intStatValues[CURRENT_RUN_TURNS] + floatStatValues[CURRENT_RUN_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS];
        float bestScore = intStatValues[BEST_RUN_TURNS] + floatStatValues[BEST_RUN_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS];
        if (currentScore < bestScore) {
            // new best run
            setStat(BEST_RUN_TURNS, intStatValues[CURRENT_RUN_TURNS]);
            setStat(BEST_RUN_DISTANCE, intStatValues[CURRENT_RUN_DISTANCE]);
            setStat(BEST_RUN_EFFECTIVE_DISTANCE, floatStatValues[CURRENT_RUN_EFFECTIVE_DISTANCE - NUMBER_OF_INT_STATS]);
        }
    }
    updateScore();
}

void Stats::penalizeForReset() {
    penalty = 15;
}

}
