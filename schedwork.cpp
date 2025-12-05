#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix &avail, const size_t dailyNeed,
                    const size_t maxShifts, DailySchedule &sched,
                    std::vector<size_t> &dailyCounts, size_t day, size_t col);

// Add your implementation of schedule() and other helper functions here

bool schedule(const AvailabilityMatrix &avail, const size_t dailyNeed,
              const size_t maxShifts, DailySchedule &sched) {
  if (avail.size() == 0U) {
    return false;
  }
  sched.clear();

  // Setup 1: Initialize schedule with dimensions N x D
  // Loop 1 allowed
  for (size_t i = 0; i < avail.size(); i++) {
    std::vector<Worker_T> daySched(dailyNeed, INVALID_ID);
    sched.push_back(daySched);
  }

  // Setup 2: Initialize worker shift counts
  size_t numWorkers = avail[0].size();
  // No explicit loop needed here (constructor does it), but counting it as
  // setup is fine.
  std::vector<size_t> dailyCounts(numWorkers, 0);

  // Start recursion
  return scheduleHelper(avail, dailyNeed, maxShifts, sched, dailyCounts, 0, 0);
}

bool scheduleHelper(const AvailabilityMatrix &avail, const size_t dailyNeed,
                    const size_t maxShifts, DailySchedule &sched,
                    std::vector<size_t> &dailyCounts, size_t day, size_t col) {
  // Base Case: All days filled
  if (day == avail.size()) {
    return true;
  }

  // Determine next cell coordinates
  size_t nextDay = day;
  size_t nextCol = col + 1;
  if (nextCol == dailyNeed) {
    nextDay = day + 1;
    nextCol = 0;
  }

  size_t numWorkers = avail[0].size();

  // Recursive Step: Try to fill sched[day][col]
  // Loop 2 allowed (Recursive search loop)
  for (Worker_T worker = 0; worker < numWorkers; worker++) {

    // Constraint 1: Worker must be available
    if (!avail[day][worker])
      continue;

    // Constraint 2: Worker must not exceed max shifts
    if (dailyCounts[worker] >= maxShifts)
      continue;

    // Constraint 3: Worker must not already be working this day
    // We only need to check the current day's filled slots (0 to col-1)
    bool alreadyScheduled = false;
    if (std::find(sched[day].begin(), sched[day].begin() + col, worker) !=
        sched[day].begin() + col) {
      alreadyScheduled = true;
    }
    if (alreadyScheduled)
      continue;

    // Do Choice
    sched[day][col] = worker;
    dailyCounts[worker]++;

    // Recurse
    if (scheduleHelper(avail, dailyNeed, maxShifts, sched, dailyCounts, nextDay,
                       nextCol)) {
      return true;
    }

    // Backtrack (Undo Choice)
    sched[day][col] = INVALID_ID;
    dailyCounts[worker]--;
  }

  return false;
}
