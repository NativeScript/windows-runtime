set(HEADER_FILES
    Ast.h
    BaseClassDeclaration.h
    ClassDeclaration.h
    ComHelpers.h
    Declaration.h
    DeclaringInterfaceForMethod.h
    DelegateDeclaration.h
    EnumDeclaration.h
    EnumMemberDeclaration.h
    EventDeclaration.h
    FieldDeclaration.h
    InterfaceDeclaration.h
    IteratorRange.h
    MetadataReader.h
    MethodDeclaration.h
    NamespaceDeclaration.h
    ParameterDeclaration.h
    pch.h
    PropertyDeclaration.h
    StructDeclaration.h
    StructFieldDeclaration.h
    TypeDeclaration.h
)

set(SOURCE_FILES
    BaseClassDeclaration.cpp
    ClassDeclaration.cpp
    ComHelpers.cpp
    Declaration.cpp
    DeclaringInterfaceForMethod.cpp
    DelegateDeclaration.cpp
    EnumDeclaration.cpp
    EnumMemberDeclaration.cpp
    EventDeclaration.cpp
    FieldDeclaration.cpp
    InterfaceDeclaration.cpp
    MetadataReader.cpp
    MethodDeclaration.cpp
    NamespaceDeclaration.cpp
    ParameterDeclaration.cpp
    PropertyDeclaration.cpp
    StructDeclaration.cpp
    StructFieldDeclaration.cpp
    TypeDeclaration.cpp
)

# TODO: PCH
# TODO: Warnings as error

source_group("" FILES ${SOURCE_FILES} ${HEADER_FILES})

add_library(Metadata STATIC ${SOURCE_FILES} ${HEADER_FILES})
