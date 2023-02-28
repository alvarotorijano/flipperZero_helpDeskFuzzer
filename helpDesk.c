#include <gui/gui.h>
#include <gui/view_port.h>

ExampleThermoContext* context = example_thermo_context_alloc();
typedef struct {
    Gui* gui = NULL;
    ViewPort* view_port = NULL;
    FuriThread* reader_thread = NULL;
    FuriMessageQueue* event_queue = NULL;
} ExampleThermoContext;

int32_t main_helpDesk(void* p) {
    return 0;
}