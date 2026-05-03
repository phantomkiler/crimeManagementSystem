#include "AdvancedFeatures.h"
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>

namespace AdvancedFeatures {

    // 4. Predictive Crime Mapping
    std::vector<std::string> PredictiveMapping::getHighRiskZones(const std::vector<std::string>& pastLocations) {
        std::map<std::string, int> hotspots;
        for (const auto& loc : pastLocations) {
            hotspots[loc]++;
        }
        
        // Convert to vector of pairs to sort by frequency
        std::vector<std::pair<std::string, int>> sortedHotspots(hotspots.begin(), hotspots.end());
        std::sort(sortedHotspots.begin(), sortedHotspots.end(),
                  [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                      return a.second > b.second;
                  });

        std::vector<std::string> highRiskZones;
        for (size_t i = 0; i < sortedHotspots.size() && i < 5; ++i) { // Return top 5
            highRiskZones.push_back(sortedHotspots[i].first);
        }
        return highRiskZones;
    }

    // 5. Automated Threat Triage
    int ThreatTriage::calculateThreatScore(const std::string& description) {
        int score = 0;
        std::string lowerDesc = description;
        std::transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(), ::tolower);

        // Simple weighted keyword dictionary
        std::unordered_map<std::string, int> keywords = {
            {"gun", 50}, {"weapon", 40}, {"blood", 75}, {"kill", 80}, {"murder", 90},
            {"noise", 5}, {"suspicious", 10}, {"robbery", 30}, {"assault", 50}, {"fire", 60}
        };

        for (const auto& pair : keywords) {
            if (lowerDesc.find(pair.first) != std::string::npos) {
                score += pair.second;
            }
        }
        return score;
    }

    // 6. RBAC with Activity Anomalies
    bool AnomalyDetector::logAndCheckAnomaly(const std::string& username, const std::string& action) {
        time_t now = time(0);
        
        // Reset count if more than 2 minutes (120 seconds) have passed
        if (now - lastActionTime[username] > 120) {
            userActionCounts[username] = 0;
        }

        userActionCounts[username]++;
        lastActionTime[username] = now;

        // If user performs more than 10 actions in 2 minutes, flag as anomaly
        if (userActionCounts[username] > 10) {
            return true;
        }
        return false;
    }

    // 1. Graph-Based Syndicate Mapping
    void SyndicateGraph::addEdge(const std::string& personA, const std::string& personB) {
        network[personA].push_back(personB);
        network[personB].push_back(personA); // undirected graph
    }

    std::vector<std::string> SyndicateGraph::getNetworkBFS(const std::string& startNode) {
        std::vector<std::string> result;
        if (network.find(startNode) == network.end()) return result;

        std::unordered_map<std::string, bool> visited;
        std::queue<std::string> q;

        q.push(startNode);
        visited[startNode] = true;

        while (!q.empty()) {
            std::string curr = q.front();
            q.pop();
            result.push_back(curr);

            for (const std::string& neighbor : network[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return result;
    }

    // 2. Cryptographic Evidence Chain (FNV-1a Hash)
    unsigned int CryptoEvidence::calculateFNV1a(const std::string& data) {
        unsigned int hash = 2166136261u;
        unsigned int fnv_prime = 16777619u;

        for (char c : data) {
            hash ^= (unsigned char)c;
            hash *= fnv_prime;
        }
        return hash;
    }

    bool CryptoEvidence::verifyEvidence(const std::string& data, unsigned int storedHash) {
        return calculateFNV1a(data) == storedHash;
    }

    // 3. Fuzzy Biometric Matching
    int BiometricMatcher::calculateLevenshtein(const std::string& a, const std::string& b) {
        size_t m = a.length();
        size_t n = b.length();
        
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

        for (size_t i = 0; i <= m; i++) dp[i][0] = i;
        for (size_t j = 0; j <= n; j++) dp[0][j] = j;

        for (size_t i = 1; i <= m; i++) {
            for (size_t j = 1; j <= n; j++) {
                if (a[i - 1] == b[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
                }
            }
        }
        return dp[m][n];
    }

    float BiometricMatcher::getSimilarityPercentage(const std::string& a, const std::string& b) {
        int distance = calculateLevenshtein(a, b);
        size_t maxLen = std::max(a.length(), b.length());
        if (maxLen == 0) return 100.0f;
        return (1.0f - ((float)distance / maxLen)) * 100.0f;
    }

}
