#include "../include/SelectionPolicy.h"
#include "../include/Facility.h"
#include <limits>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

NaiveSelection::NaiveSelection()
    : lastSelectedIndex(-1) {}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available.");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available.");
    }

    int bestIndex = 0;
    int minDifference = std::numeric_limits<int>::max();
    for (size_t i = 0; i < facilitiesOptions.size(); i++)
    {
        int lifeQuality = facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore;
        int economy = facilitiesOptions[i].getEconomyScore() + EconomyScore;
        int environment = facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore;

        int maxScore = std::max({lifeQuality, economy, environment});
        int minScore = std::min({lifeQuality, economy, environment});
        int difference = maxScore - minScore;

        if (difference < minDifference)
        {
            minDifference = difference;
            bestIndex = i;
        }
    }
    LifeQualityScore = facilitiesOptions[bestIndex].getLifeQualityScore();
    EnvironmentScore = facilitiesOptions[bestIndex].getEnvironmentScore();
    EconomyScore = facilitiesOptions[bestIndex].getEconomyScore();
    return facilitiesOptions[bestIndex];
}

const string BalancedSelection::toString() const
{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

EconomySelection::EconomySelection()
    : lastSelectedIndex(-1) {}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available.");
    }

    size_t bestIndex = 0;
    int bestEconomy = facilitiesOptions[0].getEconomyScore();

    for (size_t i = 1; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getEconomyScore() > bestEconomy)
        {
            bestEconomy = facilitiesOptions[i].getEconomyScore();
            bestIndex = i;
        }
    }
    return facilitiesOptions[bestIndex];
}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection()
    : lastSelectedIndex(-1) {}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available.");
    }

    size_t bestIndex = 0;
    int bestEnvironment = facilitiesOptions[0].getEnvironmentScore();

    for (size_t i = 1; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getEnvironmentScore() > bestEnvironment)
        {
            bestEnvironment = facilitiesOptions[i].getEnvironmentScore();
            bestIndex = i;
        }
    }
    return facilitiesOptions[bestIndex];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}