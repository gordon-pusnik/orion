//
//  Constants.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-25.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

// Multiplier for calculating sizes, positions, speeds and distances, since units are in meters, we divide
// them by the galactic scale to scale down to something managable for this sim.
const float kGalacticScale = 10000000.0;

const float kRadiusModifier = 4; // Make shit bigger so it looks right.

// Camera clipping planes, dont change this!
const float kNear = 0.000001;
const float kFar = 1000000.0;

// Joystick sensitivity
const float kJoystickSensitivity = .055f;

// PI
const float kPi =  3.14159265358f;
// 2 PI
const float k2Pi = 6.28318530718f;

// Radius of the sol in meters
const double kRadiusSol = 695500000.0 * kRadiusModifier;

// Position of the sol in meters - 150,000,000,000
const double kPosSolX = -75000000000.0;
const double kPosSolY = 0.0;
const double kPosSolZ = 75000000000.0;

// Mercury
const double kRadiusMercury = 2440000.0 * kRadiusModifier;
const double kDistanceMercury = 57910000000.0;

// Venus
const double kRadiusVenus = 6052000.0 * kRadiusModifier;
const double kDistanceVenus = 108200000000.0;

// Radius of the earth in meters
const double kRadiusEarth = 6378100.0 * kRadiusModifier;
const double kDistanceEarth = 150000000000.0;

// Position of earth in meters;
const double kPosEarthX = 40000000.0;
const double kPosEarthY = 0.0;
const double kPosEarthZ = -100000000.0;

// Radius of the moon in meters
const double kRadiusMoon = 1737400.0 * kRadiusModifier;
const double kDistanceMoon = 384000000.0;

// Position of moon in meters
const double kPosMoonX = kPosEarthX - 384000000.0;
const double kPosMoonY = kPosEarthY;
const double kPosMoonZ = kPosEarthZ;

const double kRadiusMars = 3389000.0 * kRadiusModifier;
const double kDistanceMars = 227900000000.0;

const double kRadiusJupiter = 69911000.0 * kRadiusModifier;
const double kDistanceJupiter = 778500000000.0;

const double kRadiusSaturn = 58232000.0 * kRadiusModifier;
const double kDistanceSaturn = 1433000000000.0;

const double kRadiusUranus = 25362000.0 * kRadiusModifier;
const double kDistanceUranus = 2877000000000.0;

const double kRadiusNeptune = 24622000.0 * kRadiusModifier;
const double kDistanceNeptune = 4503000000000.0;

const double kRadiusPluto = 1153000.0 * kRadiusModifier;
const double kDistancePluto = 5906376272000.0;

// Position alpha centauri
const double kPosAlphaCentauri = -4.12e16; // 41,200,000,000,000,000 - 4.12e16
const double kRadiusAlphaCentauriInSols = 1.227;

// Max distance visible star
const double kDistMaxVisibleStar = 2.0e13;

// Ship thrust
const float kThrust = 0.1;

// Speed of light in m/s
const float kSpeedOfLight = 299792458.0;

// Warp speed 1 - Speed of light, meters per second. v = w10/3 c
const float kWarpSpeed = kSpeedOfLight;
const float kWarpSpeed5 = kWarpSpeed * 214.0;
const float kWarpSpeed96 = kWarpSpeed * 1909.0;
const float kWarpSpeed999 = kWarpSpeed * 7912.0;
const float kWarpSpeed99999 = kWarpSpeed * 199516.0;

const float kWarpFactor = 2.0;

const double kAU = 149597871000.0;

// Class of stars

// O stars are called "blue",
// B stars are called "blue-white"
// A stars are called "white"
// F stars are called "yellow-white"
// G stars are called "yellow"
// K stars are called "orange"
// M stars are called "red"


#endif
