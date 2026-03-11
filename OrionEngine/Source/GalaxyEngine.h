#ifndef _GALAXY_H
#define _GALAXY_H

#include "Vector3.h"
#include <list>

//------------------------------------------------------------------------
class GalaxyStar
{
public:
    GalaxyStar();
    const Vector3dC& CalcXYZ();

    double m_theta;    // position auf der ellipse
    double m_velTheta; // angular velocity
    double m_angle;    // Schräglage der Ellipse
    double m_a;        // kleine halbachse
    double m_b;        // große halbachse
    double m_temp;     // star temperature
    double m_mag;      // brightness;
    double m_dist;     // temp distance recalculated when we need it
    int m_id; // This stars id, aka index
    Vector3dC  m_center;   // center of the elliptical orbit
    Vector3dC  m_vel;      // Current velocity (calculated)
    Vector3dC  m_pos;      // current position in kartesion koordinates
    int m_class;
};

typedef std::list<GalaxyStar> GalaxyStarList;
typedef GalaxyStarList::iterator GalaxyStarListItor;

class GalaxyPlanet
{
public:
    GalaxyPlanet();
    Vector3dC m_pos;
    int m_systemid;
    int m_id;
    int m_class;
    double m_dist;
};

typedef std::list<GalaxyPlanet> GalaxyPlanetList;
typedef GalaxyPlanetList::iterator GalaxyPlanetListItor;

class GalaxyMoon
{
public:
    GalaxyMoon();
    Vector3dC m_pos;
    int m_planetid;
    int m_id;
    int m_class;
    double m_dist;
};

typedef std::list<GalaxyMoon> GalaxyMoonList;
typedef GalaxyMoonList::iterator GalaxyMoonListItor;

//------------------------------------------------------------------------
/** \brief A class to encapsulate the geometric details of a spiral galaxy. */
class GalaxyEngine
{
public:
    GalaxyEngine(double rad = 15000,
         double radCore = 6000,
         double deltaAng = 0.019,
         double ex1=0.8,
         double ex2 = 1,
         double sigma = 0.45,
         double velInner = 200,
         double velOuter = 300,
         int numStars=20000);
    ~GalaxyEngine();

    void Reset(double rad,
             double radCore,
             double deltaAng,
             double ex1,
             double ex2,
             double sigma,
             double velInner,
             double velOuter,
             int numStars);

    void Reset();

    GalaxyStar* GetStars() const;
    GalaxyStar* GetDust() const;
    GalaxyStar* GetH2() const;

    double GetRad() const;
    double GetCoreRad() const;
    double GetFarFieldRad() const;
    double GetSigma() const;

    // Properties depending on the orbital radius

    double GetExcentricity(double rad) const;
    double GetOrbitalVelocity(double rad) const;

    double GetAngularOffset(double rad) const;
    double GetAngularOffset() const;

    double GetExInner() const;
    double GetExOuter() const;
    double GetTimeStep() const;
    double GetTime() const;
    int GetNumStars() const;
    int GetNumDust() const;
    int GetNumH2() const;


    void SingleTimeStep(double time);

    const Vector3dC& GetStarPos(int idx);

    void SetSigma(double sigma);
    void SetAngularOffset(double offset);
    void SetCoreRad(double rad);
    void SetRad(double rad);
    void SetExInner(double ex);
    void SetExOuter(double ex);

private:
    
    void InitStars(double sigma);

    // Parameters needed for defining the general structure of the galaxy

    double m_elEx1;          ///< Excentricity of the innermost ellipse
    double m_elEx2;          ///< Excentricity of the outermost ellipse

    double m_velOrigin;      ///< Velovity at the innermost core in km/s
    double m_velInner;       ///< Velocity at the core edge in km/s
    double m_velOuter;       ///< Velocity at the edge of the disk in km/s

    double m_angleOffset;    ///< Angular offset per parsec

    double m_radCore;        ///< Radius of the inner core
    double m_radGalaxy;      ///< Radius of the galaxy
    double m_radFarField;    ///< The radius after which all density waves must have circular shape
    double m_sigma;          ///< Distribution of stars
    double m_velAngle;       ///< Angular velocity of the density waves
    int m_numStars;          ///< Total number of stars
    int m_numDust;           ///< Number of Dust Particles
    int m_numH2;             ///< Number of H2 Regions

    double m_time;
    double m_timeStep;

public:

    int m_numberByRad[100];  ///< Historgramm showing distribution of stars

    GalaxyStarList getAnchorStars();
    GalaxyStarList getNeighboringStars(int anchorstarid, int anchorstarclass);
    GalaxyStarList getStarsInRange(int starid, float range);
    GalaxyPlanetList getPlanetsInStarSystem(int starid, int starclass, Vector3dC pos);
    GalaxyMoonList getMoonsForPlanet(int planetid, int planetclass, Vector3dC pos);

private:

    Vector3dC m_pos;             ///< Center of the galaxy
    GalaxyStar *m_pStars;          ///< Pointer to an array of star data
    GalaxyStar *m_pDust;           ///< Pointer to an array of dusty areas
    GalaxyStar *m_pH2;
};

#endif // _GALAXY_H
