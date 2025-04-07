#include "GroundTile.h"

void AGroundTile::InitializeTile(float InTileSize)
{
    Super::InitializeTile(InTileSize);

    SetRandomMaterial(GroundMaterials);
}
