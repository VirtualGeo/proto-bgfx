
#include <entry/entry.h>
#include <engine/geometry.h>

//class ExampleHelloWorld : public entry::AppI {
//public:
namespace entry {

void init(View & view)
{
}

int shutdown()
{
}

bool update(const View & view)
{
//    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(40, 30, 0x0f, "Hello world");
}

} // entry

//private:
//};

//int main() {
//    ExampleHelloWorld app;
////    return entry::runApp(&app);
//    return 0;
//}
