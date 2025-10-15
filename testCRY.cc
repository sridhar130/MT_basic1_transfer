#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    // Path to your CRY config and data directory
    std::string configFile = "/home/sridhar/G4Dev/cry_v1.7/configGaisser_short.txt";
    std::string cryDataDir = "/home/sridhar/G4Dev/cry_v1.7/data";

    // Read configuration
    std::ifstream inputFile(configFile);
    if (!inputFile.is_open()) {
        std::cerr << "❌ Cannot open CRY config file: " << configFile << std::endl;
        return 1;
    }

    std::string setupString((std::istreambuf_iterator<char>(inputFile)),
                             std::istreambuf_iterator<char>());

    try {
        // Initialize CRY
        CRYSetup setup(setupString, cryDataDir);
        CRYGenerator gen(&setup);

        // Generate one event
        std::vector<CRYParticle*> particles;
        gen.genEvent(&particles);

        std::cout << "✅ Generated " << particles.size() << " particles\n";
        for (auto* p : particles) {
            std::cout << "  " << CRYUtils::partName(p->id())
                      << "  KE=" << p->ke() << " MeV"
                      << "  dir=(" << p->u() << ", " << p->v() << ", " << p->w() << ")\n";
            delete p; // clean up!
        }
    } catch (std::exception& e) {
        std::cerr << "❌ Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
