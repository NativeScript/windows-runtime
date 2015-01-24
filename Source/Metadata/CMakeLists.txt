set(HEADER_FILES
    Declarations/BaseClassDeclaration.h
    Declarations/ClassDeclaration.h
    Declarations/Declaration.h
    Declarations/DelegateDeclaration.h
    Declarations/EnumDeclaration.h
    Declarations/EnumMemberDeclaration.h
    Declarations/EventDeclaration.h
    Declarations/FieldDeclaration.h
    Declarations/InterfaceDeclaration.h
    Declarations/MethodDeclaration.h
    Declarations/NamespaceDeclaration.h
    Declarations/ParameterDeclaration.h
    Declarations/PropertyDeclaration.h
    Declarations/StructDeclaration.h
    Declarations/StructFieldDeclaration.h
    Declarations/TypeDeclaration.h
    Declarations/Declarations.h

    Wrappers/IteratorRange.h

    ComHelpers.h
    DeclarationFactory.h
    DeclaringInterfaceForMethod.h
    GenericInstanceIdBuilder.h
    Metadata-Prefix.h
    MetadataReader.h
    Signature.h
)

set(SOURCE_FILES
    Declarations/BaseClassDeclaration.cpp
    Declarations/ClassDeclaration.cpp
    Declarations/Declaration.cpp
    Declarations/DelegateDeclaration.cpp
    Declarations/EnumDeclaration.cpp
    Declarations/EnumMemberDeclaration.cpp
    Declarations/EventDeclaration.cpp
    Declarations/FieldDeclaration.cpp
    Declarations/InterfaceDeclaration.cpp
    Declarations/MethodDeclaration.cpp
    Declarations/NamespaceDeclaration.cpp
    Declarations/ParameterDeclaration.cpp
    Declarations/PropertyDeclaration.cpp
    Declarations/StructDeclaration.cpp
    Declarations/StructFieldDeclaration.cpp
    Declarations/TypeDeclaration.cpp

    ComHelpers.cpp
    DeclarationFactory.cpp
    DeclaringInterfaceForMethod.cpp
    GenericInstanceIdBuilder.cpp
    Metadata-Prefix.cpp
    MetadataReader.cpp
    Signature.cpp
)

# TODO: Warnings as error

add_library(Metadata STATIC ${SOURCE_FILES} ${HEADER_FILES})

include(GroupSources)
GroupSources(Metadata)

include(PrecompiledHeaders)
SET_PRECOMPILED_HEADER(Metadata Metadata-Prefix.h Metadata-Prefix.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
