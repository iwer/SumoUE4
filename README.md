# SumoUE4

Unreal Engine Plugin to import [SUMO](https://www.eclipse.org/sumo/) *.net.xml files into Unreal Editor and generate roads. It is inspired by and builds upon the work of [CruzWay](https://github.com/AugmentedDesignLab/CruzWay) but tries to be more flexible by separating data and representation.

This should be considered WIP.

## Compatibility

* Unreal Engine 4 is supported at least with 4.27 (branch ue4, tag ue4.27)
* Unreal Engine 5 is supported up to 5.1 (on master branch)

## Dependencies

Depends on [GeoReference](https://github.com/iwer/GeoReference) and [SpatialGeometryTools](https://github.com/iwer/SpatialGeometryTools) Plugin.

## Features

#### Data Asset Import
Import of *.net.xml Files as structured data assets as per (SUMO-Docs)(https://sumo.dlr.de/docs/Networks/SUMO_Road_Networks.html), including:
* Edges and Lanes
* Traffic Light Programs (Without additionals)
* Junction including Right-of-Way
* Connections

This data can then be used to create data-driven Assets. Be aware that this data assets can get very large and should not be opened in the Unreal Editor. Instead, in Blueprints use a reference to the data asset and get individual junctions, edges, lanes etc. to construct visual representation or routing information actors.

#### Data-driven actors

* SumoJunctionActor: Geo located ProceduralMeshComponent with Lane Splines
* Sumo EdgeActor: Geo located lane Splines with Splinemeshcomponents
