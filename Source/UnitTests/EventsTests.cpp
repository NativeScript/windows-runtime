#include "UnitTests-Prefix.h"
#include <iomanip>

namespace NativeScript {
namespace UnitTests {

    TEST_CLASS(EventsTests){
        public :
            TEST_METHOD(Events){
                MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EventsClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };
    IteratorRange<ClassDeclaration::EventIterator> events{ declaration->events() };

    ClassDeclaration::EventIterator it{ events.begin() };
    const EventDeclaration& staticEvent{ *it };
    Assert::IsTrue(staticEvent.name() == L"StaticEvent");
    Assert::IsTrue(staticEvent.isStatic() == true);

    ++it;
    const EventDeclaration& emptyEvent{ *it };
    Assert::IsTrue(emptyEvent.name() == L"EmptyEvent");
    Assert::IsTrue(emptyEvent.isStatic() == false);

    ++it;
    const EventDeclaration& externalEvent{ *it };
    Assert::IsTrue(externalEvent.name() == L"ExternalEvent");
    Assert::IsTrue(externalEvent.isStatic() == false);

    ++it;
    const EventDeclaration& typedEvent{ *it };
    Assert::IsTrue(typedEvent.name() == L"GenericEvent");
    Assert::IsTrue(typedEvent.isStatic() == false);

    ++it;
    Assert::IsTrue(it == events.end());
}

TEST_METHOD(EmptyEvent) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EventsClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    const wchar_t* eventName{ L"EmptyEvent" };
    EventDeclaration emptyEvent{ *static_cast<const EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get()) };
    Assert::IsTrue(emptyEvent.name() == eventName);
    Assert::IsTrue(emptyEvent.fullName() == eventName);
    Assert::IsTrue(emptyEvent.type().fullName() == L"NativeScript.TestFixtures.EmptyDelegate");
    Assert::IsTrue(emptyEvent.addMethod().fullName() == L"add_EmptyEvent");
    Assert::IsTrue(emptyEvent.removeMethod().fullName() == L"remove_EmptyEvent");
}

TEST_METHOD(ExternalEvent) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EventsClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    const wchar_t* eventName{ L"ExternalEvent" };
    EventDeclaration externalEvent{ *static_cast<const EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get()) };
    Assert::IsTrue(externalEvent.name() == eventName);
    Assert::IsTrue(externalEvent.fullName() == eventName);
    Assert::IsTrue(externalEvent.type().fullName() == L"Windows.UI.Xaml.RoutedEventHandler");
    Assert::IsTrue(externalEvent.addMethod().fullName() == L"add_ExternalEvent");
    Assert::IsTrue(externalEvent.removeMethod().fullName() == L"remove_ExternalEvent");
}

TEST_METHOD(ExternalGenericEvent) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"NativeScript.TestFixtures.EventsClass" };
    shared_ptr<ClassDeclaration> declaration{ static_pointer_cast<ClassDeclaration>(metadataReader.findByName(name)) };

    const wchar_t* eventName{ L"GenericEvent" };
    EventDeclaration typedEvent{ *static_cast<const EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get()) };
    Assert::IsTrue(typedEvent.name() == eventName);
    Assert::IsTrue(typedEvent.fullName() == eventName);
    Assert::IsTrue(typedEvent.type().fullName() == L"Windows.Foundation.TypedEventHandler`2<Object, Int32>");
    Assert::IsTrue(typedEvent.addMethod().fullName() == L"add_GenericEvent");
    Assert::IsTrue(typedEvent.removeMethod().fullName() == L"remove_GenericEvent");
}

TEST_METHOD(GenericEvent) {
    MetadataReader metadataReader;

    const wchar_t* name{ L"Windows.UI.Core.ICoreWindow" };
    shared_ptr<InterfaceDeclaration> declaration{ static_pointer_cast<InterfaceDeclaration>(metadataReader.findByName(name)) };

    const wchar_t* eventName{ L"KeyUp" };
    EventDeclaration typedEvent{ *static_cast<const EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get()) };
    Assert::IsTrue(typedEvent.name() == eventName);
    Assert::IsTrue(typedEvent.fullName() == eventName);
    Assert::IsTrue(typedEvent.type().fullName() == L"Windows.Foundation.TypedEventHandler`2<Windows.UI.Core.CoreWindow, Windows.UI.Core.KeyEventArgs>");
    Assert::IsTrue(typedEvent.addMethod().fullName() == L"add_KeyUp");
    Assert::IsTrue(typedEvent.removeMethod().fullName() == L"remove_KeyUp");
}
};
}
}
