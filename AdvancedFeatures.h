#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>

namespace AdvancedFeatures {

    // 4. Predictive Crime Mapping
    class PredictiveMapping {
    public:
        // Analyzes locations and returns highest risk zones
        static std::vector<std::string> getHighRiskZones(const std::vector<std::string>& pastLocations);
    };

    // 5. Automated Threat Triage
    class ThreatTriage {
    public:
        static int calculateThreatScore(const std::string& description);
    };

    // 6. RBAC with Activity Anomalies
    class AnomalyDetector {
    private:
        std::unordered_map<std::string, int> userActionCounts;
        std::unordered_map<std::string, time_t> lastActionTime;
    public:
        bool logAndCheckAnomaly(const std::string& username, const std::string& action);
    };

    // 1. Graph-Based Syndicate Mapping
    class SyndicateGraph {
    private:
        std::unordered_map<std::string, std::vector<std::string>> network;
    public:
        void addEdge(const std::string& personA, const std::string& personB);
        std::vector<std::string> getNetworkBFS(const std::string& startNode);
    };

    // 2. Cryptographic Evidence Chain (FNV-1a Hash)
    class CryptoEvidence {
    public:
        static unsigned int calculateFNV1a(const std::string& data);
        static bool verifyEvidence(const std::string& data, unsigned int storedHash);
    };

    // 3. Fuzzy Biometric Matching
    class BiometricMatcher {
    public:
        static int calculateLevenshtein(const std::string& a, const std::string& b);
        static float getSimilarityPercentage(const std::string& a, const std::string& b);
    };

}
