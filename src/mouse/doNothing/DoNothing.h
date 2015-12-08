#pragma once

#include "../IMouseAlgorithm.h"

namespace doNothing {

class DoNothing : public IMouseAlgorithm {

public:
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

};

} // namespace doNothing
