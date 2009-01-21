#include <boost/utility.hpp>
#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

#include <string>
#include <iostream>

#include "RefinableObj/RefinableObj.h"
#include "ObjCryst/General.h"
#include "ObjCryst/Scatterer.h"

using namespace ObjCryst;
using namespace boost::python;

namespace {

const char* scattererdoc = "Generic type of scatterer: can be an atom, or a more complex assembly of atoms.  A Scatterer is able to give its position (in fractionnal coordinates) in the unit cell, and more generally the position of all point scattering centers (ScatteringComponent), along with the ScatteringPower associated with each position.  For simple atoms, there is only one scattering position with the associated scattering power (scattering factor, anomalous, thermic). For complex scatterers (molecules: ZScatterer) there are as many positions as atoms.  A scatterer also has a few functions to display itself in 3D. This is an abstract base class.";

// This is necessary to make sure that all virtual functions get handled
// properly. We want to create new classes in Python and have them work properly
// in the c++ code. This is not only flexible, but really awesome.
class ScattererWrap : public Scatterer, 
                      public wrapper<Scatterer>
{

    public: 

    ScattererWrap() : Scatterer() {}

    ScattererWrap(const ScattererWrap& S) : Scatterer(S) {}

    void default_SetX(const float x) 
    { Scatterer::SetX(x);}

    void SetX(const float x)
    {
        if (override SetX = this->get_override("SetX")) 
        {
            SetX(x);
            return;
        }
        default_SetX(x);
    }

    void default_SetY(const float y) 
    { Scatterer::SetY(y);}

    void SetY(const float y)
    {
        if (override SetY = this->get_override("SetY")) 
        {
            SetY(y);
            return;
        }
        default_SetY(y);
    }

    void default_SetZ(const float z) 
    { Scatterer::SetZ(z);}

    void SetZ(const float z)
    {
        if (override SetZ = this->get_override("SetZ")) 
        {
            SetZ(z);
            return;
        }
        default_SetZ(z);
    }

    void default_SetOccupancy(const float occ) 
    { Scatterer::SetOccupancy(occ);}

    void SetOccupancy(const float occ)
    {
        if (override SetOccupancy = this->get_override("SetOccupancy")) 
        {
            SetOccupancy(occ);
            return;
        }
        default_SetOccupancy(occ);
    }

    // Pure virtual
    
    Scatterer* CreateCopy() const
    {
        return this->get_override("CreateCopy")();
    }

    int GetNbComponent() const
    {
        return this->get_override("GetNbComponent")();
    }

    const ScatteringComponentList& GetScatteringComponentList() const
    {
        return this->get_override("GetScatteringComponentList")();
    }

    std::string GetComponentName(const int i) const
    {
        return this->get_override("GetComponentName")();
    }

    void Print() const
    {
        this->get_override("Print")();
    }

    std::ostream& POVRayDescription(std::ostream& os, 
            const CrystalPOVRayOptions& options) const
    {
        return this->get_override("POVRayDescription")();
    }

    void GLInitDisplayList(const bool noSymmetrics,
            const float xMin, const float xMax,
            const float yMin, const float yMax,
            const float zMin, const float zMax,
            const bool displayEnantiomer,
            const bool displayNames) const
    {
        this->get_override("GLInitDisplayList")();
    }

    void _InitRefParList()
    {
        InitRefParList();
    }

    const RefinableObjClock& GetClockScattCompList() const
    {
        return Scatterer::GetClockScattCompList();
    }

    protected:

    void InitRefParList()
    {
        this->get_override("InitRefParList")();
    }
}; // ScattererWrap


} // anonymous namespace


BOOST_PYTHON_MODULE(_scatterer)
{

    class_<ScattererWrap, boost::noncopyable, bases<RefinableObj> >
        ("Scatterer")
        //// Copy constructor
        .def(init<const ScattererWrap&>())
        //// Methods
        .def("GetX", &Scatterer::GetX)
        .def("GetY", &Scatterer::GetY)
        .def("GetZ", &Scatterer::GetZ)
        .def("GetOccupancy", &Scatterer::GetOccupancy)
        .def("SetX", &Scatterer::SetX, &ScattererWrap::default_SetX)
        .def("SetY", &Scatterer::SetY, &ScattererWrap::default_SetY)
        .def("SetZ", &Scatterer::SetZ, &ScattererWrap::default_SetZ)
        .def("SetOccupancy", &ObjCryst::Scatterer::SetOccupancy, 
                &ScattererWrap::default_SetOccupancy)
        .def("GetClockScatterer", 
                (RefinableObjClock & (Scatterer::*)())
                &Scatterer::GetClockScatterer,
                //return_value_policy<copy_non_const_reference>())
                return_internal_reference<>())
        .def("SetCrystal", &Scatterer::SetCrystal,
                with_custodian_and_ward<1,2>())
        // FIXME Needs converter
        .def("GetCrystal", (const Crystal &(Scatterer::*)() const) 
                &Scatterer::GetCrystal,
                return_value_policy<copy_const_reference>())
        // Pure virtual functions
        .def("CreateCopy", pure_virtual(&Scatterer::CreateCopy),
            return_value_policy<manage_new_object>())
        .def("GetNbComponent", pure_virtual(&Scatterer::GetNbComponent))
        .def("GetScatteringComponentList", 
                pure_virtual(&Scatterer::GetScatteringComponentList),
                return_internal_reference<>())
        .def("Print", pure_virtual(&Scatterer::Print))
        // Protected
        // FIXME Can't get these to work. Probably protected issue.
        //.def("_InitRefParList", pure_virtual(&Scatterer::InitRefParList))
        //.def("_GetClockScattCompList", 
        //        &ScattererWrap::GetClockScattCompList,
        //        return_value_policy<copy_const_reference>())
        ;

}
