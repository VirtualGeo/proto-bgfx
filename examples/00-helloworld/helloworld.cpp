
#include <engine/geometry.h>
#include <entry/entry.h>

//class ExampleHelloWorld : public entry::AppI {
//public:
namespace entry {
    int s_nWindow = 1;

void init(View& view)
{
}

void shutdown()
{
}

void preRender()
{
}

void render(const View& view)
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
