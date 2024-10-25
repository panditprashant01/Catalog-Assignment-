#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

// Function to decode a value from a given base to decimal
long long decode_value(const std::string& base, const std::string& value) {
    int b = std::stoi(base);
    long long result = 0;
    long long multiplier = 1;

    for (int i = value.length() - 1; i >= 0; --i) {
        char digit = value[i];
        if (digit >= '0' && digit <= '9') {
            result += (digit - '0') * multiplier;
        } else if (digit >= 'a' && digit <= 'f') {
            result += (digit - 'a' + 10) * multiplier;
        } else if (digit >= 'A' && digit <= 'F') {
            result += (digit - 'A' + 10) * multiplier;
        }
        multiplier *= b;
    }

    return result;
}

// Function for Lagrange interpolation
long long lagrange_interpolation(const std::vector<int>& x_vals, const std::vector<long long>& y_vals, int k, int x) {
    long long total_sum = 0;

    for (int i = 0; i < k; ++i) {
        long long term = y_vals[i];
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= (x - x_vals[j]);
                term /= (x_vals[i] - x_vals[j]);
            }
        }
        total_sum += term;
    }

    return total_sum;
}

int main() {
    // Input JSON
    std::string json_input = R"({
        "keys": {
            "n": 10,
            "k": 7
        },
        "1": {
            "base": "6",
            "value": "13444211440455345511"
        },
        "2": {
            "base": "15",
            "value": "aed7015a346d63"
        },
        "3": {
            "base": "15",
            "value": "6aeeb69631c227c"
        },
        "4": {
            "base": "16",
            "value": "e1b5e05623d881f"
        },
        "5": {
            "base": "8",
            "value": "316034514573652620673"
        },
        "6": {
            "base": "3",
            "value": "2122212201122002221120200210011020220200"
        },
        "7": {
            "base": "3",
            "value": "20120221122211000100210021102001201112121"
        },
        "8": {
            "base": "6",
            "value": "20220554335330240002224253"
        },
        "9": {
            "base": "12",
            "value": "45153788322a1255483"
        },
        "10": {
            "base": "7",
            "value": "1101613130313526312514143"
        }
    })";

    // Parse the JSON input
    nlohmann::json json_data = nlohmann::json::parse(json_input);

    int n = json_data["keys"]["n"];
    int k = json_data["keys"]["k"];

    std::vector<int> x_values;
    std::vector<long long> y_values;

    // Decode the values from JSON
    for (int i = 1; i <= n; ++i) {
        std::string base = json_data[std::to_string(i)]["base"];
        std::string value = json_data[std::to_string(i)]["value"];
        long long decoded_y = decode_value(base, value);
        
        x_values.push_back(i);
        y_values.push_back(decoded_y);
    }

    // Calculate constant term c by evaluating at x = 0
    long long secret_c = lagrange_interpolation(x_values, y_values, k, 0);

    // Print the result
    std::cout << "Secret c for the given test case: " << secret_c << std::endl;

    return 0;
}
