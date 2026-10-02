#include <cassert>
#include <iostream>
#include "quantforge/quantforge.hpp"

int main() {
    using namespace quantforge;

    // Construction + total_value
    {
        Portfolio p;
        p.add_asset("AAPL", 10, 150.0);
        p.add_asset("MSFT", 5, 300.0);
        assert(p.size() == 2);
        assert(p.total_value() == 10 * 150.0 + 5 * 300.0);
    }

    // Empty portfolio must fail RiskConfig-paired validation via RiskEngine.
    {
        Portfolio empty;
        RiskEngine engine;
        RiskConfig config;
        bool threw = false;
        try {
            engine.calculate<VaR>(empty, config);
        } catch (const InvalidPortfolioError&) {
            threw = true;
        }
        assert(threw && "Empty portfolio should raise InvalidPortfolioError");
    }

    // Invalid confidence must fail validation.
    {
        Portfolio p;
        p.add_asset("AAPL", 10, 150.0);
        RiskEngine engine;
        RiskConfig config;
        config.confidence = 1.5; // invalid
        bool threw = false;
        try {
            engine.calculate<VaR>(p, config);
        } catch (const InvalidConfigError&) {
            threw = true;
        }
        assert(threw && "confidence > 1 should raise InvalidConfigError");
    }

    std::cout << "test_portfolio: all assertions passed\n";
    return 0;
}
