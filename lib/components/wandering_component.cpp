#include "components/wandering_component.hpp"

WanderingComponent::WanderingComponent(DeathFunction on_death) : on_death(on_death) {}

std::string WanderingComponent::WanderingComponent::getNameStatic() {
    return "WanderingComponent";
}