#include "components/wandering_component.hpp"

WanderingComponent::WanderingComponent(double duration, DeathFunction on_death) : duration(duration), on_death(on_death) {}

std::string WanderingComponent::WanderingComponent::getNameStatic() {
    return "WanderingComponent";
}