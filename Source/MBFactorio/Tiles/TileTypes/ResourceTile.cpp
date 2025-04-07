#include "ResourceTile.h"

AResourceTile::AResourceTile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AResourceTile::SetResourceType(EResourceType InType)
{
	ResourceType = InType;
}