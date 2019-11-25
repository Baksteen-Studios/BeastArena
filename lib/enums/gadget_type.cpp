#include "enums/gadget_type.hpp"
#include <string>

GadgetType stringToGadgetType(std::string s) {
    if (s == "Pistol") {
        return GadgetType::Pistol;
    } else if (s == "Rifle") {
        return GadgetType::Rifle;
    } else if (s == "Sniper") {
        return GadgetType::Sniper;
    } else {
        throw NotImplementedException("GadgetType: " + s + " not implemented in stringToGadgetType.");
    }
}