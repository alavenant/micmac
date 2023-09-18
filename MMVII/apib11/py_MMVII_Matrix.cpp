#include "py_MMVII.h"
#include "pybind11/numpy.h"

#include "MMVII_Geom2D.h"


using namespace MMVII;


template<typename T>
void pyb_init_Matrix_tpl(py::module_ &m, const std::string& name) {
    using namespace std::literals;
    using namespace pybind11::literals;

    typedef cDenseMatrix<T> tDM;

    auto dm=py::class_<tDM>(m, name.c_str(),py::buffer_protocol());
            dm.def(py::init<int,int,eModeInitImage>(),"x"_a,"y"_a,"modeInitImage"_a = eModeInitImage::eMIA_NoInit);
            dm.def(py::init<int,eModeInitImage>(),"x"_a,"modeInitImage"_a = eModeInitImage::eMIA_NoInit);
            dm.def(py::init<typename tDM::tIm>(),"im2d"_a);

            // TODO : a verifier le stride/offset !
            dm.def(py::init([](py::array_t<T, py::array::c_style | py::array::forcecast> array) {
                py::buffer_info info = array.request();
                if (info.format != py::format_descriptor<T>::format())
                    throw std::runtime_error("Incompatible format: expected a array!");
                if (info.ndim != 2)
                    throw std::runtime_error("Incompatible buffer dimension!");
                tDM *m = new tDM(info.shape[1], info.shape[0]);
                T* ptr = static_cast<T *>(info.ptr);
                for (int j=0; j<info.shape[0]; j++)
                    for (int i=0; i<info.shape[1]; i++)
                        m->SetElem(i, j, *(ptr++));
                return m;
            })
            ,"array"_a);

            dm.def("dup",&tDM::Dup);
            dm.def_static("identity",&tDM::Identity,"size"_a);
//            .def_static("Diag",&cDM::Diag)
            dm.def("closestOrthog",&tDM::ClosestOrthog);

            
            dm.def("sz",&tDM::Sz);
            dm.def("show",&tDM::Show);

            dm.def("im",py::overload_cast<>(&tDM::Im),py::return_value_policy::reference_internal);
            dm.def("dIm",py::overload_cast<>(&tDM::DIm),py::return_value_policy::reference_internal);
            
            dm.def("setElem",&tDM::SetElem,"x"_a,"y"_a,"val"_a);
            dm.def("addElem",&tDM::AddElem,"x"_a,"y"_a,"val"_a);

            dm.def("getElem",py::overload_cast<int,int>(&tDM::GetElem, py::const_),"x"_a,"y"_a);
            dm.def("getElem",py::overload_cast<const cPt2di &>(&tDM::GetElem, py::const_),"pt2di"_a);

            dm.def("inverse",py::overload_cast<>(&tDM::Inverse, py::const_));

            dm.def("__repr__",[name](const tDM& m) {
                std::ostringstream ss;
                ss << name << " (" << m.Sz().y() << " rows, " << m.Sz().x() << " cols)";
                return ss.str();
            });

            dm.def("__matmul__",[](const tDM& m1, const tDM& m2) -> tDM {
                return m1*m2;
            });

            dm.def("__matmul__",[](const tDM& m1, const cDenseVect<T>& m2) -> cDenseVect<T> {

                  return m1*m2;
            });

            // TODO : a verifier le stride/offset !
            dm.def_buffer([](tDM &m) -> py::buffer_info {
                    return py::buffer_info(
                        *m.DIm().ExtractRawData2D(),                               /* Pointer to buffer */
                        sizeof(T),                          /* Size of one scalar */
                        py::format_descriptor<T>::format(), /* Python struct-style format descriptor */
                        2,                                      /* Number of dimensions */
                        { m.Sz().y(), m.Sz().x() },                 /* Buffer dimensions */
                        { sizeof(T) * m.Sz().x(),             /* Strides (in bytes) for each index */
                          sizeof(T) }
                    );})
            ;
            
}

void pyb_init_DenseMatrix(py::module_ &m)
{

    pyb_init_Matrix_tpl<double>(m,"Matrixr");
    pyb_init_Matrix_tpl<float>(m,"Matrixf");
}
