#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cmath>

double integerPow(double base, int exp) {
    if (exp == 0) {
        return 1.0; // base case
    }

    double result = 1.0;
    bool isNegative = (exp < 0);
    
    if (isNegative) {
        exp = -exp; // Work with positive exponent
    }

    for (int i = 0; i < exp; ++i) {
        result *= base;
    }

    return isNegative ? 1.0 / result : result;
}

double root(double value, int root) {
    if (root <= 0) {
        throw std::invalid_argument("Root must be positive.");
    }
    
    double low = 0;
    double high = value > 1 ? value : 1;
    double mid;

    // Use binary search to find the nth root
    for (int i = 0; i < 1000; ++i) { // Increase iterations for more precision
        mid = (low + high) / 2;
        double midPow = integerPow(mid, root);
        
        if (midPow < value) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return mid; // This is the approximate nth root of value
}

double powFraction(double a, double exp) {
    // Separate the integer and fractional parts
    int integerPart = static_cast<int>(exp);
    double fractionalPart = exp - integerPart;

    // Calculate a^integerPart
    double result = integerPow(a, integerPart);

    // Calculate the fractional part if it exists
    if (fractionalPart != 0) {
        // Convert the fractional part into numerator and denominator
        int numerator = static_cast<int>(fractionalPart * 10000 + 0.5); // Scale for precision
        int denominator = 10000; // Scale factor

        // Calculate the c-th root
        double rootValue = root(result, denominator);
        
        // Raise the root to the power of the numerator
        result *= integerPow(rootValue, numerator);
    }

    return result;
}

double parseFraction(const std::string& fraction) {
    std::istringstream ss(fraction);
    int numerator, denominator;
    char slash;

    // Parse the string for numerator/denominator
    if (!(ss >> numerator >> slash >> denominator) || slash != '/') {
        throw std::invalid_argument("Invalid fraction format.");
    }

    // Return the fractional value
    return static_cast<double>(numerator) / denominator;
}

int main() {
    double base;
    std::string exponent;

    std::cout << "Enter the base value: ";
    std::cin >> base;

    std::cout << "Enter the exponent as a fraction (e.g., 3/4): ";
    std::cin >> exponent;

    try {
        double fractionalExponent = parseFraction(exponent);
        double result = powFraction(base, fractionalExponent);
        std::cout.precision(10); // Set precision for output
        std::cout << base << "^(" << exponent << ") = " << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
