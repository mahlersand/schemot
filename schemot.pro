TEMPLATE = lib

CONFIG -= qt
CONFIG += staticlib c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    include/SchemotPack.hpp \
    include/packs_common.h \
    include/schemot.hpp \
    include/TypePack.hpp \
    include/ValuePack.hpp

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
