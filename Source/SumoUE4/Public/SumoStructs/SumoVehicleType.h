// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum ESumoVehicleType {
    SVT_Passenger UMETA(ToolTip = "Default vehicle class"),
    SVT_Private UMETA(ToolTip = "A passenger car assigned for private use"),
    SVT_Taxi UMETA(ToolTip = "Vehicle for hire with a driver"),
    SVT_Bus UMETA(ToolTip = "Urban line traffic"),
    SVT_Coach UMETA(ToolTip = "Overland transport"),
    SVT_Delivery UMETA(ToolTip = "Vehicle specialized to deliver goods"),
    SVT_Truck UMETA(ToolTip = "Vehicle designed to transport cargo"),
    SVT_Trailer UMETA(ToolTip = "Truck with trailer"),
    SVT_Emergency UMETA(ToolTip = "Vehicle designed to respond to an emergency"),
    SVT_Motorcycle UMETA(ToolTip = "Two- or three-wheeled motor vehicle"),
    SVT_Moped UMETA(ToolTip = "Motorcycle not allowed in motorways"),
    SVT_Bicycle UMETA(ToolTip = "Human-powered, pedal-driven vehicle"),
    SVT_Pedestrian UMETA(ToolTip = "Person traveling on foot"),
    SVT_Tram UMETA(ToolTip = "Rail vehicle which runs on tracks"),
    SVT_RailElectric UMETA(ToolTip = "Rail electric vehicle"),
    SVT_RailFast UMETA(ToolTip = "High-speed rail vehicle"),
    SVT_RailUrban UMETA(ToolTip = "Heavier than tram"),
    SVT_Rail UMETA(ToolTip = "Heavy rail vehicle"),
    SVT_EVehicle UMETA(ToolTip = "Future electric mobility vehicles"),
    SVT_Army UMETA(ToolTip = "Vehicle designed for military forces"),
    SVT_Ship UMETA(ToolTip = "Basic class for navigating waterways"),
    SVT_Authority UMETA(ToolTip = "Vehicle of a governmental security agency"),
    SVT_Vip UMETA(ToolTip = "A civilian security armoured car used by VIPs"),
    SVT_Hov UMETA(ToolTip = "High-occupancy vehicle (two or more passengers)"),
    SVT_Custom1 UMETA(ToolTip = "Reserved for user-defined semantics"),
    SVT_Custom2 UMETA(ToolTip = "Reserved for user-defined semantics")
};
