#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
    PrimaryActorTick.bCanEverTick = false;
}

AActor* APatrolPath::GetWaypoint(int32 Index) const
{
    if (Waypoints.IsValidIndex(Index))
    {
        return Waypoints[Index];
    }
    return nullptr;
}

int32 APatrolPath::Num() const
{
    return Waypoints.Num();
}