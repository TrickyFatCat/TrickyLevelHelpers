# TrickyLevelHelpers

A plugin for facilitating level design process.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

1. Download the latest [**package file**](https://github.com/TrickyFatCat/TrickyLevelHelpers/releases/);
2. Install the plugin either in your project or for all projects in engine:
    * Unzip the package into Plugins folder of your project, e.g. `D:\UnrealProjects\MyProject\Plugins`;
    * Unzip the package to the Plugins folder in engine folder,
      e.g. `C:\Program Files\Epic Games\UE_5.0\Engine\Plugins`;
3. Restart the project;

In this case the plugin can be used for any blueprint project.

### C++ projects

1. Create the Plugins folder in the project directory;
2. Create the TrickyAnimationComponents folder in the Plugins folder;
3. Download the plugin source code into that folder;
4. Rebuild the project;

## Content

The plugin contains:

1. [ActorGeneratorShape](#actorgeneratorshape);
2. [ ActorGeneratorSpline ](#actorgeneratorspline);
3. [ MeshesGeneratorSpline ](#meshesgeneratorspline);
4. [ SplineActor ](#splineactor);
5. [Rulers](#rulers):
    1. [ LineRuler ](#lineruler);
    2. [ RadiusRuler ](#radiusruler);
    3. [ SpeedRuler ](#speedruler);
    4. [ SplineRuler ](#splineruler);
    5. [ VolumeRuler ](#volumeruler);

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

### SplineActor

A simple actor with a spline component as a root component.

### Rulers

Rulers are simple editor only actors which allows quickly measure and visualize distances in the level.

You can place them in the level using `Place Actor` menu and search.

In addition, you can create BP versions of these rulers if you need a collection of different rulers for quick measurement.

#### LineRuler

Visualizes a given distance along X, Y, and Z axis.

1. `NoteText` - a custom text showed on top of debug information;
2. `Length` - length of each axis;
3. `MarksSpacing` - distance between marks for each axis;
4. `Speed` - if enabled, will show time needed to cover distance along axis with given speed;
5. `X` - if enabled, X axis will be shown;
6. `Y` - if enabled, Y axis will be shown;
7. `Z` - if enabled, Z axis will be shown;
8. `TextColor` - determines the color of the text;
9. `ShowInGame` - if true, the debug information will be shown while playing in editor;

#### RadiusRuler

Visualizes given radius.

1. `NoteText` - a custom text showed on top of debug information;
2. `Radius` - radius to visualize;
3. `Shape` - determines what shape to use to visualize `Radius`:
    1. `Circle`;
    2. `Sphere`;
    3. `Cylinder`;
4. `Speed` - if enabled, will show time needed to cover radius with given speed;
5. `Color` - determines the color of the text;
6. `ShowInGame` - if true, the debug information will be shown while playing in editor;

#### SpeedRuler

Visualizes distance calculated using time, speed, acceleration, deceleration.

1. `NoteText` - a custom text showed on top of debug information;
2. `MovementTime` - duration of movement;
3. `IsUniformSpeed` - if true it'll calculate distance with a uniform speed:
   1. `Speed` - uniform speed;
4. `InitialSpeed` - initial movement speed;
5. `TargetSpeed` - maximum speed;
6. `Acceleration` - movement acceleration;
7. `Deceleration` - movement deceleration;
8. `ShowCircle` - if true, will show circle to show the distance;
9. `Color` - color of movement section with `TargetSpeed`;
10. `AccelerationColor` - color of acceleration movement section;
11. `DecelerationColor` - color of deceleration movement section;
12. `ShowInGame` - if true, the debug information will be shown while playing in editor;

#### SplineRuler

Measures distance from first point to others and length of sectors between points.

1. `Points` - if enabled, points debug data will be shown;
2. `Sectors` - if enabled, sectors debug data will be shown;
3. `Speed` - if enabled, will show time needed to cover distance between point with given speed;
4. `IsLooped` - toggles if the spline is looped;
5. `SplineColor` - determines spline color;
6. `ShowInGame` - if true, the debug information will be shown while playing in editor;

#### VolumeRuler

Visualizes given distance as a cube.

1. `Points` - if enabled, points debug data will be shown;
2. `Sectors` - if enabled, sectors debug data will be shown;
3. `Speed` - if enabled, will show time needed to cover distance between point with given speed;
4. `IsLooped` - toggles if the spline is looped;
5. `SplineColor` - determines spline color;
6. `ShowInGame` - if true, the debug information will be shown while playing in editor;
