# TrickyLevelHelpers

A plugin for facilitating level design process.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

**At the moment, the package is compatible only with Unreal Engine 4.**

1. Download the latest [**package file**](https://github.com/TrickyFatCat/TrickyLevelHelpers/releases/);
2. Unzip the package to the Plugins folder in engine folder, e.g. `C:\Program Files\Epic Games\UE_4.27\Engine\Plugins`;
3. Restart the project;

In this case the plugin can be used for any blueprint project.

### C++ projects

1. Create the Plugins folder in the project directory;
2. Create the TrickyAnimationComponents folder in the Plugins folder;
3. Download the plugin source code into that folder;
4. Rebuild the project;

## Content

The plugin contains:

1. ActorGeneratorShape;
2. ActorGeneratorSpline;
3. MeshesGeneratorSpline;
4. LevelHelpersLibrary;

### ActorGeneratorShape

A helper actor which spawns actors in constructions script in precalculated locations.

#### Parameters

1. `ActorClass` - an actor class which will be spawned;
2. `Shape` - determines locations calculations:
   1. Grid:
      1. `GridSize` - amount of grid points;
      2. `SectorSize` - distance between grid points;
   2. Cube:
      1. `CubeSize` - amount of cube points;
      2. `CubeSectorSize` - distance between cube points;
   3. Ring:
      1. `ActorsAmount` - the amount of actors in the ring;
      2. `Radius` - radius of the ring;
   4. Arc:
      1. `ActorsAmount` - the amount of actors in the arc;
      2. `Radius` - radius of the arc;
      3. `Angle` - angle of the arc;
   5. Cylinder:
      1. `ActorsAmount` - the amount of actors per ring;
      2. `RingsAmount` - the amount of rings in the cylinder;
      3. `Radius` - radius of the cylinder;
      4. `RingOffset` - distance between each ring;
   6. Sphere:
      1. `ActorsAmount` - the amount of actors in the sphere;
      2. `Radius` - radius of the sphere;
      3. `MinLatitude`, `MaxLatitude` - latitude angle limits;
      4. `MinLogitude`, `MaxLogitude` - longitude angle limits;
3. `LocationOffset` - location offset relative ot the root component;
4. `RotationMode` - determines how rotation of the spawned actors will be calculated:
   1. `Manual` - rotation won't calculate automatically;
   2. `Custom` - custom mode for different shapes:
      1. `CustomRotation` - a custom rotation value for all spawned actors;
      2. `In` - rotates actors inward. Only for Ring, Arc, Cylinder, Sphere;
      3. `Out` - rotates actors outwards. Only for Ring, Arc, Cylinder, Sphere;
   3. `Forward/Backward` - rotates towards forward/backward unit vectors;
   4. `Up/Down` - rotates towards up/down unit vectors;
   5. `Left/Right` - rotates towards left/right unit vectors;
5. `Scale` - scale value for all spawned actors;

### ActorGeneratorSpline

A helper actor which spawns actors in constructions script along spline.

#### Parameters

1. `ActorClass` - an actor class which will be spawned;
2. `GenerationMode` - determines locations calculations along the spline:
   1. `Points` - actors will be spawned in spline points;
   2. `Actors` - a certain amount of actors will be spawned evenly along spline:
      1. `ActorsAmount` - the amount of actors to spawn;
   3. `Spacing` - actors will be spawned evenly using the spacing distance:
      1. `Spacing` - distance between actors;
3. `LocationOffset` - a location offset relative to the spline;
4. `RotationMode` - determines how rotation of the spawned actors will be calculated:
   1. `Manual` - rotation won't calculate automatically;
   2. `Custom` - custom mode for different shapes:
      1. `CustomRotation` - a custom rotation value for all spawned actors;
      2. `RotateAlongSpline` - toggles rotation along spline axis;
   3. `Forward/Backward` - rotates towards forward/backward unit vectors;
   4. `Up/Down` - rotates towards up/down unit vectors;
   5. `Left/Right` - rotates towards left/right unit vectors;
5. `Scale` - scale value for all spawned actors;

### MeshesGeneratorSpline

A helper actor which generates meshes along spline.

#### Parameters

1. `StaticMesh` - mesh to generate;
2. `GenerationMode` - determines locations calculations along the spline:
   1. `Points` - actors will be spawned in spline points;
   2. `Actors` - a certain amount of actors will be spawned evenly along spline:
      1. `ActorsAmount` - the amount of actors to spawn;
   3. `Spacing` - actors will be spawned evenly using the spacing distance:
      1. `Spacing` - distance between actors;
3. `LocationOffset` - a location offset relative to the spline;
4. `RotationMode` - determines how rotation of the spawned actors will be calculated:
   1. `Manual` - rotation won't calculate automatically;
   2. `Custom` - custom mode for different shapes:
      1. `CustomRotation` - a custom rotation value for all spawned actors;
      2. `RotateAlongSpline` - toggles rotation along spline axis;
   3. `Forward/Backward` - rotates towards forward/backward unit vectors;
   4. `Up/Down` - rotates towards up/down unit vectors;
   5. `Left/Right` - rotates towards left/right unit vectors;
5. `Scale` - scale value for all spawned actors;