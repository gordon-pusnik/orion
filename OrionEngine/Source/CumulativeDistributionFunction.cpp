#include "CumulativeDistributionFunction.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Utils.h"

//-------------------------------------------------------------------------------------------------
CumulativeDistributionFunction::CumulativeDistributionFunction()
    :m_pDistFun(NULL)
    ,m_vM1()
    ,m_vY1()
    ,m_vX1()
    ,m_vM2()
    ,m_vY2()
    ,m_vX2()
{}

//-------------------------------------------------------------------------------------------------
void CumulativeDistributionFunction::SetupRealistic(double I0, double k, double a, double RBulge, double min, double max, int nSteps)
{
    m_fMin = min;
    m_fMax = max;
    m_nSteps = nSteps;

    m_I0 = I0;
    m_k  = k;
    m_a  = a;
    m_RBulge = RBulge;

    m_pDistFun = &CumulativeDistributionFunction::Intensity;

    // build the distribution function
    BuildCDF(m_nSteps);
}

//-------------------------------------------------------------------------------------------------
void CumulativeDistributionFunction::BuildCDF(int nSteps)
{
    double h = (m_fMax - m_fMin) / nSteps;
    double x=0, y=0;

    m_vX1.clear();
    m_vY1.clear();
    m_vX2.clear();
    m_vY2.clear();
    m_vM1.clear();
    m_vM2.clear();

    // Simpson rule for integration of the distribution function
    m_vY1.push_back(0.0);
	m_vM1.push_back(0.0);
    m_vX1.push_back(0.0);
    for (int i=0; i<nSteps; i+=2) {
        x = (i+2) *h;
        y += h/3 * ((this->*m_pDistFun)(m_fMin + i*h) + 4*(this->*m_pDistFun)(m_fMin + (i+1)*h) + (this->*m_pDistFun)(m_fMin + (i+2)*h) );

        m_vM1.push_back((y - m_vY1.back()) / (2*h));
        m_vX1.push_back(x);
        m_vY1.push_back(y);

        //    printf("%2.2f, %2.2f, %2.2f\n", m_fMin + (i+2) * h, v, h);
    }

    // normieren
    for (std::size_t i=0; i<m_vY1.size(); ++i) {
        m_vY1[i] /= m_vY1.back();
        m_vM1[i] /= m_vY1.back();
    }

    m_vY2.push_back(0.0);
    m_vM2.push_back(0.0);

    double p=0;
    h = 1.0/nSteps;
    
    for (int i=1, k=0; i<nSteps; ++i) {
        p = (double)i * h;

        for (; m_vY1[k+1]<=p; ++k)
        {}

        y = m_vX1[k] + (p - m_vY1[k]) / m_vM1[k];

        //    LOGI("%2.4f, %2.4f, k=%d, %2.4f, %2.4f\n", p, y, k, m_vY1[k], m_vM1[k]);
        //    LOGI("%f, %f %d %f\n", m_vY2.back(), h, i, (y - m_vY2.back()) / h);
          
        m_vM2.push_back((y - m_vY2.back()) / h);
        m_vX2.push_back(p);
        m_vY2.push_back(y);
    }
}

//-------------------------------------------------------------------------------------------------
double CumulativeDistributionFunction::ValFromProp(double fVal)
{
    if (fVal<0 || fVal>1)
        LOGI("ValFromProp - out of range: %f\n", fVal);
    
    double h = 1.0 / m_vY2.size();
    
    int i = (int)(fVal / h);
    double remainder = fVal - i*h;
    
//    LOGI("ValFromProp %f %d %f %f %d\n", fVal, (int)m_vY2.size(), m_vY2[i], m_vM2[i], i);
    
    return (m_vY2[i] + m_vM2[i] * remainder) /* / m_vVal.back()*/;
}

//-------------------------------------------------------------------------------------------------
CumulativeDistributionFunction::~CumulativeDistributionFunction()
{}

//-------------------------------------------------------------------------------------------------
double CumulativeDistributionFunction::PropFromVal(double fVal)
{
    if (fVal<m_fMin || fVal>m_fMax)
        LOGI("PropFromVal - out of range: %f\n", fVal);

    double h = 2 * ((m_fMax - m_fMin) / m_nSteps);
    int i = (int)((fVal - m_fMin) / h);
    double remainder = fVal - i*h;

    //  printf("fVal=%2.2f; h=%2.2f; i=%d; m_vVal[i]=%2.2f; m_vAsc[i]=%2.2f;\n", fVal, h, i, m_vVal[i], m_vAsc[i]);

    return (m_vY1[i] + m_vM1[i] * remainder) /* / m_vVal.back()*/;
}

//-------------------------------------------------------------------------------------------------
double CumulativeDistributionFunction::IntensityBulge(double R, double I0, double k)
{
    return I0 * exp(-k*pow(R, 0.25));
}

//-------------------------------------------------------------------------------------------------
double CumulativeDistributionFunction::IntensityDisc(double R, double I0, double a)
{
    return I0 * exp(-R/a);
}

//-------------------------------------------------------------------------------------------------
double CumulativeDistributionFunction::Intensity(double x)
{
    return (x<m_RBulge) ? IntensityBulge(x, m_I0, m_k) : IntensityDisc(x-m_RBulge, IntensityBulge(m_RBulge, m_I0, m_k), m_a);
}
