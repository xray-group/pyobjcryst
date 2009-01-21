#include <boost/utility.hpp>
#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

#include <string>
#include <iostream>

#include "ObjCryst/ScatteringPower.h"
#include "ObjCryst/ScatteringPowerSphere.h"
#include "RefinableObj/RefinableObj.h"
#include "CrystVector/CrystVector.h"

using namespace boost::python;
using namespace ObjCryst;

BOOST_PYTHON_MODULE(_scatteringpowersphere)
{

    class_<ScatteringPowerSphere, bases<ScatteringPower> > ("ScatteringPowerSphere", 
         init<>())
        .def(init<const std::string&, const float, optional<const float> >())
        // Not implemented in libraries
        //.def(init<const ScatteringPowerSphere&>())
        .def("Init", &ScatteringPowerSphere::Init,
                (boost::python::arg("name"),
                boost::python::arg("radius"),
                boost::python::arg("biso")=1.0
                ))
        .def("GetRadius", &ScatteringPowerSphere::GetRadius)
        ;
}
