QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        meshprocessor.cpp \
        skeleton.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    meshprocessor.hpp \
    mutil/mat/intmatrix2.h \
    mutil/mat/intmatrix3.h \
    mutil/mat/intmatrix4.h \
    mutil/mat/mat.h \
    mutil/mat/mat_impl.h \
    mutil/mat/mat_types.h \
    mutil/mat/matrix2.h \
    mutil/mat/matrix3.h \
    mutil/mat/matrix4.h \
    mutil/math/f_math.h \
    mutil/math/fmat_math.h \
    mutil/math/fmat_math_defs.h \
    mutil/math/fmat_transform.h \
    mutil/math/i_math.h \
    mutil/math/math.h \
    mutil/math/noise.h \
    mutil/mutil.h \
    mutil/quat/quaternion.h \
    mutil/quat/quaternion_impl.h \
    mutil/settings.h \
    mutil/vec/intvector2.h \
    mutil/vec/intvector3.h \
    mutil/vec/intvector4.h \
    mutil/vec/vec.h \
    mutil/vec/vec_impl.h \
    mutil/vec/vec_types.h \
    mutil/vec/vector2.h \
    mutil/vec/vector3.h \
    mutil/vec/vector4.h \
    obj_io.h \
    skeleton.hpp

DISTFILES += \
    mutil/matrixutil.natvis
