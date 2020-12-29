#ifndef UNIVERSEPARAMETERS_H
#define UNIVERSEPARAMETERS_H

/**
 * @brief The UniverseParameters class is meant to allow the tick methods to
 * easily obtain an expandable selection of user controlled settings, without
 * expanding the function signature repeatedly.
 *
 * These settings are things that interact with swimmers, i.e. can be sensed or
 * have a direct effect on behaviour (physics etc)
 *
 * TODO going forward use this to save/load user settings
 */
class UniverseParameters {
public:
    /// A value that can hopefully be used by swimmers to coordinate behaviour
    /// through time
    double lunarCycle_ = 0.0;
    /// The average number of mutations each offspring will contain, using a
    /// gaussian/normal distribution
    double meanMutationCount_ = 2.0;
    double mutationCountStdDev_ = 1.0;
};

#endif // UNIVERSEPARAMETERS_H
