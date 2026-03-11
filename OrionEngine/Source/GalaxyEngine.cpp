#include "GalaxyEngine.h"
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <algorithm>

//------------------------------------------------------------------------
#include "GalaxyConstants.h"
#include "FastMath.h"
#include "CumulativeDistributionFunction.h"
#include "Utils.h"
#include "Random.h"
#include "Constants.h"
#include "Entity.h"

//------------------------------------------------------------------------
GalaxyStar::GalaxyStar()
    :m_theta(0)
    ,m_velTheta(0)
    ,m_a(0)
    ,m_b(0)
    ,m_center(0,0,0)
    ,m_pos(0,0,0)
    ,m_vel(0,0,0)
{
    m_class = Random::randomInRange(BROWN_DWARF, WORMHOLE + 1);
}

//-----------------------------------------------------------------------
const Vector3dC& GalaxyStar::CalcXYZ()
{
    double &a = m_a,
         &b = m_b,
         &theta = m_theta;
    const Vector3dC &p = m_center;

    double beta  = -m_angle,
         alpha = theta * DEG_TO_RAD;

    // temporaries to save cpu time
    double cosalpha = cos(alpha), //FastMath::cos(alpha),
         sinalpha = sin(alpha), //FastMath::sin(alpha),
         cosbeta  = cos(beta),  //FastMath::cos(beta),
         sinbeta  = sin(beta);  //FastMath::sin(beta);

    m_pos = Vector3dC(p.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
                    p.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta),
                    p.z + Random::randomInRange(-1000, 1000));

    return m_pos;
}

//------------------------------------------------------------------------

GalaxyPlanet::GalaxyPlanet()
    :m_pos(0,0,0)
    ,m_systemid(0)
    ,m_id(0)
    ,m_class(0)
{
}

GalaxyMoon::GalaxyMoon()
:m_pos(0,0,0)
,m_planetid(0)
,m_id(0)
,m_class(0)
{
}

//------------------------------------------------------------------------
GalaxyEngine::GalaxyEngine(double rad,
               double radCore,
               double deltaAng,
               double ex1,
               double ex2,
               double sigma,
               double velInner,
               double velOuter,
               int numStars)
    :m_elEx1(ex1)
    ,m_elEx2(ex2)
    ,m_velOrigin(30)
    ,m_velInner(velInner)
    ,m_velOuter(velOuter)
    ,m_angleOffset(deltaAng)
    ,m_radCore(radCore)
    ,m_radGalaxy(rad)
    ,m_sigma(sigma)
    ,m_velAngle(0.000001)
    ,m_numStars(numStars)
    ,m_numDust(numStars/3.5)
    ,m_numH2(200)
    ,m_time(0)
    ,m_timeStep(0)
    ,m_pos(0,0,0)
    ,m_pStars(NULL)
    ,m_pDust(NULL)
    ,m_pH2(NULL)
{
    FastMath::init();
    Reset(rad, radCore, deltaAng, ex1, ex2, sigma, velInner, velOuter, numStars);
    SingleTimeStep(1);
}

//------------------------------------------------------------------------
GalaxyEngine::~GalaxyEngine()
{
    delete [] m_pStars;
    delete [] m_pDust;
    delete [] m_pH2;
    FastMath::release();
}

//------------------------------------------------------------------------
void GalaxyEngine::Reset()
{
    Reset(m_radGalaxy,
        m_radCore,
        m_angleOffset,
        m_elEx1,
        m_elEx2,
        m_sigma,
        m_velInner,
        m_velOuter,
        m_numStars);
}

//------------------------------------------------------------------------
void GalaxyEngine::Reset(double rad,
                   double radCore,
                   double deltaAng,
                   double ex1,
                   double ex2,
                   double sigma,
                   double velInner,
                   double velOuter,
                   int numStars)
{
    m_elEx1 = ex1;
    m_elEx2 = ex2;
    m_velInner = velInner;
    m_velOuter = velOuter;
    m_elEx2 = ex2;
    m_angleOffset = deltaAng;
    m_radCore = radCore;
    m_radGalaxy = rad;
    m_radFarField = m_radGalaxy * 2;  // there is no science behind this threshold it just should look nice
    m_sigma = sigma;
    m_numStars = numStars;
    m_numDust = numStars/2;
    m_time = 0;

    for (int i=0; i<100; ++i) {
        m_numberByRad[i] = 0;
    }

    InitStars(m_sigma);
}

//------------------------------------------------------------------------
void GalaxyEngine::InitStars(double sigma)
{
    delete [] m_pDust;
    m_pDust = new GalaxyStar[m_numDust];

    delete [] m_pStars;
    m_pStars = new GalaxyStar[m_numStars];

    delete [] m_pH2;
    m_pH2 = new GalaxyStar[m_numH2*2];

    // The first three stars can be used for aligning the
    // camera with the galaxy rotation.

    // First star ist the black hole at the centre
    m_pStars[0].m_a = 0;
    m_pStars[0].m_b = 0;
    m_pStars[0].m_angle = 0;
    m_pStars[0].m_theta = 0;
    m_pStars[0].m_velTheta = 0;
    m_pStars[0].m_center = Vector3dC(0,0,0);
    m_pStars[0].m_velTheta = GetOrbitalVelocity( (m_pStars[0].m_a + m_pStars[0].m_b)/2.0 );
    m_pStars[0].m_temp = 6000;
    m_pStars[0].m_id = 0;

    // second star is at the edge of the core area
    m_pStars[1].m_a = m_radCore;
    m_pStars[1].m_b = m_radCore * GetExcentricity(m_radCore);
    m_pStars[1].m_angle = GetAngularOffset(m_radCore);
    m_pStars[1].m_theta = 0;
    m_pStars[1].m_center = Vector3dC(0,0,0);
    m_pStars[1].m_velTheta = GetOrbitalVelocity( (m_pStars[1].m_a + m_pStars[1].m_b)/2.0 );
    m_pStars[1].m_temp = 6000;
    m_pStars[1].m_id = 1;

    // third star is at the edge of the disk
    m_pStars[2].m_a = m_radGalaxy;
    m_pStars[2].m_b = m_radGalaxy * GetExcentricity(m_radGalaxy);
    m_pStars[2].m_angle = GetAngularOffset(m_radGalaxy);
    m_pStars[2].m_theta = 0;
    m_pStars[2].m_center = Vector3dC(0,0,0);
    m_pStars[2].m_velTheta = GetOrbitalVelocity( (m_pStars[2].m_a + m_pStars[2].m_b)/2.0 );
    m_pStars[2].m_temp = 6000;
    m_pStars[2].m_id = 2;

    // cell width of the histogramm
    double dh = (double)m_radFarField/100.0;

    // Initialize the stars
    CumulativeDistributionFunction cdf;
    cdf.SetupRealistic(1.0,             // Maximalintensität
                     0.02,            // k (bulge)
                     m_radGalaxy/3.0, // disc skalenlänge
                     m_radCore,       // bulge radius
                     0,               // start der intensitätskurve
                     m_radFarField,   // ende der intensitätskurve
                     1000);           // Anzahl der stützstellen
    
    for (int i=3; i<m_numStars; ++i) {
        // random value between -1 and 1
        double rad = cdf.ValFromProp(Random::randomInRange(0.0, 1.0));

        m_pStars[i].m_a = rad;
        m_pStars[i].m_b = rad * GetExcentricity(rad);
        m_pStars[i].m_angle = GetAngularOffset(rad);
        m_pStars[i].m_theta = 360.0 * Random::randomInRange(0.0, 1.0);
        m_pStars[i].m_velTheta = GetOrbitalVelocity(rad);
        m_pStars[i].m_center = Vector3dC(0,0,0);
        m_pStars[i].m_temp = 6000 + (6000 * Random::randomInRange(0.0, 1.0)) - 3000;
        m_pStars[i].m_mag = 0.1 + 0.4 * Random::randomInRange(0.0, 1.0);
        m_pStars[i].m_id = i;

        int idx = std::min(1.0/dh * (m_pStars[i].m_a + m_pStars[i].m_b)/2.0, 99.0);
        m_numberByRad[idx]++;
    }

    // Initialise Dust
    double x,y,rad;
    for (int i=0; i<m_numDust; ++i) {
        x = 2*m_radGalaxy * Random::randomInRange(0.0, 1.0) - m_radGalaxy;
        y = 2*m_radGalaxy * Random::randomInRange(0.0, 1.0) - m_radGalaxy;
        rad = sqrt(x*x+y*y);

        m_pDust[i].m_a = rad;
        m_pDust[i].m_b = rad * GetExcentricity(rad);
        m_pDust[i].m_angle = GetAngularOffset(rad);
        m_pDust[i].m_theta = 360.0 * Random::randomInRange(0.0, 1.0);
        m_pDust[i].m_velTheta = GetOrbitalVelocity( (m_pDust[i].m_a + m_pDust[i].m_b)/2.0 );
        m_pDust[i].m_center = Vector3dC(0,0,0);
        m_pDust[i].m_temp = 6000 + rad/4.0;

        m_pDust[i].m_mag = 0.015 + 0.01 * Random::randomInRange(0.0, 1.0);
        int idx = std::min(1.0/dh * (m_pDust[i].m_a + m_pDust[i].m_b)/2.0, 99.0);
        m_numberByRad[idx]++;
    }

    // Initialise Dust
    for (int i=0; i<m_numH2; ++i) {
        x = 2*(m_radGalaxy) * Random::randomInRange(0.0, 1.0) - (m_radGalaxy);
        y = 2*(m_radGalaxy) * Random::randomInRange(0.0, 1.0) - (m_radGalaxy);
        rad = sqrt(x*x+y*y);

        int k1 = 2*i;
        m_pH2[k1].m_a = rad;
        m_pH2[k1].m_b = rad * GetExcentricity(rad);
        m_pH2[k1].m_angle = GetAngularOffset(rad);
        m_pH2[k1].m_theta = 360.0 * Random::randomInRange(0.0, 1.0);
        m_pH2[k1].m_velTheta = GetOrbitalVelocity( (m_pH2[k1].m_a + m_pH2[k1].m_b)/2.0 );
        m_pH2[k1].m_center = Vector3dC(0,0,0);
        m_pH2[k1].m_temp = 6000 + (6000 * Random::randomInRange(0.0, 1.0)) - 3000;
        m_pH2[k1].m_mag = 0.1 + 0.05 * Random::randomInRange(0.0, 1.0);
        int idx = std::min(1.0/dh * (m_pH2[k1].m_a + m_pH2[k1].m_b)/2.0, 99.0);
        m_numberByRad[idx]++;

        int k2 = 2*i+1;
        m_pH2[k2].m_a = rad + 1000;
        m_pH2[k2].m_b = rad * GetExcentricity(rad);
        m_pH2[k2].m_angle = m_pH2[k1].m_angle; //GetAngularOffset(rad);
        m_pH2[k2].m_theta = m_pH2[k1].m_theta;
        m_pH2[k2].m_velTheta = m_pH2[k1].m_velTheta;
        m_pH2[k2].m_center = m_pH2[k1].m_center;
        m_pH2[k2].m_temp = m_pH2[k1].m_temp;
        m_pH2[k2].m_mag = m_pH2[k1].m_mag;
        idx = std::min(1.0/dh * (m_pH2[k2].m_a + m_pH2[k2].m_b)/2.0, 99.0);
        m_numberByRad[idx]++;
    }
}

//------------------------------------------------------------------------
double GalaxyEngine::GetSigma() const
{
    return m_sigma;
}

//------------------------------------------------------------------------
void GalaxyEngine::SetSigma(double s)
{
    m_sigma = s;
    Reset();
}

//------------------------------------------------------------------------
GalaxyStar* GalaxyEngine::GetStars() const
{
    return m_pStars;
}

//------------------------------------------------------------------------
GalaxyStar* GalaxyEngine::GetDust() const
{
    return m_pDust;
}

//------------------------------------------------------------------------
GalaxyStar* GalaxyEngine::GetH2() const
{
    return m_pH2;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetRad() const
{
    return m_radGalaxy;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetCoreRad() const
{
    return m_radCore;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetFarFieldRad() const
{
    return m_radFarField;
}

//------------------------------------------------------------------------
void GalaxyEngine::SetAngularOffset(double offset)
{
    m_angleOffset = offset;
    Reset();
}

//------------------------------------------------------------------------
/** \brief Returns the orbital velocity in degrees per year.
    \param rad Radius in parsec
*/
double GalaxyEngine::GetOrbitalVelocity(double rad) const
{
/*
    double vel_kms(0);  // velovity in kilometer per seconds

    // Calculate velovity in km per second
    if (rad<m_radCore)
    {
    double dv = (m_velInner-m_velOrigin) / m_radCore;
    vel_kms = m_velOrigin + rad * dv;
    }
    else if (rad>=m_radCore)
    {
    double dv = (m_velOuter-m_velInner) / (m_radGalaxy - m_radCore);
    vel_kms = m_velInner + dv * (rad-m_radCore);
    }

    // Calculate velocity in degree per year
    double u = 2 * M_PI * rad * PC_TO_KM;        // Umfang in km
    double time = u / (vel_kms * SEC_PER_YEAR);  // Umlaufzeit in Jahren

    return 360.0 / time;                                   // Grad pro Jahr
*/
    return 0.000005;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetExcentricity(double r) const
{
    if (r<m_radCore) {
        // Core region of the galaxy. Innermost part is round
        // excentricity increasing linear to the border of the core.
        return 1 + (r / m_radCore) * (m_elEx1-1);
    }
    else if (r>m_radCore && r<=m_radGalaxy) {
        return m_elEx1 + (r-m_radCore) / (m_radGalaxy-m_radCore) * (m_elEx2-m_elEx1);
    }
    else if (r>m_radGalaxy && r <m_radFarField) {
        // excentricity is slowly reduced to 1.
        return m_elEx2 + (r - m_radGalaxy) / (m_radFarField - m_radGalaxy) * (1-m_elEx2);
    }
    else {
        return 1;
    }
}

//------------------------------------------------------------------------
double GalaxyEngine::GetAngularOffset(double rad) const
{
    return rad * m_angleOffset;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetAngularOffset() const
{
    return m_angleOffset;
}

//------------------------------------------------------------------------
double GalaxyEngine::GetExInner() const
{
    return m_elEx1;
}

//-----------------------------------------------------------------------
double GalaxyEngine::GetExOuter() const
{
    return m_elEx2;
}

//-----------------------------------------------------------------------
void GalaxyEngine::SetRad(double rad)
{
    m_radGalaxy = rad;
    Reset();
}

//-----------------------------------------------------------------------
void GalaxyEngine::SetCoreRad(double rad)
{
    m_radCore = rad;
    Reset();
}

//-----------------------------------------------------------------------
void GalaxyEngine::SetExInner(double ex)
{
    m_elEx1 = ex;
    Reset();
}

//-----------------------------------------------------------------------
void GalaxyEngine::SetExOuter(double ex)
{
    m_elEx2 = ex;
    Reset();
}

//-----------------------------------------------------------------------
double GalaxyEngine::GetTimeStep() const
{
    return m_timeStep;
}

//-----------------------------------------------------------------------
double GalaxyEngine::GetTime() const
{
    return m_time;
}

//-----------------------------------------------------------------------
void GalaxyEngine::SingleTimeStep(double time)
{
    m_timeStep = time;
    m_time += time;

    Vector3dC posOld;
    for (int i=0; i<m_numStars; ++i) {
        m_pStars[i].m_theta += (m_pStars[i].m_velTheta * time);
        posOld = m_pStars[i].m_pos;
        m_pStars[i].CalcXYZ();

        Vector3dC b = Vector3dC(m_pStars[i].m_pos.x - posOld.x,
                                m_pStars[i].m_pos.y - posOld.y,
                                m_pStars[i].m_pos.z - posOld.z);
        m_pStars[i].m_vel = b;
    }

    for (int i=0; i<m_numDust; ++i) {
        m_pDust[i].m_theta += (m_pDust[i].m_velTheta * time);
        posOld = m_pDust[i].m_pos;
        m_pDust[i].CalcXYZ();
    }

    for (int i=0; i<m_numH2*2; ++i) {
        m_pH2[i].m_theta += (m_pH2[i].m_velTheta * time);
        posOld = m_pDust[i].m_pos;
        m_pH2[i].CalcXYZ();
    }
}

//-----------------------------------------------------------------------
const Vector3dC& GalaxyEngine::GetStarPos(int idx)
{
    if (idx >= m_numStars)
        LOGI("index out of bounds: %d\n", idx);

    return m_pStars[idx].m_pos;
}

//-----------------------------------------------------------------------
int GalaxyEngine::GetNumH2() const
{
    return m_numH2;
}


//-----------------------------------------------------------------------
int GalaxyEngine::GetNumStars() const
{
    return m_numStars;
}

//-----------------------------------------------------------------------
int GalaxyEngine::GetNumDust() const
{
    return m_numDust;
}

GalaxyStarList GalaxyEngine::getAnchorStars()
{
    GalaxyStarList starlist;
    GalaxyStar *stars = GetStars();
    
    for (int i = 0; i < GetNumStars(); i++) {
        starlist.push_back(stars[i]);
    }
    
    return starlist;
}

GalaxyStarList GalaxyEngine::getNeighboringStars(int anchorstarid, int anchorstarclass)
{
    GalaxyStarList starList;
    
    // Generate a random amount of stars between 1500 to 3500
    Random::seedRand(anchorstarid * anchorstarclass);
    int numStars = (int) Random::randomInRange(3500.0, 4500.0);
    for (int i = 0; i < numStars; i++) {
        GalaxyStar star;
        star.m_id = i + GetNumStars();
        starList.push_back(star);
    }
    
    return starList;
}

GalaxyStarList GalaxyEngine::getStarsInRange(int starid, float range)
{
    int numStars = GetNumStars();
    GalaxyStar *stars = GetStars();
    
    Vector3dC cur_pos = stars[starid].m_pos;
 
    GalaxyStarList starsInRange;
    
    stars[starid].m_dist = 0.0;
    
    // Add this one first.
    starsInRange.push_back(stars[starid]);
        
    for (int i = 0; i < numStars; i++) {
        if (i != starid) {
            double x_diff = cur_pos.x - stars[i].m_pos.x;
            double y_diff = cur_pos.y - stars[i].m_pos.y;
            double z_diff = cur_pos.z - stars[i].m_pos.z;
            float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff) * 2.0;
            
            if (dist <= range) {
                LOGI("dist: %f, %d\n", dist, i);
                stars[i].m_dist = dist;
                
                starsInRange.push_back(stars[i]);
            }
        }
    }
    
    return starsInRange;
}

GalaxyPlanetList GalaxyEngine::getPlanetsInStarSystem(int starid, int starclass, Vector3dC pos)
{
    GalaxyPlanetList entitiesInSystem;
    
    Random::seedRand(starid * starclass);
    
    int numEntities = Random::randomInRange(0.0, 20.0);
    
    for (int i = 0; i < numEntities; i++) {
        GalaxyPlanet se;
        se.m_id = i + 1;

        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        
        double radius = 6.e12; // Approx distance to pluto
        
        while (1) {
            // Generate random point in a square.
            x = Random::randomInRange(-radius, radius);
            y = Random::randomInRange(-radius, radius);
            
            // If point in circle then break
            double distance = sqrt(x*x + y*y) * 2.0;
            if (distance <= radius) {
                // Are we in the habitable zone.
                // ** TODO ** take into account star class
                int type = 0;
                
                if (distance < kAU * 0.725) {
                    type = Random::randomInRange(PLANET_GAS_GIANT, PLANET_LAVA_RINGED + 1);
                }
                else if (distance > kAU * 0.725 && distance < kAU * 3.0) {
                    type = Random::randomInRange(PLANET_GAS_GIANT, PLANET_TERRESTRIAL_OCEANIC_RINGED + 1);
                }
                else {
                    type = Random::randomInRange(PLANET_GAS_GIANT, PLANET_ICE_RINGED + 1);
                }
                
                se.m_class = type;
                
                break;
            }
        }
        
        se.m_pos = Vector3dC(x, y, z);

        double x_diff = pos.x - x;
        double y_diff = pos.y - y;
        double z_diff = pos.z - z;
        float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff) * 2.0; // Distance from us.
        
        se.m_dist = dist;
        se.m_systemid = starid;
                
        entitiesInSystem.push_back(se);
    }
    
    return entitiesInSystem;
}

GalaxyMoonList GalaxyEngine::getMoonsForPlanet(int planetid, int planetclass, Vector3dC pos)
{
    GalaxyMoonList entitiesInSystem;
    
    Random::seedRand(planetid * planetclass);
    
    int upperrange = 10;
    
    if (planetclass == PLANET_GAS_GIANT || planetclass == PLANET_GAS_GIANT_RINGED) {
        upperrange = 70;
    }
    
    int numEntities = Random::randomInRange(0.0, upperrange);
    
    for (int i = 0; i < numEntities; i++) {
        GalaxyMoon se;
        se.m_id = i + 1;
        
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        
        double radius = 10000000.0;
        
        while (1) {
            // Generate random point in a square.
            x = Random::randomInRange(-radius, radius);
            y = Random::randomInRange(-radius, radius);
            
            // If point in circle then break
            double distance = sqrt(x*x + y*y) * 2.0;
            if (distance <= radius) {
                // Are we in the habitable zone.
                // ** TODO ** take into account star class
                bool possible_terrestrial = false;
                
                int num = Random::randomInRange(1.0, 5000.0);
                
                if (num == 500) {
                    possible_terrestrial = true;
                }
                
                int type = 0;
                
                if (possible_terrestrial) {
                    type = Random::randomInRange(MOON_TERRESTRIAL, MOON_STORM + 1);
                }
                else {
                    type = Random::randomInRange(MOON_ROCK, MOON_STORM + 1);
                }
                
                se.m_class = type;
                
                break;
            }
        }
        
        se.m_pos = Vector3dC(x, y, z);
        
        double x_diff = pos.x - x;
        double y_diff = pos.y - y;
        double z_diff = pos.z - z;
        float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff) * 2.0; // Distance from us.
        
        se.m_dist = dist;
        se.m_planetid = planetid;
        
        entitiesInSystem.push_back(se);
    }
    
    return entitiesInSystem;
}
