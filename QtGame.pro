SOURCES += \
    gameobject.cpp \
    AnimationSystem/animatedmodel.cpp \
    AnimationSystem/animation.cpp \
    AnimationSystem/animator.cpp \
    AnimationSystem/joint.cpp \
    AnimationSystem/jointtransform.cpp \
    AnimationSystem/keyframe.cpp \
    debug/moc_gamewindow.cpp \
    debug/moc_mainwindow.cpp \
    debug/moc_window.cpp \
    gamewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    model.cpp \
    player.cpp \
    window.cpp \
    debug.cpp \
    cube.cpp

HEADERS += \
    gameobject.h \
    AnimationSystem/animatedmodel.h \
    AnimationSystem/animation.h \
    AnimationSystem/animator.h \
    AnimationSystem/joint.h \
    AnimationSystem/jointtransform.h \
    AnimationSystem/keyframe.h \
    debug/moc_predefs.h \
    lib/assimp/include/assimp/Compiler/poppack1.h \
    lib/assimp/include/assimp/Compiler/pstdint.h \
    lib/assimp/include/assimp/Compiler/pushpack1.h \
    lib/assimp/include/assimp/port/AndroidJNI/AndroidJNIIOSystem.h \
    lib/assimp/include/assimp/ai_assert.h \
    lib/assimp/include/assimp/anim.h \
    lib/assimp/include/assimp/BaseImporter.h \
    lib/assimp/include/assimp/Bitmap.h \
    lib/assimp/include/assimp/BlobIOSystem.h \
    lib/assimp/include/assimp/ByteSwapper.h \
    lib/assimp/include/assimp/camera.h \
    lib/assimp/include/assimp/cexport.h \
    lib/assimp/include/assimp/cfileio.h \
    lib/assimp/include/assimp/cimport.h \
    lib/assimp/include/assimp/color4.h \
    lib/assimp/include/assimp/config.h \
    lib/assimp/include/assimp/CreateAnimMesh.h \
    lib/assimp/include/assimp/DefaultIOStream.h \
    lib/assimp/include/assimp/DefaultIOSystem.h \
    lib/assimp/include/assimp/DefaultLogger.hpp \
    lib/assimp/include/assimp/Defines.h \
    lib/assimp/include/assimp/defs.h \
    lib/assimp/include/assimp/Exceptional.h \
    lib/assimp/include/assimp/Exporter.hpp \
    lib/assimp/include/assimp/fast_atof.h \
    lib/assimp/include/assimp/GenericProperty.h \
    lib/assimp/include/assimp/Hash.h \
    lib/assimp/include/assimp/Importer.hpp \
    lib/assimp/include/assimp/importerdesc.h \
    lib/assimp/include/assimp/IOStream.hpp \
    lib/assimp/include/assimp/IOStreamBuffer.h \
    lib/assimp/include/assimp/IOSystem.hpp \
    lib/assimp/include/assimp/irrXMLWrapper.h \
    lib/assimp/include/assimp/light.h \
    lib/assimp/include/assimp/LineSplitter.h \
    lib/assimp/include/assimp/LogAux.h \
    lib/assimp/include/assimp/Logger.hpp \
    lib/assimp/include/assimp/LogStream.hpp \
    lib/assimp/include/assimp/Macros.h \
    lib/assimp/include/assimp/material.h \
    lib/assimp/include/assimp/MathFunctions.h \
    lib/assimp/include/assimp/matrix3x3.h \
    lib/assimp/include/assimp/matrix4x4.h \
    lib/assimp/include/assimp/MemoryIOWrapper.h \
    lib/assimp/include/assimp/mesh.h \
    lib/assimp/include/assimp/metadata.h \
    lib/assimp/include/assimp/NullLogger.hpp \
    lib/assimp/include/assimp/ParsingUtils.h \
    lib/assimp/include/assimp/pbrmaterial.h \
    lib/assimp/include/assimp/postprocess.h \
    lib/assimp/include/assimp/Profiler.h \
    lib/assimp/include/assimp/ProgressHandler.hpp \
    lib/assimp/include/assimp/qnan.h \
    lib/assimp/include/assimp/quaternion.h \
    lib/assimp/include/assimp/RemoveComments.h \
    lib/assimp/include/assimp/scene.h \
    lib/assimp/include/assimp/SceneCombiner.h \
    lib/assimp/include/assimp/SGSpatialSort.h \
    lib/assimp/include/assimp/SkeletonMeshBuilder.h \
    lib/assimp/include/assimp/SmoothingGroups.h \
    lib/assimp/include/assimp/SpatialSort.h \
    lib/assimp/include/assimp/StandardShapes.h \
    lib/assimp/include/assimp/StreamReader.h \
    lib/assimp/include/assimp/StreamWriter.h \
    lib/assimp/include/assimp/StringComparison.h \
    lib/assimp/include/assimp/StringUtils.h \
    lib/assimp/include/assimp/Subdivision.h \
    lib/assimp/include/assimp/texture.h \
    lib/assimp/include/assimp/TinyFormatter.h \
    lib/assimp/include/assimp/types.h \
    lib/assimp/include/assimp/vector2.h \
    lib/assimp/include/assimp/vector3.h \
    lib/assimp/include/assimp/version.h \
    lib/assimp/include/assimp/Vertex.h \
    lib/assimp/include/assimp/XMLTools.h \
    gamewindow.h \
    mainwindow.h \
    mesh.h \
    model.h \
    player.h \
    window.h \
    debug.h \
    cube.h

QT           += widgets gui
QMAKE_LFLAGS

LIBS += opengl32.lib

target.path = ./qtgame
INSTALLS += target

DESTDIR = builds
OBJECTS_DIR = builds/objects

DISTFILES += \
    shaders/simple.frag \
    shaders/simple.vert \
    meshes/paratrooper.obj \
    builds/resources/meshes/paratrooper.mtl \
    builds/resources/shaders/simple.frag \
    builds/resources/shaders/simple.vert \
    builds/resources/shaders/skinned.frag \
    builds/resources/shaders/skinned.vert \
    lib/assimp/assimp-vc140-mt.lib \
    lib/assimp/assimp-vc140-mt.dll \
    lib/assimp/include/assimp/.editorconfig \
    lib/assimp/include/assimp/config.h.in


win32: LIBS += -L$$PWD/'lib/assimp/' -lassimp-vc140-mt

INCLUDEPATH += $$PWD/'lib/assimp/include'
DEPENDPATH += $$PWD/'lib/assimp/include'
